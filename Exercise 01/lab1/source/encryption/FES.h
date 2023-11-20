#pragma once

#include "encryption/Key.h"
#include "image/bitmap_image.h"
#include <iostream>

class FES
{
public:
    using key_type = Key::key_type;
    using row_type = array<BitmapImage::BitmapPixel, 16>;
    using block_type = array<array<row_type, 3>, 3>;

    [[nodiscard]] static block_type convert_key_to_block(key_type &key)
    {
        block_type result;

        //Fill all rows of the block with the same row, created by the key.
        for(int i=0; i<result.size(); i++) {
            result[i][0] = convert_key_to_row(key);
            result[i][1] = convert_key_to_row(key);
            result[i][2] = convert_key_to_row(key);
        }

        return result;
    }

    [[nodiscard]] static row_type convert_key_to_row(key_type &key)
    {
        row_type result;
        int count = 0;

        //Fill all the rows pixel using the ones created according to the bgr-values stored in key.
        for(int i=0; i<key.size(); i = i+3) {
            BitmapImage::BitmapPixel pixel(key[i + 2], key[i + 1], key[i]);
            result[count] = pixel;
            count++;
        }

        return result;
    }

    [[nodiscard]] static row_type combine_rows(row_type &row_1, row_type &row_2)
    {
        BitmapImage::BitmapPixel pixel_1;
        BitmapImage::BitmapPixel pixel_2;

        row_type result;

        //Combine the pixel at index i for all indexes.
        for(int i=0; i<row_1.size(); i++) {
            pixel_1 = row_1[i];
            pixel_2 = row_2[i];

            BitmapImage::BitmapPixel combined(
                    pixel_1.get_red_channel() ^ pixel_2.get_red_channel(),
                    pixel_1.get_green_channel() ^ pixel_2.get_green_channel(),
                    pixel_1.get_blue_channel() ^ pixel_2.get_blue_channel());

            result[i] = combined;
        }

        return result;
    }

    [[nodiscard]] static block_type encrypt_block(block_type &current_block, block_type &previous_block)
    {
        //cout << "reached encrypt block";
        block_type encrypted_block;
        row_type encrypted_row;

        // empty row to for comparison
        row_type empty_row = create_empty_row();

        // checks if the current block should be traversed row-wise or column-wise.
        bool row_wise = true;
        if(current_block[0][2] == empty_row && current_block[2][0] != empty_row) {
            row_wise = false;
        }

        //Iterate through the block.
        for (int y = 0; y < encrypted_block.size(); y++) {
            for (int x = 0; x < encrypted_block.size(); x++) {

                row_type current_row;
                row_type previous_row;

                //Iterate through the block row-wise.
                if(row_wise) {
                    current_row = current_block[y][x];
                    previous_row = previous_block[y][x];
                }
                    //Iterate through the block column-wise.
                else {
                    current_row = current_block[x][y];
                    previous_row = previous_block[x][y];
                }

                // enctypted_row is empty in case current row is empty.
                encrypted_row = empty_row;

                if (current_row != empty_row) {
                    encrypted_row = combine_rows(current_row, previous_row);

                    //Only combine with already encrypted rows if at least 1 exists.
                    if (y != 0 || x != 0) {
                        for (int eY = 0; eY <= y; eY++) {
                            for (int eX = 0; eX <= x; eX++) {
                                //Iterate through the encrypted block row-wise.
                                if(row_wise) {
                                    encrypted_row = combine_rows(encrypted_row, encrypted_block[eY][eX]);
                                }
                                    //Iterate through the encrypted block column-wise.
                                else {
                                    encrypted_row = combine_rows(encrypted_row, encrypted_block[eX][eY]);
                                }
                            }
                        }
                    }
                }

                //Iterate through the blocks row-wise.
                if(row_wise) {
                    encrypted_block[y][x] = encrypted_row;
                }
                    //Iterate through the blocks column-wise.
                else {
                    encrypted_block[x][y] = encrypted_row;
                }
            }
        }

        return encrypted_block;
    }

