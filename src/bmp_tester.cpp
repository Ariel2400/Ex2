#include "bmp_tester.hpp"

namespace testing {
namespace bmp {

void rotate_image(const std::string &imagePath, const std::string &outputPath) {
  BMP file(imagePath);
  file.rotate_image();
  file.write(outputPath);
}

void convert_to_grayscale(const std::string &imagePath,
                          const std::string &outputPath) {
  BMP file(imagePath);
  file.convert_to_grayscale();
  file.write(outputPath);
}


} // namespace bmp
} // namespace testing
