/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2015  Mark Samman <mark.samman@gmail.com>
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

#include "networkmessage.h"

#include "container.h"
#include "creature.h"
#include "player.h"

#include "position.h"
#include "rsa.h"

int32_t NetworkMessage::decodeHeader()
{
	int32_t newSize = static_cast<int32_t>(buffer[0] | buffer[1] << 8);
	length = newSize;
	return length;
}

/******************************************************************************/
std::string NetworkMessage::GetString(uint16_t stringLen/* = 0*/)
{
	if (stringLen == 0) {
		stringLen = get<uint16_t>();
	}

	if (!canRead(stringLen)) {
		return std::string();
	}

	char* v = reinterpret_cast<char*>(buffer) + position;
	position += stringLen;
	return std::string(v, stringLen);
}

Position NetworkMessage::GetPosition()
{
	Position pos;
	pos.x = get<uint16_t>();
	pos.y = get<uint16_t>();
	pos.z = GetByte();
	return pos;
}
/******************************************************************************/

void NetworkMessage::AddString(const std::string& value)
{
	size_t stringLen = value.length();
	if (!canAdd(stringLen + 2) || stringLen > 8192) {
		return;
	}

	Add<uint16_t>(stringLen);
	memcpy(buffer + position, value.c_str(), stringLen);
	position += stringLen;
	length += stringLen;
}

void NetworkMessage::AddString(const char* value)
{
	size_t stringLen = strlen(value);
	if (!canAdd(stringLen + 2) || stringLen > 8192) {
		return;
	}

	Add<uint16_t>(stringLen);
	memcpy(buffer + position, value, stringLen);
	position += stringLen;
	length += stringLen;
}

void NetworkMessage::AddDouble(double value, uint8_t precision/* = 2*/)
{
	AddByte(precision);
	Add<uint32_t>((value * std::pow(static_cast<float>(10), precision)) + INT_MAX);
}

void NetworkMessage::AddBytes(const char* bytes, size_t size)
{
	if (!canAdd(size) || size > 8192) {
		return;
	}

	memcpy(buffer + position, bytes, size);
	position += size;
	length += size;
}

void NetworkMessage::AddPaddingBytes(size_t n)
{
	if (!canAdd(n)) {
		return;
	}

	memset(&buffer[position], 0x33, n);
	length += n;
}

void NetworkMessage::AddPosition(const Position& pos)
{
	Add<uint16_t>(pos.x);
	Add<uint16_t>(pos.y);
	AddByte(pos.z);
}

void NetworkMessage::AddItem(uint16_t id, uint8_t count)
{
	const ItemType& it = Item::items[id];

	Add<uint16_t>(it.clientId);

	AddByte(0xFF);    // MARK_UNMARKED

	if (it.stackable) {
		AddByte(count);
	} else if (it.isSplash() || it.isFluidContainer()) {
		AddByte(fluidMap[count & 7]);
	}

	if (it.isAnimation) {
		AddByte(0xFE);    // random phase (0xFF for async)
	}
}

void NetworkMessage::AddItem(const Item* item)
{
	const ItemType& it = Item::items[item->getID()];

	Add<uint16_t>(it.clientId);
	AddByte(0xFF);    // MARK_UNMARKED

	if (it.stackable) {
		AddByte(std::min<uint16_t>(0xFF, item->getItemCount()));
	} else if (it.isSplash() || it.isFluidContainer()) {
		AddByte(fluidMap[item->getFluidType() & 7]);
	}

	if (it.isAnimation) {
		AddByte(0xFE);    // random phase (0xFF for async)
	}
}

void NetworkMessage::AddItemId(uint16_t itemId)
{
	Add<uint16_t>(Item::items[itemId].clientId);
}
