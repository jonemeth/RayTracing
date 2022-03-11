#pragma once

#include <color/RGB.h>

#include <string>
#include <vector>

namespace rendering {

struct ImageSize {
  size_t width;
  size_t height;
};

using ImageData = std::vector<color::RGB>;

void saveImage(std::string filename, ImageData const& data,
               ImageSize const& size);

}  // namespace rendering