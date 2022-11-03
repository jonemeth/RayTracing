
#include <modelling/Primitive.h>

namespace modelling {

Primitive::Primitive(std::shared_ptr<Material> material,
                     std::shared_ptr<NormalMap> normalMap)
    : m_material(std::move(material)), m_normalMap(std::move(normalMap)) {}

color::SColor Primitive::BRDF(geometry::Normal3D const& L,
                              geometry::Normal3D const& N,
                              geometry::Normal3D const& V,
                              geometry::Point2D const& uv) const {
  return m_material->BRDF(L, N, V, uv);
}

Reflection Primitive::reflection(geometry::Normal3D const& N,
                                 geometry::Normal3D const& V,
                                 geometry::Point2D const& uv) const {
  return m_material->reflection(N, V, uv);
}

color::SColor Primitive::transparency() const {
  return m_material->transparency();
}

bool Primitive::requiresUV() const {
  return m_material->requiresUV() || m_normalMap != nullptr;
}

geometry::Normal3D Primitive::normal(geometry::Point3D const& x,
                                     geometry::Point2D const&) const {
  return normal(x);
}

/**
 * @brief Construct a new Sphere:: Sphere object
 *
 * @param center
 * @param radius
 * @param orientation
 * @param material
 * @param normalMap
 */
Sphere::Sphere(geometry::Point3D center, geometry::Coord radius,
               geometry::Matrix<4, 4> orientation,
               std::shared_ptr<Material> material,
               std::shared_ptr<NormalMap> normalMap)
    : geometry::Sphere(center, radius),
      Primitive(std::move(material), std::move(normalMap)),
      m_orientation(std::move(orientation)),
      m_invOrientation(m_orientation.inv()) {}

geometry::Point2D Sphere::getUV(geometry::Point3D const& x) const {
  geometry::Point3D p = m_invOrientation * ((x - m_center) / std::abs(m_radius));
  geometry::Coord u = std::atan2(p.x, p.z) / (2 * M_PI) + 0.5;
  geometry::Coord v =
      std::atan2(p.y, std::sqrt(p.x * p.x + p.z * p.z)) / M_PI + 0.5;
  return geometry::Point2D{u, 1.0 - v};
}

geometry::Normal3D Sphere::normal(geometry::Point3D const& x,
                                  geometry::Point2D const& uv) const {
  geometry::Normal3D n = geometry::Sphere::normal(x);
  if (m_normalMap) {
    geometry::Vector3D d = m_normalMap->get(uv);
    geometry::Normal3D u = n % geometry::Vector3D{0, -1, 0};
    geometry::Normal3D v = n % u;
    n = geometry::Normal3D(u * d.x + v * d.y + n * d.z);
  }

  return n;
}

static geometry::Matrix<2, 3> computeLinearUVMap(
    geometry::Point3D p1, geometry::Point3D p2, geometry::Point3D p3,
    geometry::Point2D uv1, geometry::Point2D uv2, geometry::Point2D uv3) {
  geometry::Matrix<3, 3> M{
      {{p1.x, p1.y, p1.z}, {p2.x, p2.y, p2.z}, {p3.x, p3.y, p3.z}}};
  geometry::Matrix<3, 1> U{{{uv1.x}, {uv2.x}, {uv3.x}}};
  geometry::Matrix<3, 1> V{{{uv1.y}, {uv2.y}, {uv3.y}}};

  geometry::Matrix<3, 3> Minv = M.inv();

  geometry::Matrix<3, 1> ABC = Minv * U;
  geometry::Matrix<3, 1> DEF = Minv * V;

  return geometry::Matrix<2, 3>{
      {{ABC.values[0][0], ABC.values[1][0], ABC.values[2][0]},
       {DEF.values[0][0], DEF.values[1][0], DEF.values[2][0]}}};
}

/**
 * @brief Construct a new Triangle:: Triangle object
 *
 * @param p1
 * @param p2
 * @param p3
 * @param material
 * @param uv1
 * @param uv2
 * @param uv3
 * @param normalMap
 * @param su
 * @param sv
 */
Triangle::Triangle(geometry::Point3D p1, geometry::Point3D p2,
                   geometry::Point3D p3, std::shared_ptr<Material> material,
                   geometry::Point2D uv1, geometry::Point2D uv2,
                   geometry::Point2D uv3, std::shared_ptr<NormalMap> normalMap,
                   geometry::Point3D su, geometry::Point3D sv)
    : geometry::Triangle(p1, p2, p3),
      Primitive(std::move(material), std::move(normalMap)),
      m_uvMap(computeLinearUVMap(p1, p2, p3, uv1, uv2, uv3)),
      m_su(su),
      m_sv(sv) {}

geometry::Point2D Triangle::getUV(geometry::Point3D const& x) const {
  return m_uvMap * x;
}

geometry::Normal3D Triangle::normal(geometry::Point3D const& x,
                                    geometry::Point2D const& uv) const {
  geometry::Normal3D n = geometry::Triangle::normal(x);

  if (!m_normalMap) return n;

  geometry::Vector3D d = m_normalMap->get(uv);
  return geometry::Normal3D(m_su * d.x + m_sv * d.y + n * d.z);
}

Torus::Torus(geometry::Coord R, geometry::Coord r, geometry::Matrix<4, 4> view,
             std::shared_ptr<Material> material,
             std::shared_ptr<NormalMap> normalMap)
    : geometry::Torus(R, r),
      Primitive(std::move(material), std::move(normalMap)),
      m_view(std::move(view)),
      m_invView(m_view.inv()) {}

geometry::Coord Torus::intersect(geometry::Ray const& ray) const {
  geometry::Ray invRay = m_invView * ray;
  geometry::Coord t = geometry::Torus::intersect(invRay);
  if (t <= 0.0) return t;
  return (m_view * (invRay.start + t * invRay.direction) - ray.start).length();
}

geometry::Point2D Torus::getUV(geometry::Point3D const& x) const {
  geometry::Point3D p = m_invView * x;

  geometry::Coord u = std::atan2(p.y, p.x) / (2 * M_PI) + 0.5;
  geometry::Coord z = 0.9999 * std::max(-1.0, std::min(1.0, -p.z / r));
  geometry::Coord v = std::asin(z) / (M_PI * 2) + 0.5;  // 0.25 .. 0.75

  if (p.x * p.x + p.y * p.y < R * R) {
    if (v > 0.5)
      v = 1.5 - v;
    else
      v = 0.5 - v;
  }

  return geometry::Point2D{u, v};
}

geometry::Normal3D Torus::normal(geometry::Point3D const& x,
                                 geometry::Point2D const& uv) const {
  geometry::Point3D p = m_invView * x;
  geometry::Normal3D n = geometry::Torus::normal(p);

  if (m_normalMap) {
    geometry::Vector3D d = m_normalMap->get(uv);
    geometry::Normal3D u = n % geometry::Vector3D{0, 0, -1};
    if (p.x * p.x + p.y * p.y < R * R) u = -u;
    geometry::Normal3D v = n % u;

    n = geometry::Normal3D(u * d.x + v * d.y + n * d.z);
    // std::cout << uv.x << " " << uv.y << " " << d << " " << u << " " << v <<
    // " " << n << std::endl;
  }
  // std::cout << n.length() << std::endl;
  return m_view * n;
}

}  // namespace modelling