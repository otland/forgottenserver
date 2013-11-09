/**
 * The Forgotten Server - a server application for the MMORPG Tibia
 * Copyright (C) 2013  Mark Samman <mark.samman@gmail.com>
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

#ifndef __OTSERV_TASKS_H__
#define __OTSERV_TASKS_H__

#include <functional>
#include <thread>
#include <mutex>
#include <condition_variable>

const int DISPATCHER_TASK_EXPIRATION = 2000;
const auto SYSTEM_TIME_ZERO = std::chrono::system_clock::time_point(std::chrono::milliseconds(0));

class Task
{
	public:
		// DO NOT allocate this class on the stack
		Task(uint32_t ms, const std::function<void (void)>& f) : m_f(f) {
			m_expiration = std::chrono::system_clock::now() + std::chrono::milliseconds(ms);
		}
		Task(const std::function<void (void)>& f)
			: m_expiration(SYSTEM_TIME_ZERO), m_f(f) {}

		~Task() {}

		void operator()() {
			m_f();
		}

		void setDontExpire() {
			m_expiration = SYSTEM_TIME_ZERO;
		}

		bool hasExpired() const {
			if (m_expiration == SYSTEM_TIME_ZERO) {
				return false;
			}
			return m_expiration < std::chrono::system_clock::now();
		}

	protected:
		// Expiration has another meaning for scheduler tasks,
		// then it is the time the task should be added to the
		// dispatcher
		std::chrono::system_clock::time_point m_expiration;
		std::function<void (void)> m_f;
};

inline Task* createTask(std::function<void (void)> f)
{
	return new Task(f);
}

inline Task* createTask(uint32_t expiration, std::function<void (void)> f)
{
	return new Task(expiration, f);
}

enum DispatcherState {
	STATE_RUNNING,
	STATE_CLOSING,
	STATE_TERMINATED
};

class Dispatcher
{
	public:
		Dispatcher();
		~Dispatcher() {}

		void addTask(Task* task, bool push_front = false);

		void start();
		void stop();
		void shutdown();
		void join();

	protected:
		void dispatcherThread();

		void flush();

		std::thread m_thread;
		std::mutex m_taskLock;
		std::condition_variable m_taskSignal;

		std::list<Task*> m_taskList;
		DispatcherState m_threadState;
};

extern Dispatcher* g_dispatcher;

#endif
