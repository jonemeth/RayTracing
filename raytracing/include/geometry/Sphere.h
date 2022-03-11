#pragma once

#include <geometry/Point3D.h>
#include <geometry/Surface.h>

namespace geometry {

class Sphere : public Surface {
 public:
  Sphere(Point3D center, Coord radius);
  Coord intersect(Ray const& ray) override;
  geometry::Normal3D normal(geometry::Point3D const& x) override;

 private:
  Point3D m_center;
  Coord m_radius, m_radius2;
};

}  // namespace geometry