#include "../otpch.h"

#include "../town.h"

#include "../game.h"
#include "../luaapi.h"
#include "../luameta.h"
#include "../luascript.h"
#include "luaregister.h"

extern Game g_game;

namespace {

using namespace tfs::lua;

int luaTownCreate(lua_State* L)
{
	// Town(id or name)
	Town* town;
	if (isNumber(L, 2)) {
		town = g_game.map.towns.getTown(getNumber<uint32_t>(L, 2));
	} else if (lua_isstring(L, 2)) {
		town = g_game.map.towns.getTown(getString(L, 2));
	} else {
		town = nullptr;
	}

	if (town) {
		pushUserdata<Town>(L, town);
		setMetatable(L, -1, "Town");
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaTownGetId(lua_State* L)
{
	// town:getId()
	Town* town = getUserdata<Town>(L, 1);
	if (town) {
		lua_pushnumber(L, town->getID());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaTownGetName(lua_State* L)
{
	// town:getName()
	Town* town = getUserdata<Town>(L, 1);
	if (town) {
		pushString(L, town->getName());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaTownGetTemplePosition(lua_State* L)
{
	// town:getTemplePosition()
	Town* town = getUserdata<Town>(L, 1);
	if (town) {
		pushPosition(L, town->getTemplePosition());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

} // namespace

void tfs::lua::registerTown(LuaScriptInterface& lsi)
{
	lsi.registerClass("Town", "", luaTownCreate);
	lsi.registerMetaMethod("Town", "__eq", luaUserdataCompare);

	lsi.registerMethod("Town", "getId", luaTownGetId);
	lsi.registerMethod("Town", "getName", luaTownGetName);
	lsi.registerMethod("Town", "getTemplePosition", luaTownGetTemplePosition);
}
