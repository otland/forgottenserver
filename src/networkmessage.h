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

		// constructor
		NetworkMessage() {
			Reset();
		}

	protected:
		void Reset() {
			overrun = false;
			length = 0;
			position = 8;
		}

	public:
		// simply read functions for incoming message
		uint8_t GetByte() {
			if (!canRead(1)) {
				return 0;
			}

			return buffer[position++];
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

		std::string GetString(uint16_t stringLen = 0);
		Position GetPosition();

		// skips count unknown/unused bytes in an incoming message
		void SkipBytes(int count) {
			position += count;
		}

		// simply write functions for outgoing message
		void AddByte(uint8_t  value) {
			if (!canAdd(1)) {
				return;
			}

			buffer[position++] = value;
			length++;
		}

		template<typename T>
		void Add(T value) {
			if (!canAdd(sizeof(T))) {
				return;
			}

			*reinterpret_cast<T*>(buffer + position) = value;
			position += sizeof(T);
			length += sizeof(T);
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

		int32_t getLength() const {
			return length;
		}

		void setLength(int32_t newLength) {
			length = newLength;
		}

		int32_t getPosition() const {
			return position;
		}

		void setReadPos(int32_t pos) {
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
			return &buffer[header_length];
		}

	protected:
		inline bool canAdd(size_t size) const {
			return (size + position) < max_body_length;
		}

		inline bool canRead(int32_t size) {
			if ((position + size) > (size + 8) || size >= (NETWORKMESSAGE_MAXSIZE - position)) {
				overrun = true;
				return false;
			}
			return true;
		}

		uint8_t buffer[NETWORKMESSAGE_MAXSIZE];
		int32_t length;
		int32_t position;
		bool overrun;
};

#endif // #ifndef __NETWORK_MESSAGE_H__
