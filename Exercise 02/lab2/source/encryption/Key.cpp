#include "encryption/Key.h"
#include "util/Hash.h"
#include <limits>
#include <iostream>

Key::key_type Key::get_standard_key() noexcept {
	auto key = key_type{};
	for (auto i = key_type::value_type(0); i < key.size(); i++) {
		key[i] = i;
	}

	return key;
}

Key::key_type Key::produce_new_key(Key::key_type key) noexcept {
    auto new_key = key_type{};
    for (auto i = key_type::value_type(0); i < 6; i++) {
        std::uint64_t tmpval;
        std::memcpy(&tmpval, &key[i * 8], sizeof(tmpval));
        tmpval = Hash::hash(tmpval);
        std::memcpy(&new_key[i * 8], &tmpval, sizeof(tmpval));
    }

    return new_key;
}

std::uint64_t Key::hash(Key::key_type key) noexcept {
	std::uint64_t hash = 0;
	for (auto i = key_type::value_type(0); i < 6; i++) {
		std::uint64_t tmpval;
		std::memcpy(&tmpval, &key[i * 8], sizeof(tmpval));
		std::uint64_t tmphash = Hash::hash(tmpval);
		hash = Hash::combine_hashes(hash, tmphash);
	}

	return hash;
}

std::uint64_t Key::get_smallest_hash(std::span<key_type> keys) noexcept {
	Hash::hash_type smallest_hash = std::numeric_limits<Hash::hash_type>::max();
	for (auto i = std::size_t(0); i < keys.size(); i++) {
		for(auto j = std::size_t(0); j < keys[i].size(); j++) {
			auto hash = Hash::hash(keys[i][j]);

			// print out values for debugging
			// printf("KEY: %llu\n", keys[i][j]);
			// std::cout << "CURRENT HASH: " << hash << std::endl;
			// std::cout << "SMALLEST HASH: " << smallest_hash << std::endl;
			//TODO - fix this (never enters if statement)
			if(hash < smallest_hash) {
				smallest_hash = hash;
			}
		}
	}
	return smallest_hash;
}

std::uint64_t Key::get_smallest_hash_parallel(std::span<key_type> keys,  const int num_threads) noexcept {
	if(keys.empty()) {
		return std::numeric_limits<std::uint64_t>::max();
	}
	auto smallest_hash = Hash::hash(keys[0][0]);
	// #pragma omp parallel for collapse(2) num_threads(num_threads) reduction(min: smallest_hash)
	#pragma omp parallel
	{
		std::uint64_t local_smallest_hash = std::numeric_limits<std::uint64_t>::max();
		#pragma for collapse(2) num_threads(num_threads)
		for (auto i = std::size_t(0); i < keys.size(); i++) {
			for(auto j = std::size_t(0); j < keys[i].size(); j++) {
				auto hash = Hash::hash(keys[i][j]);
				#pragma omp critical 
				if(hash < local_smallest_hash) {
					local_smallest_hash = hash;
				}
			}
		}
		#pragma omp critical
		{
			if(local_smallest_hash < smallest_hash) {
				smallest_hash = local_smallest_hash;
			}
		}
	}
	return smallest_hash;
}

std::uint64_t Key::find_key(std::span<key_type> keys, std::uint64_t reference_hash) noexcept {
	for (auto i = std::size_t(0); i < keys.size(); i++) {
		for(auto j = std::size_t(0); j < keys[i].size(); j++) {
			auto hash = Hash::hash(keys[i][j]);
			if(hash == reference_hash) {
				return keys[i][j];
			}
		}
	}
	return 0;
}

std::uint64_t Key::find_key_parallel(std::span<key_type> keys, std::uint64_t reference_hash, const int num_threads) noexcept {
	auto found_key = std::uint64_t(0);
	#pragma omp parallel for collapse(2) num_threads(num_threads)
		for (auto i = std::size_t(0); i < keys.size(); i++) {
			for(auto j = std::size_t(0); j < keys[i].size(); j++) {
				auto hash = Hash::hash(keys[i][j]);
				if(hash == reference_hash) {
					found_key =  keys[i][j];
				}
			}
		}
	return found_key;
}
