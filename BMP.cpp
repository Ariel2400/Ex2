#include "Matrix.hpp"
struct BMP{
    BMPHeader header;
    BMPDibHeader DibHeader;
    BMPColorPalette palette;
    Matrix data;
};

struct BMPHeader{
    uint32_t BM;
    uint32_t fileSize;
    uint32_t reserved1;
    uint32_t reserved2;
    uint32_t offset;
};

struct BMPDibHeader
{
    uint32_t headerSize;
    int32_t width;
    int32_t height;
    uint32_t constant;
    uint32_t bitsPerPixel;
    uint32_t compression;
    uint32_t bitmapSize;
    uint32_t horizontalResolution;
    uint32_t verticalResolution;
    uint32_t paletteSize;
    uint32_t importantColors;
};

struct BMPColorPalette
{
    uint32_t blue;
    uint32_t green;
    uint32_t red;
    uint32_t alpha{0x00};
};
