/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2014  Mark Samman <mark.samman@gmail.com>
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

#ifndef FS_OPTIONAL_H_3066D9DCEF5FD19181A29F5FFC822713
#define FS_OPTIONAL_H_3066D9DCEF5FD19181A29F5FFC822713

template <typename T>
class Optional {
	public:
		Optional() : ptr(nullptr) {}
		Optional(const Optional& rhs) {
			if (rhs.ptr) {
				ptr = new T;
				*ptr = *rhs.ptr;
			} else {
				ptr = nullptr;
			}
		}
		Optional(Optional&& rhs) : ptr(rhs.ptr) {
			rhs.ptr = nullptr;
		}
		~Optional() {
			delete ptr;
		}
		void swap(Optional& other) {
			std::swap(ptr, other.ptr);
		}
		Optional& operator=(Optional other) {
			swap(other);
			return *this;
		}
		Optional& operator=(Optional&& other) {
			if (this != &other) {
				delete ptr;
				ptr = other.ptr;
				other.ptr = nullptr;
			}
			return *this;
		}
		T* operator->() const {
			return ptr;
		}
		operator bool() const {
			return ptr != nullptr;
		}
		T& operator*() {
			return *ptr;
		}

		void set(T* ptr) {
			this->ptr = ptr;
		}

	private:
		T* ptr;
};

#endif
