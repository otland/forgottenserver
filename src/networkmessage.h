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

#ifndef FS_NETWORKMESSAGE_H_B853CFED58D1413A87ACED07B2926E03
#define FS_NETWORKMESSAGE_H_B853CFED58D1413A87ACED07B2926E03

#include "const.h"
#include <limits>

class Item;
class Creature;
class Player;
struct Position;
class RSA;

class NetworkMessage
{
	public:
		// non-moveable
		NetworkMessage(NetworkMessage&&) = delete;
		NetworkMessage& operator=(NetworkMessage&&) = delete;
		typedef uint16_t MsgSize_t;
		static_assert(std::numeric_limits<MsgSize_t>::max() > NETWORKMESSAGE_MAXSIZE, "MsgSize type too small.");
		// Headers:
		// 2 bytes for unencrypted message size
		// 4 bytes for checksum
		// 2 bytes for encrypted message size
		static constexpr MsgSize_t INITIAL_BUFFER_POSITION = 8;
		enum { header_length = 2 };
		enum { crypto_length = 4 };
		enum { xtea_multiple = 8 };
		enum { max_body_length = NETWORKMESSAGE_MAXSIZE - header_length - crypto_length - xtea_multiple };
		enum { max_protocol_body_length = max_body_length - 10 };

		NetworkMessage() {
			reset();
		}

		void reset() {
			overrun = false;
			length = 0;
			position = INITIAL_BUFFER_POSITION;
		}

		// simply read functions for incoming message
		uint8_t getByte() {
			if (!canRead(1)) {
				return 0;
			}

			return buffer[position++];
		}

		uint8_t getPreviousByte() {
			return buffer[--position];
		}

		template<typename T>
		T get() {
			if (!canRead(sizeof(T))) {
				return 0;
			}

			T v = *reinterpret_cast<T*>(buffer + position);
			position += sizeof(T);
			return v;
		}

		std::string getString(uint16_t stringLen = 0);
		Position getPosition();

		// skips count unknown/unused bytes in an incoming message
		void skipBytes(int16_t count) {
			position += count;
		}

		// simply write functions for outgoing message
		void addByte(uint8_t value) {
			if (!canAdd(1)) {
				return;
			}

			buffer[position++] = value;
			length++;
		}

		template<typename T>
		void add(T value) {
			if (!canAdd(sizeof(T))) {
				return;
			}

			*reinterpret_cast<T*>(buffer + position) = value;
			position += sizeof(T);
			length += sizeof(T);
		}

		void addBytes(const char* bytes, size_t size);
		void addPaddingBytes(size_t n);

		void addString(const std::string& value);

		void addDouble(double value, uint8_t precision = 2);

		// write functions for complex types
		void addPosition(const Position& pos);
		void addItem(uint16_t id, uint8_t count);
		void addItem(const Item* item);
		void addItemId(uint16_t itemId);

		MsgSize_t getLength() const {
			return length;
		}

		void setLength(MsgSize_t newLength) {
			length = newLength;
		}

		MsgSize_t getBufferPosition() const {
			return position;
		}

		void setBufferPosition(MsgSize_t pos) {
			position = pos;
		}

		int32_t decodeHeader();

		bool isOverrun() const {
			return overrun;
		}

		uint8_t* getBuffer() {
			return buffer;
		}

		const uint8_t* getBuffer() const {
			return buffer;
		}

		uint8_t* getBodyBuffer() {
			position = 2;
			return buffer + header_length;
		}

	protected:
		inline bool canAdd(size_t size) const {
			return (size + position) < max_body_length;
		}

		inline bool canRead(int32_t size) {
			if ((position + size) > (length + 8) || size >= (NETWORKMESSAGE_MAXSIZE - position)) {
				overrun = true;
				return false;
			}
			return true;
		}

		MsgSize_t length;
		MsgSize_t position;
		bool overrun;

		uint8_t buffer[NETWORKMESSAGE_MAXSIZE];
};

#endif // #ifndef __NETWORK_MESSAGE_H__
