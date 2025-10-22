/* This file is part of Dilay
 * Copyright © 2015-2018 Alexander Bau
 * Use and redistribute under the terms of the GNU General Public License
 */
#include <QFrame>
#include "cache.hpp"
#include "camera.hpp"
#include "config.hpp"
#include "dimension.hpp"
#include "primitive/plane.hpp"
#include "primitive/ray.hpp"
#include "primitive/sphere.hpp"
#include "scene.hpp"
#include "sketch/sketchmesh-intersection.hpp"
#include "sketch/sketchmesh.hpp"
#include "sketch/path-intersection.hpp"
#include "sketch/path.hpp"
#include "state.hpp"
#include "tool/util/step.hpp"
#include "tools.hpp"
#include "view/cursor.hpp"
#include "view/double-slider.hpp"
#include "view/pointing-event.hpp"
#include "view/tool-tip.hpp"
#include "view/two-column-grid.hpp"
#include "view/viewutil.hpp"

struct ToolSketchSpheres::Impl
{
  ToolSketchSpheres*     self;
  ViewCursor             cursor;
  ViewDoubleSlider&      radiusEdit;
  ViewDoubleSlider&      heightEdit;
  SketchPathSmoothEffect smoothEffect;
  float                  stepWidthFactor;
  SketchMesh*            mesh;
  ToolUtilStep           step;

  Impl (ToolSketchSpheres* s)
    : self (s)
    , radiusEdit (ViewUtil::slider (2, 0.01f, s->cache ().get<float> ("radius", 0.1f), 1.0f))
    , heightEdit (ViewUtil::slider (2, 0.01f, s->cache ().get<float> ("height", 0.2f), 0.45f))
    , smoothEffect (SketchPathSmoothEffect (
        s->cache ().get<int> ("smooth-effect", int(SketchPathSmoothEffect::Embed))))
    , stepWidthFactor (0.0f)
    , mesh (nullptr)
  {
  }

  void setupProperties ()
  {
    ViewTwoColumnGrid& properties = this->self->properties ();

    this->self->addMirrorProperties ();

    ViewUtil::connect (this->radiusEdit, [this](float r) {
      this->cursor.radius (r);
      this->self->cache ().set ("radius", r);
    });
    properties.addStacked (QObject::tr ("Radius"), this->radiusEdit);

    ViewUtil::connect (this->heightEdit,
                       [this](float d) { this->self->cache ().set ("height", d); });
    properties.addStacked (QObject::tr ("Height"), this->heightEdit);

    properties.add (ViewUtil::horizontalLine ());

    QButtonGroup& smoothEffectEdit =
      ViewUtil::buttonGroup ({QObject::tr ("None"), QObject::tr ("Embed"),
                              QObject::tr ("Embed and adjust"), QObject::tr ("Pinch")});
    ViewUtil::connect (smoothEffectEdit, int(this->smoothEffect), [this](int id) {
      this->smoothEffect = SketchPathSmoothEffect (id);
      this->self->cache ().set ("smooth-effect", id);
    });
    properties.addStacked (QObject::tr ("Smoothing effect"), smoothEffectEdit);
  }

  void setupToolTip ()
  {
    ViewToolTip toolTip;
    toolTip.add (ViewInputEvent::MouseLeft, QObject::tr ("Drag to sketch"));
    toolTip.add (ViewInputEvent::MouseLeft, ViewInputModifier::Shift,
                 QObject::tr ("Drag to smooth"));
    toolTip.add (ViewInputEvent::R, QObject::tr ("Move to change radius"));
    this->self->state ().setToolTip (&toolTip);
  }

  void setupCursor ()
  {
    this->cursor.disable ();
    this->cursor.radius (this->radiusEdit.doubleValue ());
  }

  ToolResponse runInitialize ()
  {
    this->self->supportsMirror ();

    this->setupProperties ();
    this->setupToolTip ();
    this->setupCursor ();

    return ToolResponse::None;
  }

  void runRender () const
  {
    Camera& camera = this->self->state ().camera ();

    if (this->cursor.isEnabled ())
    {
      this->cursor.render (camera);
    }
  }

  glm::vec3 newSpherePosition (bool considerHeight, const glm::vec3& position,
                               const glm::vec3& normal)
  {
    if (considerHeight)
    {
      const float factor =
        float(this->radiusEdit.doubleValue () * (1.0f - (2.0f * this->heightEdit.doubleValue ())));
      return position - (normal * factor);
    }
    else
    {
      return position;
    }
  }

