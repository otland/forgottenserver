#include "../otpch.h"

#include "../creatureevent.h"

#include "../luaapi.h"
#include "../luaenv.h"
#include "../luameta.h"
#include "../luascript.h"
#include "../script.h"
#include "luaregister.h"

extern CreatureEvents* g_creatureEvents;
extern Scripts* g_scripts;

namespace {

using namespace tfs::lua;

int luaCreateCreatureEvent(lua_State* L)
{
	// CreatureEvent(eventName)
	if (getScriptEnv()->getScriptInterface() != &g_scripts->getScriptInterface()) {
		reportErrorFunc(L, "CreatureEvents can only be registered in the Scripts interface.");
		lua_pushnil(L);
		return 1;
	}

	CreatureEvent* creature = new CreatureEvent(getScriptEnv()->getScriptInterface());
	if (creature) {
		creature->setName(getString(L, 2));
		creature->fromLua = true;
		pushUserdata<CreatureEvent>(L, creature);
		setMetatable(L, -1, "CreatureEvent");
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaCreatureEventType(lua_State* L)
{
	// creatureevent:type(callback)
	CreatureEvent* creature = getUserdata<CreatureEvent>(L, 1);
	if (creature) {
		std::string typeName = getString(L, 2);
		std::string tmpStr = boost::algorithm::to_lower_copy(typeName);
		if (tmpStr == "login") {
			creature->setEventType(CREATURE_EVENT_LOGIN);
		} else if (tmpStr == "logout") {
			creature->setEventType(CREATURE_EVENT_LOGOUT);
		} else if (tmpStr == "think") {
			creature->setEventType(CREATURE_EVENT_THINK);
		} else if (tmpStr == "preparedeath") {
			creature->setEventType(CREATURE_EVENT_PREPAREDEATH);
		} else if (tmpStr == "death") {
			creature->setEventType(CREATURE_EVENT_DEATH);
		} else if (tmpStr == "kill") {
			creature->setEventType(CREATURE_EVENT_KILL);
		} else if (tmpStr == "advance") {
			creature->setEventType(CREATURE_EVENT_ADVANCE);
		} else if (tmpStr == "modalwindow") {
			creature->setEventType(CREATURE_EVENT_MODALWINDOW);
		} else if (tmpStr == "textedit") {
			creature->setEventType(CREATURE_EVENT_TEXTEDIT);
		} else if (tmpStr == "healthchange") {
			creature->setEventType(CREATURE_EVENT_HEALTHCHANGE);
		} else if (tmpStr == "manachange") {
			creature->setEventType(CREATURE_EVENT_MANACHANGE);
		} else if (tmpStr == "extendedopcode") {
			creature->setEventType(CREATURE_EVENT_EXTENDED_OPCODE);
		} else {
			std::cout << "[Error - CreatureEvent::configureLuaEvent] Invalid type for creature event: " << typeName
			          << std::endl;
			pushBoolean(L, false);
		}
		creature->setLoaded(true);
		pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaCreatureEventRegister(lua_State* L)
{
	// creatureevent:register()
	CreatureEvent* creature = getUserdata<CreatureEvent>(L, 1);
	if (creature) {
		if (!creature->isScripted()) {
			pushBoolean(L, false);
			return 1;
		}
		pushBoolean(L, g_creatureEvents->registerLuaEvent(creature));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaCreatureEventOnCallback(lua_State* L)
{
	// creatureevent:onLogin / logout / etc. (callback)
	CreatureEvent* creature = getUserdata<CreatureEvent>(L, 1);
	if (creature) {
		if (!creature->loadCallback()) {
			pushBoolean(L, false);
			return 1;
		}
		pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

} // namespace

void tfs::lua::registerCreatureEvent(LuaScriptInterface& lsi)
{
	// Enums
	registerEnum(lsi, CREATURE_EVENT_NONE);
	registerEnum(lsi, CREATURE_EVENT_LOGIN);
	registerEnum(lsi, CREATURE_EVENT_LOGOUT);
	registerEnum(lsi, CREATURE_EVENT_THINK);
	registerEnum(lsi, CREATURE_EVENT_PREPAREDEATH);
	registerEnum(lsi, CREATURE_EVENT_DEATH);
	registerEnum(lsi, CREATURE_EVENT_KILL);
	registerEnum(lsi, CREATURE_EVENT_ADVANCE);
	registerEnum(lsi, CREATURE_EVENT_MODALWINDOW);
	registerEnum(lsi, CREATURE_EVENT_TEXTEDIT);
	registerEnum(lsi, CREATURE_EVENT_HEALTHCHANGE);
	registerEnum(lsi, CREATURE_EVENT_MANACHANGE);
	registerEnum(lsi, CREATURE_EVENT_EXTENDED_OPCODE);

	// CreatureEvent
	lsi.registerClass("CreatureEvent", "", luaCreateCreatureEvent);
	lsi.registerMethod("CreatureEvent", "type", luaCreatureEventType);
	lsi.registerMethod("CreatureEvent", "register", luaCreatureEventRegister);
	lsi.registerMethod("CreatureEvent", "onLogin", luaCreatureEventOnCallback);
	lsi.registerMethod("CreatureEvent", "onLogout", luaCreatureEventOnCallback);
	lsi.registerMethod("CreatureEvent", "onThink", luaCreatureEventOnCallback);
	lsi.registerMethod("CreatureEvent", "onPrepareDeath", luaCreatureEventOnCallback);
	lsi.registerMethod("CreatureEvent", "onDeath", luaCreatureEventOnCallback);
	lsi.registerMethod("CreatureEvent", "onKill", luaCreatureEventOnCallback);
	lsi.registerMethod("CreatureEvent", "onAdvance", luaCreatureEventOnCallback);
	lsi.registerMethod("CreatureEvent", "onModalWindow", luaCreatureEventOnCallback);
	lsi.registerMethod("CreatureEvent", "onTextEdit", luaCreatureEventOnCallback);
	lsi.registerMethod("CreatureEvent", "onHealthChange", luaCreatureEventOnCallback);
	lsi.registerMethod("CreatureEvent", "onManaChange", luaCreatureEventOnCallback);
	lsi.registerMethod("CreatureEvent", "onExtendedOpcode", luaCreatureEventOnCallback);
}
