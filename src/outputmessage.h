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

#ifndef FS_OUTPUTMESSAGE_H_C06AAED85C7A43939F22D229297C0CC1
#define FS_OUTPUTMESSAGE_H_C06AAED85C7A43939F22D229297C0CC1

#include "networkmessage.h"
#include "connection.h"
#include "tools.h"

class Protocol;

class OutputMessage : public NetworkMessage
{
	public:
		OutputMessage() = default;

		// non-copyable
		OutputMessage(const OutputMessage&) = delete;
		OutputMessage& operator=(const OutputMessage&) = delete;

		uint8_t* getOutputBuffer() {
			return buffer + outputBufferStart;
		}

		void writeMessageLength() {
			add_header(info.length);
		}

		void addCryptoHeader(bool addChecksum) {
			if (addChecksum) {
				add_header(adlerChecksum(buffer + outputBufferStart, info.length));
			}

			writeMessageLength();
		}

		void append(const NetworkMessage& msg) {
			auto msgLen = msg.getLength();
			memcpy(buffer + info.position, msg.getBuffer() + 8, msgLen);
			info.length += msgLen;
			info.position += msgLen;
		}

		void append(const OutputMessage_ptr& msg) {
			auto msgLen = msg->getLength();
			memcpy(buffer + info.position, msg->getBuffer() + 8, msgLen);
			info.length += msgLen;
			info.position += msgLen;
		}

	private:
		template <typename T>
		void add_header(T add) {
			assert(outputBufferStart >= sizeof(T));
			outputBufferStart -= sizeof(T);
			memcpy(buffer + outputBufferStart, &add, sizeof(T));
			//added header size to the message size
			info.length += sizeof(T);
		}

		MsgSize_t outputBufferStart = INITIAL_BUFFER_POSITION;
};

class OutputMessagePool
{
	public:
		// non-copyable
		OutputMessagePool(const OutputMessagePool&) = delete;
		OutputMessagePool& operator=(const OutputMessagePool&) = delete;

		static OutputMessagePool& getInstance() {
			static OutputMessagePool instance;
			return instance;
		}

		void sendAll();
		void scheduleSendAll();

		static OutputMessage_ptr getOutputMessage();

		void addProtocolToAutosend(Protocol_ptr protocol);
		void removeProtocolFromAutosend(const Protocol_ptr& protocol);
	private:
		OutputMessagePool() = default;
		//NOTE: A vector is used here because this container is mostly read
		//and relatively rarely modified (only when a client connects/disconnects)
		std::vector<Protocol_ptr> bufferedProtocols;
};


#endif
