/* This file is part of Dilay
 * Copyright © 2015-2018 Alexander Bau
 * Use and redistribute under the terms of the GNU General Public License
 */
#include <QButtonGroup>
#include <QGridLayout>
#include <QLabel>
#include <QRadioButton>
#include <QVBoxLayout>
#include "view/two-column-grid.hpp"
#include "view/viewutil.hpp"

struct ViewTwoColumnGrid::Impl
{
  ViewTwoColumnGrid* self;
  QGridLayout&       layout;
  unsigned int       numRows;

  Impl (ViewTwoColumnGrid* s)
    : self (s)
    , layout (*new QGridLayout (this->self))
    , numRows (0)
  {
    this->layout.setColumnStretch (0, 0);
    this->layout.setColumnStretch (1, 1);
  }

  void setEqualColumnStretch ()
  {
    this->layout.setColumnStretch (0, 1);
    this->layout.setColumnStretch (1, 1);
  }

  void add (QWidget& widget)
  {
    this->layout.addWidget (&widget, this->numRows, 0, 1, 2);
    this->layout.setRowStretch (this->numRows, 0);
    this->numRows++;
  }

  void add (const QString& labelLeft, const QString& labelRight)
  {
    this->add (*new QLabel (labelLeft), *new QLabel (labelRight));
  }

  void add (const QString& label, QWidget& widget) { this->add (*new QLabel (label), widget); }

  void add (QWidget& w1, QWidget& w2)
  {
    this->layout.addWidget (&w1, this->numRows, 0);
    this->layout.addWidget (&w2, this->numRows, 1);
    this->layout.setRowStretch (this->numRows, 0);
    this->numRows++;
  }

  void add (QButtonGroup& group)
  {
    for (QAbstractButton* button : group.buttons ())
    {
      this->add (*button);
    }
  }

  void addStacked (const QString& labelText, QWidget& widget)
  {
    QVBoxLayout& stack = *new QVBoxLayout;
    QLabel&      label = *new QLabel (labelText);

    label.setAlignment (Qt::AlignHCenter);
    stack.addWidget (&label);
    stack.addWidget (&widget);

    this->layout.addLayout (&stack, this->numRows, 0, 1, 2);
    this->layout.setRowStretch (this->numRows, 0);
    this->numRows++;
  }

  void addStacked (const QString& labelText, QButtonGroup& group)
  {
    QVBoxLayout& stack = *new QVBoxLayout;
    QLabel&      label = *new QLabel (labelText);

    label.setAlignment (Qt::AlignHCenter);
    stack.addWidget (&label);

    for (QAbstractButton* button : group.buttons ())
    {
      stack.addWidget (button);
    }

    this->layout.addLayout (&stack, this->numRows, 0, 1, 2);
    this->layout.setRowStretch (this->numRows, 0);
    this->numRows++;
  }

  void addLeft (const QString& label) { this->add (label, *new QWidget); }

  void addCenter (const QString& labelText)
  {
    QLabel& label = *new QLabel (labelText);
    label.setAlignment (Qt::AlignHCenter);

    this->add (label);
  }

  void addStretcher ()
  {
    this->add (*new QWidget, *new QWidget);
    this->layout.setRowStretch (this->numRows, 1);
  }

  void addSeparator () { this->add (ViewUtil::horizontalLine ()); }

  void reset ()
  {
    QWidget* widget = nullptr;
    while ((widget = this->self->findChild<QWidget*> ()))
    {
      delete widget;
    }
    this->numRows = 0;
  }
};

ViewTwoColumnGrid::ViewTwoColumnGrid (QWidget* p) : QWidget (p), impl (new Impl (this)) {
} ViewTwoColumnGrid::~ViewTwoColumnGrid ( ) {
}
unsigned int ViewTwoColumnGrid::numRows ( ) const {
	return this->impl->numRows;
}
void ViewTwoColumnGrid::setEqualColumnStretch ( ) {
	return this->impl->setEqualColumnStretch ( );
}
void ViewTwoColumnGrid::add (QWidget& a1) {
	return this->impl->add (std::forward<QWidget&> (a1));
}
void ViewTwoColumnGrid::add (const QString& a1, const QString& a2) {
	return this->impl->add (std::forward<const QString&> (a1), std::forward<const QString&> (a2));
}
void ViewTwoColumnGrid::add (const QString& a1, QWidget& a2) {
	return this->impl->add (std::forward<const QString&> (a1), std::forward<QWidget&> (a2));
}
void ViewTwoColumnGrid::add (QWidget& a1, QWidget& a2) {
	return this->impl->add (std::forward<QWidget&> (a1), std::forward<QWidget&> (a2));
}
void ViewTwoColumnGrid::add (QButtonGroup& a1) {
	return this->impl->add (std::forward<QButtonGroup&> (a1));
}
void ViewTwoColumnGrid::addStacked (const QString& a1, QWidget& a2) {
	return this->impl->addStacked (std::forward<const QString&> (a1), std::forward<QWidget&> (a2));
}
void ViewTwoColumnGrid::addStacked (const QString& a1, QButtonGroup& a2) {
	return this->impl->addStacked (std::forward<const QString&> (a1), std::forward<QButtonGroup&> (a2));
}
void ViewTwoColumnGrid::addLeft (const QString& a1) {
	return this->impl->addLeft (std::forward<const QString&> (a1));
}
void ViewTwoColumnGrid::addCenter (const QString& a1) {
	return this->impl->addCenter (std::forward<const QString&> (a1));
}
void ViewTwoColumnGrid::addStretcher ( ) {
	return this->impl->addStretcher ( );
}
void ViewTwoColumnGrid::addSeparator ( ) {
	return this->impl->addSeparator ( );
}
void ViewTwoColumnGrid::reset ( ) {
	return this->impl->reset ( );
}
