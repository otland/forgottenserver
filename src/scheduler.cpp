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

#include "otpch.h"

#include "scheduler.h"
#include <memory>

void Scheduler::threadMain()
{
	io_service.run();
}

uint32_t Scheduler::addEvent(SchedulerTask* task)
{
	// check if the event has a valid id
	if (task->getEventId() == 0) {
		task->setEventId(++lastEventId);
	}

	// insert the event id in the list of active events
	io_service.post([this, task]() {
		auto& timer = eventIdTimerMap[task->getEventId()];
		timer.reset(new boost::asio::deadline_timer(io_service));

		timer->expires_from_now(boost::posix_time::milliseconds(task->getDelay()));
		timer->async_wait([this, task](const boost::system::error_code& error) {
			eventIdTimerMap.erase(task->getEventId());

			if (error == boost::asio::error::operation_aborted || getState() == THREAD_STATE_TERMINATED) {
				// the timer has been manually cancelled(timer->cancel()) or Scheduler::shutdown has been called
				delete task;
				return;
			}

			g_dispatcher.addTask(task, true);
		});
	});

	return task->getEventId();
}

void Scheduler::stopEvent(uint32_t eventId)
{
	if (eventId == 0) {
		return;
	}

	io_service.post([this, eventId]() {
		// search the event id..
		auto it = eventIdTimerMap.find(eventId);
		if (it == eventIdTimerMap.end()) {
			return;
		}
		it->second->cancel();
	});
}

void Scheduler::shutdown()
{
	setState(THREAD_STATE_TERMINATED);
	io_service.post([this]() {
		// cancel all active timers
		for (auto& it : eventIdTimerMap) {
			it.second->cancel();
		}

		io_service.stop();
	});
}

SchedulerTask* createSchedulerTask(uint32_t delay, std::function<void (void)> f)
{
	return new SchedulerTask(delay, std::move(f));
}
