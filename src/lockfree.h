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

#ifndef FS_LOCKFREE_H_8C707AEB7C7235A2FBC5D4EDDF03B008
#define FS_LOCKFREE_H_8C707AEB7C7235A2FBC5D4EDDF03B008

#include <atomic>

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

template <typename T>
class LockfreePoolingAllocator : public std::allocator<T>
{
	public:
		template <typename U>
		LockfreePoolingAllocator(const U&) {}
		typedef T value_type;

		T* allocate(size_t) const {
			T* p;
			if (!getFreeList().pop(p)) {
				//Acquire memory without calling the constructor of T
				p = static_cast<T*>(operator new (sizeof(T)));
			}
			return p;
		}

		void deallocate(T* p, size_t) const {
			if (!getFreeList().push(p)) {
				//Release memory without calling the destructor of T
				//(it has already been called at this point)
				operator delete(p);
			}
		}
	private:
		typedef LockfreeBoundedStack<T*, OUTPUTMESSAGE_FREE_LIST_CAPACITY> FreeList;
		static FreeList& getFreeList() noexcept {
			static FreeList freeList;
			return freeList;
		}
};

#endif
