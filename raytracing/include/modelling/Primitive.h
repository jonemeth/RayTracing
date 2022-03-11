#pragma once

#include <geometry/Point3D.h>
#include <geometry/Surface.h>
#include <modelling/Material.h>

#include <memory>

namespace modelling {

class Primitive {
 public:
  Primitive(std::unique_ptr<geometry::Surface> surface,
            std::shared_ptr<Material> material);

  geometry::Coord intersect(geometry::Ray const& ray) const;
  color::SColor BRDF(geometry::Normal3D const& L, geometry::Normal3D const& N,
                     geometry::Normal3D const& V) const;
  geometry::Normal3D normal(geometry::Point3D const& x) const;
  Reflection reflection(geometry::Normal3D const& N,
                        geometry::Normal3D const& V) const;
  Reflections reflections(geometry::Normal3D const& N,
                          geometry::Normal3D const& V, size_t n) const;
  color::SColor transparency() const;

 private:
  std::unique_ptr<geometry::Surface> m_surface;
  std::shared_ptr<Material> m_material;
};

class Sphere : public Primitive {
 public:
  Sphere(geometry::Point3D const& center, geometry::Coord const& radius,
         std::shared_ptr<Material> material);
};

class Triangle : public Primitive {
 public:
  Triangle(geometry::Point3D p1, geometry::Point3D p2, geometry::Point3D p3,
           std::shared_ptr<Material> material);
};

}  // namespace modelling