  ToolResponse runMoveEvent (const ViewPointingEvent& e)
  {
    this->step.stepWidth (this->radiusEdit.doubleValue () * this->stepWidthFactor);

    if (e.leftButton ())
    {
      if (e.modifiers () == Qt::ShiftModifier)
      {
        SketchPathIntersection intersection;

        if (this->self->intersectsScene (e, intersection))
        {
          this->cursor.enable ();
          this->cursor.position (intersection.position ());

          this->step.step (intersection.position (), [this,
                                                      &intersection](const glm::vec3& position) {
            intersection.mesh ().smoothPath (intersection.path (),
                                             PrimSphere (position, this->radiusEdit.doubleValue ()),
                                             1, this->smoothEffect, this->self->mirrorDimension ());
            return true;
          });

          this->mesh = &intersection.mesh ();
        }
      }
      else if (this->mesh)
      {
        SketchMeshIntersection intersection;
        const unsigned int     numExcludedLastPaths = this->self->mirrorEnabled () ? 2 : 1;
        bool                   considerHeight = true;

        if (this->self->intersectsScene (e, intersection, numExcludedLastPaths) == false)
        {
          const Camera&   camera = this->self->state ().camera ();
          const PrimRay   ray = camera.ray (e.position ());
          const PrimPlane plane =
            PrimPlane (this->step.position (), DimensionUtil::vector (camera.primaryDimension ()));
          float t;
          if (IntersectionUtil::intersects (ray, plane, &t))
          {
            intersection.update (t, ray.pointAt (t), plane.normal (), *this->mesh);
            considerHeight = false;
          }
        }

        if (intersection.isIntersection ())
        {
          this->cursor.enable ();
          this->cursor.position (intersection.position ());

          this->step.step (intersection.position (), [this, considerHeight,
                                                      &intersection](const glm::vec3& position) {
            this->mesh->addSphere (
              false, intersection.position (),
              this->newSpherePosition (considerHeight, position, intersection.normal ()),
              this->radiusEdit.doubleValue (), this->self->mirrorDimension ());

            return true;
          });
        }
      }
      return ToolResponse::Redraw;
    }
    else
    {
      if (this->self->onKeymap ('r'))
      {
        this->radiusEdit.setIntValue (this->radiusEdit.intValue () + e.delta ().x);
      }
      return this->runCursorUpdate (e.position ());
    }
  }

  ToolResponse runPressEvent (const ViewPointingEvent& e)
  {
    auto setupOnIntersection = [this](const SketchMeshIntersection& intersection) {
      this->cursor.enable ();
      this->cursor.position (intersection.position ());

      this->self->snapshotSketchMeshes ();
      this->self->mirrorPosition (intersection.mesh ().tree ().root ().data ().center ());

      this->mesh = &intersection.mesh ();
      this->step.position (intersection.position ());
    };

    if (e.leftButton ())
    {
      if (e.modifiers () == Qt::ShiftModifier)
      {
        SketchPathIntersection intersection;
        if (this->self->intersectsScene (e, intersection))
        {
          setupOnIntersection (intersection);

          this->mesh->smoothPath (
            intersection.path (),
            PrimSphere (intersection.position (), this->radiusEdit.doubleValue ()), 1,
            this->smoothEffect, this->self->mirrorDimension ());
        }
      }
      else
      {
        SketchMeshIntersection intersection;
        if (this->self->intersectsScene (e, intersection))
        {
          setupOnIntersection (intersection);

          const glm::vec3 spherePosition =
            this->newSpherePosition (true, intersection.position (), intersection.normal ());

          this->mesh->addSphere (true, intersection.position (), spherePosition,
                                 this->radiusEdit.doubleValue (), this->self->mirrorDimension ());
        }
        else
        {
          this->cursor.disable ();
        }
      }
      return ToolResponse::Redraw;
    }
    else
    {
      return ToolResponse::None;
    }
  }

  ToolResponse runReleaseEvent (const ViewPointingEvent&) { return this->runCommit (); }

  ToolResponse runCursorUpdate (const glm::ivec2& pos)
  {
    SketchMeshIntersection intersection;
    if (this->self->intersectsScene (pos, intersection))
    {
      this->cursor.enable ();
      this->cursor.position (intersection.position ());
    }
    else
    {
      this->cursor.disable ();
    }
    return ToolResponse::Redraw;
  }

  ToolResponse runCommit ()
  {
    this->mesh = nullptr;
    return ToolResponse::None;
  }

  void runFromConfig ()
  {
    const Config& config = this->self->config ();

    this->cursor.color (config.get<Color> ("editor/tool/cursor-color"));
    this->stepWidthFactor = config.get<float> ("editor/tool/sketch-spheres/step-width-factor");
  }
};

DELEGATE_TOOL (ToolSketchSpheres)
DELEGATE_TOOL_RUN_RENDER (ToolSketchSpheres)
DELEGATE_TOOL_RUN_MOVE_EVENT (ToolSketchSpheres)
DELEGATE_TOOL_RUN_PRESS_EVENT (ToolSketchSpheres)
DELEGATE_TOOL_RUN_RELEASE_EVENT (ToolSketchSpheres)
DELEGATE_TOOL_RUN_CURSOR_UPDATE (ToolSketchSpheres)
DELEGATE_TOOL_RUN_COMMIT (ToolSketchSpheres)
DELEGATE_TOOL_RUN_FROM_CONFIG (ToolSketchSpheres)
