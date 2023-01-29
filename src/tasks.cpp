// Copyright 2022 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "otpch.h"

#include "tasks.h"
#include "game.h"

extern Game g_game;

Task* createTaskWithStats(TaskFunc&& f, const std::string& description, const std::string& extraDescription)
{
	return new Task(std::move(f), description, extraDescription);
}

Task* createTaskWithStats(uint32_t expiration, TaskFunc&& f, const std::string& description, const std::string& extraDescription)
{
	return new Task(expiration, std::move(f), description, extraDescription);
}

void Dispatcher::threadMain()
{
	std::vector<Task*> tmpTaskList;
	// NOTE: second argument defer_lock is to prevent from immediate locking
	std::unique_lock<std::mutex> taskLockUnique(taskLock, std::defer_lock);
#ifdef STATS_ENABLED
	std::chrono::high_resolution_clock::time_point time_point;
#endif

	while (getState() != THREAD_STATE_TERMINATED) {
		// check if there are tasks waiting
		taskLockUnique.lock();
		if (taskList.empty()) {
			//if the list is empty wait for signal
#ifdef STATS_ENABLED
			time_point = std::chrono::high_resolution_clock::now();
			taskSignal.wait(taskLockUnique);
			g_stats.dispatcherWaitTime(dispatcherId) += std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - time_point).count();
#else
			taskSignal.wait(taskLockUnique);
#endif
		}
		tmpTaskList.swap(taskList);
		taskLockUnique.unlock();

		for (Task* task : tmpTaskList) {
#ifdef STATS_ENABLED
			time_point = std::chrono::high_resolution_clock::now();
#endif
			if (!task->hasExpired()) {
				++dispatcherCycle;
				// execute it
				(*task)();
			}
#ifdef STATS_ENABLED
			task->executionTime = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - time_point).count();
			g_stats.addDispatcherTask(dispatcherId, task);
#else
			delete task;
#endif
		}
		tmpTaskList.clear();
	}
}

void Dispatcher::addTask(Task* task)
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

void Dispatcher::shutdown()
{
	Task* task = createTask([this]() {
		setState(THREAD_STATE_TERMINATED);
		taskSignal.notify_one();
	});

	std::lock_guard<std::mutex> lockClass(taskLock);
	taskList.push_back(task);

	taskSignal.notify_one();
}
