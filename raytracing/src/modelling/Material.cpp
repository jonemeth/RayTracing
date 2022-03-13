#include <modelling/Material.h>

#include <algorithm>
#include <cmath>
#include <random>

namespace modelling {

Material::~Material() = default;

color::SColor Material::transparency() const {
  return color::SColor({0.0, 0.0, 0.0});
}

bool Material::requiresUV() const { return false; }

/**
 * @brief Construct a new Diffuse Material:: Diffuse Material object
 *
 * @param spectrum
 */
DiffuseMaterial::DiffuseMaterial(color::SColor spectrum,
                                 std::shared_ptr<modelling::Texture> texture)
    : m_spectrum(std::move(spectrum)), m_texture(std::move(texture)) {}

color::SColor DiffuseMaterial::BRDF(geometry::Normal3D const &,
                                    geometry::Normal3D const &,
                                    geometry::Normal3D const &,
                                    geometry::Point2D const &uv) const {
  if (m_texture != nullptr) return m_spectrum * m_texture->get(uv);
  return m_spectrum;
}

color::Intensity DiffuseMaterial::averageAlbedo() const {
  return m_spectrum.luminance() * M_PI;
}

Reflection DiffuseMaterial::reflection(geometry::Normal3D const &N,
                                       geometry::Normal3D const &V,
                                       geometry::Point2D const &uv) const {
  static std::random_device rd;
  static std::mt19937 gen(rd());
  static std::uniform_real_distribution<geometry::Coord> dist(0.0, 1.0);

  double u = dist(gen);
  double v = dist(gen);

  double theta = std::asin(std::sqrt(u));
  double phi = M_PI * 2.0 * v;

  // std::cout << u << " " << theta << " " << phi << std::endl;

  geometry::Vector3D O = N % geometry::Vector3D{0, 0, 1};
  if (O.length() < 1e-2) O = N % geometry::Vector3D{0, 1, 0};
  geometry::Vector3D P = N % O;

  geometry::Vector3D L = N * std::cos(theta) +
                         O * std::sin(theta) * std::cos(phi) +
                         P * std::sin(theta) * std::sin(phi);

  double prob = std::cos(theta) / M_PI;

  return {prob, L, BRDF(L, N, V, uv)};
}

bool DiffuseMaterial::requiresUV() const { return m_texture != nullptr; }

SpecularMaterial::SpecularMaterial(color::SColor spectrum,
                                   color::Intensity shine)
    : m_spectrum(std::move(spectrum)), m_shine(shine) {}

color::SColor SpecularMaterial::BRDF(geometry::Normal3D const &L,
                                     geometry::Normal3D const &N,
                                     geometry::Normal3D const &V,
                                     geometry::Point2D const &) const {
  geometry::Coord cos_in = L * N;

  if (cos_in > 1e-2 && m_spectrum.luminance() != 0) {
    geometry::Vector3D R = N * (2.0 * cos_in) - L;
    geometry::Coord cos_refl_out = R * V;
    if (cos_refl_out > 1e-2) {
      color::SColor ref = m_spectrum * (m_shine + 2) / M_PI / 2.0;
      return ref * std::pow(cos_refl_out, m_shine);
    }
  }

  return color::SColor(0);
}

color::Intensity SpecularMaterial::averageAlbedo() const {
  return m_spectrum.luminance() * 2 * M_PI / (m_shine + 2);
}

Reflection SpecularMaterial::reflection(geometry::Normal3D const &N,
                                        geometry::Normal3D const &V,
                                        geometry::Point2D const &uv) const {
  static std::random_device rd;
  static std::mt19937 gen(rd());
  static std::uniform_real_distribution<geometry::Coord> dist(0.0, 1.0);

  double u = dist(gen);
  double v = dist(gen);

  geometry::Coord cos_ang_V_R = std::pow(u, 1.0 / (m_shine + 1));

  double prob = (m_shine + 1) / 2 / M_PI * std::pow(cos_ang_V_R, m_shine);

  if (prob < 1e-2)
    return {0.0, geometry::Vector3D{0.0, 0.0, 0.0},
            color::SColor({0.0, 0.0, 0.0})};

  geometry::Coord sin_ang_V_R = std::sqrt(1.0 - cos_ang_V_R * cos_ang_V_R);

  geometry::Vector3D O = V % geometry::Vector3D{0, 0, 1};
  if (O.length() < 1e-2) O = V % geometry::Vector3D{0, 1, 0};
  geometry::Vector3D P = O % V;

  geometry::Vector3D R = O * sin_ang_V_R * std::cos(2.0 * M_PI * v) +
                         P * sin_ang_V_R * std::sin(2.0 * M_PI * v) +
                         V * cos_ang_V_R;

  geometry::Normal3D L = N * (N * R) * 2.0 - R;

  if (N * L < 0)
    return {0.0, geometry::Vector3D{0.0, 0.0, 0.0},
            color::SColor({0.0, 0.0, 0.0})};

  return {prob, L, BRDF(L, N, V, uv)};
}

/**
 * @brief Construct a new Ideal Reflector:: Ideal Reflector object
 *
 * @param kr
 */
IdealReflector::IdealReflector(color::SColor kr) : m_Kr(std::move(kr)) {}

color::SColor &IdealReflector::kr() { return m_Kr; }

color::SColor const &IdealReflector::kr() const { return m_Kr; }

Reflection IdealReflector::reflection(geometry::Normal3D const &N,
                                      geometry::Normal3D const &V,
                                      geometry::Point2D const &) const {
  geometry::Vector3D L = N * (N * V) * 2 - V;
  geometry::Coord cost = N * L;
  color::SColor brdf = cost > 1e-2 ? m_Kr / cost : color::SColor(0);
  return {1.0, L, brdf};
}

color::SColor IdealReflector::BRDF(geometry::Normal3D const &,
                                   geometry::Normal3D const &,
                                   geometry::Normal3D const &,
                                   geometry::Point2D const &) const {
  return color::SColor(0.0);
}

/**
 * @brief Construct a new Ideal Refractor:: Ideal Refractor object
 *
 * @param kt
 * @param N
 */
IdealRefractor::IdealRefractor(color::SColor kt, color::Intensity N)
    : m_Kt(std::move(kt)), m_N(N) {}

color::SColor &IdealRefractor::kt() { return m_Kt; }

color::SColor const &IdealRefractor::kt() const { return m_Kt; }

Reflection IdealRefractor::reflection(geometry::Normal3D const &N,
                                      geometry::Normal3D const &V,
                                      geometry::Point2D const &) const {
  geometry::Coord cosa = N * V;
  color::Intensity cn = (cosa > 0.0) ? m_N : 1.0 / m_N;
  geometry::Normal3D norm = (cosa < 0.0) ? -N : N;

  if (cosa < 0) cosa = -cosa;

  color::Intensity disc = 1.0 - (1.0 - cosa * cosa) / cn / cn;
  if (disc < 0.0)
    return {0.0, geometry::Vector3D{0.0, 0.0, 0.0},
            color::SColor({0.0, 0.0, 0.0})};

  geometry::Normal3D L = norm * (cosa / cn - std::sqrt(disc)) - V / cn;

  geometry::Coord cost = -(norm * L);
  color::SColor brdf = cost > 1e-2 ? m_Kt / cost : color::SColor({0, 0, 0});

  return {1.0, L, brdf};
}

color::SColor IdealRefractor::BRDF(geometry::Normal3D const &,
                                   geometry::Normal3D const &,
                                   geometry::Normal3D const &,
                                   geometry::Point2D const &) const {
  return color::SColor(0.0);
}

color::SColor IdealRefractor::transparency() const { return m_Kt; }

/**
 * @brief Construct a new General Material:: General Material object
 *
 * @param diffuseColor
 * @param kr
 */
GeneralMaterial::GeneralMaterial(color::SColor diffuseColor,
                                 color::SColor specularColor,
                                 color::Intensity shine, color::SColor kr,
                                 color::SColor kt, color::Intensity N,
                                 std::shared_ptr<Texture> texture)
    : DiffuseMaterial(std::move(diffuseColor), std::move(texture)),
      SpecularMaterial(std::move(specularColor), shine),
      IdealReflector(std::move(kr)),
      IdealRefractor(std::move(kt), N) {}

color::SColor GeneralMaterial::BRDF(geometry::Normal3D const &L,
                                    geometry::Normal3D const &N,
                                    geometry::Normal3D const &V,
                                    geometry::Point2D const &uv) const {
  return DiffuseMaterial::BRDF(L, N, V, uv) +
         SpecularMaterial::BRDF(L, N, V, uv);
}

Reflection GeneralMaterial::reflection(geometry::Normal3D const &N,
                                       geometry::Normal3D const &V,
                                       geometry::Point2D const &uv) const {
  static std::random_device rd;
  static std::mt19937 gen(rd());
  static std::uniform_real_distribution<double> dist(0.0, 1.0);

  double w1 = DiffuseMaterial::averageAlbedo();
  double w2 = SpecularMaterial::averageAlbedo();
  double w3 = IdealReflector::kr().luminance();
  double w4 = IdealRefractor::kt().luminance();

  double sum = w1 + w2 + w3 + w4;
  w1 /= sum;
  w2 /= sum;
  w3 /= sum;
  w4 /= sum;

  double p = dist(gen);

  if ((p -= w1) < 0) {
    Reflection r = DiffuseMaterial::reflection(N, V, uv);
    r.prob *= w1;
    return r;
  }
  if ((p -= w2) < 0) {
    Reflection r = SpecularMaterial::reflection(N, V, uv);
    r.prob *= w2;
    return r;
  }
  if ((p -= w3) < 0) {
    Reflection r = IdealReflector::reflection(N, V, uv);
    r.prob *= w3;
    return r;
  }
  Reflection r = IdealRefractor::reflection(N, V, uv);
  r.prob *= w4;
  return r;
}

color::SColor GeneralMaterial::transparency() const {
  double w1 = DiffuseMaterial::averageAlbedo();
  double w2 = SpecularMaterial::averageAlbedo();
  double w3 = IdealReflector::kr().luminance();
  double w4 = IdealRefractor::kt().luminance();

  return (DiffuseMaterial::transparency() * w1 +
          SpecularMaterial::transparency() * w2 +
          IdealReflector::transparency() * w3 +
          IdealRefractor::transparency() * w4) /
         (w1 + w2 + w3 + w4);
}

bool GeneralMaterial::requiresUV() const {
  return DiffuseMaterial::requiresUV() || SpecularMaterial::requiresUV() ||
         IdealReflector::requiresUV() || IdealRefractor::requiresUV();
}

}  // namespace modelling