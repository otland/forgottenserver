// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "otpch.h"

#include "globalevent.h"

#include "configmanager.h"
#include "pugicast.h"
#include "scheduler.h"
#include "tools.h"

extern ConfigManager g_config;

GlobalEvents::GlobalEvents() : scriptInterface("GlobalEvent Interface") { scriptInterface.initState(); }

GlobalEvents::~GlobalEvents() { clear(); }

void GlobalEvents::clear()
{
	g_scheduler.stopEvent(thinkEventId);
	thinkEventId = 0;
	g_scheduler.stopEvent(timerEventId);
	timerEventId = 0;

	thinkMap.clear();
	serverMap.clear();
	timerMap.clear();

	scriptInterface.reInitState();
}

bool GlobalEvents::registerLuaEvent(GlobalEvent_shared_ptr globalEvent)
{
	auto result = serverMap.emplace(globalEvent->getName(), globalEvent);
	if (result.second) {
		if (globalEvent->getEventType() == GLOBALEVENT_TIMER) {
			if (timerEventId == 0) {
				timerEventId = g_scheduler.addEvent(createSchedulerTask(SCHEDULER_MINTICKS, [this]() { timer(); }));
			}
		} else if (globalEvent->getEventType() == GLOBALEVENT_NONE) {
			if (thinkEventId == 0) {
				thinkEventId = g_scheduler.addEvent(createSchedulerTask(SCHEDULER_MINTICKS, [this]() { think(); }));
			}
		}
		return true;
	}

	std::cout << "[Warning - GlobalEvents::configureEvent] Duplicate registered globalevent with name: "
	          << globalEvent->getName() << std::endl;
	return false;
}

GlobalEvent_shared_ptr GlobalEvents::getGlobalEvent(const std::string& name)
{
	auto it = serverMap.find(name);
	if (it != serverMap.end()) {
		return it->second;
	}
	return nullptr;
}

void GlobalEvents::startup() const { execute(GLOBALEVENT_STARTUP); }
void GlobalEvents::shutdown() const { execute(GLOBALEVENT_SHUTDOWN); }
void GlobalEvents::save() const { execute(GLOBALEVENT_SAVE); }

void GlobalEvents::timer()
{
	auto now = OTSYS_TIME();

	int64_t nextScheduledTime = std::numeric_limits<int64_t>::max();

	auto it = timerMap.begin();
	while (it != timerMap.end()) {
		auto& globalEvent = it->second;

		int64_t nextExecutionTime = globalEvent->getNextExecution() - now;
		if (nextExecutionTime > 0) {
			if (nextExecutionTime < nextScheduledTime) {
				nextScheduledTime = nextExecutionTime;
			}

			++it;
			continue;
		}

		if (!globalEvent->executeEvent()) {
			it = timerMap.erase(it);
			continue;
		}

		nextExecutionTime = 86400000;
		if (nextExecutionTime < nextScheduledTime) {
			nextScheduledTime = nextExecutionTime;
		}

		globalEvent->setNextExecution(globalEvent->getNextExecution() + nextExecutionTime);

		++it;
	}

	if (nextScheduledTime != std::numeric_limits<int64_t>::max()) {
		timerEventId = g_scheduler.addEvent(
		    createSchedulerTask(std::max<int64_t>(1000, nextScheduledTime), [this]() { timer(); }));
	}
}

void GlobalEvents::think()
{
	int64_t now = OTSYS_TIME();

	int64_t nextScheduledTime = std::numeric_limits<int64_t>::max();
	for (auto& it : thinkMap) {
		auto& globalEvent = it.second;

		int64_t nextExecutionTime = globalEvent->getNextExecution() - now;
		if (nextExecutionTime > 0) {
			if (nextExecutionTime < nextScheduledTime) {
				nextScheduledTime = nextExecutionTime;
			}
			continue;
		}

		if (!globalEvent->executeEvent()) {
			std::cout << "[Error - GlobalEvents::think] Failed to execute event: " << globalEvent->getName()
			          << std::endl;
		}

		nextExecutionTime = globalEvent->getInterval();
		if (nextExecutionTime < nextScheduledTime) {
			nextScheduledTime = nextExecutionTime;
		}

		globalEvent->setNextExecution(globalEvent->getNextExecution() + nextExecutionTime);
	}

	if (nextScheduledTime != std::numeric_limits<int64_t>::max()) {
		thinkEventId = g_scheduler.addEvent(createSchedulerTask(nextScheduledTime, [this]() { think(); }));
	}
}

void GlobalEvents::execute(GlobalEvent_t type) const
{
	for (const auto& it : serverMap) {
		const GlobalEvent_shared_ptr globalEvent = it.second;
		if (globalEvent->getEventType() == type) {
			globalEvent->executeEvent();
		}
	}
}

GlobalEventMap GlobalEvents::getEventMap(GlobalEvent_t type)
{
	// TODO: This should be better implemented. Maybe have a map for every type.
	switch (type) {
		case GLOBALEVENT_NONE:
			return thinkMap;
		case GLOBALEVENT_TIMER:
			return timerMap;
		case GLOBALEVENT_STARTUP:
		case GLOBALEVENT_SHUTDOWN:
		case GLOBALEVENT_RECORD:
		case GLOBALEVENT_SAVE: {
			GlobalEventMap retMap;
			for (const auto& it : serverMap) {
				if (it.second->getEventType() == type) {
					retMap.emplace(it.first, it.second);
				}
			}
			return retMap;
		}
		default:
			return GlobalEventMap();
	}
}

GlobalEvent::GlobalEvent(LuaScriptInterface* interface) : Event(interface) {}

std::string_view GlobalEvent::getScriptEventName() const
{
	switch (eventType) {
		case GLOBALEVENT_STARTUP:
			return "onStartup";
		case GLOBALEVENT_SHUTDOWN:
			return "onShutdown";
		case GLOBALEVENT_RECORD:
			return "onRecord";
		case GLOBALEVENT_SAVE:
			return "onSave";
		case GLOBALEVENT_TIMER:
			return "onTime";
		default:
			return "onThink";
	}
}

bool GlobalEvent::executeRecord(uint32_t current, uint32_t old)
{
	// onRecord(current, old)
	if (!scriptInterface->reserveScriptEnv()) {
		std::cout << "[Error - GlobalEvent::executeRecord] Call stack overflow" << std::endl;
		return false;
	}

	ScriptEnvironment* env = scriptInterface->getScriptEnv();
	env->setScriptId(scriptId, scriptInterface);

	lua_State* L = scriptInterface->getLuaState();
	scriptInterface->pushFunction(scriptId);

	lua_pushnumber(L, current);
	lua_pushnumber(L, old);
	return scriptInterface->callFunction(2);
}

bool GlobalEvent::executeEvent() const
{
	if (!scriptInterface->reserveScriptEnv()) {
		std::cout << "[Error - GlobalEvent::executeEvent] Call stack overflow" << std::endl;
		return false;
	}

	ScriptEnvironment* env = scriptInterface->getScriptEnv();
	env->setScriptId(scriptId, scriptInterface);
	lua_State* L = scriptInterface->getLuaState();
	scriptInterface->pushFunction(scriptId);

	int32_t params = 0;
	if (eventType == GLOBALEVENT_NONE || eventType == GLOBALEVENT_TIMER) {
		lua_pushnumber(L, interval);
		params = 1;
	}

	return scriptInterface->callFunction(params);
}
