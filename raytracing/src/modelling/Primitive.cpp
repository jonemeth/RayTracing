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
                              geometry::Normal3D const& V) const {
  return m_material->BRDF(L, N, V);
}

geometry::Normal3D Primitive::normal(geometry::Point3D const& x) const {
  return m_surface->normal(x);
}

Reflection Primitive::reflection(geometry::Normal3D const& N,
                                 geometry::Normal3D const& V) const {
  return m_material->reflection(N, V);
}

Reflections Primitive::reflections(geometry::Normal3D const& N,
                                 geometry::Normal3D const& V, size_t n) const {
  return m_material->reflections(N, V, n);
}

color::SColor Primitive::transparency() const { return m_material->transparency(); }

Sphere::Sphere(geometry::Point3D const& center, geometry::Coord const& radius,
               std::shared_ptr<Material> material)
    : Primitive(std::make_unique<geometry::Sphere>(center, radius),
                std::move(material)) {}

Triangle::Triangle(geometry::Point3D p1, geometry::Point3D p2,
                   geometry::Point3D p3, std::shared_ptr<Material> material)
    : Primitive(std::make_unique<geometry::Triangle>(p1, p2, p3),
                std::move(material)) {}

}  // namespace modelling