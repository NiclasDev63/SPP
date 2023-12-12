#pragma once

#include "image/pixel.h"

#include <cstdint>
#include <vector>

class BitmapImage {
public:
	using BitmapPixel = Pixel<std::uint8_t>;
	using index_type = std::uint32_t;

	BitmapImage(index_type image_height, index_type image_width);

	void set_pixel(index_type y_position, index_type x_position, const BitmapPixel pixel);

	[[nodiscard]] BitmapPixel get_pixel(index_type y_position, index_type x_position) const;

	[[nodiscard]] index_type get_height() const noexcept;

	[[nodiscard]] index_type get_width() const noexcept;

	[[nodiscard]] BitmapImage transpose() const;

	[[nodiscard]] BitmapImage transpose_parallel(const int num_threads = 1) const;

	[[nodiscard]] BitmapImage get_grayscale() const;

	[[nodiscard]] BitmapImage get_grayscale_parallel(const int num_threads = 1) const;

	void mystery();

	void mystery_parallel(const int num_threads = 1);

private:
	index_type height{};
	index_type width{};

	std::vector<BitmapPixel> pixels{};
};
