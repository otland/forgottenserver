// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_XTEA_H
#define FS_XTEA_H

/**
 * @namespace xtea
 * @brief This namespace provides functions to encrypt and decrypt data using the XTEA algorithm
 * (See https://wikipedia.org/wiki/XTEA).
 */
namespace xtea {

/**
 * @typedef key
 * @brief Represents a 128-bit key. (See https://wikipedia.org/wiki/Key_size)
 */
using key = std::array<uint32_t, 4>;

/**
 * @typedef round_keys
 * @brief Represents the expanded key used across all rounds.
 */
using round_keys = std::array<uint32_t, 64>;

/**
 * @brief Expands a 128-bit key into a set of round keys.
 *
 * @param {k} The original 128-bit key to be expanded.
 * @return A precomputed set of round keys for use in encryption or decryption.
 */
round_keys expand_key(const key& k);

/**
 * @brief Encrypts data using the XTEA algorithm.
 *
 * @param {data} A pointer to the data to be encrypted. The data must be in blocks of 8 bytes.
 * @param {length} The length of the data to be encrypted, in bytes. It must be a multiple of 8.
 * @param {k} The expanded round keys used for encryption.
 */
void encrypt(uint8_t* data, size_t length, const round_keys& k);

/**
 * @brief Decrypts data using the XTEA algorithm.
 *
 * @param {data} A pointer to the data to be decrypted. The decryption is performed directly on this buffer. The data
 * must be in blocks of 8 bytes.
 * @param {length} The ength of the data to be decrypted, in bytes. It must be a multiple of 8.
 * @param {k} The expanded round keys used for decryption.
 */
void decrypt(uint8_t* data, size_t length, const round_keys& k);

} // namespace xtea

#endif // FS_XTEA_H
