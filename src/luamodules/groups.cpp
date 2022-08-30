#include "../otpch.h"

#include "../groups.h"

#include "../const.h"
#include "../game.h"
#include "../luaapi.h"
#include "../luameta.h"
#include "../luascript.h"
#include "luaregister.h"

extern Game g_game;

namespace {

using namespace tfs::lua;

int luaGroupCreate(lua_State* L)
{
	// Group(id)
	uint32_t id = getNumber<uint32_t>(L, 2);

	Group* group = g_game.groups.getGroup(id);
	if (group) {
		pushUserdata<Group>(L, group);
		setMetatable(L, -1, "Group");
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaGroupGetId(lua_State* L)
{
	// group:getId()
	Group* group = getUserdata<Group>(L, 1);
	if (group) {
		lua_pushnumber(L, group->id);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaGroupGetName(lua_State* L)
{
	// group:getName()
	Group* group = getUserdata<Group>(L, 1);
	if (group) {
		pushString(L, group->name);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaGroupGetFlags(lua_State* L)
{
	// group:getFlags()
	Group* group = getUserdata<Group>(L, 1);
	if (group) {
		lua_pushnumber(L, group->flags);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaGroupGetAccess(lua_State* L)
{
	// group:getAccess()
	Group* group = getUserdata<Group>(L, 1);
	if (group) {
		pushBoolean(L, group->access);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaGroupGetMaxDepotItems(lua_State* L)
{
	// group:getMaxDepotItems()
	Group* group = getUserdata<Group>(L, 1);
	if (group) {
		lua_pushnumber(L, group->maxDepotItems);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaGroupGetMaxVipEntries(lua_State* L)
{
	// group:getMaxVipEntries()
	Group* group = getUserdata<Group>(L, 1);
	if (group) {
		lua_pushnumber(L, group->maxVipEntries);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaGroupHasFlag(lua_State* L)
{
	// group:hasFlag(flag)
	Group* group = getUserdata<Group>(L, 1);
	if (group) {
		PlayerFlags flag = getNumber<PlayerFlags>(L, 2);
		pushBoolean(L, (group->flags & flag) != 0);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

} // namespace

void tfs::lua::registerGroup(LuaScriptInterface& lsi)
{
	lsi.registerClass("Group", "", luaGroupCreate);
	lsi.registerMetaMethod("Group", "__eq", luaUserdataCompare);

	lsi.registerMethod("Group", "getId", luaGroupGetId);
	lsi.registerMethod("Group", "getName", luaGroupGetName);
	lsi.registerMethod("Group", "getFlags", luaGroupGetFlags);
	lsi.registerMethod("Group", "getAccess", luaGroupGetAccess);
	lsi.registerMethod("Group", "getMaxDepotItems", luaGroupGetMaxDepotItems);
	lsi.registerMethod("Group", "getMaxVipEntries", luaGroupGetMaxVipEntries);
	lsi.registerMethod("Group", "hasFlag", luaGroupHasFlag);
}
