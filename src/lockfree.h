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

#ifndef FS_LOCKFREE_H_8C707AEB7C7235A2FBC5D4EDDF03B008
#define FS_LOCKFREE_H_8C707AEB7C7235A2FBC5D4EDDF03B008

#if _MSC_FULL_VER >= 190023918 // Workaround for VS2015 Update 2. Boost.Lockfree is a header-only library, so this should be safe to do.
#define _ENABLE_ATOMIC_ALIGNMENT_FIX
#endif

#include <boost/lockfree/stack.hpp>

/*
 * we use this to avoid instantiating multiple free lists for objects of the
 * same size and it can be replaced by a variable template in C++14
 *
 * template <std::size_t TSize, size_t CAPACITY>
 * boost::lockfree::stack<void*, boost::lockfree::capacity<CAPACITY> lockfreeFreeList;
 */
template <std::size_t TSize, size_t CAPACITY>
struct LockfreeFreeList
{
	using FreeList = boost::lockfree::stack<void*, boost::lockfree::capacity<CAPACITY>>;
	static FreeList& get()
	{
		static FreeList freeList;
		return freeList;
	}
};

template <typename T, size_t CAPACITY>
class LockfreePoolingAllocator : public std::allocator<T>
{
	public:
		LockfreePoolingAllocator() = default;

		template <typename U, class = typename std::enable_if<!std::is_same<U, T>::value>::type>
		explicit constexpr LockfreePoolingAllocator(const U&) {}
		using value_type = T;

		T* allocate(size_t) const {
			auto& inst = LockfreeFreeList<sizeof(T), CAPACITY>::get();
			void* p; // NOTE: p doesn't have to be initialized
			if (!inst.pop(p)) {
				//Acquire memory without calling the constructor of T
				p = operator new (sizeof(T));
			}
			return static_cast<T*>(p);
		}

		void deallocate(T* p, size_t) const {
			auto& inst = LockfreeFreeList<sizeof(T), CAPACITY>::get();
			if (!inst.bounded_push(p)) {
				//Release memory without calling the destructor of T
				//(it has already been called at this point)
				operator delete(p);
			}
		}
};

#endif
