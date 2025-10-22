/* This file is part of Dilay
 * Copyright © 2015-2018 Alexander Bau
 * Use and redistribute under the terms of the GNU General Public License
 */
#include <glm/glm.hpp>
#include "../util.hpp"
#include "view/double-slider.hpp"

struct ViewDoubleSlider::Impl
{
  ViewDoubleSlider*    self;
  const unsigned int   factor;
  const unsigned short order;

  Impl (ViewDoubleSlider* s, unsigned short numDecimals, unsigned short o)
    : self (s)
    , factor (glm::pow (10.0f, float(numDecimals)))
    , order (o)
  {
    assert (this->order > 0);

    QObject::connect (this->self, &QSlider::valueChanged, [this](int i) {
      emit this->self->doubleValueChanged (this->toDouble (i, false));
    });
  }

  double toDouble (int value, bool forceLinear) const
  {
    const unsigned short o = forceLinear ? 1 : this->order;

    if (o == 1)
    {
      return double(value) / this->factor;
    }
    else
    {
      const double min = double(this->self->minimum ()) / this->factor;
      const double max = double(this->self->maximum ()) / this->factor;
      const double d = double(value) / this->factor;
      const double norm = (d - min) / (max - min);
      const double result = min + (glm::pow (float(norm), float(o)) * (max - min));

      return result;
    }
  }

  int toInt (double value, bool forceLinear) const
  {
    const unsigned short o = forceLinear ? 1 : this->order;

    if (o == 1)
    {
      return int(std::round (value * this->factor));
    }
    else
    {
      const double min = double(this->self->minimum ()) / this->factor;
      const double max = double(this->self->maximum ()) / this->factor;
      const double norm = (value - min) / (max - min);
      const double slope =
        norm <= Util::epsilon () ? 0.0f : glm::pow (float(norm), 1.0f / float(o));

      return int(std::round (this->factor * (min + (slope * (max - min)))));
    }
  }

  double doubleValue () const { return this->toDouble (this->self->value (), false); }

  double doubleSingleStep () const { return this->toDouble (this->self->singleStep (), true); }

  void setDoubleValue (double v) { this->self->setValue (this->toInt (v, false)); }

  void setDoubleRange (double min, double max)
  {
    this->self->setRange (this->toInt (min, true), this->toInt (max, true));
  }

  void setDoubleSingleStep (double v) { this->self->setSingleStep (this->toInt (v, true)); }

  void setDoublePageStep (double v) { this->self->setPageStep (this->toInt (v, true)); }

  int intValue () const { return this->self->QSlider::value (); }

  int intSingleStep () const { return this->self->QSlider::singleStep (); }

  void setIntValue (int v) { this->self->QSlider::setValue (v); }

  void setIntRange (int min, int max) { this->self->QSlider::setRange (min, max); }

  void setIntSingleStep (int v) { this->self->QSlider::setSingleStep (v); }

  void setIntPageStep (int v) { this->self->QSlider::setPageStep (v); }

  int value () const { return this->intValue (); }

  int singleStep () const { return this->intSingleStep (); }

  void setValue (int v) { this->setIntValue (v); }

  void setRange (int min, int max) { this->setIntRange (min, max); }

  void setSingleStep (int v) { this->setIntSingleStep (v); }

  void setPageStep (int v) { this->setIntPageStep (v); }
};

ViewDoubleSlider::ViewDoubleSlider (unsigned short a1, unsigned short a2) : impl (new Impl (this, a1, a2)) {
} ViewDoubleSlider::~ViewDoubleSlider ( ) {
}
double ViewDoubleSlider::doubleValue ( ) const {
	return this->impl->doubleValue ( );
}
double ViewDoubleSlider::doubleSingleStep ( ) const {
	return this->impl->doubleSingleStep ( );
}
void ViewDoubleSlider::setDoubleValue (double a1) {
	return this->impl->setDoubleValue (std::forward<double> (a1));
}
void ViewDoubleSlider::setDoubleRange (double a1, double a2) {
	return this->impl->setDoubleRange (std::forward<double> (a1), std::forward<double> (a2));
}
void ViewDoubleSlider::setDoubleSingleStep (double a1) {
	return this->impl->setDoubleSingleStep (std::forward<double> (a1));
}
void ViewDoubleSlider::setDoublePageStep (double a1) {
	return this->impl->setDoublePageStep (std::forward<double> (a1));
}
int ViewDoubleSlider::intValue ( ) const {
	return this->impl->intValue ( );
}
int ViewDoubleSlider::intSingleStep ( ) const {
	return this->impl->intSingleStep ( );
}
void ViewDoubleSlider::setIntValue (int a1) {
	return this->impl->setIntValue (std::forward<int> (a1));
}
void ViewDoubleSlider::setIntRange (int a1, int a2) {
	return this->impl->setIntRange (std::forward<int> (a1), std::forward<int> (a2));
}
void ViewDoubleSlider::setIntSingleStep (int a1) {
	return this->impl->setIntSingleStep (std::forward<int> (a1));
}
void ViewDoubleSlider::setIntPageStep (int a1) {
	return this->impl->setIntPageStep (std::forward<int> (a1));
}
int ViewDoubleSlider::value ( ) const {
	return this->impl->value ( );
}
int ViewDoubleSlider::singleStep ( ) const {
	return this->impl->singleStep ( );
}
void ViewDoubleSlider::setValue (int a1) {
	return this->impl->setValue (std::forward<int> (a1));
}
void ViewDoubleSlider::setRange (int a1, int a2) {
	return this->impl->setRange (std::forward<int> (a1), std::forward<int> (a2));
}
void ViewDoubleSlider::setSingleStep (int a1) {
	return this->impl->setSingleStep (std::forward<int> (a1));
}
void ViewDoubleSlider::setPageStep (int a1) {
	return this->impl->setPageStep (std::forward<int> (a1));
}
