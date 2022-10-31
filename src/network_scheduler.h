// Copyright 2022 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_NETWORK_SCHEDULER_H
#define FS_NETWORK_SCHEDULER_H

#include "thread_holder_base.h"

using NetworkTaskFunc = std::function<void(void)>;
const int NETWORKSCHEDULER_TASK_EXPIRATION = 2000;
const auto SYSTEM_TIME_ZERO = std::chrono::system_clock::time_point(std::chrono::milliseconds(0));

class NetworkTask
{
public:
	// DO NOT allocate this class on the stack
	explicit NetworkTask(NetworkTaskFunc&& f) : func(std::move(f)) {}
	NetworkTask(uint32_t ms, NetworkTaskFunc&& f) :
	    expiration(std::chrono::system_clock::now() + std::chrono::milliseconds(ms)), func(std::move(f))
	{}

	virtual ~NetworkTask() = default;
	void operator()() { func(); }

	void setDontExpire() { expiration = SYSTEM_TIME_ZERO; }

	bool hasExpired() const
	{
		if (expiration == SYSTEM_TIME_ZERO) {
			return false;
		}
		return expiration < std::chrono::system_clock::now();
	}

protected:
	std::chrono::system_clock::time_point expiration = SYSTEM_TIME_ZERO;

private:
	// Expiration has another meaning for scheduler
	// tasks, then it is the time the task should
	// be added to the Network Scheduler.
	NetworkTaskFunc func;
};

NetworkTask* createNetworkTask(NetworkTaskFunc&& f);
NetworkTask* createNetworkTask(uint32_t expiration, NetworkTaskFunc&& f);

class NetworkScheduler : public ThreadHolder<NetworkScheduler>
{
public:
	void addTask(NetworkTask* task);

	void addTask(NetworkTaskFunc&& f) { addTask(new NetworkTask(std::move(f))); }

	void addTask(uint32_t expiration, NetworkTaskFunc&& f) { addTask(new NetworkTask(expiration, std::move(f))); }

	void shutdown();

	uint64_t getCycle() const { return cycle; }

	void run();

private:
	std::mutex taskLock;
	std::condition_variable taskSignal;
	std::vector<NetworkTask*> taskList;

	uint64_t cycle = 0;
};

extern NetworkScheduler g_networkScheduler;

#endif // FS_NETWORK_SCHEDULER_H
