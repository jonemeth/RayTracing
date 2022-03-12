#pragma once

#include <color/Spectrum.h>
#include <geometry/Point3D.h>

namespace modelling {

struct Reflection {
  color::Intensity prob;
  geometry::Normal3D dir;
  color::SColor color;
};

using Reflections = std::vector<Reflection>;

class Material {
 public:
  virtual ~Material();

  virtual color::SColor BRDF(geometry::Normal3D const& L,
                             geometry::Normal3D const& N,
                             geometry::Normal3D const& V) const = 0;

  virtual Reflection reflection(geometry::Normal3D const& N,
                                geometry::Normal3D const& V) const = 0;
  virtual Reflections reflections(geometry::Normal3D const& N,
                                geometry::Normal3D const& V, size_t n) const;

  virtual color::SColor transparency() const;
};

class DiffuseMaterial : virtual public Material {
 public:
  DiffuseMaterial(color::SColor spectrum);

  color::SColor BRDF(geometry::Normal3D const&, geometry::Normal3D const&,
                     geometry::Normal3D const&) const override;

  color::Intensity averageAlbedo() const;

  Reflection reflection(geometry::Normal3D const& N,
                        geometry::Normal3D const& V) const override;

 protected:
  color::SColor m_spectrum;
};

class SpecularMaterial : virtual public Material {
 public:
  SpecularMaterial(color::SColor spectrum, color::Intensity shine);

  color::SColor BRDF(geometry::Normal3D const& L, geometry::Normal3D const& N,
                     geometry::Normal3D const& V) const override;

  color::Intensity averageAlbedo() const;

  Reflection reflection(geometry::Normal3D const& N,
                        geometry::Normal3D const& V) const override;

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
                     geometry::Normal3D const&) const override;

  Reflection reflection(geometry::Normal3D const& N,
                        geometry::Normal3D const& V) const override;

  Reflections reflections(geometry::Normal3D const& N,
                        geometry::Normal3D const& V, size_t n) const override;

 private:
  color::SColor m_Kr;
};

class IdealRefractor : virtual public Material {
 public:
  IdealRefractor(color::SColor kt, color::Intensity N);

  color::SColor& kt();
  color::SColor const& kt() const;

  color::SColor BRDF(geometry::Normal3D const&, geometry::Normal3D const&,
                     geometry::Normal3D const&) const override;

  Reflection reflection(geometry::Normal3D const& N,
                        geometry::Normal3D const& V) const override;

  Reflections reflections(geometry::Normal3D const& N,
                        geometry::Normal3D const& V, size_t n) const override;

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
                  color::Intensity N);

  color::SColor BRDF(geometry::Normal3D const& L, geometry::Normal3D const& N,
                     geometry::Normal3D const& V) const override;

  Reflection reflection(geometry::Normal3D const& N,
                        geometry::Normal3D const& V) const override;

  Reflections reflections(geometry::Normal3D const& N,
                        geometry::Normal3D const& V, size_t n) const override;
  color::SColor transparency() const override;
};

}  // namespace modelling