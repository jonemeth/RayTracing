#pragma once

#include <color/Image.h>
#include <geometry/Point2D.h>

#include <string>
#include <vector>

namespace modelling {

class Texture {
 public:
  Texture(std::string filename);
  color::SColor const& get(geometry::Point2D const& p) const;

 private:
  std::vector<color::SColor> values;
  color::ImageSize size;
};

}  // namespace modelling