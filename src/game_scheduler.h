// Copyright 2022 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_SCHEDULER_H
#define FS_SCHEDULER_H

#include "network_scheduler.h"
#include "thread_holder_base.h"

static constexpr int32_t SCHEDULER_MINTICKS = 50;

class GameTask : public NetworkTask
{
public:
	void setEventId(uint32_t id) { eventId = id; }
	uint32_t getEventId() const { return eventId; }

	uint32_t getDelay() const { return delay; }

private:
	GameTask(uint32_t delay, NetworkTaskFunc&& f) : NetworkTask(std::move(f)), delay(delay) {}

	uint32_t eventId = 0;
	uint32_t delay = 0;

	friend GameTask* createGameTask(uint32_t, NetworkTaskFunc&&);
};

GameTask* createGameTask(uint32_t delay, NetworkTaskFunc&& f);

class GameScheduler : public ThreadHolder<GameScheduler>
{
public:
	uint32_t addEvent(GameTask* task);
	void stopEvent(uint32_t eventId);

	void shutdown();

	void run() { io_context.run(); }

private:
	std::atomic<uint32_t> lastEventId{0};
	std::unordered_map<uint32_t, boost::asio::steady_timer> eventIdTimerMap;
	boost::asio::io_context io_context;
	boost::asio::io_context::work work{io_context};
};

extern GameScheduler g_gameScheduler;

#endif // FS_SCHEDULER_H
