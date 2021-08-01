/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2020  Mark Samman <mark.samman@gmail.com>
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

round_keys expand_key(const key& k)
{
	constexpr uint32_t delta = 0x9E3779B9;
	round_keys expanded;

	for (uint32_t i = 0, sum = 0, next_sum = sum + delta; i < expanded.size(); i += 2, sum = next_sum, next_sum += delta) {
		expanded[i] = sum + k[sum & 3];
		expanded[i + 1] = next_sum + k[(next_sum >> 11) & 3];
	}

	return expanded;
}

void encrypt(uint8_t* data, size_t length, const round_keys& k)
{
	for (int32_t i = 0; i < k.size(); i += 2) {
		for (auto it = data, last = data + length; it < last; it += 8) {
			uint32_t left, right;
			std::memcpy(&left, it, 4);
			std::memcpy(&right, it + 4, 4);

			left += ((right << 4 ^ right >> 5) + right) ^ k[i];
			right += ((left << 4 ^ left >> 5) + left) ^ k[i + 1];

			std::memcpy(it, &left, 4);
			std::memcpy(it + 4, &right, 4);
		}
	}
}

void decrypt(uint8_t* data, size_t length, const round_keys& k)
{
	for (int32_t i = k.size() - 1; i > 0; i -= 2) {
		for (auto it = data, last = data + length; it < last; it += 8) {
			uint32_t left, right;
			std::memcpy(&left, it, 4);
			std::memcpy(&right, it + 4, 4);

			right -= ((left << 4 ^ left >> 5) + left) ^ k[i];
			left -= ((right << 4 ^ right >> 5) + right) ^ k[i - 1];

			std::memcpy(it, &left, 4);
			std::memcpy(it + 4, &right, 4);
		}
	}
}

} // namespace xtea
