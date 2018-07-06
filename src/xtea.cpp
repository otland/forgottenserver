/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2018  Mark Samman <mark.samman@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "otpch.h"

#include "xtea.h"

#include <array>
#include <assert.h>

namespace xtea {

namespace {

// precalculated key schedule
constexpr std::array<uint32_t, 33> sum = {{
    0x00000000, 0x9e3779b9, 0x3c6ef372,
    0xdaa66d2b, 0x78dde6e4, 0x1715609d,
    0xb54cda56, 0x5384540f, 0xf1bbcdc8,
    0x8ff34781, 0x2e2ac13a, 0xcc623af3,
    0x6a99b4ac, 0x08d12e65, 0xa708a81e,
    0x454021d7, 0xe3779b90, 0x81af1549,
    0x1fe68f02, 0xbe1e08bb, 0x5c558274,
    0xfa8cfc2d, 0x98c475e6, 0x36fbef9f,
    0xd5336958, 0x736ae311, 0x11a25cca,
    0xafd9d683, 0x4e11503c, 0xec48c9f5,
    0x8a8043ae, 0x28b7bd67, 0xc6ef3720,
}};

template<size_t BLOCK_SIZE>
void XTEA_encrypt(
        uint8_t data[BLOCK_SIZE * 8],
        const key& k)
{
    auto words = reinterpret_cast<uint32_t*>(data);
    alignas(16) uint32_t left[BLOCK_SIZE], right[BLOCK_SIZE];

    for (auto i = 0u; i < BLOCK_SIZE; ++i) {
        left[i] = words[2 * i];
        right[i] = words[2 * i + 1];
    }

    for (auto i = 0u; i < 32u; ++i) {
        for (auto j = 0u; j < BLOCK_SIZE; ++j) {
            left[j] += (((right[j] << 4) ^ (right[j] >> 5)) + right[j]) ^ (sum[i] + k[sum[i] & 3]);
            right[j] += (((left[j] << 4) ^ (left[j] >> 5)) + left[j]) ^ (sum[i+1] + k[(sum[i+1] >> 11) & 3]);
        }
    }

    for (auto i = 0u; i < BLOCK_SIZE; ++i) {
        words[2 * i] = left[i];
        words[2 * i + 1] = right[i];
    }
}

template<size_t BLOCK_SIZE>
void XTEA_decrypt(
        uint8_t data[BLOCK_SIZE * 8],
        const key& k)
{
    auto words = reinterpret_cast<uint32_t*>(data);
    alignas(16) uint32_t left[BLOCK_SIZE], right[BLOCK_SIZE];

    for (auto i = 0u; i < BLOCK_SIZE; ++i) {
		left[i] = words[2 * i];
		right[i] = words[2 * i + 1];
    }

    for (auto i = 32u; i > 0u; --i) {
        for (auto j = 0u; j < BLOCK_SIZE; ++j) {
            right[j] -= (((left[j] << 4) ^ (left[j] >> 5)) + left[j]) ^ (sum[i] + k[(sum[i] >> 11) & 3]);
            left[j] -= (((right[j] << 4) ^ (right[j] >> 5)) + right[j]) ^ (sum[i-1] + k[(sum[i-1]) & 3]);
        }
    }

    for (auto i = 0u; i < BLOCK_SIZE; ++i) {
        words[2 * i] = left[i];
        words[2 * i + 1] = right[i];
    }
}

} // anonymous namespace

void encrypt(uint8_t* data, size_t length, const key& k)
{
    assert(length % 8 == 0);

#ifdef __AVX512F__
    {
        constexpr auto step = 64u * 8u;
        const auto blocks = (length & ~(step - 1));
        for (auto i = 0u; i < blocks; i += step) {
            XTEA_encrypt<64>(data + i, k);
        }
        data += blocks;
        length -= blocks;
    }
#endif // __AVX512F__

#ifdef __AVX__
    {
        constexpr auto step = 16u * 8u;
        const auto blocks = (length & ~(step - 1));
        for (auto i = 0u; i < blocks; i += step) {
            XTEA_encrypt<16>(data + i, k);
        }
        data += blocks;
        length -= blocks;
    }
#endif // __AVX__

#if defined(__SSE__) || defined(__ARM_FEATURE_SIMD32)
    {
        constexpr auto step = 4u * 8u;
        const auto blocks = (length & ~(step - 1));
        for (auto i = 0u; i < blocks; i += step) {
            XTEA_encrypt<4>(data + i, k);
        }
        data += blocks;
        length -= blocks;
    }
#endif // __SSE__ || __ARM_FEATURE_SIMD32

#ifdef __x86_64__
    {
        constexpr auto step = 2u * 8u;
        const auto blocks = (length & ~(step - 1));
        for (auto i = 0u; i < blocks; i += step) {
            XTEA_encrypt<2>(data + i, k);
        }
        data += blocks;
        length -= blocks;
    }
#endif // __x86_64__

    for (auto i = 0u; i < length; i += 8u) {
        XTEA_encrypt<1>(data + i, k);
    }
}

void decrypt(uint8_t* data, size_t length, const key& k)
{
    assert(length % 8 == 0);

#ifdef __AVX512F__
    {
        constexpr auto step = 64u * 8u;
        const auto blocks = (length & ~(step - 1));
        for (auto i = 0u; i < blocks; i += step) {
            XTEA_decrypt<64>(data + i, k);
        }
        data += blocks;
        length -= blocks;
    }
#endif // __AVX512F__

#ifdef __AVX__
    {
        constexpr auto step = 16u * 8u;
        const auto blocks = (length & ~(step - 1));
        for (auto i = 0u; i < blocks; i += step) {
            XTEA_decrypt<16>(data + i, k);
        }
        data += blocks;
        length -= blocks;
    }
#endif // __AVX__

#if defined(__SSE__) || defined(__ARM_FEATURE_SIMD32)
    {
        constexpr auto step = 4u * 8u;
        const auto blocks = (length & ~(step - 1));
        for (auto i = 0u; i < blocks; i += step) {
            XTEA_decrypt<4>(data + i, k);
        }
        data += blocks;
        length -= blocks;
    }
#endif // __SSE__ || __ARM_FEATURE_SIMD32

#ifdef __x86_64__
    {
        constexpr auto step = 2u * 8u;
        const auto blocks = (length & ~(step - 1));
        for (auto i = 0u; i < blocks; i += step) {
            XTEA_decrypt<2>(data + i, k);
        }
        data += blocks;
        length -= blocks;
    }
#endif // __x86_64__

    for (auto i = 0u; i < length; i += 8u) {
        XTEA_decrypt<1>(data + i, k);
    }
}

} // namespace xtea
