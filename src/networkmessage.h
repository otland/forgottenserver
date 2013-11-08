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

#ifndef __OTSERV_NETWORK_MESSAGE_H__
#define __OTSERV_NETWORK_MESSAGE_H__

#include "definitions.h"
#include "const.h"

class Item;
class Creature;
class Player;
struct Position;
class RSA;

class NetworkMessage
{
	public:
		enum { header_length = 2 };
		enum { crypto_length = 4 };
		enum { xtea_multiple = 8 };
		enum { max_body_length = NETWORKMESSAGE_MAXSIZE - header_length - crypto_length - xtea_multiple };
		enum { max_protocol_body_length = max_body_length - 10 };

		// constructor/destructor
		NetworkMessage() {
			m_MsgBuf = m_RealBuf;
			Reset();
		}
		virtual ~NetworkMessage() {}

		// resets the internal buffer to an empty message

	protected:
		void Reset() {
			m_overrun = false;
			m_MsgSize = 0;
			m_ReadPos = 8;
		}

	public:
		// simply read functions for incoming message
		uint8_t GetByte() {
			if (!canRead(1)) {
				return 0;
			}

			return m_RealBuf[m_ReadPos++];
		}
		uint16_t GetU16() {
			if (!canRead(2)) {
				return 0;
			}

			uint16_t v = *(uint16_t*)(m_MsgBuf + m_ReadPos);
			m_ReadPos += 2;
			return v;
		}
		uint16_t GetSpriteId() {
			return GetU16();
		}
		uint32_t GetU32() {
			if (!canRead(4)) {
				return 0;
			}

			uint32_t v = *(uint32_t*)(m_MsgBuf + m_ReadPos);
			m_ReadPos += 4;
			return v;
		}
		uint32_t PeekU32() {
			if (!canRead(4)) {
				return 0;
			}

			uint32_t v = *(uint32_t*)(m_MsgBuf + m_ReadPos);
			return v;
		}
		uint64_t GetU64() {
			if (!canRead(8)) {
				return 0;
			}

			uint64_t v = *(uint64_t*)(m_MsgBuf + m_ReadPos);
			m_ReadPos += 8;
			return v;
		}
		std::string GetString(uint16_t stringlen = 0);
		std::string GetRaw() {
			return GetString(m_MsgSize - m_ReadPos);
		}
		Position GetPosition();

		// skips count unknown/unused bytes in an incoming message
		void SkipBytes(int count) {
			m_ReadPos += count;
		}

		// simply write functions for outgoing message
		void AddByte(uint8_t  value) {
			if (!canAdd(1)) {
				return;
			}

			m_RealBuf[m_ReadPos++] = value;
			m_MsgSize++;
		}
		void AddU16(uint16_t value) {
			if (!canAdd(2)) {
				return;
			}

			*(uint16_t*)(m_MsgBuf + m_ReadPos) = value;
			m_ReadPos += 2;
			m_MsgSize += 2;
		}
		void AddU32(uint32_t value) {
			if (!canAdd(4)) {
				return;
			}

			*(uint32_t*)(m_MsgBuf + m_ReadPos) = value;
			m_ReadPos += 4;
			m_MsgSize += 4;
		}
		void AddU64(uint64_t value) {
			if (!canAdd(8)) {
				return;
			}

			*(uint64_t*)(m_MsgBuf + m_ReadPos) = value;
			m_ReadPos += 8;
			m_MsgSize += 8;
		}
		void AddBytes(const char* bytes, size_t size);
		void AddPaddingBytes(size_t n);

		void AddString(const std::string& value);
		void AddString(const char* value);

		void AddDouble(double value, uint8_t precision = 2);

		// write functions for complex types
		void AddPosition(const Position& pos);
		void AddItem(uint16_t id, uint8_t count);
		void AddItem(const Item* item);
		void AddItemId(uint16_t itemId);

		int32_t getMessageLength() const {
			return m_MsgSize;
		}
		void setMessageLength(int32_t newSize) {
			m_MsgSize = newSize;
		}
		int32_t getReadPos() const {
			return m_ReadPos;
		}
		void setReadPos(int32_t pos) {
			m_ReadPos = pos;
		}

		int32_t decodeHeader();

		bool isOverrun() const {
			return m_overrun;
		}

		uint8_t* getBuffer() const {
			return m_MsgBuf;
		}
		char* getBodyBuffer() {
			m_ReadPos = 2;
			return (char*)&m_RealBuf[header_length];
		}

	protected:
		inline bool canAdd(size_t size) const {
			return (size + m_ReadPos < max_body_length);
		}

		inline bool canRead(int32_t size) {
			if ((m_ReadPos + size) > (m_MsgSize + 8) || size >= (NETWORKMESSAGE_MAXSIZE - m_ReadPos)) {
				m_overrun = true;
				return false;
			}
			return true;
		}

		int32_t m_MsgSize;
		int32_t m_ReadPos;

		bool m_overrun;

		uint8_t m_RealBuf[NETWORKMESSAGE_MAXSIZE];
		uint8_t* m_MsgBuf;
};

#endif // #ifndef __NETWORK_MESSAGE_H__
