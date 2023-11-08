#include "encryption/FES.h"

FES::block_type convert_key_to_block(const FES::key_type &key)
{
    // create an empty block
    FES::block_type block;

    // iterate over the three rows of the block
    for (int i = 0; i < 3; i++)
    {
        // add the row to the block
        block[i][0] = convert_key_to_row(key);
    }

    // return the block
    return block;
}

FES::row_type convert_key_to_row(const FES::key_type &key)
{
    FES::row_type row;

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
}

FES::row_type combine_rows(const FES::row_type &row1, const FES::row_type &row2)
{
    FES::row_type combined_row;
    for (int i = 0; i < 16; i++)
    {   
        // create a combined_pixel according to the definition
        BitmapImage::BitmapPixel combined_pixel(
            row1[i].get_red_channel() ^ row2[i].get_red_channel(),
            row1[i].get_green_channel() ^ row2[i].get_green_channel(),
            row1[i].get_blue_channel() ^ row2[i].get_blue_channel());

        combined_row[i] = combined_pixel;
    }
}

//TODO add handling off edge cases 
FES::block_type encrypt_block(const FES::block_type &current, const FES::block_type &previous)
{
    // create an empty block for the encrypted result
    FES::block_type encrypted;

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
}

BitmapImage encrypt(const BitmapImage &image, const FES::key_type &key)
{

    BitmapImage::index_type w = image.get_height();
    BitmapImage::index_type h = image.get_width();

    // create an empty image for the encrypted result
    BitmapImage encrypted_image(w, h);

    // iterate over the blocks of the image
    for (int y = 0; y < h; y += 3)
    {
        for (int x = 0; x < w; x += 48)
        {
            // create the current and previous block
            FES::block_type current_block, previous_block;

            // add....
        }
    }
    return NULL;
}
