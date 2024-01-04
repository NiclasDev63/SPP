#include "encryption/Key.h"

Key::key_type Key::get_standard_key() noexcept {
    auto key = key_type{};
    for (auto i = key_type::value_type(0); i < key.size(); i++) {
        key[i] = i;
    }

    return key;
}

Key::key_type Key::produce_new_key(Key::key_type key) noexcept {
    auto new_key = key_type{};

    for(auto i = key_type::value_type(0); i< 6; i ++) {
        std::uint64_t value;
        std::memcpy(&value, &key[i * 8], sizeof value);
        auto hash_value = Hash::hash(value);
        std::memcpy(&new_key[i * 8], &hash_value, sizeof hash_value);
    }
    return new_key;
}

std::uint64_t Key::hash(Key::key_type key) noexcept {
    std::uint64_t hash = 0;

    for(auto i = key_type::value_type(0); i < 6; i ++) {
        std::uint64_t value;
        std::memcpy(&value, &key[i * 8], sizeof value);
        value = Hash::hash(value);
        hash = Hash::combine_hashes(hash, value);
    }

    return hash;
}

std::uint64_t Key::get_smallest_hash(std::span<const key_type> span) noexcept {
    auto smallest_hash = std::numeric_limits<Hash::hash_type>::max();

    for(const auto & key : span) {
        auto hash = Key::hash(key);
        if(hash < smallest_hash)
            smallest_hash = hash;
    }

    return smallest_hash;
}

std::uint64_t Key::get_smallest_hash_parallel(std::span<const key_type> span,int num_threads) noexcept {
    Hash::hash_type smallest_hash = std::numeric_limits<Hash::hash_type>::max();
#pragma omp parallel for num_threads(num_threads) reduction(min:smallest_hash)
    for(const auto & key : span) {
        auto hash = Key::hash(key);
        if (hash < smallest_hash)
            smallest_hash = hash;
    }

    return smallest_hash;
}

Key::key_type Key::find_key(std::span<const key_type> span, std::uint64_t compare_hash_value) noexcept {
    auto result_key = key_type {};

    for(const auto & key : span) {
        auto hash = Key::hash(key);
        if(hash == compare_hash_value)
            result_key = key;
    }

    return result_key;
}

Key::key_type Key::find_key_parallel(std::span<const key_type> span, std::uint64_t compare_hash_value, int num_threads) noexcept {
    auto result_key = key_type {};

#pragma omp parallel for num_threads(num_threads)
    for(const auto & key : span) {
        auto hash = Key::hash(key);
        if(hash == compare_hash_value)
#pragma omp critical
        {
            result_key = key;
        }
    }

    return result_key;
}