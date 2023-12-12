#include "util/Hash.h"

Hash::hash_type Hash::hash(const std::uint64_t value) noexcept {
	std::uint64_t first = (value >> 14) + Hash::A;
	std::uint64_t second = ((value << 54) ^ Hash::B);
	std::uint64_t third = (value % Hash::C) * 137;
	return ((first + second) << 4) ^ third;
}

Hash::hash_type Hash::hash(const std::uint32_t value) noexcept {
	const auto promoted = static_cast<std::uint64_t>(value);
	std::uint64_t first = (value >> 14) + Hash::A;
	std::uint64_t second = ((value << 54) ^ Hash::B);
	std::uint64_t third = (value % Hash::C) * 137;
	return ((first + second) << 4) ^ third;
}

Hash::hash_type Hash::hash(const std::uint16_t value) noexcept {
	const auto promoted = static_cast<std::uint64_t>(value);
	std::uint64_t first = (value >> 14) + Hash::A;
	std::uint64_t second = ((value << 54) ^ Hash::B);
	std::uint64_t third = (value % Hash::C) * 137;
	return ((first + second) << 4) ^ third;
}

Hash::hash_type Hash::hash(const std::uint8_t value) noexcept {
	const auto promoted = static_cast<std::uint64_t>(value);
	std::uint64_t first = (value >> 14) + Hash::A;
	std::uint64_t second = ((value << 54) ^ Hash::B);
	std::uint64_t third = (value % Hash::C) * 137;
	return ((first + second) << 4) ^ third;
}



Hash::hash_type Hash::combine_hashes(const hash_type first_hash, const hash_type second_hash) noexcept {
	return first_hash ^ second_hash;
}
