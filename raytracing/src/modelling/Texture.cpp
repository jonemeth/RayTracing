#include <modelling/Texture.h>

#include <cmath>

namespace modelling {

Texture::Texture(std::string filename) {
  color::Image image = color::loadImage(filename);
  size = image.size;

  values.resize(size.width * size.height);

  std::transform(image.data.begin(), image.data.end(), values.begin(),
                 [](color::RGB const& rgb) { return color::SColor(rgb); });
}

color::SColor const& Texture::get(geometry::Point2D const& p) const {
  geometry::Coord u = fmod(p.x, 1.0);
  geometry::Coord v = fmod(p.y, 1.0);
  auto x = static_cast<size_t>(u * static_cast<geometry::Coord>(size.width));
  auto y = static_cast<size_t>(v * static_cast<geometry::Coord>(size.height));
  return values[x + size.width * y];
}

}  // namespace modelling