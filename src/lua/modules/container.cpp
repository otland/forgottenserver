#include "../../otpch.h"

#include "../../container.h"

#include "../../game.h"
#include "../api.h"
#include "../env.h"
#include "../meta.h"
#include "../register.h"
#include "../script.h"

extern Game g_game;

namespace {

int luaGetDepotId(lua_State* L)
{
	// getDepotId(uid)
	uint32_t uid = tfs::lua::getNumber<uint32_t>(L, -1);

	const auto& container = tfs::lua::getScriptEnv()->getContainerByUID(uid);
	if (!container) {
		tfs::lua::reportError(L, tfs::lua::getErrorDesc(tfs::lua::LUA_ERROR_CONTAINER_NOT_FOUND));
		tfs::lua::pushBoolean(L, false);
		return 1;
	}

	const auto& depotLocker = container->getDepotLocker();
	if (!depotLocker) {
		tfs::lua::reportError(L, "Depot not found");
		tfs::lua::pushBoolean(L, false);
		return 1;
	}

	tfs::lua::pushNumber(L, depotLocker->getDepotId());
	return 1;
}

int luaIsDepot(lua_State* L)
{
	// isDepot(uid)
	const auto& container = tfs::lua::getScriptEnv()->getContainerByUID(tfs::lua::getNumber<uint32_t>(L, -1));
	tfs::lua::pushBoolean(L, container && container->getDepotLocker());
	return 1;
}

int luaContainerCreate(lua_State* L)
{
	// Container(uid)
	uint32_t id = tfs::lua::getNumber<uint32_t>(L, 2);

	if (const auto& container = tfs::lua::getScriptEnv()->getContainerByUID(id)) {
		tfs::lua::pushSharedPtr(L, container);
		tfs::lua::setMetatable(L, -1, "Container");
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaContainerGetSize(lua_State* L)
{
	// container:getSize()
	if (const auto& container = tfs::lua::getSharedPtr<Container>(L, 1)) {
		tfs::lua::pushNumber(L, container->size());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaContainerGetCapacity(lua_State* L)
{
	// container:getCapacity()
	if (const auto& container = tfs::lua::getSharedPtr<Container>(L, 1)) {
		tfs::lua::pushNumber(L, container->capacity());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaContainerGetEmptySlots(lua_State* L)
{
	// container:getEmptySlots([recursive = false])
	const auto& container = tfs::lua::getSharedPtr<Container>(L, 1);
	if (!container) {
		lua_pushnil(L);
		return 1;
	}

	uint32_t slots = container->capacity() - container->size();
	bool recursive = tfs::lua::getBoolean(L, 2, false);
	if (recursive) {
		for (ContainerIterator it = container->iterator(); it.hasNext(); it.advance()) {
			if (const auto& tmpContainer = (*it)->getContainer()) {
				slots += tmpContainer->capacity() - tmpContainer->size();
			}
		}
	}
	tfs::lua::pushNumber(L, slots);
	return 1;
}

int luaContainerGetItemHoldingCount(lua_State* L)
{
	// container:getItemHoldingCount()
	if (const auto& container = tfs::lua::getSharedPtr<Container>(L, 1)) {
		tfs::lua::pushNumber(L, container->getItemHoldingCount());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaContainerGetItem(lua_State* L)
{
	// container:getItem(index)
	const auto& container = tfs::lua::getSharedPtr<Container>(L, 1);
	if (!container) {
		lua_pushnil(L);
		return 1;
	}

	uint32_t index = tfs::lua::getNumber<uint32_t>(L, 2);
	if (const auto& item = container->getItemByIndex(index)) {
		tfs::lua::pushSharedPtr(L, item);
		tfs::lua::setItemMetatable(L, -1, item);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaContainerHasItem(lua_State* L)
{
	// container:hasItem(item)
	const auto& item = tfs::lua::getSharedPtr<Item>(L, 2);
	if (const auto& container = tfs::lua::getSharedPtr<Container>(L, 1)) {
		tfs::lua::pushBoolean(L, container->isHoldingItem(item));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaContainerAddItem(lua_State* L)
{
	// container:addItem(itemId[, count/subType = 1[, index = INDEX_WHEREEVER[, flags = 0]]])
	const auto& container = tfs::lua::getSharedPtr<Container>(L, 1);
	if (!container) {
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

	int32_t index = tfs::lua::getNumber<int32_t>(L, 4, INDEX_WHEREEVER);
	uint32_t flags = tfs::lua::getNumber<uint32_t>(L, 5, 0);

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

		ReturnValue ret = g_game.internalAddItem(container, item, index, flags);
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

int luaContainerAddItemEx(lua_State* L)
{
	// container:addItemEx(item[, index = INDEX_WHEREEVER[, flags = 0]])
	const auto& item = tfs::lua::getSharedPtr<Item>(L, 2);
	if (!item) {
		lua_pushnil(L);
		return 1;
	}

	const auto& container = tfs::lua::getSharedPtr<Container>(L, 1);
	if (!container) {
		lua_pushnil(L);
		return 1;
	}

	if (item->hasParent()) {
		tfs::lua::reportError(L, "Item already has a parent");
		lua_pushnil(L);
		return 1;
	}

	int32_t index = tfs::lua::getNumber<int32_t>(L, 3, INDEX_WHEREEVER);
	uint32_t flags = tfs::lua::getNumber<uint32_t>(L, 4, 0);
	tfs::lua::pushNumber(L, g_game.internalAddItem(container, item, index, flags));
	return 1;
}

int luaContainerGetCorpseOwner(lua_State* L)
{
	// container:getCorpseOwner()
	if (const auto& container = tfs::lua::getSharedPtr<Container>(L, 1)) {
		tfs::lua::pushNumber(L, container->getCorpseOwner());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaContainerGetItemCountById(lua_State* L)
{
	// container:getItemCountById(itemId[, subType = -1])
	const auto& container = tfs::lua::getSharedPtr<Container>(L, 1);
	if (!container) {
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
	tfs::lua::pushNumber(L, container->getItemTypeCount(itemId, subType));
	return 1;
}

int luaContainerGetItems(lua_State* L)
{
	// container:getItems([recursive = false])
	const auto& container = tfs::lua::getSharedPtr<Container>(L, 1);
	if (!container) {
		lua_pushnil(L);
		return 1;
	}

	bool recursive = tfs::lua::getBoolean(L, 2, false);
	const auto& items = container->getItems(recursive);

	lua_createtable(L, items.size(), 0);

	int index = 0;
	for (const auto& item : items) {
		tfs::lua::pushSharedPtr(L, item);
		tfs::lua::setItemMetatable(L, -1, item);
		lua_rawseti(L, -2, ++index);
	}
	return 1;
}

} // namespace

void tfs::lua::registerContainer(LuaScriptInterface& lsi)
{
	lsi.registerGlobalVariable("INDEX_WHEREEVER", INDEX_WHEREEVER);

	registerEnum(lsi, FLAG_NOLIMIT);
	registerEnum(lsi, FLAG_IGNOREBLOCKITEM);
	registerEnum(lsi, FLAG_IGNOREBLOCKCREATURE);
	registerEnum(lsi, FLAG_CHILDISOWNER);
	registerEnum(lsi, FLAG_PATHFINDING);
	registerEnum(lsi, FLAG_IGNOREFIELDDAMAGE);
	registerEnum(lsi, FLAG_IGNORENOTMOVEABLE);
	registerEnum(lsi, FLAG_IGNOREAUTOSTACK);

	lua_register(lsi.getLuaState(), "getDepotId", luaGetDepotId);

	lua_register(lsi.getLuaState(), "isDepot", luaIsDepot);

	lsi.registerClass("Container", "Item", luaContainerCreate);
	lsi.registerMetaMethod("Container", "__eq", tfs::lua::luaUserdataCompare);
	lsi.registerMetaMethod("Container", "__gc", tfs::lua::luaSharedPtrDelete<Container>);

	lsi.registerMethod("Container", "getSize", luaContainerGetSize);
	lsi.registerMethod("Container", "getCapacity", luaContainerGetCapacity);
	lsi.registerMethod("Container", "getEmptySlots", luaContainerGetEmptySlots);
	lsi.registerMethod("Container", "getItems", luaContainerGetItems);
	lsi.registerMethod("Container", "getItemHoldingCount", luaContainerGetItemHoldingCount);
	lsi.registerMethod("Container", "getItemCountById", luaContainerGetItemCountById);

	lsi.registerMethod("Container", "getItem", luaContainerGetItem);
	lsi.registerMethod("Container", "hasItem", luaContainerHasItem);
	lsi.registerMethod("Container", "addItem", luaContainerAddItem);
	lsi.registerMethod("Container", "addItemEx", luaContainerAddItemEx);
	lsi.registerMethod("Container", "getCorpseOwner", luaContainerGetCorpseOwner);
}
