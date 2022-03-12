#include <color/Image.h>

#include <external/fpng/include/fpng.h>


namespace rendering {

void saveImage(std::string filename, Image const& image)
{
  std::vector<uint8_t> rawData;
  rawData.reserve(3 * image.data.size());

  for (auto const& rgb : image.data) {
    rawData.push_back(static_cast<uint8_t>(255 * rgb.r));
    rawData.push_back(static_cast<uint8_t>(255 * rgb.g));
    rawData.push_back(static_cast<uint8_t>(255 * rgb.b));
  }

  fpng::fpng_encode_image_to_file(filename.c_str(), rawData.data(),
                                  static_cast<uint32_t>(image.size.width),
                                  static_cast<uint32_t>(image.size.height), 3);
}

}  // namespace rendering