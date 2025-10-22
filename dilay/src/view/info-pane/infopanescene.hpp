/* This file is part of Dilay
 * Copyright © 2015-2018 Alexander Bau
 * Use and redistribute under the terms of the GNU General Public License
 */
#ifndef DILAY_VIEW_INFO_PANE_SCENE
#define DILAY_VIEW_INFO_PANE_SCENE

#include <QWidget>
#include "macro.hpp"

class ViewMainWindow;

class ViewInfoPaneScene : public QWidget
{
public:
  DECLARE_BIG2 (ViewInfoPaneScene, ViewMainWindow&, QWidget* = nullptr)

  void updateInfo ();

private:
  IMPLEMENTATION
};

#endif
