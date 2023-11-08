#include "image/pixel.h"

template <typename T>
Pixel<T>::Pixel(T color1, T color2, T color3)
{

    switch (channel_order)
    {
    case channel_order:
    BGR:
        red(color3);
        green(color2);
        blue(color1);
        break;
    case channel_order:
    BRG:
        red(color2);
        green(color3);
        blue(color1);
        break;
    case channel_order:
    GBR:
        red(color3);
        green(color1);
        blue(color2);
        break;
    case channel_order:
    GRB:
        red(color2);
        green(color1);
        blue(color3);
        break;
    case channel_order:
    RBG:
        red(color1);
        green(color3);
        blue(color2);
        break;
    case channel_order:
    RGB:
        red(color1);
        green(color2);
        blue(color3);
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