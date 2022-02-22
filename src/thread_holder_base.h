// Copyright 2022 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

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
