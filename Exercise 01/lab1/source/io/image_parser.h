#pragma once

#include <filesystem>
#include <fstream>
#include "image/bitmap_image.h"

class ImageParser
{
public:
    static BitmapImage read_bitmap(std::filesystem::path path);
};