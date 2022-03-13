#pragma once

#include <geometry/Point2D.h>
#include <geometry/Point3D.h>
#include <geometry/Surface.h>
#include <geometry/Matrix.h>
#include <modelling/Material.h>

#include <memory>

namespace modelling {

class Primitive {
 public:
  Primitive(std::unique_ptr<geometry::Surface> surface,
            std::shared_ptr<Material> material);

  geometry::Coord intersect(geometry::Ray const& ray) const;
  color::SColor BRDF(geometry::Normal3D const& L, geometry::Normal3D const& N,
                     geometry::Normal3D const& V,
                     geometry::Point3D const& x) const;
  geometry::Normal3D normal(geometry::Point3D const& x) const;
  Reflection reflection(geometry::Normal3D const& N,
                        geometry::Normal3D const& V, geometry::Point3D const& x) const;

  color::SColor transparency() const;

  virtual geometry::Point2D getUV(geometry::Point3D const& x) const = 0;

 private:
  std::unique_ptr<geometry::Surface> m_surface;
  std::shared_ptr<Material> m_material;
};

class Sphere : public Primitive {
 public:
  Sphere(geometry::Point3D const& center, geometry::Coord const& radius,
         std::shared_ptr<Material> material);
  geometry::Point2D getUV(geometry::Point3D const& x) const;
};

class Triangle : public Primitive {
 public:
  Triangle(geometry::Point3D p1, geometry::Point3D p2, geometry::Point3D p3,
           std::shared_ptr<Material> material,
           geometry::Point2D uv1 = geometry::Point2D{0, 0},
           geometry::Point2D uv2 = geometry::Point2D{1, 0},
           geometry::Point2D uv3 = geometry::Point2D{0, 1});
  geometry::Point2D getUV(geometry::Point3D const& x) const;

 private:
  geometry::Point2D m_uv1, m_uv2, m_uv3;
  geometry::Matrix<2, 3> m_uvMap;
};

}  // namespace modelling