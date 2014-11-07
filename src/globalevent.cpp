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

#include "configmanager.h"
#include "globalevent.h"
#include "tools.h"
#include "scheduler.h"
#include "pugicast.h"

extern ConfigManager g_config;

GlobalEvents::GlobalEvents() :
	m_scriptInterface("GlobalEvent Interface")
{
	m_scriptInterface.initState();
	thinkEventId = 0;
	timerEventId = 0;
}

GlobalEvents::~GlobalEvents()
{
	clear();
}

void GlobalEvents::clearMap(GlobalEventMap& map)
{
	for (const auto& it : map) {
		delete it.second;
	}
	map.clear();
}

void GlobalEvents::clear()
{
	g_scheduler.stopEvent(thinkEventId);
	thinkEventId = 0;
	g_scheduler.stopEvent(timerEventId);
	timerEventId = 0;

	clearMap(thinkMap);
	clearMap(serverMap);
	clearMap(timerMap);

	m_scriptInterface.reInitState();
}

Event* GlobalEvents::getEvent(const std::string& nodeName)
{
	if (asLowerCaseString(nodeName) == "globalevent") {
		return new GlobalEvent(&m_scriptInterface);
	}
	return nullptr;
}

bool GlobalEvents::registerEvent(Event* event, const pugi::xml_node&)
{
	GlobalEvent* globalEvent = static_cast<GlobalEvent*>(event);
	if (globalEvent->getEventType() == GLOBALEVENT_TIMER) {
		GlobalEventMap::iterator it = timerMap.find(globalEvent->getName());
		if (it == timerMap.end()) {
			timerMap.insert(std::make_pair(globalEvent->getName(), globalEvent));
			if (timerEventId == 0) {
				timerEventId = g_scheduler.addEvent(createSchedulerTask(SCHEDULER_MINTICKS, std::bind(&GlobalEvents::timer, this)));
			}

			return true;
		}
	} else if (globalEvent->getEventType() != GLOBALEVENT_NONE) {
		GlobalEventMap::iterator it = serverMap.find(globalEvent->getName());
		if (it == serverMap.end()) {
			serverMap.insert(std::make_pair(globalEvent->getName(), globalEvent));
			return true;
		}
	} else { // think event
		auto it = thinkMap.find(globalEvent->getName());
		if (it == thinkMap.end()) {
			thinkMap.insert(std::make_pair(globalEvent->getName(), globalEvent));
			if (thinkEventId == 0) {
				thinkEventId = g_scheduler.addEvent(createSchedulerTask(SCHEDULER_MINTICKS, std::bind(&GlobalEvents::think, this)));
			}
			return true;
		}
	}

	std::cout << "[Warning - GlobalEvents::configureEvent] Duplicate registered globalevent with name: " << globalEvent->getName() << std::endl;
	return false;
}

void GlobalEvents::startup() const
{
	execute(GLOBALEVENT_STARTUP);
}

void GlobalEvents::timer()
{
	time_t now = time(nullptr);

	int64_t nextScheduledTime = std::numeric_limits<int64_t>::max();

	auto it = timerMap.begin();
	while (it != timerMap.end()) {
		GlobalEvent* globalEvent = it->second;

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

		nextExecutionTime = 86400;
		if (nextExecutionTime < nextScheduledTime) {
			nextScheduledTime = nextExecutionTime;
		}

		globalEvent->setNextExecution(globalEvent->getNextExecution() + nextExecutionTime);

		++it;
	}

	if (nextScheduledTime != std::numeric_limits<int64_t>::max()) {
		timerEventId = g_scheduler.addEvent(createSchedulerTask(std::max<int64_t>(1000, nextScheduledTime * 1000),
							                std::bind(&GlobalEvents::timer, this)));
	}
}

void GlobalEvents::think()
{
	int64_t now = OTSYS_TIME();

	int64_t nextScheduledTime = std::numeric_limits<int64_t>::max();
	for (const auto& it : thinkMap) {
		GlobalEvent* globalEvent = it.second;

		int64_t nextExecutionTime = globalEvent->getNextExecution() - now;
		if (nextExecutionTime > 0) {
			if (nextExecutionTime < nextScheduledTime) {
				nextScheduledTime = nextExecutionTime;
			}
			continue;
		}

		if (!globalEvent->executeEvent()) {
			std::cout << "[Error - GlobalEvents::think] Failed to execute event: " << globalEvent->getName() << std::endl;
		}

		nextExecutionTime = globalEvent->getInterval();
		if (nextExecutionTime < nextScheduledTime) {
			nextScheduledTime = nextExecutionTime;
		}

		globalEvent->setNextExecution(globalEvent->getNextExecution() + nextExecutionTime);
	}

	if (nextScheduledTime != std::numeric_limits<int64_t>::max()) {
		thinkEventId = g_scheduler.addEvent(createSchedulerTask(nextScheduledTime, std::bind(&GlobalEvents::think, this)));
	}
}

void GlobalEvents::execute(GlobalEvent_t type) const
{
	for (const auto& it : serverMap) {
		GlobalEvent* globalEvent = it.second;
		if (globalEvent->getEventType() == type) {
			globalEvent->executeEvent();
		}
	}
}

GlobalEventMap GlobalEvents::getEventMap(GlobalEvent_t type)
{
	switch (type) {
		case GLOBALEVENT_NONE: return thinkMap;
		case GLOBALEVENT_TIMER: return timerMap;
		case GLOBALEVENT_STARTUP:
		case GLOBALEVENT_SHUTDOWN:
		case GLOBALEVENT_RECORD: {
			GlobalEventMap retMap;
			for (const auto& it : serverMap) {
				if (it.second->getEventType() == type) {
					retMap[it.first] = it.second;
				}
			}
			return retMap;
		}
		default: return GlobalEventMap();
	}
}

