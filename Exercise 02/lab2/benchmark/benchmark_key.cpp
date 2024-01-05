#include "benchmark.h"

#include "encryption/Key.h"

#include <iostream>

#include <random>

static void benchmark_three_d_one(benchmark::State& state) {
	const auto number_threads = state.range(0);

	const auto& keys = Holder::get_keys();

	//TODO Uncomment this, when correctly implemented (currently it's not)
	for (auto _ : state) {
		const auto val = Key::get_smallest_hash_parallel(keys, number_threads);
		benchmark::DoNotOptimize(val);
	}

	const auto serial_answer = Key::get_smallest_hash(keys);
	const auto parallel_answer = Key::get_smallest_hash_parallel(keys, number_threads);

	if (serial_answer != parallel_answer) {
		std::cerr << "Serial and parallel answers don't match!\n";
	}
}

static void benchmark_three_f_one(benchmark::State& state) {
	const auto number_threads = state.range(0);

	const auto& keys = Holder::get_keys();
	
	// Create a random device
	std::random_device rd;

	// Create a distribution from 0 to keys.size() - 1
	std::uniform_int_distribution<std::size_t> dist(0, keys.size() - 1);

	// Generate a random index
	std::size_t random_index = dist(rd);

	// Extract the random key
	auto random_key = keys[random_index];
	auto reference_hash = Key::hash(random_key);

	for (auto _ : state) {
		const auto val = Key::find_key_parallel(keys, reference_hash, number_threads);
		benchmark::DoNotOptimize(val);
	}

	const auto serial_answer = Key::find_key(keys, reference_hash);
	const auto parallel_answer = Key::find_key_parallel(keys, reference_hash, number_threads);

	if (serial_answer != parallel_answer) {
		std::cerr << "Serial and parallel answers don't match!\n";
	}
}

BENCHMARK(benchmark_three_d_one)->Unit(benchmark::kMillisecond)->Arg(1);
BENCHMARK(benchmark_three_d_one)->Unit(benchmark::kMillisecond)->Arg(2);
BENCHMARK(benchmark_three_d_one)->Unit(benchmark::kMillisecond)->Arg(4);

BENCHMARK(benchmark_three_f_one)->Unit(benchmark::kMillisecond)->Arg(1);
BENCHMARK(benchmark_three_f_one)->Unit(benchmark::kMillisecond)->Arg(2);
BENCHMARK(benchmark_three_f_one)->Unit(benchmark::kMillisecond)->Arg(4);
