/* This file is part of Dilay
 * Copyright © 2015-2018 Alexander Bau
 * Use and redistribute under the terms of the GNU General Public License
 */
#ifndef DILAY_STATE
#define DILAY_STATE

#include <initializer_list>
#include "macro.hpp"

class Cache;
class Camera;
class Config;
class History;
class Id;
class Mesh;
class Scene;
class Tool;
enum class ToolKey;
enum class ToolResponse;
class ViewMainWindow;
class ViewShortcut;
class ViewToolTip;

class State
{
public:
  typedef std::initializer_list<ViewShortcut> ViewShortcuts;

  DECLARE_BIG2 (State, ViewMainWindow&, Config&, Cache&)

  ViewMainWindow& mainWindow ();
  Config&         config ();
  Cache&          cache ();
  Camera&         camera ();
  History&        history ();
  Scene&          scene ();
  bool            hasTool ();
  Tool&           tool ();
  void            setTool (ToolKey);
  void            setPreviousTool ();
  void            setToolTip (const ViewToolTip*, const ViewShortcuts&);
  void            setToolTip (const ViewToolTip*);
  void            resetTool ();
  void            fromConfig ();
  void            undo ();
  void            redo ();

  void handleToolResponse (ToolResponse);

private:
  IMPLEMENTATION
};
#endif
