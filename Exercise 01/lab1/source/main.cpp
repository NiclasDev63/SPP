#include "encryption/FES.h"
#include "encryption/Key.h"
#include "io/image_parser.h"

#include <CLI/App.hpp>
#include <CLI/Config.hpp>
#include <CLI/Formatter.hpp>

#include <random>


int main(int argc, char** argv) {
	auto lab_cli_app = CLI::App{ "" };

	auto image_path = std::filesystem::path{};
	auto file_option = lab_cli_app.add_option("--file", image_path);

	auto output_path = std::filesystem::path{};
	auto output_option = lab_cli_app.add_option("--output", output_path);

	file_option->check(CLI::ExistingFile);
	output_option->check(CLI::ExistingDirectory);

	CLI11_PARSE(lab_cli_app, argc, argv);


	BitmapImage img(500, 500);
	for (int i = 0; i < 500; i++) {
		for (int j = 0; j < 500; j++) {

			std::random_device dev;
			std::mt19937 rng(dev());
			std::uniform_int_distribution<std::mt19937::result_type> dist6(0, 255);

			BitmapImage::BitmapPixel pixel(dist6(rng), dist6(rng), dist6(rng));
			//BitmapImage::BitmapPixel pixel(0, 0, 0);

			img.set_pixel(i, j, pixel);
		}
	}

	ImageParser::write_bitmap(output_path / "test2.bmp", img);


	//auto key = FES::key_type{};
	//for (auto i = FES::key_type::value_type(0); i < key.size(); i++) {
	//	key[i] = i;
	//}

	//auto encrypted_image = FES::encrypt(bitmap_image, key);
	
	//ImageParser::write_bitmap(output_path / "encrypted_image.bmp", encrypted_image);
	//ImageParser::write_bitmap(output_path / "encrypted_image.bmp", encrypted_image);

	return 0;
}
