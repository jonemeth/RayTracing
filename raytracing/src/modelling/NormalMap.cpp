#include <modelling/NormalMap.h>

#include <cmath>

namespace modelling {

NormalMap::NormalMap(std::string filename, Interpolation interpolation)
    : m_interpolation(interpolation) {
  color::Image image = color::loadImage(filename);
  size = image.size;

  values.resize(size.width * size.height);

  std::transform(image.data.begin(), image.data.end(), values.begin(),
                 [](color::RGB const& rgb) { return geometry::Vector3D{rgb.r-0.5, rgb.g-0.5, rgb.b-0.5}; });
}

geometry::Vector3D const NormalMap::get(geometry::Point2D const& p) const {
  geometry::Coord x =
      fmod(p.x, 1.0) * static_cast<geometry::Coord>(size.width - 1);
  geometry::Coord y =
      fmod(p.y, 1.0) * static_cast<geometry::Coord>(size.height - 1);

  geometry::Coord x1 = std::floor(x);
  geometry::Coord y1 = std::floor(y);
  geometry::Coord rx = fmod(x, 1.0);
  geometry::Coord ry = fmod(y, 1.0);

  size_t u1 = static_cast<size_t>(x1);
  size_t v1 = static_cast<size_t>(y1);
  size_t u2 = static_cast<size_t>(std::ceil(x));
  size_t v2 = static_cast<size_t>(std::ceil(y));

  if (m_interpolation == Bilinear) {
    return values[u1 + size.width * v1] * ((1.0 - rx) * (1.0 - ry)) +
           values[u2 + size.width * v1] * ((rx) * (1.0 - ry)) +
           values[u1 + size.width * v2] * ((1.0 - rx) * (ry)) +
           values[u2 + size.width * v2] * ((rx) * (ry));
  }

  if (rx < 0.5) {
    if (ry < 0.5)
      return values[u1 + size.width * v1];
    else
      return values[u1 + size.width * v2];
  }
  if (ry < 0.5) return values[u2 + size.width * v1];

  return values[u2 + size.width * v2];
}

}  // namespace modelling