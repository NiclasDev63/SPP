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

    [[nodiscard]] static block_type convert_key_to_block(key_type& key)
    {
        block_type result;

        //Fill all rows of the block with the same row, created by the key.
        for (int i = 0; i < result.size(); i++) {
            result[i][0] = convert_key_to_row(key);
            result[i][1] = convert_key_to_row(key);
            result[i][2] = convert_key_to_row(key);
        }

        return result;
    }

    [[nodiscard]] static row_type convert_key_to_row(key_type& key)
    {
        row_type result;
        int count = 0;

        //Fill all the rows pixel using the ones created according to the bgr-values stored in key.
        for (int i = 0; i < key.size(); i = i + 3) {
            BitmapImage::BitmapPixel pixel(key[i + 2], key[i + 1], key[i]);
            result[count] = pixel;
            count++;
        }

        return result;
    }

    [[nodiscard]] static row_type combine_rows(row_type& row_1, row_type& row_2)
    {
        BitmapImage::BitmapPixel pixel_1;
        BitmapImage::BitmapPixel pixel_2;

        row_type result;

        //Combine the pixel at index i for all indexes.
        for (int i = 0; i < 16; i++) {
            pixel_1 = row_1[i];
            pixel_2 = row_2[i];

            BitmapImage::BitmapPixel combined(
                pixel_1.get_blue_channel() ^ pixel_2.get_blue_channel(),
                pixel_1.get_green_channel() ^ pixel_2.get_green_channel(),
                pixel_1.get_red_channel() ^ pixel_2.get_red_channel());

            result[i] = combined;
        }

        return result;
    }

    //TODO: RERERRE

    [[nodiscard]] static block_type encrypt_block(block_type& current_block, block_type& previous_block)
    {
        //cout << "reached encrypt block";
        block_type encrypted_block;
        row_type encrypted_row;

        // empty row to for comparison
        row_type empty_row = create_empty_row();

        row_type current_row;
        row_type previous_row;


        for (int y = 0; y < 3; y++) {
            for (int x = 0; x < 3; x++) {
                // traverse blocks row- or column-wise
                current_row = current_block[y][x];
                previous_row = previous_block[y][x];


                // set encrypted row to combination of current and previous if current isn't empty
                encrypted_row = combine_rows(current_row, previous_row);

                // combine encrypted row with previously encrypted rows
                if (y != 0 || x != 0) {
                    // if encrypted block is traversed row-wise
                    for (int eY = 0; eY <= y; eY++) {
                        for (int eX = 0; eX < 3; eX++) {
                            // break if the coordinates of the currently encrypted row are reached
                            if (eY == y && eX == x)
                                break;
                            encrypted_row = combine_rows(encrypted_row, encrypted_block[eY][eX]);

                        }
                    }
                }

                // write encrypted row in encrypted block
                encrypted_block[y][x] = encrypted_row;

            }
        }

        return encrypted_block;
    }

    [[nodiscard]] static BitmapImage encrypt(BitmapImage& image, key_type& key)
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
        for (int y = 0; y < h; y += 3) {
            //cout << "reached first loop \n";
            // create previous block using the key
            block_type previous_block;
            previous_block = convert_key_to_block(key);

            for (int x = 0; x < w; x += 48)
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
                int last_x = x + 48;

                // write encrypted block on encrypted image
                for (int yIndex = 0; yIndex < 3; yIndex++) {
                    for (int xIndex = 0; xIndex < 3; xIndex++) {

                        // block (eX, eY), that goes to (eX + 48, eY + 3)
                        row_type row = encryptedBlock[yIndex][xIndex];

                        if (row != empty_row) {
                            // if row isn't empty, write pixels. Else go to next y-coordinate
                            if (eX < w && eY < h) {
                                // iterate through the row
                                for (int i = 0; i < 16; i++) {
                                    //cout << "reached encrypted image setpixel";
                                    encrypted_image.set_pixel(eX, eY, row[i]);
                                    eX++;
                                }
                            }
                            else {
                                eX = x;
                                eY++;
                            }

                            if (eX >= last_x) {
                                eX = x;
                                eY++;
                            }
                        }
                    }
                }

            }
        }
        return encrypted_image;
    }

    [[nodiscard]] static block_type create_block_from_image(const BitmapImage& image, BitmapImage::index_type start_x, BitmapImage::index_type start_y)
    {
        //cout << "reached create block from image \n";
        block_type block;
        row_type row;

        int y = start_y;
        int x = start_x;
        int w = image.get_width();

        for (int i = 0; i < 3; i++)
        {
            // check block row (index y) is in image
            if (y < image.get_height()) {
                // check whether row (index x) is in image
                if (x < w)
                    row = create_row_from_image(image, x, y);
                // fill row (index x) with empty row if outside of image
                else
                    row = create_empty_row();

                block[i][0] = row;
                x += 16;

                if (x < w)
                    row = create_row_from_image(image, x, y);
                else
                    row = create_empty_row();

                block[i][1] = row;
                x += 16;

                if (x < w)
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

    [[nodiscard]] static row_type create_row_from_image(const BitmapImage& image, BitmapImage::index_type start_x, BitmapImage::index_type start_y)
    {
        //cout << "reached create row from image \n";
        row_type row;
        int index = 0;

        for (int x = start_x; x < start_x + 16; x++) {
            row[index] = image.get_pixel(x, start_y);
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
        for (int i = 0; i < 16; i++) {
            BitmapImage::BitmapPixel pixel(0, 0, 0);
            result[count] = pixel;
            count++;
        }

        return result;
    }
};