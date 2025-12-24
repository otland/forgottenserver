#include "../../otpch.h"

#include "../../thing.h"
#include "../api.h"
#include "../register.h"
#include "../script.h"

namespace {

int luaVariantCreate(lua_State* L)
{
	// Variant(number or string or position or thing)
	LuaVariant variant;
	if (lua_isuserdata(L, 2)) {
		if (const auto& thing = tfs::lua::getThing(L, 2)) {
			variant.setTargetPosition(thing->getPosition());
		}
	} else if (lua_istable(L, 2)) {
		variant.setPosition(tfs::lua::getPosition(L, 2));
	} else if (tfs::lua::isNumber(L, 2)) {
		variant.setNumber(tfs::lua::getNumber<uint32_t>(L, 2));
	} else if (lua_isstring(L, 2)) {
		variant.setString(tfs::lua::getString(L, 2));
	}
	tfs::lua::pushVariant(L, variant);
	return 1;
}

int luaVariantGetNumber(lua_State* L)
{
	// Variant:getNumber()
	const LuaVariant& variant = tfs::lua::getVariant(L, 1);
	if (variant.isNumber()) {
		tfs::lua::pushNumber(L, variant.getNumber());
	} else {
		tfs::lua::pushNumber(L, 0);
	}
	return 1;
}

int luaVariantGetString(lua_State* L)
{
	// Variant:getString()
	const LuaVariant& variant = tfs::lua::getVariant(L, 1);
	if (variant.isString()) {
		tfs::lua::pushString(L, variant.getString());
	} else {
		tfs::lua::pushString(L, std::string());
	}
	return 1;
}

int luaVariantGetPosition(lua_State* L)
{
	// Variant:getPosition()
	const LuaVariant& variant = tfs::lua::getVariant(L, 1);
	if (variant.isPosition()) {
		tfs::lua::pushPosition(L, variant.getPosition());
	} else if (variant.isTargetPosition()) {
		tfs::lua::pushPosition(L, variant.getTargetPosition());
	} else {
		tfs::lua::pushPosition(L, Position());
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
