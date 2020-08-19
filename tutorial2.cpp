#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>

#include "Matrix.cpp"

using std::vector;

#pragma pack(push, 1)
struct BMPFileHeader {
    uint16_t file_type{0x4D42};          // File type always BM which is 0x4D42
    uint32_t file_size{0};               // Size of the file (in bytes)
    uint16_t reserved1{0};               // Reserved, always 0
    uint16_t reserved2{0};               // Reserved, always 0
    uint32_t offset_data{0};             // Start position of pixel data (bytes from the beginning of the file)
};
#pragma pack(pop)


#pragma pack(push, 1)
struct BMPInfoHeader {
    uint32_t size{ 0 };                      // Size of this header (in bytes)
    int32_t width{ 0 };                      // width of bitmap in pixels
    int32_t height{ 0 };                     // width of bitmap in pixels
                                             //       (if positive, bottom-up, with origin in lower left corner)
                                             //       (if negative, top-down, with origin in upper left corner)
    uint16_t planes{ 1 };                    // No. of planes for the target device, this is always 1
    uint16_t bit_count{ 0 };                 // No. of bits per pixel
    uint32_t compression{ 0 };               // 0 or 3 - uncompressed. THIS PROGRAM CONSIDERS ONLY UNCOMPRESSED BMP images
    uint32_t size_image{ 0 };                // 0 - for uncompressed images
    int32_t x_pixels_per_meter{ 0 };
    int32_t y_pixels_per_meter{ 0 };
    uint32_t colors_used{ 0 };               // No. color indexes in the color table. Use 0 for the max number of colors allowed by bit_count
    uint32_t colors_important{ 0 };          // No. of colors used for displaying the bitmap. If 0 all colors are required
};
#pragma pack(pop)


#pragma pack(push, 1)
struct BMPColorHeader {
    uint32_t red_mask{ 0x00ff0000 };         // Bit mask for the red channel
    uint32_t green_mask{ 0x0000ff00 };       // Bit mask for the green channel
    uint32_t blue_mask{ 0x000000ff };        // Bit mask for the blue channel
    uint32_t alpha_mask{ 0xff000000 };       // Bit mask for the alpha channel
    uint32_t color_space_type{ 0x73524742 }; // Default "sRGB" (0x73524742)
    uint32_t unused[16]{ 0 };                // Unused data for sRGB color space
};
#pragma pack(pop)


#pragma pack(push, 1)
struct BMP {
    BMPFileHeader file_header;
    BMPInfoHeader bmp_info_header;
    BMPColorHeader bmp_color_header;
    
    Matrix* pixels;

    BMP(const char *fname) {
        read(fname);
    }

    void read(const char *fname) {
        vector<uint8_t> data;
        std::ifstream inp{ fname, std::ios_base::binary };
        if (inp) {
            inp.read((char*)&file_header, sizeof(file_header));
            if(file_header.file_type != 0x4D42) {
                throw std::runtime_error("Error! Unrecognized file format.");
            }
            inp.read((char*)&bmp_info_header, sizeof(bmp_info_header));

            // The BMPColorHeader is used only for transparent images
            if(bmp_info_header.bit_count == 32) {
                // Check if the file has bit mask color information
                if(bmp_info_header.size >= (sizeof(BMPInfoHeader) + sizeof(BMPColorHeader))) {
                    inp.read((char*)&bmp_color_header, sizeof(bmp_color_header));
                    // Check if the pixel data is stored as BGRA and if the color space type is sRGB
                    check_color_header(bmp_color_header);
                } else {
                    std::cerr << "Warning! The file \"" << fname << "\" does not seem to contain bit mask information\n";
                    throw std::runtime_error("Error! Unrecognized file format.");
                }
            }

            // Jump to the pixel data location
            inp.seekg(file_header.offset_data, inp.beg);

            // Adjust the header fields for output.
            // Some editors will put extra info in the image file, we only save the headers and the data.
            if(bmp_info_header.bit_count == 32) {
                bmp_info_header.size = sizeof(BMPInfoHeader) + sizeof(BMPColorHeader);
                file_header.offset_data = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader) + sizeof(BMPColorHeader);
            } else {
                bmp_info_header.size = sizeof(BMPInfoHeader);
                file_header.offset_data = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader);
            }
            file_header.file_size = file_header.offset_data;

            if (bmp_info_header.height < 0) {
                throw std::runtime_error("The program can treat only BMP images with the origin in the bottom left corner!");
            }

            data.resize(bmp_info_header.width * bmp_info_header.height * bmp_info_header.bit_count / 8);

