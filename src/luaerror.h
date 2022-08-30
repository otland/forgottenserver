#ifndef TFS_LUAERROR
#define TFS_LUAERROR

namespace tfs::lua {

enum ErrorCode_t
{
	LUA_ERROR_PLAYER_NOT_FOUND,
	LUA_ERROR_CREATURE_NOT_FOUND,
	LUA_ERROR_ITEM_NOT_FOUND,
	LUA_ERROR_THING_NOT_FOUND,
	LUA_ERROR_TILE_NOT_FOUND,
	LUA_ERROR_HOUSE_NOT_FOUND,
	LUA_ERROR_COMBAT_NOT_FOUND,
	LUA_ERROR_CONDITION_NOT_FOUND,
	LUA_ERROR_AREA_NOT_FOUND,
	LUA_ERROR_CONTAINER_NOT_FOUND,
	LUA_ERROR_VARIANT_NOT_FOUND,
	LUA_ERROR_VARIANT_UNKNOWN,
	LUA_ERROR_SPELL_NOT_FOUND,
};

std::string getErrorDesc(ErrorCode_t code);

int luaErrorHandler(lua_State* L);
void reportError(const char* function, const std::string& error_desc, lua_State* L = nullptr, bool stack_trace = false);

} // namespace tfs::lua

#define reportErrorFunc(L, a) tfs::lua::reportError(__FUNCTION__, a, L, true)

#endif
