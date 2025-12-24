#include "../../otpch.h"

#include "../../game.h"
#include "../api.h"
#include "../register.h"
#include "../script.h"

extern Game g_game;

namespace {

int luaPositionCreate(lua_State* L)
{
	// Position([x = 0[, y = 0[, z = 0[, stackpos = 0]]]])
	// Position([position])
	if (lua_gettop(L) <= 1) {
		tfs::lua::pushPosition(L, Position());
		return 1;
	}

	int32_t stackpos;
	if (lua_istable(L, 2)) {
		const Position& position = tfs::lua::getPosition(L, 2, stackpos);
		tfs::lua::pushPosition(L, position, stackpos);
	} else {
		uint16_t x = tfs::lua::getNumber<uint16_t>(L, 2, 0);
		uint16_t y = tfs::lua::getNumber<uint16_t>(L, 3, 0);
		uint8_t z = tfs::lua::getNumber<uint8_t>(L, 4, 0);
		stackpos = tfs::lua::getNumber<int32_t>(L, 5, 0);

		tfs::lua::pushPosition(L, Position(x, y, z), stackpos);
	}
	return 1;
}

int luaPositionIsSightClear(lua_State* L)
{
	// position:isSightClear(positionEx[, sameFloor = true])
	bool sameFloor = tfs::lua::getBoolean(L, 3, true);
	const Position& positionEx = tfs::lua::getPosition(L, 2);
	const Position& position = tfs::lua::getPosition(L, 1);
	tfs::lua::pushBoolean(L, g_game.isSightClear(position, positionEx, sameFloor));
	return 1;
}

int luaPositionSendMagicEffect(lua_State* L)
{
	// position:sendMagicEffect(magicEffect[, player = nullptr])
	SpectatorVec spectators;
	if (lua_gettop(L) >= 3) {
		if (const auto& player = tfs::lua::getPlayer(L, 3)) {
			spectators.emplace(player);
		}
	}

	MagicEffectClasses magicEffect = tfs::lua::getNumber<MagicEffectClasses>(L, 2);
	if (magicEffect == CONST_ME_NONE) {
		tfs::lua::pushBoolean(L, false);
		return 1;
	}

	const Position& position = tfs::lua::getPosition(L, 1);
	if (!spectators.empty()) {
		Game::addMagicEffect(spectators, position, magicEffect);
	} else {
		g_game.addMagicEffect(position, magicEffect);
	}

	tfs::lua::pushBoolean(L, true);
	return 1;
}

int luaPositionSendDistanceEffect(lua_State* L)
{
	// position:sendDistanceEffect(positionEx, distanceEffect[, player = nullptr])
	SpectatorVec spectators;
	if (lua_gettop(L) >= 4) {
		if (const auto& player = tfs::lua::getPlayer(L, 4)) {
			spectators.emplace(player);
		}
	}

	ShootType_t distanceEffect = tfs::lua::getNumber<ShootType_t>(L, 3);
	const Position& positionEx = tfs::lua::getPosition(L, 2);
	const Position& position = tfs::lua::getPosition(L, 1);
	if (!spectators.empty()) {
		Game::addDistanceEffect(spectators, position, positionEx, distanceEffect);
	} else {
		g_game.addDistanceEffect(position, positionEx, distanceEffect);
	}

	tfs::lua::pushBoolean(L, true);
	return 1;
}

} // namespace

void tfs::lua::registerPosition(LuaScriptInterface& lsi)
{
	registerEnum(lsi, DIRECTION_NORTH);
	registerEnum(lsi, DIRECTION_EAST);
	registerEnum(lsi, DIRECTION_SOUTH);
	registerEnum(lsi, DIRECTION_WEST);
	registerEnum(lsi, DIRECTION_SOUTHWEST);
	registerEnum(lsi, DIRECTION_SOUTHEAST);
	registerEnum(lsi, DIRECTION_NORTHWEST);
	registerEnum(lsi, DIRECTION_NORTHEAST);

	lsi.registerClass("Position", "", luaPositionCreate);

	lsi.registerMethod("Position", "isSightClear", luaPositionIsSightClear);

	lsi.registerMethod("Position", "sendMagicEffect", luaPositionSendMagicEffect);
	lsi.registerMethod("Position", "sendDistanceEffect", luaPositionSendDistanceEffect);
}
