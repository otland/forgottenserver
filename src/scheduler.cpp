// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "otpch.h"

#include "scheduler.h"

uint32_t Scheduler::addEvent(SchedulerTask_ptr task)
{
	if (task->getEventId() == 0) {
		task->setEventId(++lastEventId);
	}

	auto eventId = task->getEventId();

	boost::asio::post(io_context, [this, scheduledTask = std::move(task)]() mutable {
		// insert the event id in the list of active events
		auto it = eventIdTimerMap.emplace(scheduledTask->getEventId(), boost::asio::steady_timer{io_context});
		auto& timer = it.first->second;

		timer.expires_after(std::chrono::milliseconds(scheduledTask->getDelay()));
		timer.async_wait(
		    [this, scheduledTask = std::move(scheduledTask)](const boost::system::error_code& error) mutable {
			    eventIdTimerMap.erase(scheduledTask->getEventId());

			    if (error == boost::asio::error::operation_aborted || getState() == THREAD_STATE_TERMINATED) {
				    // The timer was manually canceled or Scheduler::shutdown was called
				    return;
			    }

			    g_dispatcher.addTask(std::move(scheduledTask));
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
		for (auto& it : eventIdTimerMap) {
			it.second.cancel();
		}

		io_context.stop();
	});
}

SchedulerTask_ptr createSchedulerTask(uint32_t delay, TaskFunc&& f)
{
	return SchedulerTask_ptr(new SchedulerTask(delay, std::move(f)));
}
