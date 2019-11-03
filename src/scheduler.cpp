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
	std::unique_lock<std::mutex> eventLockUnique(eventLock, std::defer_lock);
	while (getState() != THREAD_STATE_TERMINATED) {
		std::cv_status ret = std::cv_status::no_timeout;

		eventLockUnique.lock();
		if (eventList.empty()) {
			eventSignal.wait(eventLockUnique);
		} else {
			ret = eventSignal.wait_until(eventLockUnique, eventList.top()->getCycle());
		}

		// the mutex is locked again now...
		if (ret == std::cv_status::timeout && !eventList.empty()) {
			// ok we had a timeout, so there has to be an event we have to execute...
			SchedulerTask* task = eventList.top();
			eventList.pop();

			// check if the event was stopped
			auto it = eventIds.find(task->getEventId());
			if (it == eventIds.end()) {
				eventLockUnique.unlock();
				delete task;
				continue;
			}
			eventIds.erase(it);
			eventLockUnique.unlock();

			task->setDontExpire();
			g_dispatcher.addTask(task, true);
		} else {
			eventLockUnique.unlock();
		}
	}
}

uint32_t Scheduler::addEvent(SchedulerTask* task)
{
	eventLock.lock();

	if (getState() != THREAD_STATE_RUNNING) {
		eventLock.unlock();
		delete task;
		return 0;
	}

	// check if the event has a valid id
	if (task->getEventId() == 0) {
		// if not generate one
		if (++lastEventId == 0) {
			lastEventId = 1;
		}

		task->setEventId(lastEventId);
	}

	// insert the event id in the list of active events
	uint32_t eventId = task->getEventId();
	eventIds.insert(eventId);

	// add the event to the queue
	eventList.push(task);

	// if the list was empty or this event is the top in the list
	// we have to signal it
	bool do_signal = (task == eventList.top());

	eventLock.unlock();

	if (do_signal) {
		eventSignal.notify_one();
	}

	return eventId;
}

bool Scheduler::stopEvent(uint32_t eventId)
{
	if (eventId == 0) {
		return false;
	}

	std::lock_guard<std::mutex> lockClass(eventLock);

	// search the event id..
	auto it = eventIds.find(eventId);
	if (it == eventIds.end()) {
		return false;
	}

	eventIds.erase(it);
	return true;
}

void Scheduler::shutdown()
{
	setState(THREAD_STATE_TERMINATED);
	eventLock.lock();

	//this list should already be empty
	while (!eventList.empty()) {
		delete eventList.top();
		eventList.pop();
	}

	eventIds.clear();
	eventLock.unlock();
	eventSignal.notify_one();
}

SchedulerTask* createSchedulerTask(uint32_t delay, std::function<void (void)> f)
{
	return new SchedulerTask(delay, std::move(f));
}
