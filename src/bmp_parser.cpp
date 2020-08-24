#include "bmp_parser.hpp"
#define BMP_DECLARATION 0x4D42
#define FILE_INFO_END 14
#define FILE_DATA_HEAD_END 54
#define PIXEL_OFFSET 3
#define BLUE_COEF 0.0722
#define GREEN_COEF 0.7152
#define RED_COEF 0.2126
#define BYTE 4

using std::vector;
class Matrix;

// Created a parameterless constructor to solve an issue with the compilation of
// the destructor
BMP::BMP() {
  throw std::runtime_error("can only create bmp object using a file path");
}

BMP::BMP(const std::string fname) { read(fname); }

void BMP::read(const std::string fname) {
  vector<uint8_t> data;
  std::ifstream inp{fname, std::ios_base::binary};
  if (inp) {
    inp.read((char *)&file_header, sizeof(file_header));
    if (file_header.file_type != BMP_DECLARATION) {
      throw std::runtime_error("Error! Unrecognized file format.");
    }
    inp.seekg(FILE_INFO_END, inp.beg);
    inp.read((char *)&bmp_info_header, sizeof(bmp_info_header));

    // Jump to the pixel data location
    inp.seekg(file_header.offset_data, inp.beg);

    data.resize(bmp_info_header.width * bmp_info_header.height *
                bmp_info_header.bit_count / 8);

    if (bmp_info_header.bit_count == 24) {
      read_24_bit(&inp, data);
    } else if (bmp_info_header.bit_count == 8) {
      read_8_bit(&inp);
    } else {
      throw std::runtime_error(
          "The program can treat only 24 or 8 bits per pixel BMP files");
    }
  } else {
    throw std::runtime_error("Unable to open the input image file.");
  }
}

void BMP::write(const std::string fname) {
  vector<uint8_t> data = matrix_to_vector(pixels);
  std::ofstream of{fname, std::ios_base::binary};
  if (of) {
    if (bmp_info_header.bit_count == 24) {
      write_24_bit(of, data);
    } else if (bmp_info_header.bit_count == 8) {
      write_8_bit(of);
    } else {
      throw std::runtime_error(
          "The program can treat only 24 or 8 bits per pixel BMP files");
    }
  } else {
    throw std::runtime_error("Unable to open the output image file.");
  }
}

void BMP::convert_to_grayscale() {
  for (int i = 0; i < bmp_info_header.height; ++i) {
    for (int j = 0; j < bmp_info_header.width; ++j) {
      double b = pixels->getValue(i, PIXEL_OFFSET * j);
      double g = pixels->getValue(i, PIXEL_OFFSET * j + 1);
      double r = pixels->getValue(i, PIXEL_OFFSET * j + 2);
      double grayscale = BLUE_COEF * b + GREEN_COEF * g + RED_COEF * r;
      pixels->setValue(i, PIXEL_OFFSET * j, grayscale);
      pixels->setValue(i, PIXEL_OFFSET * j + 1, grayscale);
      pixels->setValue(i, PIXEL_OFFSET * j + 2, grayscale);
    }
  }
  if (bmp_info_header.bit_count == 8) {
    for (int i = 0; i < bmp_color_palette.color_list->getHeight(); ++i) {
      double b = bmp_color_palette.color_list->getValue(i, 0);
      double g = bmp_color_palette.color_list->getValue(i, 1);
      double r = bmp_color_palette.color_list->getValue(i, 2);
      double grayscale = BLUE_COEF * b + GREEN_COEF * g + RED_COEF * r;
      bmp_color_palette.color_list->setValue(i, 0, grayscale);
      bmp_color_palette.color_list->setValue(i, 1, grayscale);
      bmp_color_palette.color_list->setValue(i, 2, grayscale);
    }
  }
}

