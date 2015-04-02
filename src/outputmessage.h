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

#ifndef FS_OUTPUTMESSAGE_H_C06AAED85C7A43939F22D229297C0CC1
#define FS_OUTPUTMESSAGE_H_C06AAED85C7A43939F22D229297C0CC1

#include "networkmessage.h"
#include "connection.h"
#include "tools.h"

class Protocol;

#define OUTPUT_POOL_SIZE 100

class OutputMessage : public NetworkMessage
{
	private:
		OutputMessage();

	public:
		// non-copyable
		OutputMessage(const OutputMessage&) = delete;
		OutputMessage& operator=(const OutputMessage&) = delete;

		uint8_t* getOutputBuffer() {
			return &buffer[outputBufferStart];
		}

		void writeMessageLength() {
			add_header<uint16_t>(length);
		}

		void addCryptoHeader(bool addChecksum) {
			if (addChecksum) {
				add_header<uint32_t>(adlerChecksum(buffer + outputBufferStart, length));
			}

			add_header<uint16_t>(length);
		}

		enum OutputMessageState {
			STATE_FREE,
			STATE_ALLOCATED,
			STATE_ALLOCATED_NO_AUTOSEND,
			STATE_WAITING,
		};

		Protocol* getProtocol() const {
			return m_protocol;
		}
		Connection_ptr getConnection() const {
			return connection;
		}
		int64_t getFrame() const {
			return frame;
		}

		inline void append(const NetworkMessage& msg) {
			int32_t msgLen = msg.getLength();
			memcpy(buffer + position, msg.getBuffer() + 8, msgLen);
			length += msgLen;
			position += msgLen;
		}

		inline void append(OutputMessage_ptr msg) {
			int32_t msgLen = msg->getLength();
			memcpy(buffer + position, msg->getBuffer() + 8, msgLen);
			length += msgLen;
			position += msgLen;
		}

		void setFrame(int64_t new_frame) {
			frame = new_frame;
		}

	protected:
		template <typename T>
		inline void add_header(T add) {
			if (sizeof(T) > outputBufferStart) {
				std::cout << "Error: [OutputMessage::add_header] outputBufferStart(" << outputBufferStart <<
				          ") < " << sizeof(T) << std::endl;
				return;
			}

			outputBufferStart -= sizeof(T);
			*reinterpret_cast<T*>(buffer + outputBufferStart) = add;
			//added header size to the message size
			length += sizeof(T);
		}

		void freeMessage() {
			setConnection(Connection_ptr());
			setProtocol(nullptr);
			frame = 0;

			// Allocate enough size for headers:
			// 2 bytes for unencrypted message size
			// 4 bytes for checksum
			// 2 bytes for encrypted message size
			outputBufferStart = 8;

			//setState have to be the last one
			setState(OutputMessage::STATE_FREE);
		}

		friend class OutputMessagePool;

		void setProtocol(Protocol* protocol) {
			m_protocol = protocol;
		}
		void setConnection(Connection_ptr newConnection) {
			connection = newConnection;
		}

		void setState(OutputMessageState newState) {
			state = newState;
		}
		OutputMessageState getState() const {
			return state;
		}

		Connection_ptr connection;
		Protocol* m_protocol;

		int64_t frame;
		uint32_t outputBufferStart;

		OutputMessageState state;
};

class OutputMessagePool
{
	private:
		OutputMessagePool();

	public:
		~OutputMessagePool();

		// non-copyable
		OutputMessagePool(const OutputMessagePool&) = delete;
		OutputMessagePool& operator=(const OutputMessagePool&) = delete;

		static OutputMessagePool* getInstance() {
			static OutputMessagePool instance;
			return &instance;
		}

		void send(OutputMessage_ptr msg);
		void sendAll();
		void stop() {
			m_open = false;
		}
		OutputMessage_ptr getOutputMessage(Protocol* protocol, bool autosend = true);
		void startExecutionFrame();

		int64_t getFrameTime() const {
			return frameTime;
		}

		void addToAutoSend(OutputMessage_ptr msg);

	protected:
		void configureOutputMessage(OutputMessage_ptr msg, Protocol* protocol, bool autosend);
		void releaseMessage(OutputMessage* msg);
		void internalReleaseMessage(OutputMessage* msg);

		typedef std::list<OutputMessage*> InternalOutputMessageList;
		typedef std::list<OutputMessage_ptr> OutputMessageMessageList;

		InternalOutputMessageList outputMessages;
		InternalOutputMessageList allOutputMessages;
		OutputMessageMessageList autoSendOutputMessages;
		OutputMessageMessageList toAddQueue;
		std::recursive_mutex outputPoolLock;
		int64_t frameTime;
		bool m_open;
};
#endif
