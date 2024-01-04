#pragma once

#include <array>
#include <cstdint>
#include <span>

class Key {
public:
	using key_type = std::array<std::uint8_t, 48>;

	[[nodiscard]] static key_type get_standard_key() noexcept;

	[[nodiscard]] static key_type produce_new_key(key_type key) noexcept;
	
	[[nodiscard]] static std::uint64_t hash(key_type key) noexcept;

	[[nodiscard]] static std::uint64_t get_smallest_hash(std::span<key_type> keys) noexcept;

	[[nodiscard]] static std::uint64_t get_smallest_hash_parallel(std::span<key_type> keys, const int num_threads = 1) noexcept;

	[[nodiscard]] static std::uint64_t find_key(std::span<key_type> keys, std::uint64_t reference_hash) noexcept;

	[[nodiscard]] static std::uint64_t find_key_parallel(std::span<key_type> keys, std::uint64_t reference_hash, const int num_threads = 1) noexcept;


private:

};
