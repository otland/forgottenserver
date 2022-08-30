#include "../otpch.h"

#include "../podium.h"

#include "../game.h"
#include "../luaapi.h"
#include "../luaenv.h"
#include "../luameta.h"
#include "../luascript.h"
#include "luaregister.h"

extern Game g_game;

namespace {

using namespace tfs::lua;

int luaPodiumCreate(lua_State* L)
{
	// Podium(uid)
	uint32_t id = getNumber<uint32_t>(L, 2);

	Item* item = getScriptEnv()->getItemByUID(id);
	if (item && item->getPodium()) {
		pushUserdata(L, item);
		setMetatable(L, -1, "Podium");
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPodiumGetOutfit(lua_State* L)
{
	// podium:getOutfit()
	const Podium* podium = getUserdata<const Podium>(L, 1);
	if (podium) {
		pushOutfit(L, podium->getOutfit());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPodiumSetOutfit(lua_State* L)
{
	// podium:setOutfit(outfit)
	Podium* podium = getUserdata<Podium>(L, 1);
	if (podium) {
		podium->setOutfit(getOutfit(L, 2));
		g_game.updatePodium(podium);
		pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPodiumHasFlag(lua_State* L)
{
	// podium:hasFlag(flag)
	Podium* podium = getUserdata<Podium>(L, 1);
	if (podium) {
		PodiumFlags flag = getNumber<PodiumFlags>(L, 2);
		pushBoolean(L, podium->hasFlag(flag));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPodiumSetFlag(lua_State* L)
{
	// podium:setFlag(flag, value)
	uint8_t value = getBoolean(L, 3);
	PodiumFlags flag = getNumber<PodiumFlags>(L, 2);
	Podium* podium = getUserdata<Podium>(L, 1);

	if (podium) {
		podium->setFlagValue(flag, value);
		g_game.updatePodium(podium);
		pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPodiumGetDirection(lua_State* L)
{
	// podium:getDirection()
	const Podium* podium = getUserdata<const Podium>(L, 1);
	if (podium) {
		lua_pushnumber(L, podium->getDirection());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPodiumSetDirection(lua_State* L)
{
	// podium:setDirection(direction)
	Podium* podium = getUserdata<Podium>(L, 1);
	if (podium) {
		podium->setDirection(getNumber<Direction>(L, 2));
		g_game.updatePodium(podium);
		pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

} // namespace

void tfs::lua::registerPodium(LuaScriptInterface& lsi)
{
	lsi.registerClass("Podium", "Item", luaPodiumCreate);
	lsi.registerMetaMethod("Podium", "__eq", luaUserdataCompare);

	lsi.registerMethod("Podium", "getOutfit", luaPodiumGetOutfit);
	lsi.registerMethod("Podium", "setOutfit", luaPodiumSetOutfit);
	lsi.registerMethod("Podium", "hasFlag", luaPodiumHasFlag);
	lsi.registerMethod("Podium", "setFlag", luaPodiumSetFlag);
	lsi.registerMethod("Podium", "getDirection", luaPodiumGetDirection);
	lsi.registerMethod("Podium", "setDirection", luaPodiumSetDirection);
}
