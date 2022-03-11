#pragma once

#include <color/Spectrum.h>
#include <geometry/Point3D.h>

#include <random>

namespace modelling {

struct Emission {
  color::SColor Le;
  geometry::Point3D pos;
  geometry::Ray rayToLight;
};

class Emitter {
 public:
  virtual ~Emitter() = default;

  virtual Emission emission(geometry::Point3D const& x,
                            geometry::Normal3D const& n) = 0;
};

class PositionalLight : public Emitter {
 public:
  PositionalLight(geometry::Point3D pos, color::SColor color);

  Emission emission(geometry::Point3D const& x, geometry::Normal3D const& n);

 private:
  geometry::Point3D m_pos;
  color::SColor m_color;
};

class SphereLight : public Emitter {
 private:
  using Positions = std::vector<geometry::Point3D>;

 public:
  SphereLight(geometry::Point3D pos, geometry::Coord radius,
              color::SColor color);

  Emission emission(geometry::Point3D const& x, geometry::Normal3D const& n);

 private:
  geometry::Point3D const& randomPoint();
  Positions generatePositions(geometry::Point3D pos, geometry::Coord radius);

 private:
  Positions m_positions;
  Positions::iterator m_pos_it;
  color::SColor m_color;
};

}  // namespace modelling