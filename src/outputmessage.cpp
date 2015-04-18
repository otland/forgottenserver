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

#include "outputmessage.h"
#include "protocol.h"

template <typename T>
class RebindingAllocator : public std::allocator<T>
{
	public:
		template <typename U>
		RebindingAllocator(const U&) noexcept {}
		typedef T value_type;

		T* allocate(size_t n) const noexcept {
			assert(n == 1);
			T* p;
			if (!getFreeList().pop(p)) {
				//Acquire memory without calling the constructor of T
				p = static_cast<T*>(operator new (sizeof(T)));
			}
			return p;
		}
	
		void deallocate(T* p, size_t n) const noexcept {
			assert(n == 1);
			if (!getFreeList().push(p)) {
				//Release memory without calling the destructor of T
				//(it has already been called at this point)
				operator delete(p);
			}
		}
		
		~RebindingAllocator() {
			/*getFreeList().consume_all([](T* ptr) {
				operator delete(ptr);
			});*/
			
		}
	private:
		typedef LockfreeBoundedStack<T*, OUTPUTMESSAGE_FREE_LIST_CAPACITY> FreeList;
		static FreeList& getFreeList() noexcept {
			static FreeList freeList;
			return freeList;
		}
};

class OutputMessageAllocator
{
	public:
		typedef OutputMessage value_type;
		template<typename U>
		struct rebind {typedef RebindingAllocator<U> other;};
};
OutputMessagePool::OutputMessagePool()
{
	startExecutionFrame();
}

void OutputMessagePool::startExecutionFrame()
{
	frameTime.store(OTSYS_TIME(), std::memory_order_relaxed);
}

void OutputMessagePool::internalSend(const OutputMessage_ptr& msg) const
{
	msg->getConnection()->send(msg);
}

void OutputMessagePool::send(const OutputMessage_ptr& msg) const
{
	assert(msg->getState() == OutputMessage::STATE_ALLOCATED_NO_AUTOSEND);
	internalSend(msg);
}

void OutputMessagePool::sendAll()
{
	//dispatcher thread
	const auto currentFrameTime = frameTime.load(std::memory_order_relaxed);
	const int64_t dropTime = currentFrameTime - 10000;
	const int64_t staleTime = currentFrameTime - 10;
	static OutputMessageList autoSendBuffer;
	OutputMessageList toAddBuffer;

	{
		std::lock_guard<std::mutex> lockClass(outputPoolLock);
		autoSendBuffer.splice(autoSendBuffer.end(), autoSendOutputMessages);
		toAddBuffer = std::move(toAddQueue);
	}

	for (auto& msg : toAddBuffer) {
		const int64_t msgFrame = msg->getFrame();
		if (msgFrame >= dropTime) {
			msg->setState(OutputMessage::STATE_ALLOCATED);

			if (msgFrame < staleTime) {
				autoSendBuffer.emplace_front(std::move(msg));
			} else {
				autoSendBuffer.emplace_back(std::move(msg));
			}
		} else {
			//drop messages that are older than 10 seconds
			auto connection = msg->getConnection();
			assert(connection);
			connection->getProtocol()->clearOutputBuffer(msg);
		}
	}

	for (auto it = autoSendBuffer.begin(), end = autoSendBuffer.end(); it != end; it = autoSendBuffer.erase(it)) {
		const OutputMessage_ptr& msg = *it;
		if (staleTime <= msg->getFrame()) {
			return;
		}
		internalSend(msg);
	}
}

std::vector<OutputMessage_ptr> OutputMessagePool::getOutputMessages(const std::vector<Protocol_ptr>& protocols)
{
	std::vector<OutputMessage_ptr> ret;
	ret.reserve(protocols.size());

	for (const auto& protocol : protocols) {
		auto connection = protocol->getConnection();
		if (connection && m_open.load(std::memory_order_relaxed)) {
			ret.emplace_back(internalGetMessage(std::move(connection), false));
		}
	}

	return ret;
}

OutputMessage_ptr OutputMessagePool::getOutputMessage(const Protocol_ptr& protocol, const bool autosend /*= true*/)
{
	assert(protocol);
	OutputMessage_ptr msg;
	auto connection = protocol->getConnection();
	if (!connection || !m_open.load(std::memory_order_relaxed)) {
		return msg;
	}

	msg = internalGetMessage(std::move(connection), autosend);
	return msg;
}

OutputMessage_ptr OutputMessagePool::internalGetMessage(Connection_ptr&& connection, const bool autosend)
{
	OutputMessageAllocator alloc;
	assert(connection && connection->getProtocol());
	auto msg = std::allocate_shared<OutputMessage>(alloc, std::forward<Connection_ptr>(connection), frameTime.load(std::memory_order_relaxed));

	if (autosend) {
		msg->setState(OutputMessage::STATE_ALLOCATED);
		std::lock_guard<std::mutex> lockClass(outputPoolLock);
		autoSendOutputMessages.push_back(msg);
	} else {
		msg->setState(OutputMessage::STATE_ALLOCATED_NO_AUTOSEND);
	}

	return msg;
}

void OutputMessagePool::addToAutoSend(const OutputMessage_ptr& msg)
{
	std::lock_guard<std::mutex> lockClass(outputPoolLock);
	toAddQueue.push_back(msg);
}
