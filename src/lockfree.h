/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2016  Mark Samman <mark.samman@gmail.com>
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

template <typename T, size_t CAPACITY>
class LockfreePoolingAllocator : public std::allocator<T>
{
	public:
		template <typename U>
		explicit constexpr LockfreePoolingAllocator(const U&) {}
		typedef T value_type;

		T* allocate(size_t) const {
			T* p; // NOTE: p doesn't have to be initialized
			if (!getFreeList().pop(p)) {
				//Acquire memory without calling the constructor of T
				p = static_cast<T*>(operator new (sizeof(T)));
			}
			return p;
		}

		void deallocate(T* p, size_t) const {
			if (!getFreeList().bounded_push(p)) {
				//Release memory without calling the destructor of T
				//(it has already been called at this point)
				operator delete(p);
			}
		}

	private:
		typedef boost::lockfree::stack<T*, boost::lockfree::capacity<CAPACITY>> FreeList;
		static FreeList& getFreeList() {
			static FreeList freeList;
			return freeList;
		}
};

#endif
