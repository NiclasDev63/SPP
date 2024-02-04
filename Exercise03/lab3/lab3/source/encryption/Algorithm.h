#pragma once
#include <array>
#include <cstdint>
#include <exception>
#include "Algorithm.h"
#include "image/bitmap_image.h"
#include "Key.h"
#include "FES.h"
#include "util/Hash.h"


enum EncryptionStep {
    E,
    D,
    K,
    T
};

using EncryptionScheme = std::array<EncryptionStep, 16>;

std::uint64_t encode(const EncryptionScheme& scheme) {
    std::uint64_t result = 0;

    for (std::size_t i = 0; i < scheme.size(); ++i) {
        switch (scheme[i]) {
            case EncryptionStep::E: break; // 00
            case EncryptionStep::D: result |= (1ULL << (2 * i)); break; // 01
            case EncryptionStep::K: result |= (2ULL << (2 * i)); break; // 10
            case EncryptionStep::T: result |= (3ULL << (2 * i)); break; // 11
        }
    }

    result |= (result << 32);

    return result;
}


EncryptionScheme decode(std::uint64_t code) {
    //check if first 32 bits are equal to last 32 bits
    if ((code & 0xFFFFFFFF) != ((code >> 32) & 0xFFFFFFFF)) {
        throw std::exception();
    }

    EncryptionScheme scheme;

    for (std::size_t i = 0; i < scheme.size(); ++i) {
        //get first 2 bits from code
        std::uint64_t bits = (code >> (2 * i)) & 3;

        switch (bits) {
            case 0: scheme[i] = EncryptionStep::E; break; // 00
            case 1: scheme[i] = EncryptionStep::D; break; // 01
            case 2: scheme[i] = EncryptionStep::K; break; // 10
            case 3: scheme[i] = EncryptionStep::T; break; // 11
            default: throw std::exception();
        }
    }

    return scheme;
}


BitmapImage perform_scheme(BitmapImage image, Key::key_type key, const EncryptionScheme& scheme) {
    for (std::size_t i = 0; i < scheme.size(); ++i) {
        switch (scheme[i]) {
            case E: 
                image = FES::encrypt(image, key); 
                break;
            case D: 
                image = FES::decrypt(image, key); 
                break;
            case T: 
                image = image.transpose(); 
                break;
            case K: 
                key = Key::produce_new_key(key); 
                break;
        }
    }
    return image;
}

EncryptionScheme retrieve_scheme(std::uint64_t hash) {
    //There is a total of 1025 possible schemes (0 to 1024) if indices 10-15 contain an E (zeroes) due to the fact that front and back of a scheme are identical.
    for (std::uint64_t i = 0; i < 1025; i++) {
        std::uint64_t current_value = i;

        current_value |= current_value << 32;
        
        auto current_scheme = decode(current_value);

        auto current_hash = Hash::hash(encode(current_scheme));

        if (current_hash == hash)
            return current_scheme;
    }
}