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
#include <atomic>
#include <limits>

const uint16_t OUTPUTMESSAGE_FREE_LIST_CAPACITY = 4096;

class Protocol;

class OutputMessage : public NetworkMessage
{
	public:
		OutputMessage():
			outputBufferStart(INITIAL_BUFFER_POSITION) {}

		// non-copyable
		OutputMessage(const OutputMessage&) = delete;
		OutputMessage& operator=(const OutputMessage&) = delete;
		// non-moveable
		OutputMessage(OutputMessage&&) = delete;
		OutputMessage& operator=(OutputMessage&&) = delete;

		uint8_t* getOutputBuffer() {
			return &buffer[outputBufferStart];
		}

		void writeMessageLength() {
			add_header(length);
		}

		void addCryptoHeader(bool addChecksum) {
			if (addChecksum) {
				add_header(adlerChecksum(buffer + outputBufferStart, length));
			}

			writeMessageLength();
		}

		inline void append(const NetworkMessage& msg) {
			auto msgLen = msg.getLength();
			memcpy(buffer + position, msg.getBuffer() + 8, msgLen);
			length += msgLen;
			position += msgLen;
		}

		inline void append(const OutputMessage_ptr& msg) {
			auto msgLen = msg->getLength();
			memcpy(buffer + position, msg->getBuffer() + 8, msgLen);
			length += msgLen;
			position += msgLen;
		}

	protected:
		template <typename T>
		inline void add_header(T add) {
			assert(outputBufferStart >= sizeof(T));
			outputBufferStart -= sizeof(T);
			*reinterpret_cast<T*>(buffer + outputBufferStart) = add;
			//added header size to the message size
			length += sizeof(T);
		}

		MsgSize_t outputBufferStart;
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

		static OutputMessage_ptr getOutputMessage();

		void addProtocolToAutosend(Protocol_ptr protocol);
		void removeProtocolFromAutosend(const Protocol_ptr& protocol);
	private:
		OutputMessagePool() = default;
		std::vector<Protocol_ptr> bufferedProtocols;
};

template <typename T, uint16_t CAPACITY>
class LockfreeBoundedStack {
	private:
		struct Node
		{
			static constexpr uint16_t END = std::numeric_limits<decltype(CAPACITY)>::max();
			uint16_t generation = 0;
			uint16_t index = END;
			uint16_t nextNode = END;
			uint16_t unused = 0;
			operator bool () const noexcept {
				return index != END;
			}
		};
		static_assert(sizeof(Node) == sizeof(uint64_t), "Invalid node size.");
		static_assert(CAPACITY != 0 && CAPACITY < Node::END, "Specified capacity out of range.");
	public:
		LockfreeBoundedStack() {
			for (uint16_t i = 0; i < CAPACITY; ++i) {
				nodes[i].index = i;
				internal_push(freeNodes, nodes[i]);
			}
		}

		LockfreeBoundedStack(const LockfreeBoundedStack&) = delete;
		LockfreeBoundedStack& operator=(const LockfreeBoundedStack&) = delete;

		bool pop(T& ret) noexcept {
			if (auto node = internal_pop(head)) {
				ret = std::move(getPayload(node));
				internal_push(freeNodes, node);
				return true;
			}

			return false; //The stack was empty
		}

		bool push(T value) noexcept {
			if (auto node = internal_pop(freeNodes)) {
				getPayload(node) = value;
				internal_push(head, node);
				return true;
			}

			return false; //Exceeded maximum capacity
		}
	private:
		Node internal_pop(std::atomic<Node>& listHead) noexcept {
			Node currentHead, nextNode;
			currentHead = listHead.load(std::memory_order_acquire);
			do {
				if (!currentHead) {
					break;
				}
				nextNode = getNextNode(currentHead);
			} while (!listHead.compare_exchange_weak(currentHead, nextNode,
								 std::memory_order_release,
								 std::memory_order_relaxed));

			return currentHead;
		}

		void internal_push(std::atomic<Node>& listHead, Node newNode) noexcept {
			auto currentHead = listHead.load(std::memory_order_acquire);
			newNode.generation++;
			do {
				newNode.nextNode = currentHead.index;
				nodes[newNode.index] = newNode;
			} while (!listHead.compare_exchange_weak(currentHead, newNode, 
								 std::memory_order_release,
								 std::memory_order_relaxed));
		}

		T& getPayload(Node node) noexcept {
			return payloads[node.index];
		}

		Node getNextNode(Node node) const noexcept {
			if (node.nextNode == Node::END) {
				return Node();
			}
			return nodes[node.nextNode];
		}

		T payloads[CAPACITY];
		Node nodes[CAPACITY];

		//TODO: Add padding to prevent these two from being on the same cache line
		std::atomic<Node> freeNodes;
		std::atomic<Node> head;
};

#endif
