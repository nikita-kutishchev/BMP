#include "bmp.h"

#include <format>
#include <fstream>
#include <iostream>

const std::byte BLACK{0x0};

const char cWhite = 'W';
const char cBLACK = ' ';

BMPFile::BMPFile() { data = NULL; }

BMPFile::BMPFile(const std::string& file_name) : BMPFile() { load(file_name); }

void BMPFile::load(const std::string& file_name) {
    std::ifstream fin;
    fin.open(file_name, std::ios::binary);
    if (!fin.is_open()) {
        std::cout << std::format("Can't open {}", file_name) << std::endl;
        exit(0);
    }

    fin.read((char*)&bmph, sizeof(BMPHeader));
    fin.read((char*)&dibh, sizeof(DIBHeader));

    if (dibh.bits_per_pixel != 24 && dibh.bits_per_pixel != 32) {
        std::cout << "Can't work with non 24 or 32 bit image" << std::endl;
        exit(0);
    }

    data = new std::byte[dibh.data_size];
    fin.seekg(bmph.data_offset, fin.beg);
    fin.read((char*)data, dibh.data_size);

    fin.close();
}

BMPFile::~BMPFile() {
    if (data) delete[] data;
}

// Converts (b, g, r) -> (r, g, b) and inverts rows for 24 or 32 bits images
std::byte* BMPFile::convertData() const {
    unsigned int _bytes_per_pixel = dibh.bits_per_pixel >> 3;
    unsigned int _row_size = _bytes_per_pixel * dibh.width;
    unsigned int _size = _row_size * dibh.height;
    unsigned int _pad = (4 - (_row_size % 4)) % 4;

    std::byte* _data = new std::byte[_size];
    std::byte* pData = &data[dibh.data_size - _row_size - _pad];

    if (_bytes_per_pixel == 3) {
        for (size_t i = 0; i < dibh.height; i++) {
            for (size_t pixel = 0; pixel < _row_size; pixel += _bytes_per_pixel) {
                _data[(i * _row_size) + pixel] = *(pData + 2);
                _data[(i * _row_size) + pixel + 1] = *(pData + 1);
                _data[(i * _row_size) + pixel + 2] = *(pData);

                pData += _bytes_per_pixel;
            }

            pData -= (2 * _row_size) + _pad;
        }
    } else if (_bytes_per_pixel == 4) {
        for (size_t i = 0; i < dibh.height; i++) {
            for (size_t pixel = 0; pixel < _row_size; pixel += _bytes_per_pixel) {
                _data[(i * _row_size) + pixel] = *(pData + 3);
                _data[(i * _row_size) + pixel + 1] = *(pData + 2);
                _data[(i * _row_size) + pixel + 2] = *(pData + 1);
                _data[(i * _row_size) + pixel + 3] = *(pData);

                pData += _bytes_per_pixel;
            }

            pData -= (2 * _row_size) + _pad;
        }
    }

    return _data;
}

void BMPFile::displayBMP() const {
    std::byte* _data = convertData();
    unsigned int bytes_per_pixel = dibh.bits_per_pixel / 8;
    unsigned int size = dibh.width * dibh.height * bytes_per_pixel;
    for (size_t i = 0; i < size; i += bytes_per_pixel) {
        if (i % (dibh.width * bytes_per_pixel) == 0 && i != 0) std::cout << std::endl;

        if (_data[i] == BLACK)
            std::cout << std::format("{} ", cBLACK);
        else
            std::cout << std::format("{} ", cWhite);
    }
    delete[] _data;
}