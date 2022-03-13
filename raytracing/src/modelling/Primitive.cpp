#include <geometry/Sphere.h>
#include <geometry/Triangle.h>
#include <modelling/Primitive.h>

namespace modelling {

Primitive::Primitive(std::unique_ptr<geometry::Surface> surface,
                     std::shared_ptr<Material> material)
    : m_surface(std::move(surface)), m_material(std::move(material)) {}

geometry::Coord Primitive::intersect(geometry::Ray const& ray) const {
  return m_surface->intersect(ray);
}

color::SColor Primitive::BRDF(geometry::Normal3D const& L,
                              geometry::Normal3D const& N,
                              geometry::Normal3D const& V,
                              geometry::Point3D const& x) const {
  geometry::Point2D uv =
      m_material->requiresUV() ? getUV(x) : geometry::Point2D{0.0, 0.0};
  return m_material->BRDF(L, N, V, uv);
}

geometry::Normal3D Primitive::normal(geometry::Point3D const& x) const {
  return m_surface->normal(x);
}

Reflection Primitive::reflection(geometry::Normal3D const& N,
                                 geometry::Normal3D const& V,
                                 geometry::Point3D const& x) const {
  geometry::Point2D uv =
      m_material->requiresUV() ? getUV(x) : geometry::Point2D{0.0, 0.0};
  return m_material->reflection(N, V, uv);
}

color::SColor Primitive::transparency() const {
  return m_material->transparency();
}

Sphere::Sphere(geometry::Point3D const& center, geometry::Coord const& radius,
               std::shared_ptr<Material> material)
    : Primitive(std::make_unique<geometry::Sphere>(center, radius),
                std::move(material)) {}

geometry::Point2D Sphere::getUV(geometry::Point3D const&) const {
  return geometry::Point2D{0, 0};
}

Triangle::Triangle(geometry::Point3D p1, geometry::Point3D p2,
                   geometry::Point3D p3, std::shared_ptr<Material> material,
                   geometry::Point2D uv1, geometry::Point2D uv2,
                   geometry::Point2D uv3)
    : Primitive(std::make_unique<geometry::Triangle>(p1, p2, p3),
                std::move(material)),
      m_uv1(uv1),
      m_uv2(uv2),
      m_uv3(uv3) {
  geometry::Matrix<3, 3> M{
      {{p1.x, p1.y, p1.z}, {p2.x, p2.y, p2.z}, {p3.x, p3.y, p3.z}}};
  geometry::Matrix<3, 1> U{{{m_uv1.x}, {m_uv2.x}, {m_uv3.x}}};
  geometry::Matrix<3, 1> V{{{m_uv1.y}, {m_uv2.y}, {m_uv3.y}}};

  geometry::Matrix<3, 3> Minv = M.inv();

  geometry::Matrix<3, 1> ABC = Minv * U;
  geometry::Matrix<3, 1> DEF = Minv * V;

  m_uvMap =
      geometry::Matrix<2, 3>{{{ABC.values[0][0], ABC.values[1][0], ABC.values[2][0]},
                              {DEF.values[0][0], DEF.values[1][0], DEF.values[2][0]}}};
}

geometry::Point2D Triangle::getUV(geometry::Point3D const& x) const {
  return m_uvMap * x;
}

}  // namespace modelling