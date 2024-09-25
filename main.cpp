#include "lib/BitmapRotate.h"
#include <fstream>
#include <iostream>
#include <vector>

int main() {
    std::string fileName = "../../image-rotate/teapot.bmp";

    return BitmapRotate::run(fileName);
}