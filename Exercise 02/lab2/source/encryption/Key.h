#pragma once

#include "util/Hash.h"

#include <array>
#include <cstdint>
#include <cstring>
#include <span>
#include <limits>

class Key {
public:
    using key_type = std::array<std::uint8_t, 48>;

    [[nodiscard]] static key_type get_standard_key() noexcept;

    [[nodiscard]] static key_type produce_new_key(key_type key) noexcept;

    [[nodiscard]] static std::uint64_t hash(key_type key) noexcept;

    [[nodiscard]] static std::uint64_t get_smallest_hash(std::span<const key_type> span) noexcept;

    [[nodiscard]] static std::uint64_t get_smallest_hash_parallel(std::span<const key_type> span, int num_threads = 1) noexcept;

    [[nodiscard]] static key_type find_key(std::span<const key_type> span, std::uint64_t compare_hash_value) noexcept;

    [[nodiscard]] static key_type find_key_parallel(std::span<const key_type> span, std::uint64_t compare_hash_value, int num_threads = 1) noexcept;

private:

};
