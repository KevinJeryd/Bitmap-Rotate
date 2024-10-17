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

void BitmapRotate::parseHeader(std::ifstream& bitmap, BITMAPFILEHEADER& fileHeader, BITMAPINFOHEADER& infoHeader) {
    if (!bitmap) {
        std::cerr << "Failed to open BMP file." << std::endl;
        return;
    }

    // Read the file header
    bitmap.read(reinterpret_cast<char*>(&fileHeader), sizeof(fileHeader));

    // Verify that this is a BMP file by checking bitmap id
    if (fileHeader.bfType != 0x4D42) { // 'BM' in little-endian
        std::cerr << "Not a valid BMP file." << std::endl;
        return;
    }

    // Read the info header
    bitmap.read(reinterpret_cast<char*>(&infoHeader), sizeof(infoHeader));

    printf("%s %u %s %u %s %u %s", "Size:", infoHeader.biSize, ",Width:", 
           infoHeader.biWidth, ",Height:", infoHeader.biHeight, "\n");
}

int BitmapRotate::dimensionTransform(std::pair<int, int> coord) {
    
    return 0;
}

std::pair<int, int> BitmapRotate::rotatePos(uint32_t x, uint32_t y, uint8_t rotation) {
    return std::pair{0, 0};
}

int BitmapRotate::run(const std::string& fileName) {
    std::vector<Byte> bitmap;
    BITMAPFILEHEADER fileHeader;
    BITMAPINFOHEADER infoHeader;

    std::ifstream bitmapFile = BitmapRotate::openFile(fileName);
    BitmapRotate::readFile(bitmapFile, bitmap); // Maybe remove and just read directly from file
    BitmapRotate::parseHeader(bitmapFile, fileHeader, infoHeader);

    if (fileHeader.bfType != 'BM') {
        std::cerr << "Header not correct, bitmap corrupt" << "\n";
        return 1;
    }

    bool hasAlpha = false;

    if (infoHeader.biBitCount == 32) {
        if (infoHeader.biCompression == 0 || infoHeader.biCompression == 3) {
            // Compression is BI_RGB (0) or BI_BITFIELDS (3)
            if (infoHeader.biSize >= sizeof(BITMAPINFOHEADER)) {
                uint32_t redMask = 0x00FF0000;
                uint32_t greenMask = 0x0000FF00;
                uint32_t blueMask = 0x000000FF;
                uint32_t alphaMask = 0xFF000000;

                if (infoHeader.biCompression == 3) {
                    // Read color masks
                    bitmapFile.read(reinterpret_cast<char*>(&redMask), sizeof(uint32_t));
                    bitmapFile.read(reinterpret_cast<char*>(&greenMask), sizeof(uint32_t));
                    bitmapFile.read(reinterpret_cast<char*>(&blueMask), sizeof(uint32_t));
                    // For BITMAPV4HEADER and above, read alpha mask
                    const uint32_t BITMAPV4HEADER_SIZE = 108;
                    if (infoHeader.biSize >= BITMAPV4HEADER_SIZE) {
                        bitmapFile.read(reinterpret_cast<char*>(&alphaMask), sizeof(uint32_t));
                    } else {
                        alphaMask = 0;
                    }
                }

                if (alphaMask != 0) {
                    hasAlpha = true;
                }
            }
        }
    }

    if (hasAlpha) {
        std::cout << "The BMP file has an alpha channel." << std::endl;
    } else {
        std::cout << "The BMP file does NOT have an alpha channel." << std::endl;
    }

    // Create completely black image test
    std::ofstream newBmFile;
    newBmFile.open("../../image-rotate/newBmFile.bmp", std::ios::binary);

    if (!newBmFile.is_open()) {
        std::cerr << "Failed to create new file" << std::endl;
    }

    // Write the header
    for (int i = 0; i < fileHeader.bfOffBits; i++) {
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
    for (uint32_t row = 0; row < infoHeader.biHeight*3; row++) {  // *3 because every pixel is made up of 3 bytes, one for R, G and B
        for (uint32_t index = 0; index < infoHeader.biWidth; index++) {
            uint32_t pos = fileHeader.bfOffBits;
            std::pair<int, int> rotated2DPos;
            rotated2DPos = rotatePos(row, index, 90);

            pos += (infoHeader.biWidth * row) + index; 
            newBmFile.put(bitmap[pos]);
        }
    }


    return 0;

}
