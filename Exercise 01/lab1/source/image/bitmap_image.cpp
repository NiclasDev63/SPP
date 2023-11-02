#include "image/bitmap_image.h"

BitmapImage::BitmapImage(index_type height, index_type width)
{
    if (height == 0 || height > 8192 || width == 0 || width > 8192)
    {
        throw std::exception();
    }
    this->height = height;
    this->width = width;
    pixels.resize(height, std::vector<BitmapPixel>(width));
}

[[nodiscard]] BitmapImage::index_type BitmapImage::get_height() const noexcept { return height; }
[[nodiscard]] BitmapImage::index_type BitmapImage::get_width() const noexcept { return width; }

void BitmapImage::set_pixel(BitmapImage::index_type x, BitmapImage::index_type y, const BitmapImage::BitmapPixel &pixel)
{
    is_in_picture(x, y);
    pixels[y][x] = pixel;
}

[[nodiscard]] BitmapImage::BitmapPixel BitmapImage::get_pixel(BitmapImage::index_type x, BitmapImage::index_type y) const
{
    is_in_picture(x, y);
    return pixels[y][x];
}

BitmapImage BitmapImage::transpose(BitmapImage original) const
{
    BitmapImage transposed(width, height);
    for (index_type i = 0; i < height; ++i)
    {
        for (index_type j = 0; j < width; ++j)
        {
            transposed.set_pixel(j, i, get_pixel(i, j));
        }
    }
    return transposed;
}

bool BitmapImage::is_in_picture(int16_t x, int16_t y) const
{
    return (x < 0 || x >= width || y < 0 || y >= height) ? throw std::exception() : true;
}