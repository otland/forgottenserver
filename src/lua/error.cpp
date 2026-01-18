#include "../otpch.h"

#include "error.h"

#include "api.h"
#include "env.h"
#include "script.h"

#include <boost/stacktrace.hpp>

namespace {

std::string getStackTrace(lua_State* L, std::string_view error_desc)
{
	luaL_traceback(L, L, error_desc.data(), 1);
	return tfs::lua::popString(L);
}

} // namespace

namespace tfs::lua {

std::string getErrorDesc(ErrorCode_t code)
{
	switch (code) {
		case LUA_ERROR_PLAYER_NOT_FOUND:
			return "Player not found";
		case LUA_ERROR_CREATURE_NOT_FOUND:
			return "Creature not found";
		case LUA_ERROR_ITEM_NOT_FOUND:
			return "Item not found";
		case LUA_ERROR_THING_NOT_FOUND:
			return "Thing not found";
		case LUA_ERROR_TILE_NOT_FOUND:
			return "Tile not found";
		case LUA_ERROR_HOUSE_NOT_FOUND:
			return "House not found";
		case LUA_ERROR_COMBAT_NOT_FOUND:
			return "Combat not found";
		case LUA_ERROR_CONDITION_NOT_FOUND:
			return "Condition not found";
		case LUA_ERROR_AREA_NOT_FOUND:
			return "Area not found";
		case LUA_ERROR_CONTAINER_NOT_FOUND:
			return "Container not found";
		case LUA_ERROR_VARIANT_NOT_FOUND:
			return "Variant not found";
		case LUA_ERROR_VARIANT_UNKNOWN:
			return "Unknown variant type";
		case LUA_ERROR_SPELL_NOT_FOUND:
			return "Spell not found";
		default:
			return "Bad error code";
	}
}

int luaErrorHandler(lua_State* L)
{
	std::string errorMessage = popString(L);
	pushString(L, getStackTrace(L, errorMessage));
	return 1;
}

void reportError(lua_State* L, std::string_view error_desc, bool stacktrace /*= false*/)
{
	auto [scriptId, luaScriptInterface, callbackId, timerEvent] = getScriptEnv()->getEventInfo();

	std::cout << "\nLua Script Error: ";

	if (luaScriptInterface) {
		std::cout << '[' << luaScriptInterface->getInterfaceName() << "]\n";

		if (timerEvent) {
			std::cout << "in a timer event called from:\n";
		}

		if (callbackId) {
			std::cout << "in callback: " << luaScriptInterface->getFileById(callbackId) << '\n';
		}

		std::cout << luaScriptInterface->getFileById(scriptId) << '\n';
	}

	if (stacktrace) {
		std::cout << "\nStack trace:\n" << boost::stacktrace::stacktrace() << '\n';

		if (L) {
			std::cout << "\nLua stack trace:\n" << getStackTrace(L, error_desc) << '\n';
		} else {
			std::cout << "\n" << error_desc << '\n';
		}
	}
}

} // namespace tfs::lua
