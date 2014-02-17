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

#include "otpch.h"

#include "scheduler.h"

Scheduler::Scheduler()
{
	m_lastEventId = 0;
	m_threadState = STATE_TERMINATED;
}

void Scheduler::start()
{
	m_threadState = STATE_RUNNING;
	m_thread = std::thread(&Scheduler::schedulerThread, this);
}

void Scheduler::schedulerThread()
{
	std::unique_lock<std::mutex> eventLockUnique(m_eventLock, std::defer_lock);
	while (m_threadState != STATE_TERMINATED) {
		SchedulerTask* task = nullptr;
		std::cv_status ret = std::cv_status::no_timeout;
		bool runTask = false;

		eventLockUnique.lock();
		if (m_eventList.empty()) {
			m_eventSignal.wait(eventLockUnique);
		} else {
			ret = m_eventSignal.wait_until(eventLockUnique, m_eventList.top()->getCycle());
		}

		// the mutex is locked again now...
		if (ret == std::cv_status::timeout && m_threadState != STATE_TERMINATED) {
			// ok we had a timeout, so there has to be an event we have to execute...
			task = m_eventList.top();
			m_eventList.pop();

			// check if the event was stopped
			auto it = m_eventIds.find(task->getEventId());
			if (it != m_eventIds.end()) {
				// was not stopped so we should run it
				runTask = true;
				m_eventIds.erase(it);
			}
		}

		eventLockUnique.unlock();

		// add task to dispatcher
		if (task) {
			// if it was not stopped
			if (runTask) {
				// Expiration has another meaning for dispatcher tasks, reset it
				task->setDontExpire();
				g_dispatcher.addTask(task);
			} else {
				// was stopped, have to be deleted here
				delete task;
			}
		}
	}
}

uint32_t Scheduler::addEvent(SchedulerTask* task)
{
	bool do_signal = false;
	m_eventLock.lock();

	if (Scheduler::m_threadState == Scheduler::STATE_RUNNING) {
		// check if the event has a valid id
		if (task->getEventId() == 0) {
			// if not generate one
			if (++m_lastEventId == 0) {
				m_lastEventId = 1;
			}

			task->setEventId(m_lastEventId);
		}

		// insert the eventid in the list of active events
		m_eventIds.insert(task->getEventId());

		// add the event to the queue
		m_eventList.push(task);

		// if the list was empty or this event is the top in the list
		// we have to signal it
		do_signal = (task == m_eventList.top());
	} else {
		m_eventLock.unlock();
		delete task;
		task = nullptr;
		return 0;
	}

	m_eventLock.unlock();

	if (do_signal) {
		m_eventSignal.notify_one();
	}

	return task->getEventId();
}

bool Scheduler::stopEvent(uint32_t eventid)
{
	if (eventid == 0) {
		return false;
	}

	std::lock_guard<std::mutex> lockGuard(m_eventLock);

	// search the event id..
	auto it = m_eventIds.find(eventid);
	if (it == m_eventIds.end()) {
		return false;
	}

	m_eventIds.erase(it);
	return true;
}

void Scheduler::stop()
{
	std::lock_guard<std::mutex> lockGuard(m_eventLock);
	m_threadState = Scheduler::STATE_CLOSING;
}

void Scheduler::shutdown()
{
	std::lock_guard<std::mutex> lockGuard(m_eventLock);
	m_threadState = Scheduler::STATE_TERMINATED;

	//this list should already be empty
	while (!m_eventList.empty()) {
		delete m_eventList.top();
		m_eventList.pop();
	}

	m_eventIds.clear();
}

void Scheduler::join()
{
	m_thread.join();
}
