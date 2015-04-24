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
class PoolingAllocator : public std::allocator<T>
{
	public:
		template <typename U>
		PoolingAllocator(const U&) noexcept {}
		typedef T value_type;

		T* allocate(size_t n) const {
			T* p;
			if (!getFreeList().pop(p)) {
				//Acquire memory without calling the constructor of T
				p = static_cast<T*>(operator new (sizeof(T)));
			}
			return p;
		}

		void deallocate(T* p, size_t n) const {
			if (!getFreeList().push(p)) {
				//Release memory without calling the destructor of T
				//(it has already been called at this point)
				operator delete(p);
			}
		}

		~PoolingAllocator() {
			T* ptr;
			auto& freeList = getFreeList();
			while(freeList.pop(ptr)) {
				operator delete(ptr);
			}
		}
	private:
		typedef LockfreeBoundedStack<T*, OUTPUTMESSAGE_FREE_LIST_CAPACITY> FreeList;
		static FreeList& getFreeList() {
			static FreeList freeList;
			return freeList;
		}
};

class OutputMessageAllocator
{
	public:
		typedef OutputMessage value_type;
		template<typename U>
		struct rebind {typedef PoolingAllocator<U> other;};
};

void OutputMessagePool::sendAll()
{
	//dispatcher thread
	static int64_t lastSend = 0;

	auto currentFrame = OTSYS_TIME();
	auto staleTime = currentFrame - 10;
	if (lastSend > staleTime) {
		return;
	}
	lastSend = currentFrame;

	for (auto& protocol : bufferedProtocols) {
		auto& msg = protocol->getCurrentBuffer();
		if (msg) {
			protocol->send(std::move(msg));
		}
	}
}

void OutputMessagePool::addProtocolToAutosend(Protocol_ptr protocol) 
{
	//dispatcher thread
	bufferedProtocols.emplace_back(protocol);
}

void OutputMessagePool::removeProtocolFromAutosend(const Protocol_ptr& protocol)
{
	//dispatcher thread
	auto it = std::find(bufferedProtocols.begin(), bufferedProtocols.end(), protocol);
	if (it != bufferedProtocols.end()) {
		bufferedProtocols.erase(it);
	}
}

OutputMessage_ptr OutputMessagePool::getOutputMessage()
{
	return std::allocate_shared<OutputMessage>(OutputMessageAllocator());
}
