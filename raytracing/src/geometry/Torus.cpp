#include <geometry/Torus.h>

#include <cmath>
#include <vector>

namespace geometry {


static inline double sign(double v) { return v > 0 ? 1 : (v < 0 ? -1 : 0); }

#define EPSILON (1e-8)

Torus::Torus(Coord R0, Coord r0) : R(R0), r(r0) {}

Coord Torus::intersect(Ray const& ray) const {
  Point3D const& ro = ray.start;
  Normal3D const& rd = ray.direction;

  // Source: https://www.shadertoy.com/view/4sBGDy
  double po = 1.0;
  double Ra2 = R * R;
  double ra2 = r * r;

  double m = ro * ro;
  double n = ro * rd;

  // bounding sphere
  {
    double h = n * n - m + (R + r) * (R + r);
    if (h < 0.0) return -1.0;
    // double t = -n-sqrt(h); // could use this to compute intersections from
    // ro+t*rd
  }

  // find quartic equation
  double k = (m - ra2 - Ra2) / 2.0;
  double k3 = n;
  double k2 = n * n + Ra2 * rd.z * rd.z + k;
  double k1 = k * n + Ra2 * ro.z * rd.z;
  double k0 = k * k + Ra2 * ro.z * ro.z - Ra2 * ra2;

#if 1
  // prevent |c1| from being too close to zero
  if (std::abs(k3 * (k3 * k3 - k2) + k1) < 1e-8) {
    po = -1.0;
    double tmp = k1;
    k1 = k3;
    k3 = tmp;
    k0 = 1.0 / k0;
    k1 = k1 * k0;
    k2 = k2 * k0;
    k3 = k3 * k0;
  }
#endif

  double c2 = 2.0 * k2 - 3.0 * k3 * k3;
  double c1 = k3 * (k3 * k3 - k2) + k1;
  double c0 = k3 * (k3 * (-3.0 * k3 * k3 + 4.0 * k2) - 8.0 * k1) + 4.0 * k0;

  c2 /= 3.0;
  c1 *= 2.0;
  c0 /= 3.0;

  double Q = c2 * c2 + c0;
  double R = 3.0 * c0 * c2 - c2 * c2 * c2 - c1 * c1;

  double h = R * R - Q * Q * Q;
  double z = 0.0;
  if (h < 0.0) {
    // 4 intersections
    double sQ = std::sqrt(Q);
    z = 2.0 * sQ * cos(acos(R / (sQ * Q)) / 3.0);
  } else {
    // 2 intersections
    double sQ = std::pow(std::sqrt(h) + std::abs(R), 1.0 / 3.0);
    z = sign(R) * std::abs(sQ + Q / sQ);
  }
  z = c2 - z;

  double d1 = z - 3.0 * c2;
  double d2 = z * z - 3.0 * c0;
  if (std::abs(d1) < 1e-8) {
    if (d2 < 0.0) return -1.0;
    d2 = std::sqrt(d2);
  } else {
    if (d1 < 0.0) return -1.0;
    d1 = std::sqrt(d1 / 2.0);
    d2 = c1 / d1;
  }

  //----------------------------------

  double result = 1e20;

  h = d1 * d1 - z + d2;
  if (h > 0.0) {
    h = std::sqrt(h);
    double t1 = -d1 - h - k3;
    t1 = (po < 0.0) ? 2.0 / t1 : t1;
    double t2 = -d1 + h - k3;
    t2 = (po < 0.0) ? 2.0 / t2 : t2;
    if (t1 > 0.0) result = t1;
    if (t2 > 0.0) result = std::min(result, t2);
  }

  h = d1 * d1 - z - d2;
  if (h > 0.0) {
    h = std::sqrt(h);
    double t1 = d1 - h - k3;
    t1 = (po < 0.0) ? 2.0 / t1 : t1;
    double t2 = d1 + h - k3;
    t2 = (po < 0.0) ? 2.0 / t2 : t2;
    if (t1 > 0.0) result = std::min(result, t1);
    if (t2 > 0.0) result = std::min(result, t2);
  }
  if(result > 1e19 || result < 1e-8)
    return -1;
  return result;
}

geometry::Normal3D Torus::normal(geometry::Point3D const& x) const {
  Normal3D dir(x.x, x.y, 0.0);
  return x - R*dir;
}

}  // namespace geometry