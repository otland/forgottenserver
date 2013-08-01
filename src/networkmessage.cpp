/**
 * The Forgotten Server - a server application for the MMORPG Tibia
 * Copyright (C) 2013  Mark Samman <mark.samman@gmail.com>
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

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <string>
#include <iostream>
#include <sstream>

#include "networkmessage.h"

#include "container.h"
#include "creature.h"
#include "player.h"

#include "position.h"
#include "rsa.h"

int32_t NetworkMessage::decodeHeader()
{
	int32_t size = (int32_t)(m_RealBuf[0] | m_RealBuf[1] << 8);
	m_MsgSize = size;
	return size;
}

/******************************************************************************/
std::string NetworkMessage::GetString(uint16_t stringlen/* = 0*/)
{
	if (stringlen == 0) {
		stringlen = GetU16();
	}

	if (!canRead(stringlen)) {
		return std::string();
	}

	char* v = (char*)m_RealBuf + m_ReadPos;
	m_ReadPos += stringlen;
	return std::string(v, stringlen);
}

Position NetworkMessage::GetPosition()
{
	Position pos;
	pos.x = GetU16();
	pos.y = GetU16();
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

	AddU16(stringlen);
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

	AddU16(stringlen);
	strcpy((char*)m_MsgBuf + m_ReadPos, value);
	m_ReadPos += stringlen;
	m_MsgSize += stringlen;
}

void NetworkMessage::AddDouble(double value, uint8_t precision/* = 2*/)
{
	AddByte(precision);
	AddU32((value * std::pow((float)10, precision)) + INT_MAX);
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

void NetworkMessage::AddPaddingBytes(uint32_t n)
{
	if (!canAdd(n)) {
		return;
	}

	memset((void*)&m_MsgBuf[m_ReadPos], 0x33, n);
	m_MsgSize += n;
}

void NetworkMessage::AddPosition(const Position& pos)
{
	AddU16(pos.x);
	AddU16(pos.y);
	AddByte(pos.z);
}

void NetworkMessage::AddItem(uint16_t id, uint8_t count)
{
	const ItemType& it = Item::items[id];

	AddU16(it.clientId);

	AddByte(0xFF);    // MARK_UNMARKED

	if (it.stackable) {
		AddByte(count);
	} else if (it.isSplash() || it.isFluidContainer()) {
		uint32_t fluidIndex = count % 8;
		AddByte(fluidMap[fluidIndex]);
	}

	if (it.isAnimation) {
		AddByte(0xFE);    // random phase (0xFF for async)
	}
}

void NetworkMessage::AddItem(const Item* item)
{
	const ItemType& it = Item::items[item->getID()];

	AddU16(it.clientId);
	AddByte(0xFF);    // MARK_UNMARKED

	if (it.stackable) {
		AddByte(std::min<uint16_t>(0xFF, item->getSubType()));
	} else if (it.isSplash() || it.isFluidContainer()) {
		uint32_t fluidIndex = item->getSubType() % 8;
		AddByte(fluidMap[fluidIndex]);
	}

	if (it.isAnimation) {
		AddByte(0xFE);    // random phase (0xFF for async)
	}
}

void NetworkMessage::AddItemId(uint16_t itemId)
{
	const ItemType& it = Item::items[itemId];
	AddU16(it.clientId);
}
