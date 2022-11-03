#include <geometry/Sphere.h>

#include <cmath>

namespace geometry {

#define EPSILON (1e-8)

Sphere::Sphere(Point3D center, Coord radius)
    : m_center(std::move(center)),
      m_radius(std::move(radius)),
      m_radius2(m_radius * m_radius) {}

Coord Sphere::intersect(Ray const& ray) const {
  Vector3D dist = ray.start - m_center;
  Coord b = (dist * ray.direction) * 2.0;
  Coord a = ray.direction * ray.direction;
  Coord c = dist * dist - m_radius2;

  Coord discr = b * b - 4.0 * a * c;
  if (discr < 0.0) return -1.0;
  Coord sqrt_discr = std::sqrt(discr);
  Coord t1 = (-b + sqrt_discr) / 2.0 / a;
  Coord t2 = (-b - sqrt_discr) / 2.0 / a;

  if (t1 < EPSILON) {
    if (t2 < EPSILON) return -1.0;
    return t2;
  }

  if (t2 < EPSILON) return t1;

  return t1 < t2 ? t1 : t2;
}

geometry::Normal3D Sphere::normal(geometry::Point3D const& x) const {
  if (m_radius < 0) return m_center - x;
  return x - m_center;
}

}  // namespace geometry