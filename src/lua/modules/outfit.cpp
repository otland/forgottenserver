#include "../../otpch.h"

#include "../../outfit.h"

#include "../api.h"
#include "../register.h"
#include "../script.h"

namespace {

int luaOutfitCreate(lua_State* L)
{
	// Outfit(looktype)
	const Outfit* outfit = Outfits::getInstance().getOutfitByLookType(tfs::lua::getNumber<uint16_t>(L, 2));
	if (outfit) {
		tfs::lua::pushOutfit(L, outfit);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaOutfitCompare(lua_State* L)
{
	// outfit == outfitEx
	Outfit outfitEx = tfs::lua::getOutfitClass(L, 2);
	Outfit outfit = tfs::lua::getOutfitClass(L, 1);
	tfs::lua::pushBoolean(L, outfit == outfitEx);
	return 1;
}

} // namespace

void tfs::lua::registerOutfit(LuaScriptInterface& lsi)
{
	lsi.registerClass("Outfit", "", luaOutfitCreate);
	lsi.registerMetaMethod("Outfit", "__eq", luaOutfitCompare);
}
