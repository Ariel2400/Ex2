#include "bmp_tester.hpp"

int main(void) {
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
  testing::bmp::rotate_image("lena-color.bmp", "lena_output.bmp");
  return 0;
}