/* This file is part of Dilay
 * Copyright © 2015-2018 Alexander Bau
 * Use and redistribute under the terms of the GNU General Public License
 */
#ifndef DILAY_VIEW_TOOL_PANE
#define DILAY_VIEW_TOOL_PANE

#include <QDockWidget>
#include "macro.hpp"

enum class ToolKey;
class QPushButton;
class ViewGlWidget;
class ViewTwoColumnGrid;

enum class ViewToolPaneSelection
{
  Sculpt,
  Sketch
};

class ViewToolPane : public QDockWidget
{
public:
	ViewToolPane (ViewGlWidget&, QWidget* = nullptr); ViewToolPane (const ViewToolPane&) = delete; ViewToolPane (ViewToolPane&&) = delete; const ViewToolPane& operator= (const ViewToolPane&) = delete; const ViewToolPane& operator= (ViewToolPane&&) = delete; ~ViewToolPane ( );

  ViewTwoColumnGrid&    properties ();
  void                  forceWidth ();
  ViewToolPaneSelection selection () const;
  void                  setButtonState (ToolKey, bool);
  QString               buttonText (ToolKey) const;

private:
  IMPLEMENTATION
};

#endif
