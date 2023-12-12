#include "benchmark.h"

#include "encryption/FES.h"
#include "encryption/Key.h"
#include "image/bitmap_image.h"

#include <iostream>

static bool check_image_equality(const BitmapImage& first, const BitmapImage& second) {
	if (first.get_height() != second.get_height()) {
		return false;
	}

	if (first.get_width() != second.get_width()) {
		return false;
	}

	for (auto y = std::uint32_t{ 0 }; y < first.get_height(); y++) {
		for (auto x = std::uint32_t{ 0 }; x < first.get_width(); x++) {
			const auto& first_pixel = first.get_pixel(y, x);
			const auto first_r = first_pixel.get_red_channel();
			const auto first_g = first_pixel.get_green_channel();
			const auto first_b = first_pixel.get_blue_channel();

			const auto& second_pixel = second.get_pixel(y, x);
			const auto second_r = second_pixel.get_red_channel();
			const auto second_g = second_pixel.get_green_channel();
			const auto second_b = second_pixel.get_blue_channel();

			if ((first_r != second_r) | (first_g != second_g) | (first_b != second_b)) {
				return false;
			}
		}
	}

	return true;
}

static void benchmark_two_a_one(benchmark::State& state) {
	const auto number_threads = state.range(0);

	for (auto _ : state) {
	}
}

static void benchmark_two_a_two(benchmark::State& state) {
	const auto number_threads = state.range(0);

	for (auto _ : state) {
	}
}

static void benchmark_two_b_one(benchmark::State& state) {
	const auto number_threads = state.range(0);

	for (auto _ : state) {
	}
}

static void benchmark_two_b_two(benchmark::State& state) {
	const auto number_threads = state.range(0);

	for (auto _ : state) {
	}
}

static void benchmark_two_c_one(benchmark::State& state) {
	const auto number_threads = state.range(0);

	for (auto _ : state) {
	}
}

static void benchmark_two_c_two(benchmark::State& state) {
	const auto number_threads = state.range(0);

	for (auto _ : state) {
	}
}

static void benchmark_two_d_one(benchmark::State& state) {
	const auto number_threads = state.range(0);

	const auto& image = Holder::get_square_image();

	for (auto _ : state) {
		const auto val = image.transpose_parallel(number_threads);
		benchmark::DoNotOptimize(val);
	}

	const auto serial_answer = image.transpose();
	const auto parallel_answer = image.transpose_parallel(number_threads);

	if (!check_image_equality(serial_answer, parallel_answer)) {
		std::cerr << "Serial and parallel answers don't match!\n";
	}
}

static void benchmark_two_e_one(benchmark::State& state) {
	const auto number_threads = state.range(0);

	for (auto _ : state) {
	}
}

static void benchmark_two_f_one(benchmark::State& state) {
	const auto number_threads = state.range(0);

	for (auto _ : state) {
	}
}

static void benchmark_two_f_two(benchmark::State& state) {
	const auto number_threads = state.range(0);

	for (auto _ : state) {
	}
}

BENCHMARK(benchmark_two_a_one)->Unit(benchmark::kMillisecond)->Arg(1);
BENCHMARK(benchmark_two_a_one)->Unit(benchmark::kMillisecond)->Arg(2);
BENCHMARK(benchmark_two_a_one)->Unit(benchmark::kMillisecond)->Arg(4);

BENCHMARK(benchmark_two_a_two)->Unit(benchmark::kMillisecond)->Arg(1);
BENCHMARK(benchmark_two_a_two)->Unit(benchmark::kMillisecond)->Arg(2);
BENCHMARK(benchmark_two_a_two)->Unit(benchmark::kMillisecond)->Arg(4);

BENCHMARK(benchmark_two_b_one)->Unit(benchmark::kMillisecond)->Arg(1);
BENCHMARK(benchmark_two_b_one)->Unit(benchmark::kMillisecond)->Arg(2);
BENCHMARK(benchmark_two_b_one)->Unit(benchmark::kMillisecond)->Arg(4);

BENCHMARK(benchmark_two_b_two)->Unit(benchmark::kMillisecond)->Arg(1);
BENCHMARK(benchmark_two_b_two)->Unit(benchmark::kMillisecond)->Arg(2);
BENCHMARK(benchmark_two_b_two)->Unit(benchmark::kMillisecond)->Arg(4);

BENCHMARK(benchmark_two_c_one)->Unit(benchmark::kMillisecond)->Arg(1);
BENCHMARK(benchmark_two_c_one)->Unit(benchmark::kMillisecond)->Arg(2);
BENCHMARK(benchmark_two_c_one)->Unit(benchmark::kMillisecond)->Arg(4);

BENCHMARK(benchmark_two_c_two)->Unit(benchmark::kMillisecond)->Arg(1);
BENCHMARK(benchmark_two_c_two)->Unit(benchmark::kMillisecond)->Arg(2);
BENCHMARK(benchmark_two_c_two)->Unit(benchmark::kMillisecond)->Arg(4);

BENCHMARK(benchmark_two_d_one)->Unit(benchmark::kMillisecond)->Arg(1);
BENCHMARK(benchmark_two_d_one)->Unit(benchmark::kMillisecond)->Arg(2);
BENCHMARK(benchmark_two_d_one)->Unit(benchmark::kMillisecond)->Arg(4);

BENCHMARK(benchmark_two_e_one)->Unit(benchmark::kMillisecond)->Arg(1);
BENCHMARK(benchmark_two_e_one)->Unit(benchmark::kMillisecond)->Arg(2);
BENCHMARK(benchmark_two_e_one)->Unit(benchmark::kMillisecond)->Arg(4);

BENCHMARK(benchmark_two_f_one)->Unit(benchmark::kMillisecond)->Arg(1);
BENCHMARK(benchmark_two_f_one)->Unit(benchmark::kMillisecond)->Arg(2);
BENCHMARK(benchmark_two_f_one)->Unit(benchmark::kMillisecond)->Arg(4);

BENCHMARK(benchmark_two_f_two)->Unit(benchmark::kMillisecond)->Arg(1);
BENCHMARK(benchmark_two_f_two)->Unit(benchmark::kMillisecond)->Arg(2);
BENCHMARK(benchmark_two_f_two)->Unit(benchmark::kMillisecond)->Arg(4);
