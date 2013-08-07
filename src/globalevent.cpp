/**
 * The Forgotten Server - a server application for the MMORPG Tibia
 * Copyright (C) 2013  Mark Samman <mark.samman@gmail.com>
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

#include "globalevent.h"
#include "tools.h"
#include "player.h"
#include "scheduler.h"

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
	for (GlobalEventMap::iterator it = map.begin(); it != map.end(); ++it) {
		delete it->second;
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

	return NULL;
}

bool GlobalEvents::registerEvent(Event* event, xmlNodePtr)
{
	GlobalEvent* globalEvent = dynamic_cast<GlobalEvent*>(event);
	if (!globalEvent) {
		return false;
	}

	if (globalEvent->getEventType() == GLOBALEVENT_TIMER) {
		GlobalEventMap::iterator it = timerMap.find(globalEvent->getName());
		if (it == timerMap.end()) {
			timerMap.insert(std::make_pair(globalEvent->getName(), globalEvent));
			if (timerEventId == 0) {
				timerEventId = g_scheduler.addEvent(createSchedulerTask(SCHEDULER_MINTICKS,
				                                    boost::bind(&GlobalEvents::timer, this)));
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
		GlobalEventMap::iterator it = thinkMap.find(globalEvent->getName());

		if (it == thinkMap.end()) {
			thinkMap.insert(std::make_pair(globalEvent->getName(), globalEvent));

			if (thinkEventId == 0) {
				thinkEventId = g_scheduler.addEvent(createSchedulerTask(SCHEDULER_MINTICKS,
				                                    boost::bind(&GlobalEvents::think, this)));
			}

			return true;
		}
	}

	std::cout << "[Warning - GlobalEvents::configureEvent] Duplicate registered globalevent with name: " << globalEvent->getName() << std::endl;
	return false;
}

void GlobalEvents::startup()
{
	execute(GLOBALEVENT_STARTUP);
}

void GlobalEvents::timer()
{
	time_t now = time(NULL);

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
			std::cout << "[Error - GlobalEvents::timer] Failed to execute event: " << globalEvent->getName() << std::endl;
		}

		if (g_config.getBoolean(ConfigManager::SHUTDOWN_AT_SERVERSAVE)) {
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
							                boost::bind(&GlobalEvents::timer, this)));
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
		thinkEventId = g_scheduler.addEvent(createSchedulerTask(std::max<int64_t>(SCHEDULER_MINTICKS, nextScheduledTime),
											boost::bind(&GlobalEvents::think, this)));
	}
}

void GlobalEvents::execute(GlobalEvent_t type)
{
	for (GlobalEventMap::iterator it = serverMap.begin(); it != serverMap.end(); ++it) {
		GlobalEvent* globalEvent = it->second;
		if (globalEvent->getEventType() == type) {
			globalEvent->executeEvent();
		}
	}
}

GlobalEventMap GlobalEvents::getEventMap(GlobalEvent_t type)
{
	switch (type) {
		case GLOBALEVENT_NONE:
			return thinkMap;

		case GLOBALEVENT_TIMER:
			return timerMap;

		case GLOBALEVENT_STARTUP:
		case GLOBALEVENT_SHUTDOWN:
		case GLOBALEVENT_RECORD: {
			GlobalEventMap retMap;

			for (GlobalEventMap::iterator it = serverMap.begin(); it != serverMap.end(); ++it) {
				if (it->second->getEventType() == type) {
					retMap[it->first] = it->second;
				}
			}

			return retMap;
		}

		default:
			break;
	}

	return GlobalEventMap();
}

GlobalEvent::GlobalEvent(LuaScriptInterface* _interface):
	Event(_interface)
{
	m_nextExecution = 0;
	m_interval = 0;
}

bool GlobalEvent::configureEvent(xmlNodePtr p)
{
	std::string strValue;

	if (!readXMLString(p, "name", strValue)) {
		std::cout << "[Error - GlobalEvent::configureEvent] No name for a globalevent." << std::endl;
		return false;
	}

	m_name = strValue;
	m_eventType = GLOBALEVENT_NONE;

	if (readXMLString(p, "type", strValue)) {
		std::string tmpStrValue = asLowerCaseString(strValue);

		if (tmpStrValue == "startup" || tmpStrValue == "start" || tmpStrValue == "load") {
			m_eventType = GLOBALEVENT_STARTUP;
		} else if (tmpStrValue == "shutdown" || tmpStrValue == "quit" || tmpStrValue == "exit") {
			m_eventType = GLOBALEVENT_SHUTDOWN;
		} else if (tmpStrValue == "record" || tmpStrValue == "playersrecord") {
			m_eventType = GLOBALEVENT_RECORD;
		} else {
			std::cout << "[Error - GlobalEvent::configureEvent] No valid type \"" << strValue << "\" for globalevent with name " << m_name << std::endl;
			return false;
		}

		return true;
	} else if (readXMLString(p, "time", strValue) || readXMLString(p, "at", strValue)) {
		std::vector<int32_t> params = vectorAtoi(explodeString(strValue, ":"));

		if (params[0] < 0 || params[0] > 23) {
			std::cout << "[Error - GlobalEvent::configureEvent] No valid hour \"" << strValue << "\" for globalevent with name " << m_name << std::endl;
			return false;
		}

		m_interval |= params[0] << 16;
		int32_t hour = params[0];
		int32_t min = 0;
		int32_t sec = 0;

		if (params.size() > 1) {
			if (params[1] < 0 || params[1] > 59) {
				std::cout << "[Error - GlobalEvent::configureEvent] No valid minute \"" << strValue << "\" for globalevent with name " << m_name << std::endl;
				return false;
			}

			min = params[1];

			if (params.size() > 2) {
				if (params[2] < 0 || params[2] > 59) {
					std::cout << "[Error - GlobalEvent::configureEvent] No valid second \"" << strValue << "\" for globalevent with name " << m_name << std::endl;
					return false;
				}

				sec = params[2];
			}
		}

		time_t current_time = time(NULL);
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
		return true;
	}

	int32_t intValue;

	if (readXMLInteger(p, "interval", intValue)) {
		m_interval = std::max<int32_t>(SCHEDULER_MINTICKS, intValue);
		m_nextExecution = OTSYS_TIME() + m_interval;
		return true;
	}

	std::cout << "[Error - GlobalEvent::configureEvent] No interval for globalevent with name " << m_name << std::endl;
	return false;
}

std::string GlobalEvent::getScriptEventName()
{
	switch (m_eventType) {
		case GLOBALEVENT_STARTUP:
			return "onStartup";
		case GLOBALEVENT_SHUTDOWN:
			return "onShutdown";
		case GLOBALEVENT_RECORD:
			return "onRecord";
		case GLOBALEVENT_TIMER:
			return "onTime";
		default:
			break;
	}

	return "onThink";
}

uint32_t GlobalEvent::executeRecord(uint32_t current, uint32_t old)
{
	//onRecord(current, old, cid)
	ScriptEnvironment* env = m_scriptInterface->getScriptEnv();

	env->setScriptId(m_scriptId, m_scriptInterface);

	lua_State* L = m_scriptInterface->getLuaState();
	m_scriptInterface->pushFunction(m_scriptId);

	lua_pushnumber(L, current);
	lua_pushnumber(L, old);
	return m_scriptInterface->callFunction(2);
}

uint32_t GlobalEvent::executeEvent()
{
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
