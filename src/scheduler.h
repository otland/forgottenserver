/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2019  Mark Samman <mark.samman@gmail.com>
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
#include <unordered_map>

#include "thread_holder_base.h"

static constexpr int32_t SCHEDULER_MINTICKS = 50;

class SchedulerTask : public Task
{
	public:
		void setEventId(uint32_t id) {
			eventId = id;
		}
		uint32_t getEventId() const {
			return eventId;
		}

		uint32_t getDelay() const {
			return delay;
		}
	private:
		SchedulerTask(uint32_t delay, TaskFunc&& f) : Task(std::move(f)), delay(delay) {}

		uint32_t eventId = 0;
		uint32_t delay = 0;

		friend SchedulerTask* createSchedulerTask(uint32_t, TaskFunc&&);
};

SchedulerTask* createSchedulerTask(uint32_t delay, TaskFunc&& f);

class Scheduler : public ThreadHolder<Scheduler>
{
	public:
		uint32_t addEvent(SchedulerTask* task);
		void stopEvent(uint32_t eventId);

		void shutdown();

		void threadMain() { io_context.run(); }
	private:
		std::atomic<uint32_t> lastEventId{0};
		std::unordered_map<uint32_t, boost::asio::steady_timer> eventIdTimerMap;
		boost::asio::io_context io_context;
		boost::asio::io_context::work work{io_context};
};

extern Scheduler g_scheduler;

#endif
