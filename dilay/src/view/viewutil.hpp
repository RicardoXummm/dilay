/* This file is part of Dilay
 * Copyright © 2015-2018 Alexander Bau
 * Use and redistribute under the terms of the GNU General Public License
 */
#ifndef VIEW_UTIL
#define VIEW_UTIL

#include <functional>
#include <glm/fwd.hpp>
#include <vector>

class ViewDoubleSlider;
class ViewResolutionSlider;
class QAbstractSpinBox;
class QAction;
class QButtonGroup;
class QCheckBox;
class QDoubleSpinBox;
class QFrame;
class QKeySequence;
class QLineEdit;
class QMenu;
class QPoint;
class QPushButton;
class QRadioButton;
class QSlider;
class QSpinBox;
class QString;
class QTabWidget;
class QToolButton;
class QWidget;

namespace ViewUtil
{
  QSpinBox&             spinBox (int, int, int, int = 1);
  QDoubleSpinBox&       spinBox (float, float, float, float);
  QPushButton&          pushButton (const QString&, bool = false);
  QToolButton&          toolButton (const QString&);
  QRadioButton&         radioButton (const QString&, bool = false);
  QCheckBox&            checkBox (const QString&, bool = false);
  QSlider&              slider (int, int, int);
  ViewDoubleSlider&     slider (unsigned short, float, float, float, unsigned short = 1);
  ViewResolutionSlider& resolutionSlider (float, float, float);
  QButtonGroup&         buttonGroup (const std::vector<QString>&);
  QFrame&               horizontalLine ();
  QWidget&              emptyWidget ();
  QLineEdit&            lineEdit (float, unsigned short = 2);
  QLineEdit&            lineEdit (float, float, float, unsigned short = 2);
  QLineEdit&            lineEdit (int);
  QLineEdit&            lineEdit (int, int, int);
  glm::uvec2            toUVec2 (const QPoint&);
  glm::ivec2            toIVec2 (const QPoint&);
  QPoint                toQPoint (const glm::uvec2&);
  QPoint                toQPoint (const glm::ivec2&);
  void                  connect (const QSpinBox&, const std::function<void(int)>&);
  void                  connect (const QDoubleSpinBox&, const std::function<void(double)>&);
  void                  connect (const QPushButton&, const std::function<void()>&);
  void                  connect (const QButtonGroup&, int, const std::function<void(int)>&);
  void                  connect (const QCheckBox&, const std::function<void(bool)>&);
  void                  connect (const QRadioButton&, const std::function<void(bool)>&);
  void                  connect (const QSlider&, const std::function<void(int)>&);
  void                  connect (const ViewResolutionSlider&, const std::function<void(float)>&);
  void                  connect (const ViewDoubleSlider&, const std::function<void(float)>&);
  void                  connect (const QAction&, const std::function<void()>&);
  void                  connectFloat (const QLineEdit&, const std::function<void(float)>&);
  void                  connectInt (const QLineEdit&, const std::function<void(int)>&);
  QWidget&              stretcher (bool, bool);
  void                  select (QButtonGroup&, int);
  void                  deselect (QAbstractSpinBox&);
  void                  adjustSize (QTabWidget&);
  bool                  question (QWidget&, const QString&);
  void                  error (QWidget&, const QString&);
  void                  about (QWidget&, const QString&);
  void                  info (QWidget&, const QString&);
  QAction& addAction (QMenu&, const QString&, const QKeySequence&, const std::function<void()>&);
  QAction& addCheckableAction (QMenu&, const QString&, const QKeySequence&, bool,
                               const std::function<void(bool)>&);
};

#endif
