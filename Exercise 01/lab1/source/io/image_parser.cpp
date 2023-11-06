#include <filesystem>
#include <fstream>
#include "image_parser.h"
#include "image/bitmap_image.h"
#include "pixel.h"

using namespace std;

BitmapImage ImageParser::read_bitmap(filesystem::path path)
{
    if (!filesystem::exists(path))
    {
        throw exception();
    }
    ifstream file(path, ios::binary);
    if (file.is_open())
    {
        char header[54];
        file.read(header, 54);

        int width = *reinterpret_cast<int*>(&header[18]);
        int height = *reinterpret_cast<int*>(&header[22]);
        int bfOffBits = *reinterpret_cast<int*>(&header[10]);


        if (width <= 0 || height <= 0 || bfOffBits <= 0) {
            throw runtime_error("Ungültige Datei");
        }

        BitmapImage image = BitmapImage(height, width);

        for(int i = 0; i < height; i++) {
            for(int j = 0; j < width; j++) {
                BitmapImage::BitmapPixel pixel = BitmapImage::BitmapPixel(file.get(), 0, 0);
                image.set_pixel(i, j, pixel);
            }
        }
        file.close();

        return image;
    }
}