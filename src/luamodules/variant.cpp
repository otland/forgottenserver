#include "../otpch.h"

#include "../luaapi.h"
#include "../luascript.h"
#include "../thing.h"
#include "luaregister.h"

namespace {

using namespace tfs::lua;

int luaVariantCreate(lua_State* L)
{
	// Variant(number or string or position or thing)
	LuaVariant variant;
	if (lua_isuserdata(L, 2)) {
		if (Thing* thing = getThing(L, 2)) {
			variant.setTargetPosition(thing->getPosition());
		}
	} else if (lua_istable(L, 2)) {
		variant.setPosition(getPosition(L, 2));
	} else if (isNumber(L, 2)) {
		variant.setNumber(getNumber<uint32_t>(L, 2));
	} else if (lua_isstring(L, 2)) {
		variant.setString(getString(L, 2));
	}
	pushVariant(L, variant);
	return 1;
}

int luaVariantGetNumber(lua_State* L)
{
	// Variant:getNumber()
	const LuaVariant& variant = getVariant(L, 1);
	if (variant.isNumber()) {
		lua_pushnumber(L, variant.getNumber());
	} else {
		lua_pushnumber(L, 0);
	}
	return 1;
}

int luaVariantGetString(lua_State* L)
{
	// Variant:getString()
	const LuaVariant& variant = getVariant(L, 1);
	if (variant.isString()) {
		pushString(L, variant.getString());
	} else {
		pushString(L, std::string());
	}
	return 1;
}

int luaVariantGetPosition(lua_State* L)
{
	// Variant:getPosition()
	const LuaVariant& variant = getVariant(L, 1);
	if (variant.isPosition()) {
		pushPosition(L, variant.getPosition());
	} else if (variant.isTargetPosition()) {
		pushPosition(L, variant.getTargetPosition());
	} else {
		pushPosition(L, Position());
	}
	return 1;
}

} // namespace

void tfs::lua::registerVariant(LuaScriptInterface& lsi)
{
	lsi.registerClass("Variant", "", luaVariantCreate);

	lsi.registerMethod("Variant", "getNumber", luaVariantGetNumber);
	lsi.registerMethod("Variant", "getString", luaVariantGetString);
	lsi.registerMethod("Variant", "getPosition", luaVariantGetPosition);
}
