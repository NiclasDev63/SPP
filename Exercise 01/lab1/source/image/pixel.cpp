#include "image/pixel.h"

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