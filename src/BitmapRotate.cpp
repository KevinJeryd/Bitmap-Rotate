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

int BitmapRotate::dimensionTransform(std::pair<int, int> coord) {
    return 0;
}

std::pair<int, int> BitmapRotate::rotatePos(uint32_t x, uint32_t y, uint8_t rotation) {
    return std::pair{0, 0};
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

    // Write the header
    for (int i = 0; i < bh.offset; i++) {
        newBmFile.put(bitmap[i]);
    }
    
    // Plan
    // Create empty array, will become new file
    // Pos[0] in empty array will be the rotated value, will have to calculate where that value is in the existing bitmapArr
    // Because the data is laid out as a contigous array of bytes, and not an array of array symbolising every row in the picture, a counter is where we are in 2D space essentially
    // The counter will server as which row we are on "virtually", and go up by one every time we have gone width amount of indices
    // This is needed because the linear algebra operations operator on 2D vectors and we technically also operate in 2D space
    // To calculate the same position as [x, y] in our 1D array we need to convert it using: (No. elements per row * which row) + index on that row

    // Visualisation
    // If we want the value in the 2 row of the image (0th indexed) and index 3 we need to calculate
    // (Number of elements per row * which row) + index, which in the below case would be 4*2+3 = 11 
    // [[1, 0, 0, 1], [0, 0, 0, 1], [0, 1, 1, 0], [1, 1, 1, 1]]

    // Write picture using 2D to 1D conversion
    for (uint32_t row = 0; row < bh.height*3; row++) { // Why the fuck does height*3 produce the correct output??? Is bh.height parsed wrong? No, I print it and it's 420, which is the height... 
        for (uint32_t index = 0; index < bh.width; index++) {
            uint32_t pos = bh.offset;
            std::pair<int, int> rotated2DPos;
            rotated2DPos = rotatePos(row, index, 90);

            pos += (bh.width * row) + index; 
            newBmFile.put(bitmap[pos]);
        }
    }


    return 0;

}