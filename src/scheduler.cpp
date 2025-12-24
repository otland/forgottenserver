// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "otpch.h"

#include "scheduler.h"

extern Dispatcher g_dispatcher;

Scheduler g_scheduler;

uint32_t Scheduler::addEvent(SchedulerTask_ptr&& task)
{
	// check if the event has a valid id
	if (task->getEventId() == 0) {
		task->setEventId(++lastEventId);
	}

	uint32_t eventId = task->getEventId();

	boost::asio::post(io_context, [this, task = std::move(task)]() mutable {
		// insert the event id in the list of active events
		auto it = eventIdTimerMap.emplace(task->getEventId(), boost::asio::steady_timer{io_context});
		auto& timer = it.first->second;

		timer.expires_after(std::chrono::milliseconds(task->getDelay()));
		timer.async_wait([this, task = std::move(task)](const boost::system::error_code& error) mutable {
			eventIdTimerMap.erase(task->getEventId());

			if (error == boost::asio::error::operation_aborted || getState() == THREAD_STATE_TERMINATED) {
				// the timer has been manually canceled(timer->cancel()) or Scheduler::shutdown has been called
				return;
			}

			g_dispatcher.addTask(std::move(task));
		});
	});

	return eventId;
}

void Scheduler::stopEvent(uint32_t eventId)
{
	if (eventId == 0) {
		return;
	}

	boost::asio::post(io_context, [this, eventId]() {
		// search the event id
		auto it = eventIdTimerMap.find(eventId);
		if (it != eventIdTimerMap.end()) {
			it->second.cancel();
		}
	});
}

void Scheduler::shutdown()
{
	setState(THREAD_STATE_TERMINATED);
	boost::asio::post(io_context, [this]() {
		// cancel all active timers
		for (auto&& timer : eventIdTimerMap | std::views::values) {
			timer.cancel();
		}

		io_context.stop();
	});
}

SchedulerTask_ptr createSchedulerTask(uint32_t delay, TaskFunc&& f)
{
	return SchedulerTask_ptr(new SchedulerTask(delay, std::move(f)));
}
