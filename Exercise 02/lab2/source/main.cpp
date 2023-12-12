#include "encryption/FES.h"
#include "encryption/Key.h"
#include "io/image_parser.h"

#include <CLI/App.hpp>
#include <CLI/Config.hpp>
#include <CLI/Formatter.hpp>

#include <bit>

int main(int argc, char** argv) {
	auto lab_cli_app = CLI::App{ "" };

	auto image_path = std::filesystem::path{};
	auto file_option = lab_cli_app.add_option("--file", image_path);

	auto output_path = std::filesystem::path{};
	auto output_option = lab_cli_app.add_option("--output", output_path);

	file_option->check(CLI::ExistingFile);
	output_option->check(CLI::ExistingDirectory);

	CLI11_PARSE(lab_cli_app, argc, argv);

	const auto& endianness = std::endian::native;

	auto loaded_file = ImageParser::read_bitmap(image_path);
	ImageParser::write_bitmap(output_path / "out.bmp", loaded_file);

	return 0;
}
