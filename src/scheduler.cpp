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

void Scheduler::threadMain()
{
	io_service.run();
}

uint32_t Scheduler::addEvent(SchedulerTask* task)
{
	// check if the event has a valid id
	if (task->getEventId() == 0) {
		uint32_t tmpEventId = ++lastEventId;
		// if not generate one
		if (tmpEventId == 0) {
			tmpEventId = ++lastEventId;
		}
		task->setEventId(tmpEventId);
	}

	// insert the event id in the list of active events
	io_service.post([this, task]() {
		boost::asio::deadline_timer* timer;
		if (timerCacheVec.empty()) {
			timer = new boost::asio::deadline_timer(io_service);
		} else {
			timer = timerCacheVec.back();
			timerCacheVec.pop_back();
		}
		eventIdTimerMap[task->getEventId()] = timer;

		timer->expires_from_now(boost::posix_time::milliseconds(task->getDelay()));
		timer->async_wait([this, task, timer](const boost::system::error_code& error) {
			timerCacheVec.push_back(timer);
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

		// thanks to cancel we will have all the timers in timerCacheVec
		// deleted them to prevent memory leak
		// this must be a new task since timer->cancel() adds a new task for each cancel so we want to delete the timers only after all cancel callbacks were called
		io_service.post([this]() {
			for (auto* timer : timerCacheVec) {
				delete timer;
			}
		});

		io_service.stop();
	});
}

SchedulerTask* createSchedulerTask(uint32_t delay, std::function<void (void)> f)
{
	return new SchedulerTask(delay, std::move(f));
}
