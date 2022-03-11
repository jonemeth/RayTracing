#pragma once

#include <geometry/Point3D.h>

namespace modelling {

class Camera {
 public:
  Camera(geometry::Point3D vrp, geometry::Vector3D u,
         geometry::Vector3D v, geometry::Point3D eye);

  geometry::Ray getRay(geometry::Coord x, geometry::Coord y) const;

 private:
  geometry::Point3D m_vrp;
  geometry::Vector3D m_u, m_v;
  geometry::Point3D m_eye;
};

}  // namespace modelling