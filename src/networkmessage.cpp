/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2014  Mark Samman <mark.samman@gmail.com>
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
	int32_t size = static_cast<int32_t>(m_RealBuf[0] | m_RealBuf[1] << 8);
	m_MsgSize = size;
	return size;
}

/******************************************************************************/
std::string NetworkMessage::GetString(uint16_t stringlen/* = 0*/)
{
	if (stringlen == 0) {
		stringlen = get<uint16_t>();
	}

	if (!canRead(stringlen)) {
		return std::string();
	}

	char* v = reinterpret_cast<char*>(m_RealBuf) + m_ReadPos;
	m_ReadPos += stringlen;
	return std::string(v, stringlen);
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
	size_t stringlen = value.length();
	if (!canAdd(stringlen + 2) || stringlen > 8192) {
		return;
	}

	add<uint16_t>(stringlen);
	memcpy(m_MsgBuf + m_ReadPos, value.c_str(), stringlen);
	m_ReadPos += stringlen;
	m_MsgSize += stringlen;
}

void NetworkMessage::AddString(const char* value)
{
	size_t stringlen = strlen(value);
	if (!canAdd(stringlen + 2) || stringlen > 8192) {
		return;
	}

	add<uint16_t>(stringlen);
	memcpy(m_MsgBuf + m_ReadPos, value, stringlen);
	m_ReadPos += stringlen;
	m_MsgSize += stringlen;
}

void NetworkMessage::AddDouble(double value, uint8_t precision/* = 2*/)
{
	AddByte(precision);
	add<uint32_t>((value * std::pow(static_cast<float>(10), precision)) + INT_MAX);
}

void NetworkMessage::AddBytes(const char* bytes, size_t size)
{
	if (!canAdd(size) || size > 8192) {
		return;
	}

	memcpy(m_MsgBuf + m_ReadPos, bytes, size);
	m_ReadPos += size;
	m_MsgSize += size;
}

void NetworkMessage::AddPaddingBytes(size_t n)
{
	if (!canAdd(n)) {
		return;
	}

	memset(&m_MsgBuf[m_ReadPos], 0x33, n);
	m_MsgSize += n;
}

void NetworkMessage::AddPosition(const Position& pos)
{
	add<uint16_t>(pos.x);
	add<uint16_t>(pos.y);
	AddByte(pos.z);
}

void NetworkMessage::AddItem(uint16_t id, uint8_t count)
{
	const ItemType& it = Item::items[id];

	add<uint16_t>(it.clientId);

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

	add<uint16_t>(it.clientId);
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
	add<uint16_t>(Item::items[itemId].clientId);
}