            // Here we check if we need to take into account row padding
            if (bmp_info_header.width % 4 == 0) {
                inp.read((char*)data.data(), data.size());
                file_header.file_size += data.size();
            }
            else {
                uint32_t row_stride = bmp_info_header.width * bmp_info_header.bit_count / 8;
                uint32_t new_stride = make_stride_aligned(4);
                std::vector<uint8_t> padding_row(new_stride - row_stride);

                for (int y = 0; y < bmp_info_header.height; ++y) {
                    inp.read((char*)(data.data() + row_stride * y), row_stride);
                    inp.read((char*)padding_row.data(), padding_row.size());
                }
                file_header.file_size += data.size() + bmp_info_header.height * padding_row.size();
            }
        }
        else {
            throw std::runtime_error("Unable to open the input image file.");
        }
        pixels = vectorToMatrix(data, bmp_info_header.height, bmp_info_header.width * 3);
    }


    void write(const char *fname) {
        vector<uint8_t> data = matrixToVector(pixels);
        std::ofstream of{ fname, std::ios_base::binary };
        if (of) {
            if (bmp_info_header.bit_count == 32) {
                write_headers_and_data(of, data);
            }
            else if (bmp_info_header.bit_count == 24) {
                if (bmp_info_header.width % 4 == 0) {
                    write_headers_and_data(of, data);
                }
                else {
                    uint32_t new_stride = make_stride_aligned(4);
                    std::vector<uint8_t> padding_row(new_stride - row_stride);

                    write_headers(of);

                    for (int y = 0; y < bmp_info_header.height; ++y) {
                        of.write((const char*)(data.data() + row_stride * y), row_stride);
                        of.write((const char*)padding_row.data(), padding_row.size());
                    }
                }
            }
            else {
                throw std::runtime_error("The program can treat only 24 or 32 bits per pixel BMP files");
            }
        }
        else {
            throw std::runtime_error("Unable to open the output image file.");
        }
    }

    void convert_to_grayscale() {
        for (int i = 0; i < bmp_info_header.height; i++) {
            for (int j = 0; j < bmp_info_header.width; j++) {
                double b = pixels->getValue(i, 3 * j);
                double g = pixels->getValue(i, 3 * j + 1);
                double r = pixels->getValue(i, 3 * j + 2);
                double grayscale = 0.0722 * b + 0.7152 * g + 0.2126 * r;
                pixels->setValue(i, 3 * j, grayscale);
                pixels->setValue(i, 3 * j + 1, grayscale);
                pixels->setValue(i, 3 * j + 2, grayscale);
            }
        }
    }

    void rotate_image() {
        Matrix* rotated_pixels = new Matrix(pixels->getHeight(), pixels->getWidth());
        for (int i = 0; i < bmp_info_header.height; i++) {
            for (int j = 0; j < bmp_info_header.width; j++) {
                rotated_pixels->setValue(i, 3 * j, pixels->getValue(j, 3 * (bmp_info_header.height - i)));
                rotated_pixels->setValue(i, 3 * j + 1, pixels->getValue(j, 3 * (bmp_info_header.height - i) + 1));
                rotated_pixels->setValue(i, 3 * j + 2, pixels->getValue(j, 3 * (bmp_info_header.height - i) + 2));
            }
        }
        for (int i = 0; i < bmp_info_header.height; i++) {
            for (int j = 0; j < bmp_info_header.width; j++) {
                pixels->setValue(i, 3 * j, rotated_pixels->getValue(i, 3 * j));
                pixels->setValue(i, 3 * j + 1, rotated_pixels->getValue(i, 3 * j + 1));
                pixels->setValue(i, 3 * j + 2, rotated_pixels->getValue(i, 3 * j + 2));
            }
        }
    }
    
private:
    Matrix* vectorToMatrix(vector<uint8_t> vector, int height, int width) {
        Matrix* matrix = new Matrix(height, width);
        for (uint32_t i = 0; i < height; i++) {
            for (uint32_t j = 0; j < width; j++) {
                matrix->setValue(i, j, vector[i * width + j]);
            }
        }
        return matrix;
    }

    vector<uint8_t> matrixToVector(Matrix* matrix) {       
        vector<uint8_t> vector;
        uint32_t channels = bmp_info_header.bit_count / 8;
        vector.resize(matrix->getHeight() *  matrix->getWidth());
        for (uint32_t i = 0; i < matrix->getHeight(); i++) {
            for (uint32_t j = 0; j < matrix->getWidth(); j++) {
                vector.at(i * matrix->getWidth() + j) = matrix->getValue(i, j);
            }
        }
        return vector;
    }
    
    // Check if the pixel data is stored as BGRA and if the color space type is sRGB
    void check_color_header(BMPColorHeader &bmp_color_header) {
        BMPColorHeader expected_color_header;
        if(expected_color_header.red_mask != bmp_color_header.red_mask ||
            expected_color_header.blue_mask != bmp_color_header.blue_mask ||
            expected_color_header.green_mask != bmp_color_header.green_mask ||
            expected_color_header.alpha_mask != bmp_color_header.alpha_mask) {
            throw std::runtime_error("Unexpected color mask format! The program expects the pixel data to be in the BGRA format");
        }
        if(expected_color_header.color_space_type != bmp_color_header.color_space_type) {
            throw std::runtime_error("Unexpected color space type! The program expects sRGB values");
        }
    }

    uint32_t row_stride{ 0 };

    void write_headers(std::ofstream &of) {
        of.write((const char*)&file_header, sizeof(file_header));
        of.write((const char*)&bmp_info_header, sizeof(bmp_info_header));
        if(bmp_info_header.bit_count == 32) {
            of.write((const char*)&bmp_color_header, sizeof(bmp_color_header));
        }
    }

    void write_headers_and_data(std::ofstream &of, vector<uint8_t> data) {
        write_headers(of);
        of.write((const char*)data.data(), data.size());
    }

    // Add 1 to the row_stride until it is divisible with align_stride
    uint32_t make_stride_aligned(uint32_t align_stride) {
        uint32_t new_stride = row_stride;
        while (new_stride % align_stride != 0) {
            new_stride++;
        }
        return new_stride;
    }
};
#pragma pack(pop)

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
    BMP bmp3("lena-color.bmp");
    bmp3.rotate_image();
    bmp3.write("lena-color_fill_region.bmp");
}