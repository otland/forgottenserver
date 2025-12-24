#include "../../otpch.h"

#include "../../house.h"

#include "../../bed.h"
#include "../../game.h"
#include "../../housetile.h"
#include "../../iologindata.h"
#include "../../iomapserialize.h"
#include "../../player.h"
#include "../../town.h"
#include "../api.h"
#include "../meta.h"
#include "../register.h"
#include "../script.h"

extern Game g_game;

namespace {

int luaHouseCreate(lua_State* L)
{
	// House(id)
	const auto& house = g_game.getHouseById(tfs::lua::getNumber<uint32_t>(L, 2));
	if (house) {
		tfs::lua::pushSharedPtr(L, house);
		tfs::lua::setMetatable(L, -1, "House");
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaHouseGetId(lua_State* L)
{
	// house:getId()
	const auto& house = tfs::lua::getSharedPtr<House>(L, 1);
	if (house) {
		tfs::lua::pushNumber(L, house->getId());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaHouseGetName(lua_State* L)
{
	// house:getName()
	const auto& house = tfs::lua::getSharedPtr<House>(L, 1);
	if (house) {
		tfs::lua::pushString(L, house->getName());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaHouseGetTown(lua_State* L)
{
	// house:getTown()
	const auto& house = tfs::lua::getSharedPtr<House>(L, 1);
	if (!house) {
		lua_pushnil(L);
		return 1;
	}

	const Town* town = g_game.map.towns.getTown(house->getTownId());
	if (town) {
		tfs::lua::pushTown(L, *town);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaHouseGetExitPosition(lua_State* L)
{
	// house:getExitPosition()
	const auto& house = tfs::lua::getSharedPtr<House>(L, 1);
	if (house) {
		tfs::lua::pushPosition(L, house->getEntryPosition());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaHouseGetRent(lua_State* L)
{
	// house:getRent()
	const auto& house = tfs::lua::getSharedPtr<House>(L, 1);
	if (house) {
		tfs::lua::pushNumber(L, house->getRent());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaHouseSetRent(lua_State* L)
{
	// house:setRent(rent)
	uint32_t rent = tfs::lua::getNumber<uint32_t>(L, 2);
	const auto& house = tfs::lua::getSharedPtr<House>(L, 1);
	if (house) {
		house->setRent(rent);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaHouseGetPaidUntil(lua_State* L)
{
	// house:getPaidUntil()
	const auto& house = tfs::lua::getSharedPtr<House>(L, 1);
	if (house) {
		tfs::lua::pushNumber(L, house->getPaidUntil());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaHouseSetPaidUntil(lua_State* L)
{
	// house:setPaidUntil(timestamp)
	time_t timestamp = tfs::lua::getNumber<time_t>(L, 2);
	const auto& house = tfs::lua::getSharedPtr<House>(L, 1);
	if (house) {
		house->setPaidUntil(timestamp);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaHouseGetPayRentWarnings(lua_State* L)
{
	// house:getPayRentWarnings()
	const auto& house = tfs::lua::getSharedPtr<House>(L, 1);
	if (house) {
		tfs::lua::pushNumber(L, house->getPayRentWarnings());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaHouseSetPayRentWarnings(lua_State* L)
{
	// house:setPayRentWarnings(warnings)
	uint32_t warnings = tfs::lua::getNumber<uint32_t>(L, 2);
	const auto& house = tfs::lua::getSharedPtr<House>(L, 1);
	if (house) {
		house->setPayRentWarnings(warnings);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaHouseGetOwnerName(lua_State* L)
{
	// house:getOwnerName()
	const auto& house = tfs::lua::getSharedPtr<House>(L, 1);
	if (house) {
		tfs::lua::pushString(L, house->getOwnerName());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaHouseGetOwnerGuid(lua_State* L)
{
	// house:getOwnerGuid()
	const auto& house = tfs::lua::getSharedPtr<House>(L, 1);
	if (house) {
		tfs::lua::pushNumber(L, house->getOwner());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaHouseSetOwnerGuid(lua_State* L)
{
	// house:setOwnerGuid(guid[, updateDatabase = true])
	const auto& house = tfs::lua::getSharedPtr<House>(L, 1);
	if (house) {
		uint32_t guid = tfs::lua::getNumber<uint32_t>(L, 2);
		bool updateDatabase = tfs::lua::getBoolean(L, 3, true);
		house->setOwner(guid, updateDatabase);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaHouseStartTrade(lua_State* L)
{
	// house:startTrade(player, tradePartner)
	const auto& house = tfs::lua::getSharedPtr<House>(L, 1);
	const auto& player = tfs::lua::getSharedPtr<Player>(L, 2);
	const auto& tradePartner = tfs::lua::getSharedPtr<Player>(L, 3);

	if (!player || !tradePartner || !house) {
		lua_pushnil(L);
		return 1;
	}

	if (!tradePartner->getPosition().isInRange(player->getPosition(), 2, 2, 0)) {
		tfs::lua::pushNumber(L, RETURNVALUE_TRADEPLAYERFARAWAY);
		return 1;
	}

	if (house->getOwner() != player->getGUID()) {
		tfs::lua::pushNumber(L, RETURNVALUE_YOUDONTOWNTHISHOUSE);
		return 1;
	}

	if (g_game.getHouseByPlayerId(tradePartner->getGUID())) {
		tfs::lua::pushNumber(L, RETURNVALUE_TRADEPLAYERALREADYOWNSAHOUSE);
		return 1;
	}

	if (IOLoginData::hasBiddedOnHouse(tradePartner->getGUID())) {
		tfs::lua::pushNumber(L, RETURNVALUE_TRADEPLAYERHIGHESTBIDDER);
		return 1;
	}

	const auto& transferItem = house->getTransferItem();
	if (!transferItem) {
		tfs::lua::pushNumber(L, RETURNVALUE_YOUCANNOTTRADETHISHOUSE);
		return 1;
	}

	transferItem->getParent()->setParent(player);
	if (!g_game.internalStartTrade(player, tradePartner, transferItem)) {
		house->resetTransferItem();
	}

	tfs::lua::pushNumber(L, RETURNVALUE_NOERROR);
	return 1;
}

int luaHouseGetBeds(lua_State* L)
{
	// house:getBeds()
	const auto& house = tfs::lua::getSharedPtr<House>(L, 1);
	if (!house) {
		lua_pushnil(L);
		return 1;
	}

	const auto& beds = house->getBeds() | tfs::views::lock_weak_ptrs | std::ranges::to<std::vector>();
	lua_createtable(L, beds.size(), 0);

	int index = 0;
	for (const auto& bedItem : beds) {
		tfs::lua::pushSharedPtr(L, bedItem);
		tfs::lua::setItemMetatable(L, -1, bedItem);
		lua_rawseti(L, -2, ++index);
	}
	return 1;
}

int luaHouseGetBedCount(lua_State* L)
{
	// house:getBedCount()
	const auto& house = tfs::lua::getSharedPtr<House>(L, 1);
	if (house) {
		tfs::lua::pushNumber(L, house->getBedCount());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaHouseGetDoors(lua_State* L)
{
	// house:getDoors()
	const auto& house = tfs::lua::getSharedPtr<House>(L, 1);
	if (!house) {
		lua_pushnil(L);
		return 1;
	}

	const auto& doors = house->getDoors() | tfs::views::lock_weak_ptrs | std::ranges::to<std::vector>();
	lua_createtable(L, doors.size(), 0);

	int index = 0;
	for (const auto& door : doors) {
		tfs::lua::pushSharedPtr(L, door);
		tfs::lua::setItemMetatable(L, -1, door);
		lua_rawseti(L, -2, ++index);
	}
	return 1;
}

int luaHouseGetDoorCount(lua_State* L)
{
	// house:getDoorCount()
	const auto& house = tfs::lua::getSharedPtr<House>(L, 1);
	if (house) {
		tfs::lua::pushNumber(L, house->getDoors().size());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaHouseGetDoorIdByPosition(lua_State* L)
{
	// house:getDoorIdByPosition(position)
	const auto& house = tfs::lua::getSharedPtr<House>(L, 1);
	if (!house) {
		lua_pushnil(L);
		return 1;
	}

	if (const auto& door = house->getDoorByPosition(tfs::lua::getPosition(L, 2))) {
		tfs::lua::pushNumber(L, door->getDoorId());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaHouseGetTiles(lua_State* L)
{
	// house:getTiles()
	const auto& house = tfs::lua::getSharedPtr<House>(L, 1);
	if (!house) {
		lua_pushnil(L);
		return 1;
	}

	const auto& tiles = house->getTiles() | tfs::views::lock_weak_ptrs | std::ranges::to<std::vector>();
	lua_createtable(L, tiles.size(), 0);

	int index = 0;
	for (const auto& tile : tiles) {
		tfs::lua::pushSharedPtr(L, tile);
		tfs::lua::setMetatable(L, -1, "Tile");
		lua_rawseti(L, -2, ++index);
	}
	return 1;
}

int luaHouseGetItems(lua_State* L)
{
	// house:getItems()
	const auto& house = tfs::lua::getSharedPtr<House>(L, 1);
	if (!house) {
		lua_pushnil(L);
		return 1;
	}

	const auto& tiles = house->getTiles();
	lua_newtable(L);

	int index = 0;
	for (const auto& tile : tiles | tfs::views::lock_weak_ptrs) {
		if (TileItemVector* itemVector = tile->getItemList()) {
			for (const auto& item : *itemVector) {
				tfs::lua::pushSharedPtr(L, item);
				tfs::lua::setItemMetatable(L, -1, item);
				lua_rawseti(L, -2, ++index);
			}
		}
	}
	return 1;
}

int luaHouseGetTileCount(lua_State* L)
{
	// house:getTileCount()
	const auto& house = tfs::lua::getSharedPtr<House>(L, 1);
	if (house) {
		tfs::lua::pushNumber(L, house->getTiles().size());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaHouseCanEditAccessList(lua_State* L)
{
	// house:canEditAccessList(listId, player)
	const auto& house = tfs::lua::getSharedPtr<House>(L, 1);
	if (!house) {
		lua_pushnil(L);
		return 1;
	}

	uint32_t listId = tfs::lua::getNumber<uint32_t>(L, 2);
	const auto& player = tfs::lua::getPlayer(L, 3);

	tfs::lua::pushBoolean(L, house->canEditAccessList(listId, player));
	return 1;
}

int luaHouseGetAccessList(lua_State* L)
{
	// house:getAccessList(listId)
	const auto& house = tfs::lua::getSharedPtr<House>(L, 1);
	if (!house) {
		lua_pushnil(L);
		return 1;
	}

	std::string list;
	uint32_t listId = tfs::lua::getNumber<uint32_t>(L, 2);
	if (house->getAccessList(listId, list)) {
		tfs::lua::pushString(L, list);
	} else {
		tfs::lua::pushBoolean(L, false);
	}
	return 1;
}

int luaHouseSetAccessList(lua_State* L)
{
	// house:setAccessList(listId, list)
	const auto& house = tfs::lua::getSharedPtr<House>(L, 1);
	if (!house) {
		lua_pushnil(L);
		return 1;
	}

	uint32_t listId = tfs::lua::getNumber<uint32_t>(L, 2);
	const std::string& list = tfs::lua::getString(L, 3);
	house->setAccessList(listId, list);
	tfs::lua::pushBoolean(L, true);
	return 1;
}

int luaHouseKickPlayer(lua_State* L)
{
	// house:kickPlayer(player, targetPlayer)
	const auto& house = tfs::lua::getSharedPtr<House>(L, 1);
	if (!house) {
		lua_pushnil(L);
		return 1;
	}

	tfs::lua::pushBoolean(L, house->kickPlayer(tfs::lua::getPlayer(L, 2), tfs::lua::getPlayer(L, 3)));
	return 1;
}

int luaHouseSave(lua_State* L)
{
	// house:save()
	const auto& house = tfs::lua::getSharedPtr<House>(L, 1);
	if (!house) {
		lua_pushnil(L);
		return 1;
	}

	tfs::lua::pushBoolean(L, IOMapSerialize::saveHouse(house));
	return 1;
}

} // namespace

void tfs::lua::registerHouse(LuaScriptInterface& lsi)
{
	registerEnum(lsi, GUEST_LIST);
	registerEnum(lsi, SUBOWNER_LIST);

	lsi.registerClass("House", "", luaHouseCreate);
	lsi.registerMetaMethod("House", "__eq", tfs::lua::luaUserdataCompare);

	lsi.registerMethod("House", "getId", luaHouseGetId);
	lsi.registerMethod("House", "getName", luaHouseGetName);
	lsi.registerMethod("House", "getTown", luaHouseGetTown);
	lsi.registerMethod("House", "getExitPosition", luaHouseGetExitPosition);

	lsi.registerMethod("House", "getRent", luaHouseGetRent);
	lsi.registerMethod("House", "setRent", luaHouseSetRent);

	lsi.registerMethod("House", "getPaidUntil", luaHouseGetPaidUntil);
	lsi.registerMethod("House", "setPaidUntil", luaHouseSetPaidUntil);

	lsi.registerMethod("House", "getPayRentWarnings", luaHouseGetPayRentWarnings);
	lsi.registerMethod("House", "setPayRentWarnings", luaHouseSetPayRentWarnings);

	lsi.registerMethod("House", "getOwnerName", luaHouseGetOwnerName);
	lsi.registerMethod("House", "getOwnerGuid", luaHouseGetOwnerGuid);
	lsi.registerMethod("House", "setOwnerGuid", luaHouseSetOwnerGuid);
	lsi.registerMethod("House", "startTrade", luaHouseStartTrade);

	lsi.registerMethod("House", "getBeds", luaHouseGetBeds);
	lsi.registerMethod("House", "getBedCount", luaHouseGetBedCount);

	lsi.registerMethod("House", "getDoors", luaHouseGetDoors);
	lsi.registerMethod("House", "getDoorCount", luaHouseGetDoorCount);
	lsi.registerMethod("House", "getDoorIdByPosition", luaHouseGetDoorIdByPosition);

	lsi.registerMethod("House", "getTiles", luaHouseGetTiles);
	lsi.registerMethod("House", "getItems", luaHouseGetItems);
	lsi.registerMethod("House", "getTileCount", luaHouseGetTileCount);

	lsi.registerMethod("House", "canEditAccessList", luaHouseCanEditAccessList);
	lsi.registerMethod("House", "getAccessList", luaHouseGetAccessList);
	lsi.registerMethod("House", "setAccessList", luaHouseSetAccessList);

	lsi.registerMethod("House", "kickPlayer", luaHouseKickPlayer);

	lsi.registerMethod("House", "save", luaHouseSave);
}
