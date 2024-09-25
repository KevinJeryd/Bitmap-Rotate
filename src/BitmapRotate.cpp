#include "BitmapRotate.h"

std::ifstream BitmapRotate::openFile(const std::string& fileName) {
    std::ifstream bitmapFile;
    bitmapFile.open(fileName, std::ios::binary);

    if (!bitmapFile.is_open()) {
        std::cerr << "Failed to open bitmap" << '\n';
        return {};
    } 

    return bitmapFile;
}

void BitmapRotate::readFile(std::ifstream& bitmapFile, std::vector<Byte>& bitmap) {
    char byte;

    while (bitmapFile.get(byte)) {
        bitmap.push_back(byte);        
    }

    bitmapFile.close();
}

void BitmapRotate::parseHeader(const std::vector<Byte>& bitmap, BitmapHeader& bh) {
    bh.headerField[0] = bitmap[0];
    bh.headerField[1] = bitmap[1];

    // Extract image size
    for (int i = 2; i < 6; i++) { 
        bh.size |= bitmap[i] << 8*(i-2);
    }

    // Extract where image data starts
    for (int i = 10; i < 14; i++) {
        bh.offset |= bitmap[i] << 8*(i-10);
    }

    // Extract width of image
    for (int i = 18; i < 22; i++) {
        bh.width |= bitmap[i] << 8*(i-18);
    }

    // Extract height of image
    for (int i = 22; i < 26; i++) {
        bh.height |= bitmap[i] << 8*(i-22);
    }

    printf("%s %u %s %u %s %u %s", "Size:", bh.size, ",Width:", bh.width, ",Height:", bh.height, "\n");
}

int BitmapRotate::run(const std::string& fileName) {
    std::vector<Byte> bitmap;
    BitmapHeader bh;

    std::ifstream bitmapFile = BitmapRotate::openFile(fileName);
    BitmapRotate::readFile(bitmapFile, bitmap);
    BitmapRotate::parseHeader(bitmap, bh);

    if (bh.headerField[0] != 'B' && bh.headerField[1] != 'M') {
        std::cerr << "Header not correct, bitmap corrupt" << "\n";
        return 1;
    }

    // Create completely black image test
    std::ofstream newBmFile;
    newBmFile.open("../../image-rotate/newBmFile.bmp", std::ios::binary);

    if (!newBmFile.is_open()) {
        std::cerr << "Failed to create new file" << std::endl;
    }

    for (int i = 0; i < bh.offset; i++) {
        newBmFile.put(bitmap[i]);
    }
    
    for (int i = bh.offset; i < bitmap.size(); i++) {
        newBmFile.put(0x00);
    }

    return 0;

}