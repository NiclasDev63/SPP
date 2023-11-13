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

    static block_type convert_key_to_block(const key_type &key) {
        // create an empty block
        FES::block_type block;

        // iterate over the three rows of the block
        for (int i = 0; i < 3; i++)
        {
            // add the row to the block
            block[i][0] = FES::convert_key_to_row(key);
        }

        // return the block
        return block;
    };
    static row_type convert_key_to_row(const key_type &key) {
        row_type row;

        // iterate over the 16 pixels of the row
        for (int i = 0; i < 16; i++)
        {
            // calculate the index of the first byte of the pixel in the key
            int index = i * 3;

            // create a pixel from the three bytes of the key
            // The order of the color channels is blue, green, red
            BitmapImage::BitmapPixel pixel(key[index + 2], key[index + 1], key[index]);

            // add the pixel to the row
            row[i] = pixel;
        }

        // return the row
        return row;
    };
    static row_type combine_rows(const row_type &row1, const row_type &row2) {
        row_type combined_row;
        for (int i = 0; i < 16; i++)
        {
            // create a combined_pixel according to the definition
            BitmapImage::BitmapPixel combined_pixel(
                row1[i].get_red_channel() ^ row2[i].get_red_channel(),
                row1[i].get_green_channel() ^ row2[i].get_green_channel(),
                row1[i].get_blue_channel() ^ row2[i].get_blue_channel());

            combined_row[i] = combined_pixel;
        }
        return combined_row;
    };


    //TODO add handling off edge cases 
    static block_type encrypt_block(const block_type &current, const block_type &previous) {
        // create an empty block for the encrypted result
        block_type encrypted;

        // iterate over the three rows of the block
        for (int i = 0; i < 3; i++)
        {
            // get the current and previous rows
            FES::row_type current_row = current[i][0];
            FES::row_type previous_row = previous[i][0];

            // combine the current and previous rows
            FES::row_type combined_row = combine_rows(current_row, previous_row);

            // combine the resulting row with all already encrypted rows
            for (int j = 0; j < i; j++)
            {
                for (int k = 0; k < 3; k++)
                {
                    combined_row = combine_rows(combined_row, encrypted[j][k]);
                }
            }

            // store the encrypted row in the encrypted block
            encrypted[i][0] = combined_row;
        }

        // return the encrypted block
        return encrypted;
    };
    static block_type encrypt(const BitmapImage &image, const key_type &key) {

        BitmapImage::index_type w = image.get_height();
        BitmapImage::index_type h = image.get_width();

        // create an empty image for the encrypted result
        BitmapImage encrypted_image(w, h);

        block_type emptyBlock;

        // iterate over the blocks of the image
        for (int y = 0; y < h; y += 3)
        {
            for (int x = 0; x < w; x += 48)
            {
                // create the current and previous block
                block_type current_block, previous_block;

                // add....
            }
        }
        return emptyBlock;
    };
};
