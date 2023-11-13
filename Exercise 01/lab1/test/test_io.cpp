#include "test.h"

#include "image/bitmap_image.h"
#include "io/image_parser.h"

#include <exception>
#include <filesystem>

class IOTest : public LabTest {};

BitmapImage::BitmapPixel construct_pixel(const std::uint8_t red, const std::uint8_t green, const std::uint8_t blue) {
	switch (BitmapImage::BitmapPixel::channel_order) {
	case ChannelOrder::BGR:
		return { blue, green, red };
		break;
	case ChannelOrder::BRG:
		return { blue, red, green };
		break;
	case ChannelOrder::GBR:
		return { green, blue, red };
		break;
	case ChannelOrder::GRB:
		return { green, red, blue };
		break;
	case ChannelOrder::RBG:
		return { red, blue, green };
		break;
	case ChannelOrder::RGB:
		return { red, green, blue };
		break;
	default:
		EXPECT_TRUE(false) << "ChannelOrder for BitmapImage::BitmapPixel has no valid value!\n";
	}

	return {};
}

TEST_F(IOTest, test_two_a_one) {
	BitmapImage::BitmapPixel::channel_order = ChannelOrder::RGB;

	const auto path = get_path();

	ASSERT_THROW(auto val = ImageParser::read_bitmap(path), std::exception);
	ASSERT_THROW(auto val = ImageParser::read_bitmap(path / "inputkeahw3z4dn8o3qtzdn9wz/"), std::exception);
	ASSERT_THROW(auto val = ImageParser::read_bitmap(path / "input/pic_sma6o239nhtdjq8odnztfqnll.txt"), std::exception);
	ASSERT_THROW(auto val = ImageParser::read_bitmap(path / "input/pic_small.pnq9nzrcq9zmg"), std::exception);
	ASSERT_THROW(auto val = ImageParser::read_bitmap(path / "input/please_dont_exist.owa3z58923q4724u.bmp"), std::exception);

	auto read_image = ImageParser::read_bitmap(path / "input/pic_small.bmp");

	ASSERT_EQ(read_image.get_height(), 768);
	ASSERT_EQ(read_image.get_width(), 1024);

	// Who knows in what order they store their pixel
	try {
		const auto pixel_0_0 = read_image.get_pixel(0, 0);
		const auto pixel_767_0 = read_image.get_pixel(767, 0);
		const auto pixel_0_1023 = read_image.get_pixel(0, 1023);
		const auto pixel_767_1023 = read_image.get_pixel(767, 1023);

		ASSERT_EQ(pixel_0_0, construct_pixel(77, 80, 67));
		ASSERT_EQ(pixel_767_0, construct_pixel(64, 82, 117));
		ASSERT_EQ(pixel_0_1023, construct_pixel(133, 131, 97));
		ASSERT_EQ(pixel_767_1023, construct_pixel(84, 100, 135));
	}
	catch (std::exception ex) {
		const auto pixel_0_0 = read_image.get_pixel(0, 0);
		const auto pixel_0_767 = read_image.get_pixel(0, 767);
		const auto pixel_1023_0 = read_image.get_pixel(1023, 0);
		const auto pixel_1023_767 = read_image.get_pixel(1023, 767);

		ASSERT_EQ(pixel_0_0, construct_pixel(77, 80, 67));
		ASSERT_EQ(pixel_0_767, construct_pixel(64, 82, 117));
		ASSERT_EQ(pixel_1023_0, construct_pixel(133, 131, 97));
		ASSERT_EQ(pixel_1023_767, construct_pixel(84, 100, 135));
	}
}

TEST_F(IOTest, test_two_a_two) {
}

TEST_F(IOTest, test_two_b_one) {
}

TEST_F(IOTest, test_two_b_two) {
}
