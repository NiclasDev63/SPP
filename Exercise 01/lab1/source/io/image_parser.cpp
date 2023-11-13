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

    if (!file.is_open()) 
    {
        throw exception();
    }

    const int fileHeaderSize = 14;
    const int informationHeaderSize = 40;

    unsigned char fileHeader[fileHeaderSize];
    file.read(reinterpret_cast<char*>(fileHeader), fileHeaderSize);

    //Throws exception, if file is not a bitmap image
    if (fileHeader[0] != 'B' || fileHeader[1] != 'M') {
        throw exception();
    }

    int fileSize = fileHeader[2] + (fileHeader[3] << 8) + (fileHeader[4] << 16) + (fileHeader[5] << 24);
    int bfOffBits = fileHeader[10];

    unsigned char informationHeader[informationHeaderSize];
    file.read(reinterpret_cast<char*>(informationHeader), informationHeaderSize);

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

    //Skips bytes until the image starts
    file.ignore(bfOffBits - fileHeaderSize - informationHeaderSize);

    unsigned char red, green, blue; 
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            file.read(reinterpret_cast<char*>(&blue), 1);
            file.read(reinterpret_cast<char*>(&green), 1);
            file.read(reinterpret_cast<char*>(&red), 1);
            Pixel pixel(red, green, blue);
            image.set_pixel(x, y, pixel);
        }
        file.ignore(padding);
    }
    file.close();

    return image;
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