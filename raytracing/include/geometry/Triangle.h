#pragma once

#include <geometry/Point3D.h>
#include <geometry/Surface.h>

namespace geometry {

class Triangle : public Surface {
 public:
  Triangle(Point3D p1, Point3D p2, Point3D p3);
  Coord intersect(Ray const& ray) override;
  geometry::Normal3D normal(geometry::Point3D const&) override;

 private:
  Point3D m_p1, m_p2, m_p3;
  Normal3D m_normal;
  Vector3D m_A, m_B, m_C;
};

}  // namespace geometry