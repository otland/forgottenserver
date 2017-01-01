/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2017  Mark Samman <mark.samman@gmail.com>
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

#ifndef FS_THREAD_HOLDER_H_BEB56FC46748E71D15A5BF0773ED2E67
#define FS_THREAD_HOLDER_H_BEB56FC46748E71D15A5BF0773ED2E67

#include <thread>
#include <atomic>
#include "enums.h"

template <typename Derived>
class ThreadHolder
{
	public:
		ThreadHolder() {}
		void start() {
			setState(THREAD_STATE_RUNNING);
			thread = std::thread(&Derived::threadMain, static_cast<Derived*>(this));
		}

		void stop() {
			setState(THREAD_STATE_CLOSING);
		}

		void join() {
			if (thread.joinable()) {
				thread.join();
			}
		}
	protected:
		void setState(ThreadState newState) {
			threadState.store(newState, std::memory_order_relaxed);
		}

		ThreadState getState() const {
			return threadState.load(std::memory_order_relaxed);
		}
	private:
		std::atomic<ThreadState> threadState{THREAD_STATE_TERMINATED};
		std::thread thread;
};

#endif
