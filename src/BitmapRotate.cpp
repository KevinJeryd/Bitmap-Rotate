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

        /* // Reading individual bits
        for (int i = 7; i >= 0; i--) {
            bitmap.push_back((byte >> i) & 1); // Go through every bit in the byte and store it
        }
        */
    }
}

void BitmapRotate::parseHeader(const std::vector<Byte>& bitmap, BitmapHeader& bh) {
    bh.headerField[0] = bitmap[0];
    bh.headerField[1] = bitmap[1];

    bh.size = 0;
    bh.offset = 0;

    for (int i = 5; i >= 2; i--) {  // Gives correct answer now, but doesn't make sense. It's in opposite order of little end
        printf("%02x", bitmap[i]);
        bh.size <<= 8;
        bh.size |=  bitmap[i];
        printf("%s %x", " ", bh.size);
        printf("\n");
    }


    printf("%x", bh.size);
    printf("%s", " ");  // Prints 813ba, but should be ba130800 if little endian imo, how does it read? I thought little endian
                        // had least significant byte all the way to the left...?
    printf("%u", bh.size); 


    for (int i = 13; i >= 10; i--) {
        bh.offset <<= 8;
        bh.offset |= bitmap[i];
    }

    printf("\n");
    printf("%x", bh.offset);
    printf("\n");

    // Extract width of image
    for (int i = 21; i >= 18; i--) {
        bh.width <<= 8;
        bh.width |= bitmap[i];
    }

    // Extract height of image
    for (int i = 25; i >= 22; i--) {
        bh.height <<= 8;
        bh.height |= bitmap[i];
    }

    printf("%s %u %s %u %s", "Width: ", bh.width, ", Height: ", bh.height, "\n");
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

    // Data is getting read correctly, very nice
    for (int i = 0; i < 6; i++) {
        printf("%x", bitmap[bh.offset + i]);
        if (i == 2) {
            std::cout << std::endl;
        }
    }


}