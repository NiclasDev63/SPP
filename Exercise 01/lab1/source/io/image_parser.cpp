#include <filesystem>
#include <fstream>
#include "image_parser.h"
#include "image/bitmap_image.h"
#include "image/pixel.h"

// Noch nicht fertig, nicht sicher wie genau bmp datei eingelesen werden soll
BitmapImage ImageParser::read_bitmap(std::filesystem::path path)
{
    if (!filesystem::exists(path))
    {
        throw exception();
    }
    ifstream file(path, ios::binary);
    if (file.is_open())
    {

        // read in bmp header
        char header[54];
        file.read(header, 54);

        int width = *reinterpret_cast<int *>(&header[18]);
        int height = *reinterpret_cast<int *>(&header[22]);
        int bfOffBits = *reinterpret_cast<int *>(&header[10]);

        if (width <= 0 || height <= 0 || bfOffBits <= 0)
        {
            throw runtime_error("Ungültige Datei");
        }

        BitmapImage image = BitmapImage(height, width);

        char r, g, b;
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                file.get(r);
                file.get(g);
                file.get(b);
                BitmapImage::BitmapPixel pixel = BitmapImage::BitmapPixel(r, g, b);
                image.set_pixel(i, j, pixel);
            }
        }
        file.close();

        return image;
    }
}

void ImageParser::write_bitmap(std::filesystem::path path, const BitmapImage &image)
{
    if (!filesystem::exists(path))
    {
        throw exception();
    }

    // for(int i = 0; i < w; i++) {
    //     for(int j = 0; j < h; j++) {

    //     }
    // }
}

unsigned char *ImageParser::generate_bitmap_file_header(BitmapImage::index_type height, BitmapImage image)
{
    BitmapImage::index_type w = image.get_height();
    BitmapImage::index_type h = image.get_width();

    // 54 header bytes + 3 byte per pixel
    size_t file_size = 54 + (3 * w * h);

    static unsigned char file_header[] = {
        0, 0,       /// signature
        0, 0, 0, 0, /// image file size in bytes
        0, 0, 0, 0, /// reserved
        0, 0, 0, 0, /// start of pixel array
    };

    file_header[0] = (unsigned char)('B');
    file_header[1] = (unsigned char)('M');
    file_header[2] = (unsigned char)(file_size);
    file_header[3] = (unsigned char)(file_size >> 8);
    file_header[4] = (unsigned char)(file_size >> 16);
    file_header[5] = (unsigned char)(file_size >> 24);
    file_header[10] = (unsigned char)(54);

    return file_header;
}