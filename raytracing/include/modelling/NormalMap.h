#pragma once

#include <color/Image.h>
#include <geometry/Point2D.h>
#include <geometry/Point3D.h>

#include <string>
#include <vector>

namespace modelling {

class NormalMap {
 public:
  enum Interpolation { Nearest = 1, Bilinear = 2 };

 public:
  NormalMap(std::string filename, Interpolation interpolation = Bilinear);
  geometry::Vector3D const get(geometry::Point2D const& p) const;

 private:
  std::vector<geometry::Vector3D> values;
  color::ImageSize size;
  Interpolation m_interpolation;
};

}  // namespace modelling