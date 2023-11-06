#pragma once

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
	static ChannelOrder channel_order;

	// standard constructor
    Pixel() : red(0), green(0), blue(0);

	Pixel(T red, T green, T blue) : red(red), green(green), blue(blue);

	// returns the intensitiy of the respective color
	[[nodiscard]] T get_red_channel() const noexcept;
	[[nodiscard]] T get_green_channel() const noexcept;
	[[nodiscard]] T get_blue_channel() const noexcept;

	// overload the "==" operator
	bool operator==(const Pixel &other) const;

private:
	T red;
	T green;
	T blue;
};
