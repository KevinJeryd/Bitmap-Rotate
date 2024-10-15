#include "lib/BitmapRotate.h"
#include <fstream>
#include <iostream>
#include <vector>

int main() {
    // TODO: Take rotation amount as input
    std::string fileName = "../../image-rotate/teapot.bmp";
    return BitmapRotate::run(fileName);
}