#pragma once

#include <geometry/Matrix.h>
#include <geometry/Point2D.h>
#include <geometry/Point3D.h>
#include <geometry/Sphere.h>
#include <geometry/Surface.h>
#include <geometry/Triangle.h>
#include <modelling/Material.h>
#include <modelling/NormalMap.h>

#include <memory>

namespace modelling {

class Primitive : virtual public geometry::Surface {
 public:
  Primitive(std::shared_ptr<Material> material,
            std::shared_ptr<NormalMap> normalMap);

  color::SColor BRDF(geometry::Normal3D const& L, geometry::Normal3D const& N,
                     geometry::Normal3D const& V,
                     geometry::Point2D const& uv) const;

  Reflection reflection(geometry::Normal3D const& N,
                        geometry::Normal3D const& V,
                        geometry::Point2D const& uv) const;

  color::SColor transparency() const;

  bool requiresUV() const;

  virtual geometry::Point2D getUV(geometry::Point3D const& x) const = 0;

  virtual geometry::Normal3D normal(geometry::Point3D const& x,
                                    geometry::Point2D const& uv) const = 0;
  using Surface::normal;

 protected:
  std::shared_ptr<Material> m_material;
  std::shared_ptr<NormalMap> m_normalMap;
};

class Sphere : public geometry::Sphere, public Primitive {
 public:
  Sphere(geometry::Point3D center, geometry::Coord radius,
         geometry::Matrix<4, 4> orientation, std::shared_ptr<Material> material,
         std::shared_ptr<NormalMap> normalMap = nullptr);

  geometry::Point2D getUV(geometry::Point3D const& x) const override;
  geometry::Normal3D normal(geometry::Point3D const& x,
                            geometry::Point2D const& uv) const override;

 private:
  geometry::Matrix<4, 4> m_orientation;
  geometry::Matrix<4, 4> m_invOrientation;
};

class Triangle : public geometry::Triangle, public Primitive {
 public:
  Triangle(geometry::Point3D p1, geometry::Point3D p2, geometry::Point3D p3,
           std::shared_ptr<Material> material,
           geometry::Point2D uv1 = geometry::Point2D{0, 0},
           geometry::Point2D uv2 = geometry::Point2D{1, 0},
           geometry::Point2D uv3 = geometry::Point2D{0, 1},
           std::shared_ptr<NormalMap> normalMap = nullptr,
           geometry::Point3D su = geometry::Point3D{1, 0, 0},
           geometry::Point3D sv = geometry::Point3D{0, 1, 0});

  geometry::Point2D getUV(geometry::Point3D const& x) const override;
  geometry::Normal3D normal(geometry::Point3D const& x,
                            geometry::Point2D const& uv) const override;

 private:
  geometry::Matrix<2, 3> m_uvMap;
  geometry::Point3D m_su, m_sv;
};

}  // namespace modelling