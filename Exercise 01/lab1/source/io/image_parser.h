#pragma once

#include <filesystem>
#include <fstream>
#include "image/bitmap_image.h"

class ImageParser
{
public:
<<<<<<< Updated upstream
    static BitmapImage read_bitmap(std::filesystem::path path);
    static void write_bitmap(std::filesystem::path path, const BitmapImage &image);
    static unsigned char *generate_bitmap_file_header(BitmapImage::index_type height, BitmapImage image);
    static unsigned char *generate_bitmap_info_header(BitmapImage::index_type width, BitmapImage::index_type height);
    == == == =
                 static BitmapImage read_bitmap(std::filesystem::path path)
    {
        if (!filesystem::exists(path))
        {
            throw exception();
        }
        ifstream file(path, ios::binary);

        if (!file.is_open())
        {
            throw exception();
        }

        unsigned char fileHeader[FILE_HEADER_SIZE];
        file.read(reinterpret_cast<char *>(fileHeader), FILE_HEADER_SIZE);

        // Throws exception, if file is not a bitmap image
        if (fileHeader[0] != 'B' || fileHeader[1] != 'M')
        {
            throw exception();
        }

        int fileSize = fileHeader[2] + (fileHeader[3] << 8) + (fileHeader[4] << 16) + (fileHeader[5] << 24);
        int bfOffBits = fileHeader[10] + (fileHeader[11] << 8) + (fileHeader[12] << 16) + (fileHeader[13] << 24);

        unsigned char informationHeader[INFORMATION_HEADER_SIZE];
        file.read(reinterpret_cast<char *>(informationHeader), INFORMATION_HEADER_SIZE);

        int width = informationHeader[4] + (informationHeader[5] << 8) + (informationHeader[6] << 16) + (informationHeader[7] << 24);
        int height = informationHeader[8] + (informationHeader[9] << 8) + (informationHeader[10] << 16) + (informationHeader[11] << 24);

        if (width <= 0 || height <= 0 || bfOffBits <= 0)
        {
            throw exception();
        }

        BitmapImage image(height, width);

        /*
         * Calculate padding.
         * There are biWidth amount of pixels per row. Each pixel contains 3 byte (one per color).
         * Therefore, 4 - (biWidth * 3) % 4 gives back the padding.
         * If (biWidth * 3) % 4 equals 0, then the padding is 4, which is redundant.
         * This can be corrected by another % 4, resulting in the formula.
         */
        const int padding = ((4 - (width * 3) % 4) % 4);

        // Skips bytes until the image starts
        file.ignore(bfOffBits - FILE_HEADER_SIZE - INFORMATION_HEADER_SIZE);

        unsigned char red, green, blue;
        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                file.read(reinterpret_cast<char *>(&red), 1);
                file.read(reinterpret_cast<char *>(&green), 1);
                file.read(reinterpret_cast<char *>(&blue), 1);
                Pixel pixel(red, green, blue);
                image.set_pixel(x, y, pixel);
            }
            file.ignore(padding);
        }
        file.close();

        return image;
    };

    static void write_bitmap(std::filesystem::path path, const BitmapImage &image)
    {
        // Commented out because of unexpected exception
        /*      if (!filesystem::exists(path))
              {
                  printf("testAA");
                  throw exception();
              }*/

        ofstream file(path, ios::binary);

        const int w = image.get_width();
        const int h = image.get_height();

        std::vector<unsigned char> file_header = generate_bitmap_file_header(w, h);
        std::vector<unsigned char> info_header = generate_bitmap_info_header(w, h);

        file.write(reinterpret_cast<char *>(file_header.data()), FILE_HEADER_SIZE);
        file.write(reinterpret_cast<char *>(info_header.data()), INFORMATION_HEADER_SIZE);

        const int padding = ((4 - (width * 3) % 4) % 4);

        for (int i = 0; i < w; i++)
        {
            for (int j = 0; j < h; j++)
            {
                Pixel pixel = image.get_pixel(i, j);
                BitmapImage::index_type color[] = {pixel.get_red_channel(), pixel.get_green_channel(), pixel.get_blue_channel()};
                file.write(reinterpret_cast<char *>(color), 3);
            }
            // write padding for each row
            file.write(reinterpret_cast<char *>(&padding), padding);
        }

        file.close();
    };

    static std::vector<unsigned char> generate_bitmap_file_header(BitmapImage::index_type width, BitmapImage::index_type height)
    {
        const int padding = ((4 - (width * 3) % 4) % 4);

        const int file_size = FILE_HEADER_SIZE + INFORMATION_HEADER_SIZE + (3 * width * height) + (padding * height);

        std::vector<unsigned char> file_header(FILE_HEADER_SIZE, 0);

        file_header[0] = (unsigned char)('B');
        file_header[1] = (unsigned char)('M');
        file_header[2] = (unsigned char)(file_size);
        file_header[3] = (unsigned char)(file_size >> 8);
        file_header[4] = (unsigned char)(file_size >> 16);
        file_header[5] = (unsigned char)(file_size >> 24);
        file_header[10] = (unsigned char)(54);

        return file_header;
    };

    static std::vector<unsigned char> generate_bitmap_info_header(BitmapImage::index_type width, BitmapImage::index_type height)
    {
        std::vector<unsigned char> information_header(INFORMATION_HEADER_SIZE, 0);

        // Information header size
        information_header[0] = INFORMATION_HEADER_SIZE;
        information_header[1] = 0;
        information_header[2] = 0;
        information_header[3] = 0;

        // Width
        information_header[4] = width;
        information_header[5] = width >> 8;
        information_header[6] = width >> 16;
        information_header[7] = width >> 24;

        // Height
        information_header[8] = height;
        information_header[9] = height >> 8;
        information_header[10] = height >> 16;
        information_header[11] = height >> 24;

        // Planes
        information_header[12] = 1;
        information_header[13] = 0;

        // Bpp (Bits per pixel)
        information_header[14] = 24;
        information_header[15] = 0;

        // Compression
        information_header[16] = 0;
        information_header[17] = 0;
        information_header[18] = 0;
        information_header[19] = 0;

        // Size image (0 because no compression)
        information_header[20] = 0;
        information_header[21] = 0;
        information_header[22] = 0;
        information_header[23] = 0;

        // X pixels per meter (no specification)
        information_header[24] = 0;
        information_header[25] = 0;
        information_header[26] = 0;
        information_header[27] = 0;

        // Y pixels per meter (no specification)
        information_header[28] = 0;
        information_header[29] = 0;
        information_header[30] = 0;
        information_header[31] = 0;

        // Colors used (no color palette)
        information_header[32] = 0;
        information_header[33] = 0;
        information_header[34] = 0;
        information_header[35] = 0;

        // Important colors (no color palette)
        information_header[36] = 0;
        information_header[37] = 0;
        information_header[38] = 0;
        information_header[39] = 0;

        return information_header;
    };
>>>>>>> Stashed changes
};