void BMP::rotate_image() {
  uint32_t temp = bmp_info_header.height;
  bmp_info_header.height = bmp_info_header.width;
  bmp_info_header.width = temp;
  auto rotated_pixels =
      std::make_unique<Matrix>(bmp_info_header.height, bmp_info_header.width * PIXEL_OFFSET);
  for (int i = 0; i < bmp_info_header.height; ++i) {
    for (int j = 0; j < bmp_info_header.width; ++j) {
      rotated_pixels->setValue(
          i, PIXEL_OFFSET * j, pixels->getValue(j, PIXEL_OFFSET * (bmp_info_header.height - i)));
      rotated_pixels->setValue(
          i, PIXEL_OFFSET * j + 1,
          pixels->getValue(j, PIXEL_OFFSET * (bmp_info_header.height - i) + 1));
      rotated_pixels->setValue(
          i, PIXEL_OFFSET * j + 2,
          pixels->getValue(j, PIXEL_OFFSET * (bmp_info_header.height - i) + 2));
    }
  }
  pixels = new Matrix(bmp_info_header.height, bmp_info_header.width * PIXEL_OFFSET);
  for (int i = 0; i < bmp_info_header.height; ++i) {
    for (int j = 0; j < bmp_info_header.width; ++j) {
      pixels->setValue(i, PIXEL_OFFSET * j, rotated_pixels->getValue(i, PIXEL_OFFSET * j));
      pixels->setValue(i, PIXEL_OFFSET * j + 1, rotated_pixels->getValue(i, PIXEL_OFFSET * j + 1));
      pixels->setValue(i, PIXEL_OFFSET * j + 2, rotated_pixels->getValue(i, PIXEL_OFFSET * j + 2));
    }
  }
  if (bmp_info_header.bit_count == 8) {
    auto rotated_pixel_color_indexes =
        std::make_unique<Matrix>(bmp_info_header.height, bmp_info_header.width);
    for (int i = 0; i < bmp_info_header.height; ++i) {
      for (int j = 0; j < bmp_info_header.width; ++j) {
        rotated_pixel_color_indexes->setValue(
            i, j,
            bmp_color_palette.pixel_color_indexes->getValue(
                j, bmp_info_header.height - i));
      }
    }
    bmp_color_palette.pixel_color_indexes =
        new Matrix(bmp_info_header.height, bmp_info_header.width);
    for (int i = 0; i < bmp_info_header.height; ++i) {
      for (int j = 0; j < bmp_info_header.width; ++j) {
        bmp_color_palette.pixel_color_indexes->setValue(
            i, j, rotated_pixel_color_indexes->getValue(i, j));
      }
    }
  }
}

BMP::~BMP() {
  delete bmp_color_palette.color_list;
  delete bmp_color_palette.pixel_color_indexes;
  delete pixels;
}

Matrix* BMP::vector_to_matrix(vector<uint8_t> vector, int height, int width) {
  auto matrix = new Matrix(height, width);
  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; ++j) {
      matrix->setValue(i, j, vector[i * width + j]);
    }
  }
  return matrix;
}

vector<uint8_t> BMP::matrix_to_vector(Matrix *matrix) {
  vector<uint8_t> vector;
  vector.resize(matrix->getHeight() * matrix->getWidth());
  for (int i = 0; i < matrix->getHeight(); ++i) {
    for (int j = 0; j < matrix->getWidth(); ++j) {
      vector.at(i * matrix->getWidth() + j) = matrix->getValue(i, j);
    }
  }
  return vector;
}

uint32_t row_stride{0};

void BMP::write_headers(std::ofstream &of) {
  of.write((const char *)&file_header, sizeof(file_header));
  of.write((const char *)&bmp_info_header, sizeof(bmp_info_header));
}

void BMP::write_headers_and_data(std::ofstream &of, vector<uint8_t> data) {
  write_headers(of);
  of.write((const char *)data.data(), data.size());
}

// Add 1 to the row_stride until it is divisible with align_stride
uint32_t BMP::make_stride_aligned(uint32_t align_stride) {
  uint32_t new_stride = row_stride;
  while (new_stride % align_stride != 0) {
    new_stride++;
  }
  return new_stride;
}

void BMP::write_24_bit(std::ofstream &of, vector<uint8_t> data) {
  if (bmp_info_header.width % BYTE == 0) {
    write_headers_and_data(of, data);
  } else {
    uint32_t new_stride = make_stride_aligned(BYTE);
    std::vector<uint8_t> padding_row(new_stride - row_stride);

    write_headers(of);

    for (int y = 0; y < bmp_info_header.height; ++y) {
      of.write((const char *)(data.data() + row_stride * y), row_stride);
      of.write((const char *)padding_row.data(), padding_row.size());
    }
  }
}

