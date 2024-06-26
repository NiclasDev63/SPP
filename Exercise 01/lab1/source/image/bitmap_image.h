#pragma once

#include <stdlib.h>
#include <vector>
#include "pixel.h"

using namespace std;


class BitmapImage
{
public:
    using BitmapPixel = Pixel<uint8_t>;
    using index_type = int16_t;

    BitmapImage(index_type height = 0, index_type width = 0) {
        if (height == 0 || height > 8192 || width == 0 || width > 8192)
        {
            throw exception();
        }
        this->height = height;
        this->width = width;
        pixels.resize(height, std::vector<BitmapPixel>(width));
    };

    [[nodiscard]] index_type get_height() const noexcept { return height; };
    [[nodiscard]] index_type get_width() const noexcept { return width; };

    void set_pixel(index_type x, index_type y, const BitmapPixel &pixel) {
        is_in_picture(x, y);
        pixels[y][x] = pixel;
    };

    [[nodiscard]] BitmapPixel get_pixel(index_type x, index_type y) const {
        is_in_picture(x, y);
        return pixels[y][x];
    };

    BitmapImage transpose(const BitmapImage &original) const {
        BitmapImage transposed(width, height);
        for (index_type i = 0; i < height; ++i)
        {
            for (index_type j = 0; j < width; ++j)
            {
                transposed.set_pixel(j, i, get_pixel(i, j));
            }
        }
        return transposed;
    };

private:
    bool is_in_picture(BitmapImage::index_type x, BitmapImage::index_type y) const {
        return (x < 0 || x >= width || y < 0 || y >= height) ? throw std::exception() : true;
    };
    index_type height;
    index_type width;
    vector<vector<BitmapPixel>> pixels;
};