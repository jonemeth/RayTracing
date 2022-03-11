#include <modelling/Camera.h>

namespace modelling {

Camera::Camera(geometry::Point3D vrp, geometry::Vector3D u,
         geometry::Vector3D v, geometry::Point3D eye)
    : m_vrp(vrp), m_u(u), m_v(v), m_eye(eye) {}

geometry::Ray Camera::getRay(geometry::Coord x, geometry::Coord y) const {
  geometry::Point3D p = m_vrp + x*m_u + y*m_v;
  return geometry::Ray{p, p-m_eye};
}

}  // namespace modelling