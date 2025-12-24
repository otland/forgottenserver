#include "../../otpch.h"

#include "../../teleport.h"

#include "../api.h"
#include "../env.h"
#include "../meta.h"
#include "../register.h"
#include "../script.h"

namespace {

int luaTeleportCreate(lua_State* L)
{
	// Teleport(uid)
	uint32_t id = tfs::lua::getNumber<uint32_t>(L, 2);

	const auto& item = tfs::lua::getScriptEnv()->getItemByUID(id);
	if (item && item->getTeleport()) {
		tfs::lua::pushSharedPtr(L, item);
		tfs::lua::setMetatable(L, -1, "Teleport");
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaTeleportGetDestination(lua_State* L)
{
	// teleport:getDestination()
	if (const auto& teleport = tfs::lua::getSharedPtr<Teleport>(L, 1)) {
		tfs::lua::pushPosition(L, teleport->getDestPos());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaTeleportSetDestination(lua_State* L)
{
	// teleport:setDestination(position)
	if (const auto& teleport = tfs::lua::getSharedPtr<Teleport>(L, 1)) {
		teleport->setDestPos(tfs::lua::getPosition(L, 2));
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

} // namespace

void tfs::lua::registerTeleport(LuaScriptInterface& lsi)
{
	lsi.registerClass("Teleport", "Item", luaTeleportCreate);
	lsi.registerMetaMethod("Teleport", "__eq", tfs::lua::luaUserdataCompare);
	lsi.registerMetaMethod("Teleport", "__gc", tfs::lua::luaSharedPtrDelete<Teleport>);

	lsi.registerMethod("Teleport", "getDestination", luaTeleportGetDestination);
	lsi.registerMethod("Teleport", "setDestination", luaTeleportSetDestination);
}
