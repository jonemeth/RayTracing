#include <geometry/Triangle.h>

#include <cmath>

namespace geometry {

Triangle::Triangle(Point3D p1, Point3D p2, Point3D p3)
    : m_p1(std::move(p1)),
      m_p2(std::move(p2)),
      m_p3(std::move(p3)),
      m_normal((m_p3 - m_p1) % (m_p2 - m_p1)),
      m_A(m_p2 - m_p1),
      m_B(m_p3 - m_p2),
      m_C(m_p1 - m_p3) {}

Coord Triangle::intersect(Ray const& ray) {
  static Coord EPS = 1e-8;

  Coord denominator = m_normal * ray.direction;
  if (denominator <= EPS && denominator >= -EPS) return -1;

  Coord t = (m_normal * m_p1 - m_normal * ray.start) / denominator;

  if (t < EPS) return -1;

  geometry::Point3D p = ray.start + t * ray.direction;

  if (m_A % (p - m_p1) * m_normal > -EPS) return -1;
  if (m_B % (p - m_p2) * m_normal > -EPS) return -1;
  if (m_C % (p - m_p3) * m_normal > -EPS) return -1;

  return t;
}

geometry::Normal3D Triangle::normal(geometry::Point3D const&) {
  return m_normal;
}

}  // namespace geometry