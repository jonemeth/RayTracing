#include <rendering/Image.h>

#include <external/fpng/include/fpng.h>


namespace rendering {

void saveImage(std::string filename, ImageData const& data,
               ImageSize const& size) {
  std::vector<uint8_t> rawData;
  rawData.reserve(3 * data.size());

  for (auto const& rgb : data) {
    rawData.push_back(static_cast<uint8_t>(255 * rgb.r));
    rawData.push_back(static_cast<uint8_t>(255 * rgb.g));
    rawData.push_back(static_cast<uint8_t>(255 * rgb.b));
  }

  fpng::fpng_encode_image_to_file(filename.c_str(), rawData.data(),
                                  static_cast<uint32_t>(size.width),
                                  static_cast<uint32_t>(size.height), 3);
}

}  // namespace rendering