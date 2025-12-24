#include "../../otpch.h"

#include "../../podium.h"

#include "../../game.h"
#include "../api.h"
#include "../env.h"
#include "../meta.h"
#include "../register.h"
#include "../script.h"

extern Game g_game;

namespace {

int luaPodiumCreate(lua_State* L)
{
	// Podium(uid)
	uint32_t id = tfs::lua::getNumber<uint32_t>(L, 2);

	const auto& item = tfs::lua::getScriptEnv()->getItemByUID(id);
	if (item && item->getPodium()) {
		tfs::lua::pushSharedPtr(L, item);
		tfs::lua::setMetatable(L, -1, "Podium");
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPodiumGetOutfit(lua_State* L)
{
	// podium:getOutfit()
	if (const auto& podium = tfs::lua::getSharedPtr<const Podium>(L, 1)) {
		tfs::lua::pushOutfit(L, podium->getOutfit());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPodiumSetOutfit(lua_State* L)
{
	// podium:setOutfit(outfit)
	if (const auto& podium = tfs::lua::getSharedPtr<Podium>(L, 1)) {
		podium->setOutfit(tfs::lua::getOutfit(L, 2));
		g_game.updatePodium(podium);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPodiumHasFlag(lua_State* L)
{
	// podium:hasFlag(flag)
	if (const auto& podium = tfs::lua::getSharedPtr<Podium>(L, 1)) {
		PodiumFlags flag = tfs::lua::getNumber<PodiumFlags>(L, 2);
		tfs::lua::pushBoolean(L, podium->hasFlag(flag));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPodiumSetFlag(lua_State* L)
{
	// podium:setFlag(flag, value)
	uint8_t value = tfs::lua::getBoolean(L, 3);
	PodiumFlags flag = tfs::lua::getNumber<PodiumFlags>(L, 2);
	const auto& podium = tfs::lua::getSharedPtr<Podium>(L, 1);

	if (podium) {
		podium->setFlagValue(flag, value);
		g_game.updatePodium(podium);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPodiumGetDirection(lua_State* L)
{
	// podium:getDirection()
	if (const auto& podium = tfs::lua::getSharedPtr<const Podium>(L, 1)) {
		tfs::lua::pushNumber(L, podium->getDirection());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPodiumSetDirection(lua_State* L)
{
	// podium:setDirection(direction)
	if (const auto& podium = tfs::lua::getSharedPtr<Podium>(L, 1)) {
		podium->setDirection(tfs::lua::getNumber<Direction>(L, 2));
		g_game.updatePodium(podium);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

} // namespace

void tfs::lua::registerPodium(LuaScriptInterface& lsi)
{
	lsi.registerClass("Podium", "Item", luaPodiumCreate);
	lsi.registerMetaMethod("Podium", "__eq", tfs::lua::luaUserdataCompare);
	lsi.registerMetaMethod("Podium", "__gc", tfs::lua::luaSharedPtrDelete<Podium>);

	lsi.registerMethod("Podium", "getOutfit", luaPodiumGetOutfit);
	lsi.registerMethod("Podium", "setOutfit", luaPodiumSetOutfit);
	lsi.registerMethod("Podium", "hasFlag", luaPodiumHasFlag);
	lsi.registerMethod("Podium", "setFlag", luaPodiumSetFlag);
	lsi.registerMethod("Podium", "getDirection", luaPodiumGetDirection);
	lsi.registerMethod("Podium", "setDirection", luaPodiumSetDirection);
}
