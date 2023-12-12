#pragma once

#include "benchmark/benchmark.h"

#include "encryption/Key.h"
#include "image/bitmap_image.h"

#include <cstdint>
#include <span>
#include <vector>

class Holder {
	constexpr static std::uint32_t square = 1024;

public:
	static void init(const std::size_t number_keys) {
		keys.reserve(number_keys);
		keys.emplace_back(Key::get_standard_key());

		for (auto i = std::size_t(1); i < number_keys; i++) {
			keys.emplace_back(Key::produce_new_key(keys[i - 1]));
		}

		for (auto y = 0; y < square; y++) {
			for (auto x = 0; x < square; x++) {
				const auto& pixel = BitmapImage::BitmapPixel{ static_cast<std::uint8_t>(y % 256), static_cast<std::uint8_t>(x % 256), static_cast<std::uint8_t>((x + y * x) % 256) };
				square_image.set_pixel(y, x, pixel);
			}
		}
	}

	static const std::vector<Key::key_type>& get_keys() {
		return keys;
	}

	static const BitmapImage& get_square_image() {
		return square_image;
	}

private:
	static inline std::vector<Key::key_type> keys{};

	static inline BitmapImage square_image{ square, square };
};
