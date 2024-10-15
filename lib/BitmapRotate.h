#ifndef BITMAP_ROTATE_H
#define BITMAP_ROTATE_H

#include <vector>
#include <string>
#include <fstream>
#include <iostream> 
#include <array>

typedef unsigned char Byte;

// Structure for windows bitmap header specifically, see https://en.wikipedia.org/wiki/BMP_file_format#DIB_header_(bitmap_information_header)
struct BitmapHeader { 
    std::array<Byte, 2> headerField;        // Tells us what type of bitmap header
    int32_t size = 0;                       // Size in bytes of the image
    uint16_t res1;
    uint16_t res2;
    uint32_t offset = 0;                    // Start address of bitmap image data
    uint32_t width = 0;                     // Width of image
    uint32_t height = 0;                    // Height of image
};

class BitmapRotate {
    private:
        static int dimensionTransform(std::pair<int, int> coord);
        static std::pair<int, int> rotatePos(const uint32_t x, const uint32_t y, uint8_t rotation); 
        static std::ifstream openFile(const std::string& filename);
        static void readFile(std::ifstream& bitmapFile, std::vector<Byte>& bitmap);
        static void parseHeader(const std::vector<Byte>& bitmap, BitmapHeader& bh);
    public:
        static int run(const std::string& fileName);
};

#endif