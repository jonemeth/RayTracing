#include <modelling/Emitter.h>

#include <cmath>

namespace modelling {

static const geometry::Coord EPS = 1e-2;

PositionalLight::PositionalLight(geometry::Point3D pos, color::SColor color)
    : m_pos(std::move(pos)), m_color(std::move(color)) {}

Emission PositionalLight::emission(geometry::Point3D const& x,
                                   geometry::Normal3D const& n) {
  geometry::Normal3D L = m_pos - x;
  geometry::Coord cost = L * n;

  if (cost <= EPS) return {color::SColor(0.0), m_pos, {x, L}};

  return {(m_color) / (((x - m_pos) * (x - m_pos)) / cost), m_pos, {x, L}};
}

SphereLight::SphereLight(geometry::Point3D pos, geometry::Coord radius,
                         color::SColor color)
    : m_positions(generatePositions(pos, radius)),
      m_pos_it(m_positions.begin()),
      m_color(std::move(color)) {}

Emission SphereLight::emission(geometry::Point3D const& x,
                               geometry::Normal3D const& n) {
  geometry::Point3D pos = randomPoint();
  geometry::Normal3D L = pos - x;
  geometry::Coord cost = L * n;

  if (cost <= EPS) return {color::SColor(0.0), pos, {x, L}};

  return {m_color / (((x - pos) * (x - pos)) / cost), pos, {x, L}};
}

geometry::Point3D const& SphereLight::randomPoint() {
  geometry::Point3D const& ret = *m_pos_it;
  if (++m_pos_it == m_positions.end()) m_pos_it = m_positions.begin();
  return ret;
}

SphereLight::Positions SphereLight::generatePositions(geometry::Point3D pos,
                                                      geometry::Coord radius) {
  SphereLight::Positions positions;
  for (geometry::Coord u = 0.0; u < 1.0; u += 0.25) {
    for (geometry::Coord v = 0.0; v < 1.0; v += 0.25) {
      geometry::Coord theta = 2.0 * M_PI * u;
      geometry::Coord phi = std::acos(2.0 * v - 1.0);

      positions.push_back({pos.x + radius * std::cos(theta) * std::sin(phi),
                             pos.y + radius * std::sin(theta) * std::sin(phi),
                             pos.z + radius * std::cos(phi)});
    }
  }
  return positions;
}

}  // namespace modelling