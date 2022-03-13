#pragma once

#include <color/Spectrum.h>
#include <geometry/Point3D.h>
#include <modelling/Texture.h>

#include <memory>

namespace modelling {

struct Reflection {
  color::Intensity prob;
  geometry::Normal3D dir;
  color::SColor color;
};

class Material {
 public:
  virtual ~Material();

  virtual color::SColor BRDF(geometry::Normal3D const& L,
                             geometry::Normal3D const& N,
                             geometry::Normal3D const& V,
                             geometry::Point2D const& uv) const = 0;

  virtual Reflection reflection(geometry::Normal3D const& N,
                                geometry::Normal3D const& V, geometry::Point2D const& uv) const = 0;

  virtual color::SColor transparency() const;

  virtual bool requiresUV() const;
};

class DiffuseMaterial : virtual public Material {
 public:
  DiffuseMaterial(color::SColor spectrum,
                  std::shared_ptr<modelling::Texture> texture = nullptr);

  color::SColor BRDF(geometry::Normal3D const&, geometry::Normal3D const&,
                     geometry::Normal3D const&, geometry::Point2D const& uv) const override;

  color::Intensity averageAlbedo() const;

  Reflection reflection(geometry::Normal3D const& N,
                        geometry::Normal3D const& V, geometry::Point2D const& uv) const override;

  bool requiresUV() const override;

 protected:
  color::SColor m_spectrum;
  std::shared_ptr<modelling::Texture> m_texture;
};

class SpecularMaterial : virtual public Material {
 public:
  SpecularMaterial(color::SColor spectrum, color::Intensity shine);

  color::SColor BRDF(geometry::Normal3D const& L, geometry::Normal3D const& N,
                     geometry::Normal3D const& V, geometry::Point2D const& uv) const override;

  color::Intensity averageAlbedo() const;

  Reflection reflection(geometry::Normal3D const& N,
                        geometry::Normal3D const& V, geometry::Point2D const& uv) const override;

 protected:
  color::SColor m_spectrum;
  color::Intensity m_shine;
};

class IdealReflector : virtual public Material {
 public:
  IdealReflector(color::SColor kr);

  color::SColor& kr();
  color::SColor const& kr() const;

  color::SColor BRDF(geometry::Normal3D const&, geometry::Normal3D const&,
                     geometry::Normal3D const&, geometry::Point2D const& uv) const override;

  Reflection reflection(geometry::Normal3D const& N,
                        geometry::Normal3D const& V, geometry::Point2D const& uv) const override;

 private:
  color::SColor m_Kr;
};

class IdealRefractor : virtual public Material {
 public:
  IdealRefractor(color::SColor kt, color::Intensity N);

  color::SColor& kt();
  color::SColor const& kt() const;

  color::SColor BRDF(geometry::Normal3D const&, geometry::Normal3D const&,
                     geometry::Normal3D const&, geometry::Point2D const& uv) const override;

  Reflection reflection(geometry::Normal3D const& N,
                        geometry::Normal3D const& V, geometry::Point2D const& uv) const override;

  color::SColor transparency() const override;

 private:
  color::SColor m_Kt;
  color::Intensity m_N;
};

class GeneralMaterial : public DiffuseMaterial,
                        public SpecularMaterial,
                        public IdealReflector,
                        public IdealRefractor {
 public:
  GeneralMaterial(color::SColor diffuseColor, color::SColor specularColor,
                  color::Intensity shine, color::SColor kr, color::SColor kt,
                  color::Intensity N, std::shared_ptr<Texture> texture=nullptr);

  color::SColor BRDF(geometry::Normal3D const& L, geometry::Normal3D const& N,
                     geometry::Normal3D const& V, geometry::Point2D const& uv) const override;

  Reflection reflection(geometry::Normal3D const& N,
                        geometry::Normal3D const& V, geometry::Point2D const& uv) const override;

  color::SColor transparency() const override;
  bool requiresUV() const override;
};

}  // namespace modelling