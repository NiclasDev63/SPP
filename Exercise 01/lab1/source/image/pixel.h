#pragma once

#include <stdlib.h>
#include <vector>

// DO NOT CHANGE THIS ENUM
enum class ChannelOrder
{
	BGR,
	BRG,
	GBR,
	GRB,
	RBG,
	RGB
};

template <class T>
class Pixel
{
public:
	// contains the channel order
	inline static ChannelOrder channel_order = ChannelOrder::RGB;

	// standard constructor
	Pixel(): red(0), green(0), blue(0) {}

	Pixel(T red, T green, T blue) : red(red), green(green), blue(blue) {}

	// returns the intensitiy of the respective color
	[[nodiscard]] T get_green_channel() const noexcept { return green; };
	[[nodiscard]] T get_red_channel() const noexcept { return red; };
	[[nodiscard]] T get_blue_channel() const noexcept { return blue; };

	// overload the "==" operator
	bool operator==(const Pixel &other) const {
        return red == other.red && green == other.green && blue == other.blue;
    };

private:
	T red;
	T green;
	T blue;
};
