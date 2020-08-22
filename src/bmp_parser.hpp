
#include <fstream>
#include <iostream>
#include <stdint.h>
#include <vector>
#include "Matrix.h"
#ifndef BMP_PARSER_H
#define BMP_PARSER_H
using std::vector;
struct BMPFileHeader {
  uint16_t file_type{0x4D42}; // File type always BM which is 0x4D42
  uint32_t file_size{0};      // Size of the file (in bytes)
  uint16_t reserved1{0};      // Reserved, always 0
  uint16_t reserved2{0};      // Reserved, always 0
  uint32_t offset_data{
      0}; // Start position of pixel data (bytes from the beginning of the file)
};

struct BMPInfoHeader {
  uint32_t size{0}; // Size of this header (in bytes)
  int32_t width{0}; // width of bitmap in pixels
  int32_t height{
      0}; // width of bitmap in pixels
          //       (if positive, bottom-up, with origin in lower left corner)
          //       (if negative, top-down, with origin in upper left corner)
  uint16_t planes{1}; // No. of planes for the target device, this is always 1
  uint16_t bit_count{0};   // No. of bits per pixel
  uint32_t compression{0}; // 0 or 3 - uncompressed. THIS PROGRAM CONSIDERS ONLY
                           // UNCOMPRESSED BMP images
  uint32_t size_image{0};  // 0 - for uncompressed images
  int32_t x_pixels_per_meter{0};
  int32_t y_pixels_per_meter{0};
  uint32_t colors_used{0}; // No. color indexes in the color table. Use 0 for
                           // the max number of colors allowed by bit_count
  uint32_t colors_important{0}; // No. of colors used for displaying the bitmap.
                                // If 0 all colors are required
};

struct BMPColorPalette {
  Matrix *color_list = nullptr;
  Matrix *pixel_color_indexes = nullptr;
};


struct BMP{
    BMPFileHeader file_header;
    BMPInfoHeader bmp_info_header;
    BMPColorPalette bmp_color_palette;
    Matrix *pixels = nullptr;
    public:
    BMP(const std::string fname);
    void read(const std::string fname);
    void write(const std::string fname);
    void convert_to_grayscale();
    void rotate_image();
    Matrix *vector_to_matrix(vector<uint8_t> vector, int height, int width);
    vector<uint8_t> matrix_to_vector(Matrix *matrix);
    void write_headers(std::ofstream &of);
    void write_headers_and_data(std::ofstream &of, vector<uint8_t> data);
    uint32_t make_stride_aligned(uint32_t align_stride);
    void write_24_bit(std::ofstream &of, vector<uint8_t> data);
    void write_8_bit(std::ofstream &of, vector<uint8_t> data);
    void read_24_bit(std::ifstream *inp, vector<uint8_t> data);
    void read_8_bit(std::ifstream *inp, vector<uint8_t> data);
};
#endif