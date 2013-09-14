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

#include <boost/function.hpp>
#include <boost/thread.hpp>

const int DISPATCHER_TASK_EXPIRATION = 2000;

class Task
{
	public:
		// DO NOT allocate this class on the stack
		Task(uint32_t ms, const boost::function<void (void)>& f) : m_f(f) {
			m_expiration = boost::get_system_time() + boost::posix_time::milliseconds(ms);
		}
		Task(const boost::function<void (void)>& f)
			: m_expiration(boost::date_time::not_a_date_time), m_f(f) {}

		~Task() {}

		void operator()() {
			m_f();
		}

		void setDontExpire() {
			m_expiration = boost::date_time::not_a_date_time;
		}

		bool hasExpired() const {
			if (m_expiration == boost::date_time::not_a_date_time) {
				return false;
			}

			return m_expiration < boost::get_system_time();
		}

	protected:
		// Expiration has another meaning for scheduler tasks,
		// then it is the time the task should be added to the
		// dispatcher
		boost::system_time m_expiration;
		boost::function<void (void)> m_f;
};

inline Task* createTask(boost::function<void (void)> f)
{
	return new Task(f);
}

inline Task* createTask(uint32_t expiration, boost::function<void (void)> f)
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

		boost::thread m_thread;
		boost::mutex m_taskLock;
		boost::condition_variable m_taskSignal;

		std::list<Task*> m_taskList;
		DispatcherState m_threadState;
};

extern Dispatcher g_dispatcher;

#endif
