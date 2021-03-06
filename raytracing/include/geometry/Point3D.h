#pragma once

#include <geometry/Matrix.h>
#include <geometry/Point2D.h>
#include <geometry/types.h>

#include <cmath>
#include <iostream>

namespace geometry {

struct Point3D {
  Coord x, y, z;
  Coord length() const { return std::sqrt(x * x + y * y + z * z); }

  inline Point3D operator-() const { return Point3D{x, y, z}; }

  inline Point3D operator+(Point3D const &p2) const {
    return Point3D{x + p2.x, y + p2.y, z + p2.z};
  }

  inline Point3D operator-(Point3D const &p2) const {
    return Point3D{x - p2.x, y - p2.y, z - p2.z};
  }

  inline Coord operator*(Point3D const &p2) const {
    return x * p2.x + y * p2.y + z * p2.z;
  }

  inline Point3D operator*(Coord s) const {
    return Point3D{s * x, s * y, s * z};
  }

  inline Point3D operator/(Coord s) const {
    return Point3D{x / s, y / s, z / s};
  }

  inline Point3D operator%(Point3D const &p2) const {
    return Point3D{y * p2.z - z * p2.y, z * p2.x - x * p2.z,
                   x * p2.y - y * p2.x};
  }
};

using Vector3D = Point3D;

inline std::ostream &operator<<(std::ostream &os, Point3D const &p) {
  os << "[" << p.x << "," << p.y << "," << p.z << "]";
  return os;
}

inline Point3D operator*(Coord s, Point3D const &p) { return p * s; }

struct Normal3D : Vector3D {
  static constexpr Coord EPS = 1e-12;

  Normal3D(Vector3D const &v) : Normal3D(v.x, v.y, v.z) {}

  Normal3D(Coord x0, Coord y0, Coord z0) {
    Coord length = EPS + std::sqrt(x0 * x0 + y0 * y0 + z0 * z0);
    x = x0 / length;
    y = y0 / length;
    z = z0 / length;
  }

  inline Normal3D operator-() const { return Normal3D{-x, -y, -z}; }

  inline Normal3D operator%(Normal3D const &p2) const {
    return Normal3D{y * p2.z - z * p2.y, z * p2.x - x * p2.z,
                    x * p2.y - y * p2.x};
  }
};

struct Ray {
  Point3D start;
  Normal3D direction;
};

inline Point2D operator*(Matrix<2, 3> const &M, Point3D const &p) {
  Matrix<2, 1> R = M * Matrix<3, 1>{{{p.x}, {p.y}, {p.z}}};
  return {R.values[0][0], R.values[1][0]};
}

inline Point3D operator*(Matrix<4, 4> const &M, Point3D const &p) {
  Matrix<4, 1> R = M * Matrix<4, 1>{{{p.x}, {p.y}, {p.z}, {1.0}}};
  return {R.values[0][0] / R.values[3][0], R.values[1][0] / R.values[3][0],
          R.values[2][0] / R.values[3][0]};
}

inline Normal3D operator*(Matrix<4, 4> const &M, Normal3D const &n) {
  Matrix<4, 1> R = M * Matrix<4, 1>{{{n.x}, {n.y}, {n.z}, {1.0}}};
  Point3D p = {R.values[0][0] / R.values[3][0], R.values[1][0] / R.values[3][0],
               R.values[2][0] / R.values[3][0]};

  Matrix<4, 1> O = M * Matrix<4, 1>{{{0}, {0}, {0}, {1.0}}};
  Point3D x = {O.values[0][0] / O.values[3][0], O.values[1][0] / O.values[3][0],
               O.values[2][0] / O.values[3][0]};
  return p - x;
}

inline Ray operator*(Matrix<4, 4> const &M, Ray const &ray) {
  Point3D x = M * ray.start;
  return {x, M * (ray.start + ray.direction) - x};
}

}  // namespace geometry