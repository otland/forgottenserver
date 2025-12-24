#include "../../otpch.h"

#include "../../tile.h"

#include "../../combat.h"
#include "../../game.h"
#include "../../housetile.h"
#include "../api.h"
#include "../env.h"
#include "../meta.h"
#include "../register.h"
#include "../script.h"

extern Game g_game;

namespace {

int luaTileCreate(lua_State* L)
{
	// Tile(x, y, z)
	// Tile(position)
	std::shared_ptr<Tile> tile;
	if (lua_istable(L, 2)) {
		tile = g_game.map.getTile(tfs::lua::getPosition(L, 2));
	} else {
		uint8_t z = tfs::lua::getNumber<uint8_t>(L, 4);
		uint16_t y = tfs::lua::getNumber<uint16_t>(L, 3);
		uint16_t x = tfs::lua::getNumber<uint16_t>(L, 2);
		tile = g_game.map.getTile(x, y, z);
	}

	if (tile) {
		tfs::lua::pushSharedPtr(L, tile);
		tfs::lua::setMetatable(L, -1, "Tile");
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaTileRemove(lua_State* L)
{
	// tile:remove()
	const auto& tile = tfs::lua::getSharedPtr<Tile>(L, 1);
	if (!tile) {
		lua_pushnil(L);
		return 1;
	}

	if (g_game.isTileInCleanList(tile)) {
		g_game.removeTileToClean(tile);
	}

	g_game.map.removeTile(tile->getPosition());
	tfs::lua::pushBoolean(L, true);
	return 1;
}

int luaTileGetPosition(lua_State* L)
{
	// tile:getPosition()
	if (const auto& tile = tfs::lua::getSharedPtr<Tile>(L, 1)) {
		tfs::lua::pushPosition(L, tile->getPosition());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaTileGetGround(lua_State* L)
{
	// tile:getGround()
	const auto& tile = tfs::lua::getSharedPtr<Tile>(L, 1);
	if (tile && tile->getGround()) {
		tfs::lua::pushSharedPtr(L, tile->getGround());
		tfs::lua::setItemMetatable(L, -1, tile->getGround());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaTileGetThing(lua_State* L)
{
	// tile:getThing(index)
	int32_t index = tfs::lua::getNumber<int32_t>(L, 2);
	const auto& tile = tfs::lua::getSharedPtr<Tile>(L, 1);
	if (!tile) {
		lua_pushnil(L);
		return 1;
	}

	const auto& thing = tile->getThing(index);
	if (!thing) {
		lua_pushnil(L);
		return 1;
	}

	if (const auto& creature = thing->asCreature()) {
		tfs::lua::pushSharedPtr(L, creature);
		tfs::lua::setCreatureMetatable(L, -1, creature);
	} else if (const auto& item = thing->asItem()) {
		tfs::lua::pushSharedPtr(L, item);
		tfs::lua::setItemMetatable(L, -1, item);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaTileGetThingCount(lua_State* L)
{
	// tile:getThingCount()
	if (const auto& tile = tfs::lua::getSharedPtr<Tile>(L, 1)) {
		tfs::lua::pushNumber(L, tile->getThingCount());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaTileGetTopVisibleThing(lua_State* L)
{
	// tile:getTopVisibleThing(creature)
	const auto& creature = tfs::lua::getCreature(L, 2);
	const auto& tile = tfs::lua::getSharedPtr<Tile>(L, 1);
	if (!tile) {
		lua_pushnil(L);
		return 1;
	}

	const auto& thing = tile->getTopVisibleThing(creature);
	if (!thing) {
		lua_pushnil(L);
		return 1;
	}

	if (const auto& visibleCreature = thing->asCreature()) {
		tfs::lua::pushSharedPtr(L, visibleCreature);
		tfs::lua::setCreatureMetatable(L, -1, visibleCreature);
	} else if (const auto& visibleItem = thing->asItem()) {
		tfs::lua::pushSharedPtr(L, visibleItem);
		tfs::lua::setItemMetatable(L, -1, visibleItem);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaTileGetTopTopItem(lua_State* L)
{
	// tile:getTopTopItem()
	const auto& tile = tfs::lua::getSharedPtr<Tile>(L, 1);
	if (!tile) {
		lua_pushnil(L);
		return 1;
	}

	if (const auto& item = tile->getTopTopItem()) {
		tfs::lua::pushSharedPtr(L, item);
		tfs::lua::setItemMetatable(L, -1, item);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaTileGetTopDownItem(lua_State* L)
{
	// tile:getTopDownItem()
	const auto& tile = tfs::lua::getSharedPtr<Tile>(L, 1);
	if (!tile) {
		lua_pushnil(L);
		return 1;
	}

	if (const auto& item = tile->getTopDownItem()) {
		tfs::lua::pushSharedPtr(L, item);
		tfs::lua::setItemMetatable(L, -1, item);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaTileGetFieldItem(lua_State* L)
{
	// tile:getFieldItem()
	const auto& tile = tfs::lua::getSharedPtr<Tile>(L, 1);
	if (!tile) {
		lua_pushnil(L);
		return 1;
	}

	if (const auto& item = tile->getFieldItem()) {
		tfs::lua::pushSharedPtr(L, item);
		tfs::lua::setItemMetatable(L, -1, item);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaTileGetItemById(lua_State* L)
{
	// tile:getItemById(itemId[, subType = -1])
	const auto& tile = tfs::lua::getSharedPtr<Tile>(L, 1);
	if (!tile) {
		lua_pushnil(L);
		return 1;
	}

	uint16_t itemId;
	if (tfs::lua::isNumber(L, 2)) {
		itemId = tfs::lua::getNumber<uint16_t>(L, 2);
	} else {
		itemId = Item::items.getItemIdByName(tfs::lua::getString(L, 2));
		if (itemId == 0) {
			lua_pushnil(L);
			return 1;
		}
	}
	int32_t subType = tfs::lua::getNumber<int32_t>(L, 3, -1);

	if (const auto& item = g_game.findItemOfType(tile, itemId, false, subType)) {
		tfs::lua::pushSharedPtr(L, item);
		tfs::lua::setItemMetatable(L, -1, item);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaTileGetItemByType(lua_State* L)
{
	// tile:getItemByType(itemType)
	const auto& tile = tfs::lua::getSharedPtr<Tile>(L, 1);
	if (!tile) {
		lua_pushnil(L);
		return 1;
	}

	bool found;

	ItemTypes_t itemType = tfs::lua::getNumber<ItemTypes_t>(L, 2);
	switch (itemType) {
		case ITEM_TYPE_TELEPORT:
			found = tile->hasFlag(TILESTATE_TELEPORT);
			break;
		case ITEM_TYPE_MAGICFIELD:
			found = tile->hasFlag(TILESTATE_MAGICFIELD);
			break;
		case ITEM_TYPE_MAILBOX:
			found = tile->hasFlag(TILESTATE_MAILBOX);
			break;
		case ITEM_TYPE_TRASHHOLDER:
			found = tile->hasFlag(TILESTATE_TRASHHOLDER);
			break;
		case ITEM_TYPE_BED:
			found = tile->hasFlag(TILESTATE_BED);
			break;
		case ITEM_TYPE_DEPOT:
			found = tile->hasFlag(TILESTATE_DEPOT);
			break;
		default:
			found = true;
			break;
	}

	if (!found) {
		lua_pushnil(L);
		return 1;
	}

	if (const auto& item = tile->getGround()) {
		const ItemType& it = Item::items[item->getID()];
		if (it.type == itemType) {
			tfs::lua::pushSharedPtr(L, item);
			tfs::lua::setItemMetatable(L, -1, item);
			return 1;
		}
	}

	if (const TileItemVector* items = tile->getItemList()) {
		for (const auto& item : *items) {
			const ItemType& it = Item::items[item->getID()];
			if (it.type == itemType) {
				tfs::lua::pushSharedPtr(L, item);
				tfs::lua::setItemMetatable(L, -1, item);
				return 1;
			}
		}
	}

	lua_pushnil(L);
	return 1;
}

int luaTileGetItemByTopOrder(lua_State* L)
{
	// tile:getItemByTopOrder(topOrder)
	const auto& tile = tfs::lua::getSharedPtr<Tile>(L, 1);
	if (!tile) {
		lua_pushnil(L);
		return 1;
	}

	int32_t topOrder = tfs::lua::getNumber<int32_t>(L, 2);

	const auto& item = tile->getItemByTopOrder(topOrder);
	if (!item) {
		lua_pushnil(L);
		return 1;
	}

	tfs::lua::pushSharedPtr(L, item);
	tfs::lua::setItemMetatable(L, -1, item);
	return 1;
}

int luaTileGetItemCountById(lua_State* L)
{
	// tile:getItemCountById(itemId[, subType = -1])
	const auto& tile = tfs::lua::getSharedPtr<Tile>(L, 1);
	if (!tile) {
		lua_pushnil(L);
		return 1;
	}

	int32_t subType = tfs::lua::getNumber<int32_t>(L, 3, -1);

	uint16_t itemId;
	if (tfs::lua::isNumber(L, 2)) {
		itemId = tfs::lua::getNumber<uint16_t>(L, 2);
	} else {
		itemId = Item::items.getItemIdByName(tfs::lua::getString(L, 2));
		if (itemId == 0) {
			lua_pushnil(L);
			return 1;
		}
	}

	tfs::lua::pushNumber(L, tile->getItemTypeCount(itemId, subType));
	return 1;
}

int luaTileGetBottomCreature(lua_State* L)
{
	// tile:getBottomCreature()
	const auto& tile = tfs::lua::getSharedPtr<Tile>(L, 1);
	if (!tile) {
		lua_pushnil(L);
		return 1;
	}

	const auto& creature = tile->getBottomCreature();
	if (!creature) {
		lua_pushnil(L);
		return 1;
	}

	tfs::lua::pushSharedPtr(L, creature);
	tfs::lua::setCreatureMetatable(L, -1, creature);
	return 1;
}

int luaTileGetTopCreature(lua_State* L)
{
	// tile:getTopCreature()
	const auto& tile = tfs::lua::getSharedPtr<Tile>(L, 1);
	if (!tile) {
		lua_pushnil(L);
		return 1;
	}

	const auto& creature = tile->getTopCreature();
	if (!creature) {
		lua_pushnil(L);
		return 1;
	}

	tfs::lua::pushSharedPtr(L, creature);
	tfs::lua::setCreatureMetatable(L, -1, creature);
	return 1;
}

int luaTileGetBottomVisibleCreature(lua_State* L)
{
	// tile:getBottomVisibleCreature(creature)
	const auto& tile = tfs::lua::getSharedPtr<Tile>(L, 1);
	if (!tile) {
		lua_pushnil(L);
		return 1;
	}

	const auto& creature = tfs::lua::getCreature(L, 2);
	if (!creature) {
		lua_pushnil(L);
		return 1;
	}

	if (const auto& visibleCreature = tile->getBottomVisibleCreature(creature)) {
		tfs::lua::pushSharedPtr(L, visibleCreature);
		tfs::lua::setCreatureMetatable(L, -1, visibleCreature);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaTileGetTopVisibleCreature(lua_State* L)
{
	// tile:getTopVisibleCreature(creature)
	const auto& tile = tfs::lua::getSharedPtr<Tile>(L, 1);
	if (!tile) {
		lua_pushnil(L);
		return 1;
	}

	const auto& creature = tfs::lua::getCreature(L, 2);
	if (!creature) {
		lua_pushnil(L);
		return 1;
	}

	if (const auto& visibleCreature = tile->getTopVisibleCreature(creature)) {
		tfs::lua::pushSharedPtr(L, visibleCreature);
		tfs::lua::setCreatureMetatable(L, -1, visibleCreature);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaTileGetItems(lua_State* L)
{
	// tile:getItems()
	const auto& tile = tfs::lua::getSharedPtr<Tile>(L, 1);
	if (!tile) {
		lua_pushnil(L);
		return 1;
	}

	TileItemVector* itemVector = tile->getItemList();
	if (!itemVector) {
		lua_pushnil(L);
		return 1;
	}

	lua_createtable(L, itemVector->size(), 0);

	int index = 0;
	for (const auto& item : *itemVector) {
		tfs::lua::pushSharedPtr(L, item);
		tfs::lua::setItemMetatable(L, -1, item);
		lua_rawseti(L, -2, ++index);
	}
	return 1;
}

int luaTileGetItemCount(lua_State* L)
{
	// tile:getItemCount()
	const auto& tile = tfs::lua::getSharedPtr<Tile>(L, 1);
	if (!tile) {
		lua_pushnil(L);
		return 1;
	}

	tfs::lua::pushNumber(L, tile->getItemCount());
	return 1;
}

int luaTileGetDownItemCount(lua_State* L)
{
	// tile:getDownItemCount()
	if (const auto& tile = tfs::lua::getSharedPtr<Tile>(L, 1)) {
		tfs::lua::pushNumber(L, tile->getDownItemCount());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaTileGetTopItemCount(lua_State* L)
{
	// tile:getTopItemCount()
	const auto& tile = tfs::lua::getSharedPtr<Tile>(L, 1);
	if (!tile) {
		lua_pushnil(L);
		return 1;
	}

	tfs::lua::pushNumber(L, tile->getTopItemCount());
	return 1;
}

int luaTileGetCreatures(lua_State* L)
{
	// tile:getCreatures()
	const auto& tile = tfs::lua::getSharedPtr<Tile>(L, 1);
	if (!tile) {
		lua_pushnil(L);
		return 1;
	}

	CreatureVector* creatureVector = tile->getCreatures();
	if (!creatureVector) {
		lua_pushnil(L);
		return 1;
	}

	lua_createtable(L, creatureVector->size(), 0);

	int index = 0;
	for (const auto& creature : *creatureVector) {
		tfs::lua::pushSharedPtr(L, creature);
		tfs::lua::setCreatureMetatable(L, -1, creature);
		lua_rawseti(L, -2, ++index);
	}
	return 1;
}

int luaTileGetCreatureCount(lua_State* L)
{
	// tile:getCreatureCount()
	const auto& tile = tfs::lua::getSharedPtr<Tile>(L, 1);
	if (!tile) {
		lua_pushnil(L);
		return 1;
	}

	tfs::lua::pushNumber(L, tile->getCreatureCount());
	return 1;
}

int luaTileHasProperty(lua_State* L)
{
	// tile:hasProperty(property[, item])
	const auto& tile = tfs::lua::getSharedPtr<Tile>(L, 1);
	if (!tile) {
		lua_pushnil(L);
		return 1;
	}

	std::shared_ptr<Item> item = nullptr;
	if (lua_gettop(L) >= 3) {
		item = tfs::lua::getSharedPtr<Item>(L, 3);
	}

	ITEMPROPERTY property = tfs::lua::getNumber<ITEMPROPERTY>(L, 2);
	if (item) {
		tfs::lua::pushBoolean(L, tile->hasProperty(item, property));
	} else {
		tfs::lua::pushBoolean(L, tile->hasProperty(property));
	}
	return 1;
}

int luaTileGetThingIndex(lua_State* L)
{
	// tile:getThingIndex(thing)
	const auto& tile = tfs::lua::getSharedPtr<Tile>(L, 1);
	if (!tile) {
		lua_pushnil(L);
		return 1;
	}

	if (const auto& thing = tfs::lua::getThing(L, 2)) {
		tfs::lua::pushNumber(L, tile->getThingIndex(thing));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaTileHasFlag(lua_State* L)
{
	// tile:hasFlag(flag)
	if (const auto& tile = tfs::lua::getSharedPtr<Tile>(L, 1)) {
		tileflags_t flag = tfs::lua::getNumber<tileflags_t>(L, 2);
		tfs::lua::pushBoolean(L, tile->hasFlag(flag));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaTileQueryAdd(lua_State* L)
{
	// tile:queryAdd(thing[, flags])
	const auto& tile = tfs::lua::getSharedPtr<Tile>(L, 1);
	if (!tile) {
		lua_pushnil(L);
		return 1;
	}

	if (const auto& thing = tfs::lua::getThing(L, 2)) {
		uint32_t flags = tfs::lua::getNumber<uint32_t>(L, 3, 0);
		tfs::lua::pushNumber(L, tile->queryAdd(0, thing, 1, flags));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaTileAddItem(lua_State* L)
{
	// tile:addItem(itemId[, count/subType = 1[, flags = 0]])
	const auto& tile = tfs::lua::getSharedPtr<Tile>(L, 1);
	if (!tile) {
		lua_pushnil(L);
		return 1;
	}

	uint16_t itemId;
	if (tfs::lua::isNumber(L, 2)) {
		itemId = tfs::lua::getNumber<uint16_t>(L, 2);
	} else {
		itemId = Item::items.getItemIdByName(tfs::lua::getString(L, 2));
		if (itemId == 0) {
			lua_pushnil(L);
			return 1;
		}
	}

	const ItemType& it = Item::items[itemId];

	int32_t itemCount = 1;
	int32_t subType = 1;
	uint32_t count = tfs::lua::getNumber<uint32_t>(L, 3, 1);

	if (it.hasSubType()) {
		if (it.stackable) {
			itemCount = std::ceil(count / static_cast<float>(ITEM_STACK_SIZE));
		}

		subType = count;
	} else {
		itemCount = std::max<int32_t>(1, count);
	}

	bool hasTable = itemCount > 1;
	if (hasTable) {
		lua_newtable(L);
	} else if (itemCount == 0) {
		lua_pushnil(L);
		return 1;
	}

	uint32_t flags = tfs::lua::getNumber<uint32_t>(L, 4, 0);

	for (int32_t i = 1; i <= itemCount; ++i) {
		int32_t stackCount = std::min<int32_t>(subType, ITEM_STACK_SIZE);
		const auto& item = Item::CreateItem(itemId, stackCount);
		if (!item) {
			tfs::lua::reportError(L, tfs::lua::getErrorDesc(tfs::lua::LUA_ERROR_ITEM_NOT_FOUND));
			if (!hasTable) {
				lua_pushnil(L);
			}
			return 1;
		}

		if (it.stackable) {
			subType -= stackCount;
		}

		ReturnValue ret = g_game.internalAddItem(tile, item, INDEX_WHEREEVER, flags);
		if (ret != RETURNVALUE_NOERROR) {
			if (!hasTable) {
				lua_pushnil(L);
			}
			return 1;
		}

		if (hasTable) {
			tfs::lua::pushNumber(L, i);
			tfs::lua::pushSharedPtr(L, item);
			tfs::lua::setItemMetatable(L, -1, item);
			lua_settable(L, -3);
		} else {
			tfs::lua::pushSharedPtr(L, item);
			tfs::lua::setItemMetatable(L, -1, item);
		}
	}
	return 1;
}

int luaTileAddItemEx(lua_State* L)
{
	// tile:addItemEx(item[, flags = 0])
	const auto& item = tfs::lua::getSharedPtr<Item>(L, 2);
	if (!item) {
		lua_pushnil(L);
		return 1;
	}

	const auto& tile = tfs::lua::getSharedPtr<Tile>(L, 1);
	if (!tile) {
		lua_pushnil(L);
		return 1;
	}

	if (item->hasParent()) {
		tfs::lua::reportError(L, "Item already has a parent");
		lua_pushnil(L);
		return 1;
	}

	uint32_t flags = tfs::lua::getNumber<uint32_t>(L, 3, 0);
	tfs::lua::pushNumber(L, g_game.internalAddItem(tile, item, INDEX_WHEREEVER, flags));
	return 1;
}

int luaTileGetHouse(lua_State* L)
{
	// tile:getHouse()
	const auto& tile = tfs::lua::getSharedPtr<Tile>(L, 1);
	if (!tile) {
		lua_pushnil(L);
		return 1;
	}

	if (const auto& houseTile = tile->getHouseTile()) {
		tfs::lua::pushSharedPtr(L, houseTile->getHouse());
		tfs::lua::setMetatable(L, -1, "House");
	} else {
		lua_pushnil(L);
	}
	return 1;
}

} // namespace

void tfs::lua::registerTile(LuaScriptInterface& lsi)
{
	registerEnum(lsi, TILESTATE_NONE);
	registerEnum(lsi, TILESTATE_PROTECTIONZONE);
	registerEnum(lsi, TILESTATE_NOPVPZONE);
	registerEnum(lsi, TILESTATE_NOLOGOUT);
	registerEnum(lsi, TILESTATE_PVPZONE);
	registerEnum(lsi, TILESTATE_FLOORCHANGE);
	registerEnum(lsi, TILESTATE_FLOORCHANGE_DOWN);
	registerEnum(lsi, TILESTATE_FLOORCHANGE_NORTH);
	registerEnum(lsi, TILESTATE_FLOORCHANGE_SOUTH);
	registerEnum(lsi, TILESTATE_FLOORCHANGE_EAST);
	registerEnum(lsi, TILESTATE_FLOORCHANGE_WEST);
	registerEnum(lsi, TILESTATE_TELEPORT);
	registerEnum(lsi, TILESTATE_MAGICFIELD);
	registerEnum(lsi, TILESTATE_MAILBOX);
	registerEnum(lsi, TILESTATE_TRASHHOLDER);
	registerEnum(lsi, TILESTATE_BED);
	registerEnum(lsi, TILESTATE_DEPOT);
	registerEnum(lsi, TILESTATE_BLOCKSOLID);
	registerEnum(lsi, TILESTATE_BLOCKPATH);
	registerEnum(lsi, TILESTATE_IMMOVABLEBLOCKSOLID);
	registerEnum(lsi, TILESTATE_IMMOVABLEBLOCKPATH);
	registerEnum(lsi, TILESTATE_IMMOVABLENOFIELDBLOCKPATH);
	registerEnum(lsi, TILESTATE_NOFIELDBLOCKPATH);
	registerEnum(lsi, TILESTATE_FLOORCHANGE_SOUTH_ALT);
	registerEnum(lsi, TILESTATE_FLOORCHANGE_EAST_ALT);
	registerEnum(lsi, TILESTATE_SUPPORTS_HANGABLE);

	registerEnum(lsi, ZONE_PROTECTION);
	registerEnum(lsi, ZONE_NOPVP);
	registerEnum(lsi, ZONE_PVP);
	registerEnum(lsi, ZONE_NOLOGOUT);
	registerEnum(lsi, ZONE_NORMAL);

	lsi.registerClass("Tile", "", luaTileCreate);
	lsi.registerMetaMethod("Tile", "__eq", tfs::lua::luaUserdataCompare);

	lsi.registerMethod("Tile", "remove", luaTileRemove);

	lsi.registerMethod("Tile", "getPosition", luaTileGetPosition);
	lsi.registerMethod("Tile", "getGround", luaTileGetGround);
	lsi.registerMethod("Tile", "getThing", luaTileGetThing);
	lsi.registerMethod("Tile", "getThingCount", luaTileGetThingCount);
	lsi.registerMethod("Tile", "getTopVisibleThing", luaTileGetTopVisibleThing);

	lsi.registerMethod("Tile", "getTopTopItem", luaTileGetTopTopItem);
	lsi.registerMethod("Tile", "getTopDownItem", luaTileGetTopDownItem);
	lsi.registerMethod("Tile", "getFieldItem", luaTileGetFieldItem);

	lsi.registerMethod("Tile", "getItemById", luaTileGetItemById);
	lsi.registerMethod("Tile", "getItemByType", luaTileGetItemByType);
	lsi.registerMethod("Tile", "getItemByTopOrder", luaTileGetItemByTopOrder);
	lsi.registerMethod("Tile", "getItemCountById", luaTileGetItemCountById);

	lsi.registerMethod("Tile", "getBottomCreature", luaTileGetBottomCreature);
	lsi.registerMethod("Tile", "getTopCreature", luaTileGetTopCreature);
	lsi.registerMethod("Tile", "getBottomVisibleCreature", luaTileGetBottomVisibleCreature);
	lsi.registerMethod("Tile", "getTopVisibleCreature", luaTileGetTopVisibleCreature);

	lsi.registerMethod("Tile", "getItems", luaTileGetItems);
	lsi.registerMethod("Tile", "getItemCount", luaTileGetItemCount);
	lsi.registerMethod("Tile", "getDownItemCount", luaTileGetDownItemCount);
	lsi.registerMethod("Tile", "getTopItemCount", luaTileGetTopItemCount);

	lsi.registerMethod("Tile", "getCreatures", luaTileGetCreatures);
	lsi.registerMethod("Tile", "getCreatureCount", luaTileGetCreatureCount);

	lsi.registerMethod("Tile", "getThingIndex", luaTileGetThingIndex);

	lsi.registerMethod("Tile", "hasProperty", luaTileHasProperty);
	lsi.registerMethod("Tile", "hasFlag", luaTileHasFlag);

	lsi.registerMethod("Tile", "queryAdd", luaTileQueryAdd);
	lsi.registerMethod("Tile", "addItem", luaTileAddItem);
	lsi.registerMethod("Tile", "addItemEx", luaTileAddItemEx);

	lsi.registerMethod("Tile", "getHouse", luaTileGetHouse);
}