void BMP::write_8_bit(std::ofstream &of) {
  write_headers(of);
  std::vector<uint8_t> color_palette =
      matrix_to_vector(bmp_color_palette.color_list);
  std::vector<uint8_t> color_palette_padding(1);
  int num_of_colors = bmp_color_palette.color_list->getHeight();
  for (int i = 0; i < num_of_colors; ++i) {
    of.write((const char *)(color_palette.data() + PIXEL_OFFSET * i), PIXEL_OFFSET);
    of.write((const char *)color_palette_padding.data(), 1);
  }
  std::vector<uint8_t> pixel_color_indexes =
      matrix_to_vector(bmp_color_palette.pixel_color_indexes);
  if (bmp_info_header.width % BYTE == 0) {
    of.write((const char *)pixel_color_indexes.data(),
             pixel_color_indexes.size());
  } else {
    // uint32_t image_stride = bmp_info_header.width;
    uint32_t new_stride = make_stride_aligned(BYTE);
    std::vector<uint8_t> pixel_color_indexes_padding(new_stride - row_stride);
    for (int i = 0; i < bmp_info_header.height; ++i) {
      of.write((const char *)(pixel_color_indexes.data() + row_stride * i),
               row_stride);
      of.write((const char *)pixel_color_indexes_padding.data(),
               pixel_color_indexes_padding.size());
    }
  }
}

void BMP::read_24_bit(std::ifstream *inp, vector<uint8_t> data) {
  if (bmp_info_header.width % BYTE == 0) {
    inp->read((char *)data.data(), data.size());
    file_header.file_size += data.size();
  } else {
    uint32_t row_stride_24bit =
        bmp_info_header.width * bmp_info_header.bit_count / 8;
    uint32_t new_stride = make_stride_aligned(BYTE);
    std::vector<uint8_t> padding_row(new_stride - row_stride);
    for (int i = 0; i < bmp_info_header.height; ++i) {
      inp->read((char *)(data.data() + row_stride_24bit * i), row_stride_24bit);
      inp->read((char *)padding_row.data(), padding_row.size());
    }
    file_header.file_size +=
        data.size() + bmp_info_header.height * padding_row.size();
  }
  pixels =
      vector_to_matrix(data, bmp_info_header.height, bmp_info_header.width * PIXEL_OFFSET);
}

void BMP::read_8_bit(std::ifstream *inp) {
  inp->seekg(FILE_DATA_HEAD_END, inp->beg);
  int color_palette_size = file_header.offset_data - FILE_DATA_HEAD_END;
  int num_of_colors = color_palette_size / BYTE;
  std::vector<uint8_t> bgr_values(num_of_colors * PIXEL_OFFSET);
  std::vector<uint8_t> padding(1);
  for (int i = 0; i < num_of_colors; ++i) {
    inp->read((char *)(bgr_values.data() + PIXEL_OFFSET * i), PIXEL_OFFSET);
    inp->read((char *)padding.data(), 1);
  }
  bmp_color_palette.color_list = vector_to_matrix(bgr_values, num_of_colors, PIXEL_OFFSET);
  std::vector<uint8_t> color_indexes(bmp_info_header.height *
                                     bmp_info_header.width);
  if (bmp_info_header.width % BYTE == 0) {
    inp->read((char *)color_indexes.data(), color_indexes.size());
  } else {
    uint32_t row_stride_8bit = bmp_info_header.width;
    uint32_t new_stride = make_stride_aligned(BYTE);
    std::vector<uint8_t> padding_row(new_stride - row_stride);
    for (int i = 0; i < bmp_info_header.height; ++i) {
      inp->read((char *)(color_indexes.data() + row_stride_8bit * i),
                row_stride_8bit);
      inp->read((char *)padding_row.data(), padding_row.size());
    }
  }
  bmp_color_palette.pixel_color_indexes = vector_to_matrix(
      color_indexes, bmp_info_header.height, bmp_info_header.width);
  pixels = new Matrix(bmp_info_header.height, bmp_info_header.width * PIXEL_OFFSET);
  for (int i = 0; i < bmp_info_header.height; ++i) {
    for (int j = 0; j < bmp_info_header.width; ++j) {
      pixels->setValue(
          i, PIXEL_OFFSET * j,
          bmp_color_palette.color_list->getValue(
              bmp_color_palette.pixel_color_indexes->getValue(i, j), 0));
      pixels->setValue(
          i, PIXEL_OFFSET * j + 1,
          bmp_color_palette.color_list->getValue(
              bmp_color_palette.pixel_color_indexes->getValue(i, j), 1));
      pixels->setValue(
          i, PIXEL_OFFSET * j + 2,
          bmp_color_palette.color_list->getValue(
              bmp_color_palette.pixel_color_indexes->getValue(i, j), 2));
    }
  }
}