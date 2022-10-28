// Copyright 2022 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_THREAD_HOLDER_BASE_H
#define FS_THREAD_HOLDER_BASE_H

#include "enums.h"

template <typename Derived>
class ThreadHolder
{
public:
	ThreadHolder() {}
	void start()
	{
		setState(THREAD_STATE_RUNNING);
		thread = std::thread(&Derived::run, static_cast<Derived*>(this));
	}

	void stop() { setState(THREAD_STATE_CLOSING); }
	void shutdown() { setState(THREAD_STATE_CLOSING); }

	void join()
	{
		if (thread.joinable()) {
			thread.join();
		}
	}

protected:
	ThreadState getState() const { return state.load(std::memory_order_relaxed); }

private:
	std::thread thread;
	std::atomic<ThreadState> state{THREAD_STATE_TERMINATED};

	void setState(ThreadState newState) { state.store(newState, std::memory_order_relaxed); }
};

#endif // FS_THREAD_HOLDER_BASE_H
