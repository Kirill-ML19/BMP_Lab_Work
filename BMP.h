#ifndef BMP_H
#define BMP_H

#include <string>
#include <vector>
#include <cstdint>

#pragma pack(push, 1)
struct BMPHeader {
    uint16_t bfType;
    uint32_t bfSize;
    uint16_t bfReserved1;
    uint16_t bfReserved2;
    uint32_t bfOffBits;
};

struct DIBHeader {
    uint32_t biSize;
    int32_t  biWidth;
    int32_t  biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    int32_t  biXPelsPerMeter;
    int32_t  biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
};
#pragma pack(pop)

struct RGB {
    uint8_t blue;
    uint8_t green;
    uint8_t red;
};

class BMP {
public:
    BMP();
    bool read(const std::string& filename);
    bool write(const std::string& filename) const;
    void rotate(int angle); // only 90 degrees clockwise
    void blur();

private:
    BMPHeader bmpHeader;
    DIBHeader dibHeader;
    std::vector<std::vector<RGB>> pixels;

    void rotate90();
    std::vector<std::vector<RGB>> getBlurred() const;
};

#endif // BMP_H
