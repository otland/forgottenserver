/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2015  Mark Samman <mark.samman@gmail.com>
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

#include "otpch.h"

#include "tasks.h"
#include "outputmessage.h"
#include "game.h"

extern Game g_game;

Dispatcher::Dispatcher()
{
	threadState = THREAD_STATE_TERMINATED;
}

void Dispatcher::start()
{
	threadState = THREAD_STATE_RUNNING;
	thread = std::thread(&Dispatcher::dispatcherThread, this);
}

void Dispatcher::dispatcherThread()
{
	OutputMessagePool* outputPool = OutputMessagePool::getInstance();

	// NOTE: second argument defer_lock is to prevent from immediate locking
	std::unique_lock<std::mutex> taskLockUnique(taskLock, std::defer_lock);

	while (threadState != THREAD_STATE_TERMINATED) {
		// check if there are tasks waiting
		taskLockUnique.lock();

		if (taskList.empty()) {
			//if the list is empty wait for signal
			taskSignal.wait(taskLockUnique);
		}

		if (!taskList.empty() && threadState != THREAD_STATE_TERMINATED) {
			// take the first task
			Task* task = taskList.front();
			taskList.pop_front();
			taskLockUnique.unlock();

			if (!task->hasExpired()) {
				// execute it
				outputPool->startExecutionFrame();
				(*task)();
				outputPool->sendAll();

				g_game.clearSpectatorCache();
			}
			delete task;
		} else {
			taskLockUnique.unlock();
		}
	}
}

void Dispatcher::addTask(Task* task, bool push_front /*= false*/)
{
	bool do_signal = false;

	taskLock.lock();

	if (threadState == THREAD_STATE_RUNNING) {
		do_signal = taskList.empty();

		if (push_front) {
			taskList.push_front(task);
		} else {
			taskList.push_back(task);
		}
	} else {
		delete task;
	}

	taskLock.unlock();

	// send a signal if the list was empty
	if (do_signal) {
		taskSignal.notify_one();
	}
}

void Dispatcher::flush()
{
	while (!taskList.empty()) {
		Task* task = taskList.front();
		taskList.pop_front();
		(*task)();
		delete task;

		OutputMessagePool* outputPool = OutputMessagePool::getInstance();
		if (outputPool) {
			outputPool->sendAll();
		}

		g_game.clearSpectatorCache();
	}
}

void Dispatcher::stop()
{
	taskLock.lock();
	threadState = THREAD_STATE_CLOSING;
	taskLock.unlock();
}

void Dispatcher::shutdown()
{
	taskLock.lock();
	threadState = THREAD_STATE_TERMINATED;
	flush();
	taskLock.unlock();
	taskSignal.notify_one();
}

void Dispatcher::join()
{
	if (thread.joinable()) {
		thread.join();
	}
}