GlobalEvent::GlobalEvent(LuaScriptInterface* _interface):
	Event(_interface)
{
	m_nextExecution = 0;
	m_interval = 0;
}

bool GlobalEvent::configureEvent(const pugi::xml_node& node)
{
	pugi::xml_attribute nameAttribute = node.attribute("name");
	if (!nameAttribute) {
		std::cout << "[Error - GlobalEvent::configureEvent] Missing name for a globalevent" << std::endl;
		return false;
	}

	m_name = nameAttribute.as_string();
	m_eventType = GLOBALEVENT_NONE;

	pugi::xml_attribute attr;
	if ((attr = node.attribute("time"))) {
		std::vector<int32_t> params = vectorAtoi(explodeString(attr.as_string(), ":"));
		if (params.front() < 0 || params.front() > 23) {
			std::cout << "[Error - GlobalEvent::configureEvent] Invalid hour \"" << attr.as_string() << "\" for globalevent with name: " << m_name << std::endl;
			return false;
		}

		m_interval |= params.front() << 16;
		int32_t hour = params.front();
		int32_t min = 0;
		int32_t sec = 0;

		if (params.size() > 1) {
			if (params[1] < 0 || params[1] > 59) {
				std::cout << "[Error - GlobalEvent::configureEvent] Invalid minute \"" << attr.as_string() << "\" for globalevent with name: " << m_name << std::endl;
				return false;
			}

			min = params[1];

			if (params.size() > 2) {
				if (params[2] < 0 || params[2] > 59) {
					std::cout << "[Error - GlobalEvent::configureEvent] Invalid second \"" << attr.as_string() << "\" for globalevent with name: " << m_name << std::endl;
					return false;
				}

				sec = params[2];
			}
		}

		time_t current_time = time(nullptr);
		tm* timeinfo = localtime(&current_time);
		timeinfo->tm_hour = hour;
		timeinfo->tm_min = min;
		timeinfo->tm_sec = sec;
		time_t difference = (time_t)difftime(mktime(timeinfo), current_time);

		if (difference < 0) {
			difference += 86400;
		}

		m_nextExecution = current_time + difference;
		m_eventType = GLOBALEVENT_TIMER;
	} else if ((attr = node.attribute("type"))) {
		std::string tmpStrValue = asLowerCaseString(attr.as_string());
		if (tmpStrValue == "startup" || tmpStrValue == "start" || tmpStrValue == "load") {
			m_eventType = GLOBALEVENT_STARTUP;
		} else if (tmpStrValue == "shutdown" || tmpStrValue == "quit" || tmpStrValue == "exit") {
			m_eventType = GLOBALEVENT_SHUTDOWN;
		} else if (tmpStrValue == "record" || tmpStrValue == "playersrecord") {
			m_eventType = GLOBALEVENT_RECORD;
		} else {
			std::cout << "[Error - GlobalEvent::configureEvent] No valid type \"" << attr.as_string() << "\" for globalevent with name " << m_name << std::endl;
			return false;
		}
	} else if ((attr = node.attribute("interval"))) {
		m_interval = std::max<int32_t>(SCHEDULER_MINTICKS, pugi::cast<int32_t>(attr.value()));
		m_nextExecution = OTSYS_TIME() + m_interval;
	} else {
		std::cout << "[Error - GlobalEvent::configureEvent] No interval for globalevent with name " << m_name << std::endl;
		return false;
	}
	return true;
}

std::string GlobalEvent::getScriptEventName() const
{
	switch (m_eventType) {
		case GLOBALEVENT_STARTUP: return "onStartup";
		case GLOBALEVENT_SHUTDOWN: return "onShutdown";
		case GLOBALEVENT_RECORD: return "onRecord";
		case GLOBALEVENT_TIMER: return "onTime";
		default: return "onThink";
	}
}

bool GlobalEvent::executeRecord(uint32_t current, uint32_t old)
{
	//onRecord(current, old, cid)
	if (!m_scriptInterface->reserveScriptEnv()) {
		std::cout << "[Error - GlobalEvent::executeRecord] Call stack overflow" << std::endl;
		return false;
	}

	ScriptEnvironment* env = m_scriptInterface->getScriptEnv();
	env->setScriptId(m_scriptId, m_scriptInterface);

	lua_State* L = m_scriptInterface->getLuaState();
	m_scriptInterface->pushFunction(m_scriptId);

	lua_pushnumber(L, current);
	lua_pushnumber(L, old);
	return m_scriptInterface->callFunction(2);
}

bool GlobalEvent::executeEvent()
{
	if (!m_scriptInterface->reserveScriptEnv()) {
		std::cout << "[Error - GlobalEvent::executeEvent] Call stack overflow" << std::endl;
		return false;
	}

	ScriptEnvironment* env = m_scriptInterface->getScriptEnv();
	env->setScriptId(m_scriptId, m_scriptInterface);
	lua_State* L = m_scriptInterface->getLuaState();
	m_scriptInterface->pushFunction(m_scriptId);

	int32_t params = 0;
	if (m_eventType == GLOBALEVENT_NONE || m_eventType == GLOBALEVENT_TIMER) {
		lua_pushnumber(L, m_interval);
		params = 1;
	}

	return m_scriptInterface->callFunction(params);
}
