#pragma once

#include <color/Image.h>
#include <geometry/Point2D.h>

#include <string>
#include <vector>

namespace modelling {

class Texture {
 public:
  enum Interpolation { Nearest = 1, Bilinear = 2 };

 public:
  Texture(std::string filename, Interpolation interpolation = Bilinear);
  color::SColor const get(geometry::Point2D const& p) const;

 private:
  std::vector<color::SColor> values;
  color::ImageSize size;
  Interpolation m_interpolation;
};

}  // namespace modelling