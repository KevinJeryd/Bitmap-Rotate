#ifndef BITMAP_ROTATE_H
#define BITMAP_ROTATE_H

#include <vector>
#include <string>
#include <fstream>
#include <iostream> 
#include <array>

typedef unsigned char Byte;

struct BitmapHeader { 
    std::array<Byte, 2> headerField;
    uint32_t size;
    uint16_t res1;
    uint16_t res2;
    uint32_t offset; // Start address of bitmap image data
};

class BitmapRotate {
    private:
        static std::ifstream openFile(const std::string& filename);
        static void readFile(std::ifstream& bitmapFile, std::vector<Byte>& bitmap);
        static void parseHeader(const std::vector<Byte>& bitmap, BitmapHeader& bh);
    public:
        static int run(const std::string& fileName);
};

#endif