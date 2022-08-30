#include "../otpch.h"

#include "../luaapi.h"
#include "../luascript.h"
#include "../tools.h"
#include "luaregister.h"

namespace {

using namespace tfs::lua;

// os
int luaSystemTime(lua_State* L)
{
	// os.mtime()
	lua_pushnumber(L, OTSYS_TIME());
	return 1;
}

// table
int luaTableCreate(lua_State* L)
{
	// table.create(arrayLength, keyLength)
	lua_createtable(L, getNumber<int32_t>(L, 1), getNumber<int32_t>(L, 2));
	return 1;
}

int luaTablePack(lua_State* L)
{
	// table.pack(...)
	int i;
	int n = lua_gettop(L);     /* number of elements to pack */
	lua_createtable(L, n, 1);  /* create result table */
	lua_insert(L, 1);          /* put it at index 1 */
	for (i = n; i >= 1; i--) { /* assign elements */
		lua_rawseti(L, 1, i);
	}
	if (luaL_callmeta(L, -1, "__index") != 0) {
		lua_replace(L, -2);
	}
	lua_pushinteger(L, n);
	lua_setfield(L, 1, "n"); /* t.n = number of elements */
	return 1;                /* return table */
}

} // namespace

void tfs::lua::registerStdlib(LuaScriptInterface& lsi)
{
	// os
	lsi.registerMethod("os", "mtime", luaSystemTime);

	// table
	lsi.registerMethod("table", "create", luaTableCreate);
	lsi.registerMethod("table", "pack", luaTablePack);
}
