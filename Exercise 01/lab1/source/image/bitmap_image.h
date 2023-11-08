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

    BitmapImage(index_type height = 0, index_type width = 0);

    [[nodiscard]] index_type get_height() const noexcept;
    [[nodiscard]] index_type get_width() const noexcept;

    void set_pixel(index_type x, index_type y, const BitmapPixel &pixel);

    [[nodiscard]] BitmapPixel get_pixel(index_type x, index_type y) const;

    BitmapImage transpose(const BitmapImage &original) const;

private:
    bool is_in_picture(BitmapImage::index_type x, BitmapImage::index_type y) const;
    index_type height;
    index_type width;
    vector<vector<BitmapPixel>> pixels;
};