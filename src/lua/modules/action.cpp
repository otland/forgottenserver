#include "../../otpch.h"

#include "../../actions.h"
#include "../../script.h"
#include "../api.h"
#include "../env.h"
#include "../meta.h"
#include "../register.h"
#include "../script.h"

extern Actions* g_actions;
extern Scripts* g_scripts;

namespace {

int luaCreateAction(lua_State* L)
{
	// Action()
	if (tfs::lua::getScriptEnv()->getScriptInterface() != &g_scripts->getScriptInterface()) {
		tfs::lua::reportError(L, "Actions can only be registered in the Scripts interface.");
		lua_pushnil(L);
		return 1;
	}

	Action* action = new Action(tfs::lua::getScriptEnv()->getScriptInterface());
	action->fromLua = true;
	tfs::lua::pushUserdata(L, action);
	tfs::lua::setMetatable(L, -1, "Action");
	return 1;
}

int luaActionOnUse(lua_State* L)
{
	// action:onUse(callback)
	Action* action = tfs::lua::getUserdata<Action>(L, 1);
	if (action) {
		if (!action->loadCallback()) {
			tfs::lua::pushBoolean(L, false);
			return 1;
		}
		action->scripted = true;
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaActionRegister(lua_State* L)
{
	// action:register()
	Action* action = tfs::lua::getUserdata<Action>(L, 1);
	if (action) {
		if (!action->isScripted()) {
			tfs::lua::pushBoolean(L, false);
			return 1;
		}
		tfs::lua::pushBoolean(L, g_actions->registerLuaEvent(action));
		g_actions->clearItemIdRange(action);
		g_actions->clearUniqueIdRange(action);
		g_actions->clearActionIdRange(action);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaActionItemId(lua_State* L)
{
	// action:id(ids)
	Action* action = tfs::lua::getUserdata<Action>(L, 1);
	if (action) {
		int parameters = lua_gettop(L) - 1; // - 1 because self is a parameter aswell, which we want to skip ofc
		if (parameters > 1) {
			for (int i = 0; i < parameters; ++i) {
				g_actions->addItemId(action, tfs::lua::getNumber<uint16_t>(L, 2 + i));
			}
		} else {
			g_actions->addItemId(action, tfs::lua::getNumber<uint16_t>(L, 2));
		}
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaActionActionId(lua_State* L)
{
	// action:aid(aids)
	Action* action = tfs::lua::getUserdata<Action>(L, 1);
	if (action) {
		int parameters = lua_gettop(L) - 1; // - 1 because self is a parameter aswell, which we want to skip ofc
		if (parameters > 1) {
			for (int i = 0; i < parameters; ++i) {
				g_actions->addActionId(action, tfs::lua::getNumber<uint16_t>(L, 2 + i));
			}
		} else {
			g_actions->addActionId(action, tfs::lua::getNumber<uint16_t>(L, 2));
		}
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaActionUniqueId(lua_State* L)
{
	// action:uid(uids)
	Action* action = tfs::lua::getUserdata<Action>(L, 1);
	if (action) {
		int parameters = lua_gettop(L) - 1; // - 1 because self is a parameter aswell, which we want to skip ofc
		if (parameters > 1) {
			for (int i = 0; i < parameters; ++i) {
				g_actions->addUniqueId(action, tfs::lua::getNumber<uint16_t>(L, 2 + i));
			}
		} else {
			g_actions->addUniqueId(action, tfs::lua::getNumber<uint16_t>(L, 2));
		}
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaActionAllowFarUse(lua_State* L)
{
	// action:allowFarUse(bool)
	Action* action = tfs::lua::getUserdata<Action>(L, 1);
	if (action) {
		action->setAllowFarUse(tfs::lua::getBoolean(L, 2));
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaActionBlockWalls(lua_State* L)
{
	// action:blockWalls(bool)
	Action* action = tfs::lua::getUserdata<Action>(L, 1);
	if (action) {
		action->setCheckLineOfSight(tfs::lua::getBoolean(L, 2));
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaActionCheckFloor(lua_State* L)
{
	// action:checkFloor(bool)
	Action* action = tfs::lua::getUserdata<Action>(L, 1);
	if (action) {
		action->setCheckFloor(tfs::lua::getBoolean(L, 2));
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

} // namespace

void tfs::lua::registerAction(LuaScriptInterface& lsi)
{
	lsi.registerClass("Action", "", luaCreateAction);
	lsi.registerMethod("Action", "onUse", luaActionOnUse);
	lsi.registerMethod("Action", "register", luaActionRegister);
	lsi.registerMethod("Action", "id", luaActionItemId);
	lsi.registerMethod("Action", "aid", luaActionActionId);
	lsi.registerMethod("Action", "uid", luaActionUniqueId);
	lsi.registerMethod("Action", "allowFarUse", luaActionAllowFarUse);
	lsi.registerMethod("Action", "blockWalls", luaActionBlockWalls);
	lsi.registerMethod("Action", "checkFloor", luaActionCheckFloor);
}
