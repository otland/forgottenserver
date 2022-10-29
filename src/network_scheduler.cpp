// Copyright 2022 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "otpch.h"

#include "network_scheduler.h"

#include "enums.h"
#include "game.h"

extern Game g_game;

NetworkTask* createNetworkTask(NetworkTaskFunc&& f) { return new NetworkTask(std::move(f)); }

NetworkTask* createNetworkTask(uint32_t expiration, NetworkTaskFunc&& f) { return new NetworkTask(expiration, std::move(f)); }

void NetworkScheduler::run()
{
	std::vector<NetworkTask*> tmpTaskList;
	// NOTE: second argument defer_lock is to prevent from immediate locking
	std::unique_lock<std::mutex> taskLockUnique(taskLock, std::defer_lock);

	while (getState() != THREAD_STATE_TERMINATED) {
		// check if there are tasks waiting
		taskLockUnique.lock();
		if (taskList.empty()) {
			// if the list is empty wait for signal
			taskSignal.wait(taskLockUnique);
		}
		tmpTaskList.swap(taskList);
		taskLockUnique.unlock();

		for (NetworkTask* task : tmpTaskList) {
			if (!task->hasExpired()) {
				++cycle;
				// execute it
				(*task)();
			}
			delete task;
		}
		tmpTaskList.clear();
	}
}

void NetworkScheduler::addTask(NetworkTask* task)
{
	bool do_signal = false;

	taskLock.lock();

	if (getState() == THREAD_STATE_RUNNING) {
		do_signal = taskList.empty();
		taskList.push_back(task);
	} else {
		delete task;
	}

	taskLock.unlock();

	// send a signal if the list was empty
	if (do_signal) {
		taskSignal.notify_one();
	}
}

void NetworkScheduler::shutdown()
{
	NetworkTask* task = createNetworkTask([this]() {
		ThreadHolder::shutdown();
		taskSignal.notify_one();
	});

	std::lock_guard<std::mutex> lockClass(taskLock);
	taskList.push_back(task);

	taskSignal.notify_one();
}
