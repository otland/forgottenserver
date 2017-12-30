/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2017  Mark Samman <mark.samman@gmail.com>
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

#ifndef FS_GLOBALEVENT_H_B3FB9B848EA3474B9AFC326873947E3C
#define FS_GLOBALEVENT_H_B3FB9B848EA3474B9AFC326873947E3C
#include "baseevents.h"

#include "const.h"

enum GlobalEvent_t {
	GLOBALEVENT_NONE,
	GLOBALEVENT_TIMER,

	GLOBALEVENT_STARTUP,
	GLOBALEVENT_SHUTDOWN,
	GLOBALEVENT_RECORD,
};

class GlobalEvent;
using GlobalEventMap = std::map<std::string, GlobalEvent*>;

class GlobalEvents final : public BaseEvents
{
	public:
		GlobalEvents();
		~GlobalEvents();

		// non-copyable
		GlobalEvents(const GlobalEvents&) = delete;
		GlobalEvents& operator=(const GlobalEvents&) = delete;

		void startup() const;

		void timer();
		void think();
		void execute(GlobalEvent_t type) const;

		GlobalEventMap getEventMap(GlobalEvent_t type);
		static void clearMap(GlobalEventMap& map);

	private:
		std::string getScriptBaseName() const override {
			return "globalevents";
		}
		void clear() override;

		Event* getEvent(const std::string& nodeName) override;
		bool registerEvent(Event* event, const pugi::xml_node& node) override;

		LuaScriptInterface& getScriptInterface() override {
			return scriptInterface;
		}
		LuaScriptInterface scriptInterface;

		GlobalEventMap thinkMap, serverMap, timerMap;
		int32_t thinkEventId = 0, timerEventId = 0;
};

class GlobalEvent final : public Event
{
	public:
		explicit GlobalEvent(LuaScriptInterface* interface);

		bool configureEvent(const pugi::xml_node& node) override;

		bool executeRecord(uint32_t current, uint32_t old);
		bool executeEvent();

		GlobalEvent_t getEventType() const {
			return eventType;
		}

		const std::string& getName() const {
			return name;
		}

		uint32_t getInterval() const {
			return interval;
		}

		int64_t getNextExecution() const {
			return nextExecution;
		}
		void setNextExecution(int64_t time) {
			nextExecution = time;
		}

	private:
		GlobalEvent_t eventType = GLOBALEVENT_NONE;

		std::string getScriptEventName() const override;

		std::string name;
		int64_t nextExecution = 0;
		uint32_t interval = 0;
};

#endif
