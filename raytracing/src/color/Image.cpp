#include <color/Image.h>
#include <png.h>
#include <stdio.h>

#include <iostream>

// Source: http://zarb.org/~gc/html/libpng.html

namespace color {

void saveImage(std::string filename, Image const& image) {
  int bit_depth = 8;
  int color_type = PNG_COLOR_TYPE_RGB;
  png_bytep* row_pointers = new png_bytep[image.size.height];

  std::vector<png_byte> rawData;
  rawData.reserve(3 * image.data.size());

  for (auto const& rgb : image.data) {
    rawData.push_back(static_cast<png_byte>(255 * rgb.r));
    rawData.push_back(static_cast<png_byte>(255 * rgb.g));
    rawData.push_back(static_cast<png_byte>(255 * rgb.b));
  }

  for (size_t i = 0; i < image.size.height; ++i)
    row_pointers[i] = rawData.data() + i * 3 * image.size.width;

  png_structp png_ptr;
  png_infop info_ptr;

  /* create file */
  FILE* fp = fopen(filename.c_str(), "wb");
  if (!fp) throw "[write_png_file] File could not be opened for writing";

  /* initialize stuff */
  png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

  if (!png_ptr) throw "[write_png_file] png_create_write_struct failed";

  info_ptr = png_create_info_struct(png_ptr);
  if (!info_ptr) throw "[write_png_file] png_create_info_struct failed";

  if (setjmp(png_jmpbuf(png_ptr)))
    throw "[write_png_file] Error during init_io";

  png_init_io(png_ptr, fp);

  /* write header */
  if (setjmp(png_jmpbuf(png_ptr)))
    throw "[write_png_file] Error during writing header";

  auto width = static_cast<png_uint_32>(image.size.width);
  auto height = static_cast<png_uint_32>(image.size.height);

  png_set_IHDR(png_ptr, info_ptr, width, height, bit_depth, color_type,
               PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE,
               PNG_FILTER_TYPE_BASE);

  png_write_info(png_ptr, info_ptr);

  /* write bytes */
  if (setjmp(png_jmpbuf(png_ptr)))
    throw "[write_png_file] Error during writing bytes";

  png_write_image(png_ptr, row_pointers);

  /* end write */
  if (setjmp(png_jmpbuf(png_ptr)))
    throw "[write_png_file] Error during end of write";

  png_write_end(png_ptr, NULL);

  /* cleanup heap allocation */
  delete[] row_pointers;

  fclose(fp);
}

Image loadImage(std::string filename) {
  png_structp png_ptr;
  png_infop info_ptr;
  png_byte header[8];  // 8 is the maximum size that can be checked

  /* open file and test for it being a png */
  FILE* fp = fopen(filename.c_str(), "rb");
  if (!fp) throw "[read_png_file] File could not be opened for reading";

  if (fread(header, 1, 8, fp) != 8)
    throw "[read_png_file] Could not read header";

  if (png_sig_cmp(header, 0, 8))
    throw "[read_png_file] File is not recognized as a PNG file";

  /* initialize stuff */
  png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

  if (!png_ptr) throw "[read_png_file] png_create_read_struct failed";

  info_ptr = png_create_info_struct(png_ptr);
  if (!info_ptr) throw "[read_png_file] png_create_info_struct failed";

  if (setjmp(png_jmpbuf(png_ptr))) throw "[read_png_file] Error during init_io";

  png_init_io(png_ptr, fp);
  png_set_sig_bytes(png_ptr, 8);

  png_read_info(png_ptr, info_ptr);

  size_t width = png_get_image_width(png_ptr, info_ptr);
  size_t height = png_get_image_height(png_ptr, info_ptr);
  png_byte color_type = png_get_color_type(png_ptr, info_ptr);
  png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);

  if (color_type != PNG_COLOR_TYPE_RGB)
    throw "[read_png_file] Not an RGB image";

  if (bit_depth != 8)
    throw "[read_png_file] Image is not 8-bit format";

  size_t row_bytes = png_get_rowbytes(png_ptr, info_ptr);

  if (row_bytes != 3 * width) throw "[read_png_file] Wrong number of row bytes";

  std::vector<png_byte> rawData;
  rawData.resize(3 * height * width);

  png_bytep* row_pointers = new png_bytep[height];
  for (size_t i = 0; i < height; ++i)
    row_pointers[i] = rawData.data() + i * 3 * width;

  png_read_image(png_ptr, row_pointers);
  // std::cout << width << " " << height << " " << int(color_type) << " " <<
  // int(bit_depth) << std::endl;
  //  std::cout << png_get_rowbytes(png_ptr,info_ptr) << std::endl;
  //  std::cout << rawData.size() << " " << width << " " << height << std::endl;

  delete[] row_pointers;

  color::ImageData data;
  data.reserve(height * width);

  for (size_t i = 0; i < height; ++i)
    for (size_t j = 0; j < width; ++j) {
      color::Intensity r =
          static_cast<color::Intensity>(rawData[0 + 3 * (i * width + j)]) /
          255.0;
      color::Intensity g =
          static_cast<color::Intensity>(rawData[1 + 3 * (i * width + j)]) /
          255.0;
      color::Intensity b =
          static_cast<color::Intensity>(rawData[2 + 3 * (i * width + j)]) /
          255.0;
      data.push_back(color::RGB(r, g, b));
    }

  return {color::ImageSize{width, height}, data};
}

}  // namespace color