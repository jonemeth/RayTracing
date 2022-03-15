#pragma once

#include <geometry/Point3D.h>

namespace geometry {

class Surface {
 public:
  virtual ~Surface();
  virtual Coord intersect(Ray const& ray) const = 0;
  virtual geometry::Normal3D normal(geometry::Point3D const& x) const = 0;
};

}  // namespace geometry