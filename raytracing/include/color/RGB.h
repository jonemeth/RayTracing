#pragma once

#include <color/Spectrum.h>

#include <iostream>

namespace color {

#define LAMBDALOW (392)
#define LAMBDAHIGH (689)

void ColorMatch(Lambda lambda, Intensity &r, Intensity &g,
                Intensity &b);

struct RGB {
  RGB(Intensity r0, Intensity g0, Intensity b0) {
    r = r0;
    g = g0;
    b = b0;
  }

  template <size_t nLambdas>
  RGB(Spectrum<nLambdas> const &spectrum) {
    this->r = 0.0;
    this->g = 0.0;
    this->b = 0.0;

    Lambda prevLambda = LAMBDALOW;
    Lambdas const &lambdas = spectrum.lambdas();
    Lambdas const &intensities = spectrum.intensities();

    for (size_t i = 0; i < lambdas.size(); ++i) {
      Intensity r, g, b;
      Lambda dl;
      ColorMatch(lambdas[i], r, g, b);
      dl = (lambdas[i] - prevLambda) / (LAMBDAHIGH - LAMBDALOW);
      this->r += r * intensities[i] * dl;
      this->g += g * intensities[i] * dl;
      this->b += b * intensities[i] * dl;
      prevLambda = lambdas[i];
    }
    this->r = std::max(color::Intensity(0.0), std::min(color::Intensity(1.0), this->r));
    this->g = std::max(color::Intensity(0.0), std::min(color::Intensity(1.0), this->g));
    this->b = std::max(color::Intensity(0.0), std::min(color::Intensity(1.0), this->b));
  }

  operator SColor() const {
    SColor c;
    c.intensities()[0] = b;
    c.intensities()[1] = g;
    c.intensities()[2] = r;
    return c;    
  }

  Intensity r, g, b;
};



inline std::ostream &operator<<(std::ostream &os, RGB const &rgb) {
  os << "[" << rgb.r << "," << rgb.g << "," << rgb.b << "]";
  return os;
}

}  // namespace color