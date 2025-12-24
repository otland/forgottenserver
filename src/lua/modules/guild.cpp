#include "../../otpch.h"

#include "../../guild.h"

#include "../../game.h"
#include "../api.h"
#include "../meta.h"
#include "../register.h"
#include "../script.h"

extern Game g_game;

namespace {

int luaGuildCreate(lua_State* L)
{
	// Guild(id)
	uint32_t id = tfs::lua::getNumber<uint32_t>(L, 2);

	if (const auto& guild = g_game.getGuild(id)) {
		tfs::lua::pushSharedPtr(L, guild);
		tfs::lua::setMetatable(L, -1, "Guild");
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaGuildGetId(lua_State* L)
{
	// guild:getId()
	if (const auto& guild = tfs::lua::getSharedPtr<Guild>(L, 1)) {
		tfs::lua::pushNumber(L, guild->getId());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaGuildGetName(lua_State* L)
{
	// guild:getName()
	if (const auto& guild = tfs::lua::getSharedPtr<Guild>(L, 1)) {
		tfs::lua::pushString(L, guild->getName());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaGuildGetMembersOnline(lua_State* L)
{
	// guild:getMembersOnline()
	const auto& guild = tfs::lua::getSharedPtr<const Guild>(L, 1);
	if (!guild) {
		lua_pushnil(L);
		return 1;
	}

	const auto& members = guild->getMembersOnline() | tfs::views::lock_weak_ptrs | std::ranges::to<std::vector>();
	lua_createtable(L, members.size(), 0);

	int index = 0;
	for (const auto& player : members) {
		tfs::lua::pushSharedPtr(L, player);
		tfs::lua::setMetatable(L, -1, "Player");
		lua_rawseti(L, -2, ++index);
	}
	return 1;
}

int luaGuildAddRank(lua_State* L)
{
	// guild:addRank(id, name, level)
	if (const auto& guild = tfs::lua::getSharedPtr<Guild>(L, 1)) {
		uint32_t id = tfs::lua::getNumber<uint32_t>(L, 2);
		const std::string& name = tfs::lua::getString(L, 3);
		uint8_t level = tfs::lua::getNumber<uint8_t>(L, 4);
		guild->addRank(id, name, level);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaGuildGetRankById(lua_State* L)
{
	// guild:getRankById(id)
	const auto& guild = tfs::lua::getSharedPtr<Guild>(L, 1);
	if (!guild) {
		lua_pushnil(L);
		return 1;
	}

	uint32_t id = tfs::lua::getNumber<uint32_t>(L, 2);
	if (auto rank = guild->getRankById(id)) {
		lua_createtable(L, 0, 3);
		tfs::lua::setField(L, "id", rank->id);
		tfs::lua::setField(L, "name", rank->name);
		tfs::lua::setField(L, "level", rank->level);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaGuildGetRankByLevel(lua_State* L)
{
	// guild:getRankByLevel(level)
	const auto& guild = tfs::lua::getSharedPtr<const Guild>(L, 1);
	if (!guild) {
		lua_pushnil(L);
		return 1;
	}

	uint8_t level = tfs::lua::getNumber<uint8_t>(L, 2);
	if (auto rank = guild->getRankByLevel(level)) {
		lua_createtable(L, 0, 3);
		tfs::lua::setField(L, "id", rank->id);
		tfs::lua::setField(L, "name", rank->name);
		tfs::lua::setField(L, "level", rank->level);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaGuildGetMotd(lua_State* L)
{
	// guild:getMotd()
	if (const auto& guild = tfs::lua::getSharedPtr<Guild>(L, 1)) {
		tfs::lua::pushString(L, guild->getMotd());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaGuildSetMotd(lua_State* L)
{
	// guild:setMotd(motd)
	if (const auto& guild = tfs::lua::getSharedPtr<Guild>(L, 1)) {
		const std::string& motd = tfs::lua::getString(L, 2);
		guild->setMotd(motd);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

} // namespace

void tfs::lua::registerGuild(LuaScriptInterface& lsi)
{
	lsi.registerClass("Guild", "", luaGuildCreate);
	lsi.registerMetaMethod("Guild", "__eq", tfs::lua::luaUserdataCompare);

	lsi.registerMethod("Guild", "getId", luaGuildGetId);
	lsi.registerMethod("Guild", "getName", luaGuildGetName);
	lsi.registerMethod("Guild", "getMembersOnline", luaGuildGetMembersOnline);

	lsi.registerMethod("Guild", "addRank", luaGuildAddRank);
	lsi.registerMethod("Guild", "getRankById", luaGuildGetRankById);
	lsi.registerMethod("Guild", "getRankByLevel", luaGuildGetRankByLevel);

	lsi.registerMethod("Guild", "getMotd", luaGuildGetMotd);
	lsi.registerMethod("Guild", "setMotd", luaGuildSetMotd);
}
