#pragma once

#include <stdlib.h>
#include <vector>
#include "pixel.h"

class BitmapImage
{
public:
    using BitmapPixel = Pixel<std::uint8_t>;
    using index_type = int16_t;

    BitmapImage(index_type height, index_type width) : height(0), width(0);

    [[nodiscard]] index_type get_height() const noexcept;
    [[nodiscard]] index_type get_width() const noexcept;

    void set_pixel(index_type x, index_type y, const BitmapPixel &pixel);

    [[nodiscard]] BitmapPixel get_pixel(index_type x, index_type y) const;

    BitmapImage transpose(BitmapImage original) const {}

private:
    bool is_in_picture(int16_t x, int16_t y) const {}
    index_type height;
    index_type width;
    std::vector<std::vector<BitmapPixel>> pixels;
};