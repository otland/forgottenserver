#include "../otpch.h"

#include "../teleport.h"

#include "../luaapi.h"
#include "../luaenv.h"
#include "../luameta.h"
#include "../luascript.h"
#include "luaregister.h"

namespace {

using namespace tfs::lua;

int luaTeleportCreate(lua_State* L)
{
	// Teleport(uid)
	uint32_t id = getNumber<uint32_t>(L, 2);

	Item* item = getScriptEnv()->getItemByUID(id);
	if (item && item->getTeleport()) {
		pushUserdata(L, item);
		setMetatable(L, -1, "Teleport");
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaTeleportGetDestination(lua_State* L)
{
	// teleport:getDestination()
	Teleport* teleport = getUserdata<Teleport>(L, 1);
	if (teleport) {
		pushPosition(L, teleport->getDestPos());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaTeleportSetDestination(lua_State* L)
{
	// teleport:setDestination(position)
	Teleport* teleport = getUserdata<Teleport>(L, 1);
	if (teleport) {
		teleport->setDestPos(getPosition(L, 2));
		pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

} // namespace

void tfs::lua::registerTeleport(LuaScriptInterface& lsi)
{
	lsi.registerClass("Teleport", "Item", luaTeleportCreate);
	lsi.registerMetaMethod("Teleport", "__eq", luaUserdataCompare);

	lsi.registerMethod("Teleport", "getDestination", luaTeleportGetDestination);
	lsi.registerMethod("Teleport", "setDestination", luaTeleportSetDestination);
}
