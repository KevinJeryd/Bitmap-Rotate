#ifndef BITMAP_ROTATE_H
#define BITMAP_ROTATE_H

#include <vector>
#include <string>
#include <fstream>
#include <iostream> 
#include <array>

typedef unsigned char Byte;

struct Pixel {
  uint8_t R;
  uint8_t G;
  uint8_t B;
};

#pragma pack(push, 1)
struct BITMAPFILEHEADER {
    uint16_t bfType;      // File type, must be 'BM'
    uint32_t bfSize;      // Size of the file in bytes
    uint16_t bfReserved1; // Reserved, must be zero
    uint16_t bfReserved2; // Reserved, must be zero
    uint32_t bfOffBits;   // Offset to start of pixel data
};

struct BITMAPINFOHEADER {
    uint32_t biSize;          // Size of this header
    int32_t  biWidth;         // Width of bitmap in pixels
    int32_t  biHeight;        // Height of bitmap in pixels
    uint16_t biPlanes;        // Number of color planes, must be 1
    uint16_t biBitCount;      // Bits per pixel
    uint32_t biCompression;   // Compression type
    uint32_t biSizeImage;     // Size of image data in bytes
    int32_t  biXPelsPerMeter; // Horizontal resolution
    int32_t  biYPelsPerMeter; // Vertical resolution
    uint32_t biClrUsed;       // Number of colors used
    uint32_t biClrImportant;  // Number of important colors
};

#pragma pack(pop)

class BitmapRotate {
    private:
        static int dimensionTransform(std::pair<int, int> coord);
        static std::pair<int, int> rotatePos(const uint32_t x, const uint32_t y, uint8_t rotation); 
        static std::ifstream openFile(const std::string& filename);
        static void readFile(std::ifstream& bitmapFile, std::vector<Byte>& bitmap);
        static void parseHeader(std::ifstream& bitmap, BITMAPFILEHEADER& fileHeader, BITMAPINFOHEADER& infoHeader);
    public:
        static int run(const std::string& fileName);
};

#endif
