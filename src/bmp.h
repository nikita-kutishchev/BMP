#pragma once
#include <string>

#pragma pack(push, 1)
typedef struct BMPHeader {
    unsigned char id[2];       // ID field (42h, 4Dh)
    unsigned int file_size;    // Size of the BMP file (header + data)
    std::byte _[4];            // Application specific (Unused)
    unsigned int data_offset;  // Offset where the pixel array (bitmap data) can be found
} BMPHeader;

typedef struct DIBHeader {
    unsigned int header_size;       // Number of bytes in the DIB header (from this point)
    unsigned int width;             // Width of the bitmap in pixels
    int height;                     // Height of the bitmap in pixels. Positive for bottom to top pixel order.
    unsigned short planes;          // Number of color planes being used
    unsigned short bits_per_pixel;  // Number of bits per pixel
    unsigned int compression_method;
    unsigned int data_size;    // Size of the raw bitmap data (including padding)
    unsigned int pHorizontal;  // Print resolution of the image
    unsigned int pVertical;
    unsigned int colors;  // Number of colors in the palette
    unsigned int important_colors;
} DIBHeader;
#pragma pack(pop)

class BMPFile {
   private:
    BMPHeader bmph;
    DIBHeader dibh;
    std::byte* data;
    std::byte* convertData() const;

   public:
    BMPFile();
    explicit BMPFile(const std::string& file_name);
    void load(const std::string& file_name);
    void displayBMP() const;
    ~BMPFile();
};