#pragma once

#include <geometry/Matrix.h>
#include <geometry/Point3D.h>
#include <geometry/Surface.h>

namespace geometry {

class Torus : virtual public Surface {
 public:
  Torus(Coord R0, Coord r0);
  Coord intersect(Ray const& ray) const override;
  geometry::Normal3D normal(geometry::Point3D const& x) const override;

 protected:
  Coord R;
  Coord r;
};

}  // namespace geometry