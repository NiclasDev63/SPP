#include "encryption.cuh"

#include <cooperative_groups.h>
#include <cuda.h>
#include <cuda_runtime_api.h>
#include <device_launch_parameters.h>

__global__ void hash(const std::uint64_t* const values, std::uint64_t* const hashes, const unsigned int length) {
	unsigned int index = blockIdx.x * blockDim.x + threadIdx.x;

	if (index < length) {
		constexpr auto val_a = std::uint64_t{ 5'647'095'006'226'412'969 };
		constexpr auto val_b = std::uint64_t{ 41'413'938'183'913'153 };
		constexpr auto val_c = std::uint64_t{ 6'225'658'194'131'981'369 };

		const std::uint64_t value = values[index];

		const auto val_1 = (value >> 14) + val_a;
		const auto val_2 = (value << 54) ^ val_b;
		const auto val_3 = (val_1 + val_2) << 4;
		const auto val_4 = (value % val_c) * 137;

		const auto final_hash = val_3 ^ val_4;

		hashes[index] = final_hash;
	}
}

__global__ void flat_hash(const std::uint64_t* const values, std::uint64_t* const hashes, const unsigned int length) {
	//Determine how many values each thread has to hash. Considering there is only one {tx, 1, 1} block
	int max = blockDim.x;
	int values_per_thread = (length + max - 1) / max;
	
	unsigned int index = threadIdx.x * values_per_thread;

	if (index < length) {

		constexpr auto val_a = std::uint64_t{ 5'647'095'006'226'412'969 };
		constexpr auto val_b = std::uint64_t{ 41'413'938'183'913'153 };
		constexpr auto val_c = std::uint64_t{ 6'225'658'194'131'981'369 };

		for (int i = 0; i < values_per_thread; i++) {
			if (index + i < length) {
				const auto value = values[index + i];

				const auto val_1 = (value >> 14) + val_a;
				const auto val_2 = (value << 54) ^ val_b;
				const auto val_3 = (val_1 + val_2) << 4;
				const auto val_4 = (value % val_c) * 137;

				hashes[index + i] = val_3 ^ val_4;
			}
		}
	}
}

__global__ void find_hash(const std::uint64_t* const hashes, unsigned int* const indices, const unsigned int length, const std::uint64_t searched_hash, unsigned int* const ptr) {
	unsigned int index = blockIdx.x * blockDim.x + threadIdx.x;

	if (index < length) {
		//Mark if thread at index contains matching hash.
		if (hashes[index] == searched_hash) {
			indices[index] = 1;
		}
	}
}

__global__ void hash_schemes(std::uint64_t* const hashes, const unsigned int length) {
	//Determine how many schemes each thread has to hash.
	int max = blockDim.x * blockIdx.x;
	int values_per_thread = (length + max - 1) / max;

	unsigned int index = (blockIdx.x * blockDim.x + threadIdx.x) * values_per_thread;

	if (index < length) {
			constexpr auto val_a = std::uint64_t{ 5'647'095'006'226'412'969 };
			constexpr auto val_b = std::uint64_t{ 41'413'938'183'913'153 };
			constexpr auto val_c = std::uint64_t{ 6'225'658'194'131'981'369 };

		for (int i = 0; i < values_per_thread; i++) {
			if (index + i < length) {
				//Calculate encoded scheme from index
				std::uint64_t scheme = index + i;
				scheme |= (scheme << 32);

				//Hash scheme and write to hashes[]
				const auto val_1 = (scheme >> 14) + val_a;
				const auto val_2 = (scheme << 54) ^ val_b;
				const auto val_3 = (val_1 + val_2) << 4;
				const auto val_4 = (scheme % val_c) * 137;

				hashes[index + i] = val_3 ^ val_4;
			}
		}	
	}
}