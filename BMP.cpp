#include "BMP.h"
#include <fstream>
#include <iostream>
#include <cmath>

BMP::BMP() = default;

bool BMP::read(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) return false;

    file.read(reinterpret_cast<char*>(&bmpHeader), sizeof(bmpHeader));
    file.read(reinterpret_cast<char*>(&dibHeader), sizeof(dibHeader));

    if (bmpHeader.bfType != 0x4D42 || dibHeader.biBitCount != 24 || dibHeader.biCompression != 0)
        return false;

    int width = dibHeader.biWidth;
    int height = std::abs(dibHeader.biHeight);
    int rowSize = (3 * width + 3) & ~3;
    pixels.assign(height, std::vector<RGB>(width));

    file.seekg(bmpHeader.bfOffBits, std::ios::beg);
    for (int y = height - 1; y >= 0; --y) {
        file.read(reinterpret_cast<char*>(pixels[y].data()), 3 * width);
        file.ignore(rowSize - 3 * width);
    }

    return true;
}

bool BMP::write(const std::string& filename) const {
    std::ofstream file(filename, std::ios::binary);
    if (!file) return false;

    file.write(reinterpret_cast<const char*>(&bmpHeader), sizeof(bmpHeader));
    file.write(reinterpret_cast<const char*>(&dibHeader), sizeof(dibHeader));

    int width = dibHeader.biWidth;
    int height = std::abs(dibHeader.biHeight);
    int rowSize = (3 * width + 3) & ~3;
    std::vector<uint8_t> padding(rowSize - 3 * width, 0);

    for (int y = height - 1; y >= 0; --y) {
        file.write(reinterpret_cast<const char*>(pixels[y].data()), 3 * width);
        file.write(reinterpret_cast<const char*>(padding.data()), padding.size());
    }

    return true;
}

void BMP::rotate(int angle) {
    int oldHeight = pixels.size();
    int oldWidth = pixels[0].size();

    std::vector<std::vector<RGB>> rotated;

    if (angle == 90) {
        rotated.resize(oldWidth, std::vector<RGB>(oldHeight));
        for (int y = 0; y < oldHeight; ++y)
            for (int x = 0; x < oldWidth; ++x)
                rotated[x][oldHeight - 1 - y] = pixels[y][x];

        std::swap(dibHeader.biWidth, dibHeader.biHeight);
    } else if (angle == 180) {
        rotated.resize(oldHeight, std::vector<RGB>(oldWidth));
        for (int y = 0; y < oldHeight; ++y)
            for (int x = 0; x < oldWidth; ++x)
                rotated[oldHeight - 1 - y][oldWidth - 1 - x] = pixels[y][x];

    } else if (angle == 270) {
        rotated.resize(oldWidth, std::vector<RGB>(oldHeight));
        for (int y = 0; y < oldHeight; ++y)
            for (int x = 0; x < oldWidth; ++x)
                rotated[oldWidth - 1 - x][y] = pixels[y][x];

        std::swap(dibHeader.biWidth, dibHeader.biHeight);
    } else {
        std::cerr << "Unsupported rotation angle: " << angle << std::endl;
        return;
    }

    pixels = std::move(rotated);
}

void BMP::blur() {
    const float kernel[3][3] = {
        {1.f / 16, 2.f / 16, 1.f / 16},
        {2.f / 16, 4.f / 16, 2.f / 16},
        {1.f / 16, 2.f / 16, 1.f / 16}
    };

    std::vector<std::vector<RGB>> newPixels = pixels;
    int height = pixels.size();
    int width = pixels[0].size();

    for (int y = 1; y < height - 1; ++y) {
        for (int x = 1; x < width - 1; ++x) {
            float sumR = 0, sumG = 0, sumB = 0;
            
            for (int dy = -1; dy <= 1; ++dy) {
                for (int dx = -1; dx <= 1; ++dx) {
                    const RGB& p = pixels[y + dy][x + dx];
                    float k = kernel[dy + 1][dx + 1];

                    sumR += p.red * k;
                    sumG += p.green * k;
                    sumB += p.blue * k;
                }
            }

            newPixels[y][x].red   = static_cast<uint8_t>(sumR);
            newPixels[y][x].green = static_cast<uint8_t>(sumG);
            newPixels[y][x].blue  = static_cast<uint8_t>(sumB);
        }
    }

    pixels = newPixels;
}
