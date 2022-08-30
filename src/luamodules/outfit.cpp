#include "../otpch.h"

#include "../outfit.h"

#include "../luaapi.h"
#include "../luascript.h"
#include "luaregister.h"

namespace {

using namespace tfs::lua;

int luaOutfitCreate(lua_State* L)
{
	// Outfit(looktype)
	const Outfit* outfit = Outfits::getInstance().getOutfitByLookType(getNumber<uint16_t>(L, 2));
	if (outfit) {
		pushOutfitClass(L, outfit);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaOutfitCompare(lua_State* L)
{
	// outfit == outfitEx
	Outfit outfitEx = getOutfitClass(L, 2);
	Outfit outfit = getOutfitClass(L, 1);
	pushBoolean(L, outfit == outfitEx);
	return 1;
}

} // namespace

void tfs::lua::registerOutfit(LuaScriptInterface& lsi)
{
	lsi.registerClass("Outfit", "", luaOutfitCreate);
	lsi.registerMetaMethod("Outfit", "__eq", luaOutfitCompare);
}
