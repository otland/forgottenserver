// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "otpch.h"

#include "tasks.h"

#include "enums.h"
#include "game.h"

extern Game g_game;

Task_ptr createTask(TaskFunc&& f) { return std::make_unique<Task>(std::move(f)); }

Task_ptr createTask(uint32_t expiration, TaskFunc&& f) { return std::make_unique<Task>(expiration, std::move(f)); }

void Dispatcher::threadMain()
{
	std::vector<std::unique_ptr<Task>> tmpTaskList;
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

		for (auto &task : tmpTaskList) {
			if (!task->hasExpired()) {
				++dispatcherCycle;
				// execute it
				(*task)();
			}
		}
		tmpTaskList.clear();
	}
}

void Dispatcher::addTask(Task_ptr task)
{
	bool do_signal = false;

	taskLock.lock();

	if (getState() == THREAD_STATE_RUNNING) {
		do_signal = taskList.empty();
		taskList.push_back(std::move(task));
	}

	taskLock.unlock();

	// send a signal if the list was empty
	if (do_signal) {
		taskSignal.notify_one();
	}
}

void Dispatcher::shutdown()
{
	Task_ptr task = createTask([this]() {
		setState(THREAD_STATE_TERMINATED);
		taskSignal.notify_one();
	});

	std::lock_guard<std::mutex> lockClass(taskLock);
	taskList.push_back(std::move(task));

	taskSignal.notify_one();
}
