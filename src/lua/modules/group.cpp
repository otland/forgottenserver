#include "../../otpch.h"

#include "../../const.h"
#include "../../game.h"
#include "../../groups.h"
#include "../api.h"
#include "../meta.h"
#include "../register.h"
#include "../script.h"

extern Game g_game;

namespace {

int luaGroupCreate(lua_State* L)
{
	// Group(id)
	uint32_t id = tfs::lua::getNumber<uint32_t>(L, 2);

	Group* group = g_game.groups.getGroup(id);
	if (group) {
		tfs::lua::pushUserdata(L, group);
		tfs::lua::setMetatable(L, -1, "Group");
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaGroupGetId(lua_State* L)
{
	// group:getId()
	Group* group = tfs::lua::getUserdata<Group>(L, 1);
	if (group) {
		tfs::lua::pushNumber(L, group->id);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaGroupGetName(lua_State* L)
{
	// group:getName()
	Group* group = tfs::lua::getUserdata<Group>(L, 1);
	if (group) {
		tfs::lua::pushString(L, group->name);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaGroupGetFlags(lua_State* L)
{
	// group:getFlags()
	Group* group = tfs::lua::getUserdata<Group>(L, 1);
	if (group) {
		tfs::lua::pushNumber(L, group->flags);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaGroupGetAccess(lua_State* L)
{
	// group:getAccess()
	Group* group = tfs::lua::getUserdata<Group>(L, 1);
	if (group) {
		tfs::lua::pushBoolean(L, group->access);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaGroupGetMaxDepotItems(lua_State* L)
{
	// group:getMaxDepotItems()
	Group* group = tfs::lua::getUserdata<Group>(L, 1);
	if (group) {
		tfs::lua::pushNumber(L, group->maxDepotItems);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaGroupGetMaxVipEntries(lua_State* L)
{
	// group:getMaxVipEntries()
	Group* group = tfs::lua::getUserdata<Group>(L, 1);
	if (group) {
		tfs::lua::pushNumber(L, group->maxVipEntries);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaGroupHasFlag(lua_State* L)
{
	// group:hasFlag(flag)
	Group* group = tfs::lua::getUserdata<Group>(L, 1);
	if (group) {
		PlayerFlags flag = tfs::lua::getNumber<PlayerFlags>(L, 2);
		tfs::lua::pushBoolean(L, (group->flags & flag) != 0);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

} // namespace

void tfs::lua::registerGroup(LuaScriptInterface& lsi)
{
	lsi.registerClass("Group", "", luaGroupCreate);
	lsi.registerMetaMethod("Group", "__eq", tfs::lua::luaUserdataCompare);

	lsi.registerMethod("Group", "getId", luaGroupGetId);
	lsi.registerMethod("Group", "getName", luaGroupGetName);
	lsi.registerMethod("Group", "getFlags", luaGroupGetFlags);
	lsi.registerMethod("Group", "getAccess", luaGroupGetAccess);
	lsi.registerMethod("Group", "getMaxDepotItems", luaGroupGetMaxDepotItems);
	lsi.registerMethod("Group", "getMaxVipEntries", luaGroupGetMaxVipEntries);
	lsi.registerMethod("Group", "hasFlag", luaGroupHasFlag);
}
