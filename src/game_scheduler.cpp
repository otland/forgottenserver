// Copyright 2022 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "otpch.h"

#include "game_scheduler.h"

uint32_t GameScheduler::addEvent(GameTask* task)
{
	// check if the event has a valid id
	if (task->getEventId() == 0) {
		task->setEventId(++lastEventId);
	}

	boost::asio::post(io_context, [this, task]() {
		// insert the event id in the list of active events
		auto it = eventIdTimerMap.emplace(task->getEventId(), boost::asio::steady_timer{io_context});
		auto& timer = it.first->second;

		timer.expires_from_now(std::chrono::milliseconds(task->getDelay()));
		timer.async_wait([this, task](const boost::system::error_code& error) {
			eventIdTimerMap.erase(task->getEventId());

			if (error == boost::asio::error::operation_aborted || getState() == THREAD_STATE_TERMINATED) {
				// the timer has been manually canceled(timer->cancel()) or Scheduler::shutdown has been called
				delete task;
				return;
			}

			g_networkScheduler.addTask(task);
		});
	});

	return task->getEventId();
}

void GameScheduler::stopEvent(uint32_t eventId)
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

void GameScheduler::shutdown()
{
	ThreadHolder::shutdown();
	boost::asio::post(io_context, [this]() {
		// cancel all active timers
		for (auto& it : eventIdTimerMap) {
			it.second.cancel();
		}

		io_context.stop();
	});
}

GameTask* createGameTask(uint32_t delay, NetworkTaskFunc&& f) { return new GameTask(delay, std::move(f)); }
