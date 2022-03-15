#pragma once

#include <geometry/Matrix.h>
#include <geometry/Point3D.h>
#include <geometry/Surface.h>

namespace geometry {

class Sphere : virtual public Surface {
 public:
  Sphere(Point3D center, Coord radius);
  Coord intersect(Ray const& ray) const override;
  geometry::Normal3D normal(geometry::Point3D const& x) const override;

 protected:
  Point3D m_center;
  Coord m_radius, m_radius2;
};

}  // namespace geometry