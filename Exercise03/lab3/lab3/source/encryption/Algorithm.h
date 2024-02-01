#pragma once
#include <array>
#include <cstdint>
#include <exception>
#include "Algorithm.h"
#include "image/bitmap_image.h"
#include "Key.h"
#include "FES.h"


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
            case E: break; // 00
            case D: result |= (1ULL << (2 * i)); break; // 01
            case K: result |= (2ULL << (2 * i)); break; // 10
            case T: result |= (3ULL << (2 * i)); break; // 11
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
            case 0: scheme[i] = E; break; // 00
            case 1: scheme[i] = D; break; // 01
            case 2: scheme[i] = K; break; // 10
            case 3: scheme[i] = T; break; // 11
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
                image.transpose(); 
                break;
            case K: 
                key = Key::produce_new_key(key); 
                break;
        }
    }
    return image;
}