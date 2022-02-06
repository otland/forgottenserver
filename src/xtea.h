/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2019  Mark Samman <mark.samman@gmail.com>
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

#ifndef TFS_XTEA_H
#define TFS_XTEA_H

namespace xtea {

using key = std::array<uint32_t, 4>;
using round_keys = std::array<uint32_t, 64>;

round_keys expand_key(const key& k);
void encrypt(uint8_t* data, size_t length, const round_keys& k);
void decrypt(uint8_t* data, size_t length, const round_keys& k);

} // namespace xtea

#endif // TFS_XTEA_H
