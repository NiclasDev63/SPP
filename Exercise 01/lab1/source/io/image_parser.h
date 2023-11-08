#pragma once

#include <filesystem>
#include <fstream>
#include "image/bitmap_image.h"

class ImageParser
{
public:
    static BitmapImage read_bitmap(std::filesystem::path path);
    static void write_bitmap(std::filesystem::path path, const BitmapImage &image);
    static unsigned char *generate_bitmap_file_header(BitmapImage::index_type height, BitmapImage image);
    static unsigned char *generate_bitmap_info_header(BitmapImage::index_type width, BitmapImage::index_type height);
};