/* This file is part of Dilay
 * Copyright © 2015-2018 Alexander Bau
 * Use and redistribute under the terms of the GNU General Public License
 */
#ifndef DILAY_TOOL_SCULPT
#define DILAY_TOOL_SCULPT

#include "tool.hpp"

class QString;
class SculptBrush;
class ToolUtilMovement;
class ViewCursor;
class ViewDoubleSlider;
class ViewTwoColumnGrid;

class ToolSculpt : public Tool
{
public:
	ToolSculpt (State&, const char*); ToolSculpt (const ToolSculpt&) = delete; ToolSculpt (ToolSculpt&&) = delete; const ToolSculpt& operator= (const ToolSculpt&) = delete; const ToolSculpt& operator= (ToolSculpt&&) = delete; ~ToolSculpt ( );

protected:
  SculptBrush& brush ();
  ViewCursor&  cursor ();
  void         addDefaultToolTip (ViewToolTip&, bool, bool) const;
  void         sculpt ();
  bool drawlikeStroke (const ViewPointingEvent&, bool, const std::function<void()>* = nullptr);
  bool grablikeStroke (const ViewPointingEvent&, ToolUtilMovement&);
  void registerSecondarySlider (ViewDoubleSlider&);

private:
	struct Impl; std::unique_ptr<Impl> impl;

  ToolResponse runInitialize ();
  void         runRender () const;
  ToolResponse runPointingEvent (const ViewPointingEvent&);
  ToolResponse runCursorUpdate (const glm::ivec2&);
  ToolResponse runCommit ();
  void         runFromConfig ();

  virtual void runSetupBrush (SculptBrush&) = 0;
  virtual void runSetupCursor (ViewCursor&) = 0;
  virtual void runSetupProperties (ViewTwoColumnGrid&) = 0;
  virtual void runSetupToolTip (ViewToolTip&) = 0;
  virtual bool runSculptPointingEvent (const ViewPointingEvent&) = 0;
};

#define DECLARE_TOOL_SCULPT(keyName)                        \
  class Tool##keyName : public ToolSculpt                   \
  {                                                         \
  public:                                                   \
    DECLARE_BIG2 (Tool##keyName, State&)                    \
                                                            \
    ToolKey getKey () const { return ToolKey::keyName; }    \
                                                            \
  private:                                                  \
    IMPLEMENTATION                                          \
    void runSetupBrush (SculptBrush&);                      \
    void runSetupCursor (ViewCursor&);                      \
    void runSetupProperties (ViewTwoColumnGrid&);           \
    void runSetupToolTip (ViewToolTip&);                    \
    bool runSculptPointingEvent (const ViewPointingEvent&); \
  };

#define DELEGATE_TOOL_SCULPT(name)                                       \
  DELEGATE_BIG2_BASE (name, (State & s), (this), ToolSculpt, (s, #name)) \
  DELEGATE1 (void, name, runSetupBrush, SculptBrush&);                   \
  DELEGATE1 (void, name, runSetupCursor, ViewCursor&);                   \
  DELEGATE1 (void, name, runSetupProperties, ViewTwoColumnGrid&);        \
  DELEGATE1 (void, name, runSetupToolTip, ViewToolTip&);                 \
  DELEGATE1 (bool, name, runSculptPointingEvent, const ViewPointingEvent&)

#endif
