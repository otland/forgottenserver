/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2014  Mark Samman <mark.samman@gmail.com>
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

#ifndef FS_SCHEDULER_H_2905B3D5EAB34B4BA8830167262D2DC1
#define FS_SCHEDULER_H_2905B3D5EAB34B4BA8830167262D2DC1

#include "tasks.h"
#include <unordered_set>
#include <queue>

#include <condition_variable>

#define SCHEDULER_MINTICKS 50

class SchedulerTask : public Task
{
	public:
		~SchedulerTask() {}

		void setEventId(uint32_t eventid) {
			m_eventid = eventid;
		}
		uint32_t getEventId() const {
			return m_eventid;
		}

		std::chrono::system_clock::time_point getCycle() const {
			return m_expiration;
		}

		bool operator<(const SchedulerTask& other) const {
			return getCycle() > other.getCycle();
		}

	protected:
		SchedulerTask(uint32_t delay, const std::function<void (void)>& f) : Task(delay, f) {
			m_eventid = 0;
		}

		uint32_t m_eventid;

		friend SchedulerTask* createSchedulerTask(uint32_t, const std::function<void (void)>&);
};

inline SchedulerTask* createSchedulerTask(uint32_t delay, const std::function<void (void)>& f)
{
	return new SchedulerTask(std::max<uint32_t>(delay, SCHEDULER_MINTICKS), f);
}

class lessSchedTask : public std::binary_function<SchedulerTask*&, SchedulerTask*&, bool>
{
	public:
		bool operator()(SchedulerTask*& t1, SchedulerTask*& t2) {
			return (*t1) < (*t2);
		}
};

class Scheduler
{
	public:
		Scheduler();
		~Scheduler() {}

		uint32_t addEvent(SchedulerTask* task);
		bool stopEvent(uint32_t eventId);

		void start();
		void stop();
		void shutdown();
		void join();

		enum SchedulerState {
			STATE_RUNNING,
			STATE_CLOSING,
			STATE_TERMINATED
		};

	protected:
		void schedulerThread();

		std::thread m_thread;
		std::mutex m_eventLock;
		std::condition_variable m_eventSignal;

		uint32_t m_lastEventId;
		std::priority_queue<SchedulerTask*, std::vector<SchedulerTask*>, lessSchedTask > m_eventList;
		std::unordered_set<uint32_t> m_eventIds;
		SchedulerState m_threadState;
};

extern Scheduler g_scheduler;

#endif
