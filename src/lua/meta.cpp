#include "../otpch.h"

#include "meta.h"

#include "../creature.h"
#include "../item.h"

namespace tfs::lua {

void setMetatable(lua_State* L, int32_t index, std::string_view name)
{
	luaL_getmetatable(L, name.data());
	lua_setmetatable(L, index - 1);
}

void setWeakMetatable(lua_State* L, int32_t index, const std::string& name)
{
	static std::set<std::string> weakObjectTypes;
	const std::string& weakName = name + "_weak";

	auto result = weakObjectTypes.emplace(name);
	if (result.second) {
		luaL_getmetatable(L, name.data());
		int childMetatable = lua_gettop(L);

		luaL_newmetatable(L, weakName.data());
		int metatable = lua_gettop(L);

		static const std::vector<std::string> methodKeys = {"__index", "__metatable", "__eq"};
		for (const std::string& metaKey : methodKeys) {
			lua_getfield(L, childMetatable, metaKey.data());
			lua_setfield(L, metatable, metaKey.data());
		}

		static const std::vector<int> methodIndexes = {'h', 'p', 't'};
		for (int metaIndex : methodIndexes) {
			lua_rawgeti(L, childMetatable, metaIndex);
			lua_rawseti(L, metatable, metaIndex);
		}

		lua_pushnil(L);
		lua_setfield(L, metatable, "__gc");

		lua_remove(L, childMetatable);
	} else {
		luaL_getmetatable(L, weakName.data());
	}
	lua_setmetatable(L, index - 1);
}

void setItemMetatable(lua_State* L, int32_t index, const std::shared_ptr<const Item>& item)
{
	if (item->getContainer()) {
		luaL_getmetatable(L, "Container");
	} else if (item->getTeleport()) {
		luaL_getmetatable(L, "Teleport");
	} else if (item->getPodium()) {
		luaL_getmetatable(L, "Podium");
	} else {
		luaL_getmetatable(L, "Item");
	}
	lua_setmetatable(L, index - 1);
}

void setCreatureMetatable(lua_State* L, int32_t index, const std::shared_ptr<const Creature>& creature)
{
	if (creature->getPlayer()) {
		luaL_getmetatable(L, "Player");
	} else if (creature->asMonster()) {
		luaL_getmetatable(L, "Monster");
	} else {
		luaL_getmetatable(L, "Npc");
	}
	lua_setmetatable(L, index - 1);
}

} // namespace tfs::lua
