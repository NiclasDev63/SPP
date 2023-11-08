#pragma once
#include "encryption/Key.h"
#include "image/bitmap_image.h"

using namespace std;

class FES
{
public:
    using key_type = Key::key_type;
    using row_type = std::array<BitmapImage::BitmapPixel, 16>;
    using block_type = array<array<row_type, 3>, 3>;

    static block_type convert_key_to_block(const key_type &key);
    static row_type convert_key_to_row(const key_type &key);
    static block_type combine_rows(const row_type &row1, const row_type &row2);
    static block_type encrypt_block(const block_type &current, const block_type &previous);
    static block_type encrypt(const BitmapImage &image, const key_type &key);
};
