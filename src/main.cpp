#include <format>
#include <iostream>
#include <string>

#include "bmp.h"

int main(int argc, const char* argv[]) {
    if (argc < 2) {
        std::cout << "no file is entered" << std::endl;
        return 1;
    }

    BMPFile file(argv[1]);
    file.displayBMP();

    return 0;
}