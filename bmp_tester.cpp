#include "bmp_tester.hpp"
#include "bmp_parser.hpp"
namespace testing {
namespace bmp {
void rotate_image(const std::string &imagePath, const std::string &outputPath) {
  BMP *file = new BMP(imagePath);
  file->rotate_image();
  file->write(outputPath);
}
void convert_to_grayscale(const std::string &imagePath,
                          const std::string &outputPath) {
  BMP *file = new BMP(imagePath);
  file->convert_to_grayscale();
  file->write(outputPath);

} 
int main() {
  /*
  // Read an image from disk and write it back:
  BMP bmp("Shapes.bmp");
  bmp.write("Shapes_copy.bmp");

  // Create a BMP image in memory, modify it, save it on disk
  BMP bmp2(800, 600);
  bmp2.fill_region(50, 20, 100, 200, 0, 0, 255, 255);
  bmp2.write("img_test.bmp");
  */

  // Create a 24 bits/pixel BMP image in memory, modify it, save it on disk
  BMP bmp3("lena.bmp");
  bmp3.convert_to_grayscale();
  bmp3.write("lena_output.bmp");
  return 0;
}
} 
} // namespace testing
