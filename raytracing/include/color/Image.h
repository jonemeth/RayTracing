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

struct Image {
  ImageSize size;
  ImageData data;
};

void saveImage(std::string filename, Image const& image);

}  // namespace rendering