    [[nodiscard]] static BitmapImage encrypt(BitmapImage &image, key_type &key)
    {
        BitmapImage::index_type w = image.get_width();
        BitmapImage::index_type h = image.get_height();

        // create an empty image for the encrypted result
        BitmapImage encrypted_image(h, w);

        // create block to store encrypted block
        block_type encryptedBlock;

        // create empty row for comparison
        row_type empty_row = create_empty_row();

        // iterate over the blocks of the image
        for (int y = 0; y < h; y += 3){
            //cout << "reached first loop \n";
            // create previous block using the key
            block_type previous_block;
            previous_block = convert_key_to_block(key);

            for (int x = 0; x < w + 47; x += 48)
            {
                //cout << "reached second loop \n";
                // create the current block
                block_type current_block;
                current_block = create_block_from_image(image, x, y);

                encryptedBlock = encrypt_block(current_block, previous_block);

                previous_block = current_block;

                // safe the indexes of the pixel coordinates of the encrypted_image
                int eX = x;
                int eY = y;

                // write encrypted block on encrypted image
                for(int yIndex =0; yIndex<3; yIndex++) {
                    for (int xIndex = 0; xIndex < 3; xIndex++) {

                        // block (eX, eY), that goes to (eX + 48, eY + 3)
                        row_type row = encryptedBlock[yIndex][xIndex];

                        // x-coordinate of the end of the row
                        int end_row = eX + 16;

                        // if row isn't empty, write pixels. Else go to next y-coordinate
                        if (row != empty_row) {

                            // iterate through the row
                            for (int i = 0; i < 16; i++) {
                                //cout << "reached encrypted image setpixel";
                                encrypted_image.set_pixel(eX, eY, row[i]);
                                eX++;

                            }

                            if(eX >= x + 47) {
                                eX = x;
                                eY++;
                            }
                        }
                        else {
                            eX = x;
                            eY++;
                        }
                    }
                }

            }
        }
        return encrypted_image;
    }

    [[nodiscard]] static block_type create_block_from_image(const BitmapImage &image, BitmapImage::index_type start_x, BitmapImage::index_type start_y)
    {
        //cout << "reached create block from image \n";
        block_type block;
        row_type row;

        int y = start_y;
        int x = start_x;
        int w = image.get_width();

        for(int i = 0; i < 3; i++)
        {
            // check block row (index y) is in image
            if(y < image.get_height()) {
                // check whether row (index x) is in image
                if(x < w)
                    row = create_row_from_image(image, x, y);
                    // fill row (index x) with empty row if outside of image
                else
                    row = create_empty_row();

                block[i][0] = row;
                x += 16;

                if(x < w)
                    row = create_row_from_image(image, x, y);
                else
                    row = create_empty_row();

                block[i][1] = row;
                x += 16;

                if(x < w)
                    row = create_row_from_image(image, x, y);
                else
                    row = create_empty_row();

                block[i][2] = row;
            }
                // fill row (index y) with empty rows if outside of image
            else {
                row = create_empty_row();
                block[i][0] = row;
                block[i][1] = row;
                block[i][2] = row;
            }
            // go to next row (index y)
            y++;
            x = start_x;
        }
        return block;
    }

    [[nodiscard]] static row_type create_row_from_image(const BitmapImage &image, BitmapImage::index_type start_x, BitmapImage::index_type start_y)
    {
        //cout << "reached create row from image \n";
        row_type row;
        int index =0;

        for (int x = start_x; x < start_x + 16; x++){
            row[index] = image.get_pixel(x,start_y);
            index++;
        }

        return row;
    }

    [[nodiscard]] static row_type create_empty_row()
    {
        //cout << "reached create empty row \n";
        row_type result;
        int count = 0;

        //Fill all the pixel in the row with zeroes using the default constructor.
        for(int i=0; i<16; i++) {
            BitmapImage::BitmapPixel pixel(0,0,0);
            result[count] = pixel;
            count++;
        }

        return result;
    }
};