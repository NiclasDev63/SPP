#include "image/pixel.h"

template <typename T>
Pixel<T>::Pixel(T color1, T color2, T color3)
{
    
    switch (channel_order)
    {
    case channel_order:BGR:
        Pixel::red(color3);
        Pixel::green(color2);
        Pixel::blue(color1);
        break;
    case channel_order:BRG:
        Pixel::red(color2);
        Pixel::green(color3);
        Pixel::blue(color1);
        break;
    case channel_order:GBR:
        Pixel::red(color3);
        Pixel::green(color1);
        Pixel::blue(color2);
        break;
    case channel_order:GRB:
        Pixel::red(color2);
        Pixel::green(color1);
        Pixel::blue(color3);
        break;
    case channel_order:RBG:
        Pixel::red(color1);
        Pixel::green(color3);
        Pixel::blue(color2);
        break;
    case channel_order:RGB:
        Pixel::red(color1);
        Pixel::green(color2);
        Pixel::blue(color3);
        break;
    default:
        break;
    }
}

template <typename T>
[[nodiscard]] T Pixel<T>::get_red_channel() const noexcept { return red; }

template <typename T>
[[nodiscard]] T Pixel<T>::get_green_channel() const noexcept { return green; }

template <typename T>
[[nodiscard]] T Pixel<T>::get_blue_channel() const noexcept { return blue; }

template <typename T>
bool Pixel<T>::operator==(const Pixel &other) const
{
    return red == other.red && green == other.green && blue == other.blue;
}