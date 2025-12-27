#include "../../otpch.h"

#include "../../globalevent.h"

#include "../../script.h"
#include "../../tools.h"
#include "../api.h"
#include "../env.h"
#include "../meta.h"
#include "../register.h"
#include "../script.h"

extern GlobalEvents* g_globalEvents;
extern Scripts* g_scripts;

namespace {

int luaCreateGlobalEvent(lua_State* L)
{
	// GlobalEvent(eventName)
	if (tfs::lua::getScriptEnv()->getScriptInterface() != &g_scripts->getScriptInterface()) {
		tfs::lua::reportError(L, "GlobalEvents can only be registered in the Scripts interface.");
		lua_pushnil(L);
		return 1;
	}

	GlobalEvent* globalEvent = new GlobalEvent(tfs::lua::getScriptEnv()->getScriptInterface());
	globalEvent->setName(tfs::lua::getString(L, 2));
	globalEvent->setEventType(GLOBALEVENT_NONE);
	globalEvent->fromLua = true;
	tfs::lua::pushUserdata(L, globalEvent);
	tfs::lua::setMetatable(L, -1, "GlobalEvent");
	return 1;
}

int luaGlobalEventType(lua_State* L)
{
	// globalevent:type(callback)
	GlobalEvent* global = tfs::lua::getUserdata<GlobalEvent>(L, 1);
	if (global) {
		std::string typeName = tfs::lua::getString(L, 2);
		std::string tmpStr = boost::algorithm::to_lower_copy(typeName);
		if (tmpStr == "timer") {
			global->setEventType(GLOBALEVENT_TIMER);
		} else {
			std::cout << "[Error - luaGlobalEventType] Invalid type for global event: " << typeName << '\n';
			tfs::lua::pushBoolean(L, false);
		}
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaGlobalEventRegister(lua_State* L)
{
	// globalevent:register()
	GlobalEvent* globalevent = tfs::lua::getUserdata<GlobalEvent>(L, 1);
	if (globalevent) {
		if (!globalevent->isScripted()) {
			tfs::lua::pushBoolean(L, false);
			return 1;
		}

		if (globalevent->getEventType() == GLOBALEVENT_NONE && globalevent->getInterval() == 0) {
			std::cout << "[Error - luaGlobalEventRegister] No interval for globalevent with name "
			          << globalevent->getName() << '\n';
			tfs::lua::pushBoolean(L, false);
			return 1;
		}

		tfs::lua::pushBoolean(L, g_globalEvents->registerLuaEvent(globalevent));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaGlobalEventOnCallback(lua_State* L)
{
	// globalevent:onThink / onTime
	GlobalEvent* globalevent = tfs::lua::getUserdata<GlobalEvent>(L, 1);
	if (globalevent) {
		if (!globalevent->loadCallback()) {
			tfs::lua::pushBoolean(L, false);
			return 1;
		}
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaGlobalEventTime(lua_State* L)
{
	// globalevent:time(time)
	GlobalEvent* globalevent = tfs::lua::getUserdata<GlobalEvent>(L, 1);
	if (globalevent) {
		std::string timer = tfs::lua::getString(L, 2);
		std::vector<int32_t> params = vectorAtoi(explodeString(timer, ":"));

		int32_t hour = params.front();
		if (hour < 0 || hour > 23) {
			std::cout << "[Error - GlobalEvent::configureEvent] Invalid hour \"" << timer
			          << "\" for globalevent with name: " << globalevent->getName() << '\n';
			tfs::lua::pushBoolean(L, false);
			return 1;
		}

		globalevent->setInterval(hour << 16);

		int32_t min = 0;
		int32_t sec = 0;
		if (params.size() > 1) {
			min = params[1];
			if (min < 0 || min > 59) {
				std::cout << "[Error - GlobalEvent::configureEvent] Invalid minute \"" << timer
				          << "\" for globalevent with name: " << globalevent->getName() << '\n';
				tfs::lua::pushBoolean(L, false);
				return 1;
			}

			if (params.size() > 2) {
				sec = params[2];
				if (sec < 0 || sec > 59) {
					std::cout << "[Error - GlobalEvent::configureEvent] Invalid second \"" << timer
					          << "\" for globalevent with name: " << globalevent->getName() << '\n';
					tfs::lua::pushBoolean(L, false);
					return 1;
				}
			}
		}

		time_t current_time = time(nullptr);
		tm* timeinfo = localtime(&current_time);
		timeinfo->tm_hour = hour;
		timeinfo->tm_min = min;
		timeinfo->tm_sec = sec;

		time_t difference = static_cast<time_t>(difftime(mktime(timeinfo), current_time));
		if (difference < 0) {
			difference += 86400;
		}

		globalevent->setNextExecution((current_time + difference) * 1000);
		globalevent->setEventType(GLOBALEVENT_TIMER);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaGlobalEventInterval(lua_State* L)
{
	// globalevent:interval(interval)
	GlobalEvent* globalevent = tfs::lua::getUserdata<GlobalEvent>(L, 1);
	if (globalevent) {
		globalevent->setInterval(tfs::lua::getNumber<uint32_t>(L, 2));
		globalevent->setNextExecution(OTSYS_TIME() + tfs::lua::getNumber<uint32_t>(L, 2));
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

} // namespace

void tfs::lua::registerGlobalEvent(LuaScriptInterface& lsi)
{
	lsi.registerClass("GlobalEvent", "", luaCreateGlobalEvent);
	lsi.registerMethod("GlobalEvent", "type", luaGlobalEventType);
	lsi.registerMethod("GlobalEvent", "register", luaGlobalEventRegister);
	lsi.registerMethod("GlobalEvent", "time", luaGlobalEventTime);
	lsi.registerMethod("GlobalEvent", "interval", luaGlobalEventInterval);
	lsi.registerMethod("GlobalEvent", "onThink", luaGlobalEventOnCallback);
	lsi.registerMethod("GlobalEvent", "onTime", luaGlobalEventOnCallback);
}
