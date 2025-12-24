#include "../../otpch.h"

#include "../../combat.h"
#include "../../creature.h"
#include "../../game.h"
#include "../../matrixarea.h"
#include "../../player.h"
#include "../api.h"
#include "../env.h"
#include "../meta.h"
#include "../register.h"
#include "../script.h"

extern Game g_game;
extern LuaEnvironment g_luaEnvironment;

namespace {

int luaIsValidUID(lua_State* L)
{
	// isValidUID(uid)
	const auto& thing = tfs::lua::getScriptEnv()->getThingByUID(tfs::lua::getNumber<uint32_t>(L, -1));
	tfs::lua::pushBoolean(L, thing != nullptr);
	return 1;
}

int luaIsMoveable(lua_State* L)
{
	// isMoveable(uid)
	// isMovable(uid)
	const auto& thing = tfs::lua::getScriptEnv()->getThingByUID(tfs::lua::getNumber<uint32_t>(L, -1));
	if (!thing) {
		tfs::lua::reportError(L, tfs::lua::getErrorDesc(tfs::lua::LUA_ERROR_THING_NOT_FOUND));
		tfs::lua::pushBoolean(L, false);
		return 1;
	}

	if (const auto& item = thing->asItem()) {
		tfs::lua::pushBoolean(L, item->isPushable());
	} else if (const auto& creature = thing->asCreature()) {
		tfs::lua::pushBoolean(L, creature->isPushable());
	} else if (const auto& tile = thing->asTile()) {
		tfs::lua::pushBoolean(L, false);
	} else {
		tfs::lua::pushBoolean(L, false);
	}
	return 1;
}

} // namespace

void tfs::lua::registerThing(LuaScriptInterface& lsi)
{
	lua_register(lsi.getLuaState(), "isValidUID", luaIsValidUID);

	lua_register(lsi.getLuaState(), "isMovable", luaIsMoveable);
}
