// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "otpch.h"

#include "container.h"

#include "depotchest.h"
#include "game.h"
#include "housetile.h"
#include "inbox.h"
#include "iomap.h"
#include "spectators.h"
#include "storeinbox.h"

extern Game g_game;

Container::Container(uint16_t type) : Container(type, items[type].maxItems) {}

Container::Container(uint16_t type, uint16_t size, bool unlocked /*= true*/, bool pagination /*= false*/) :
    Item(type), maxSize(size), unlocked(unlocked), pagination(pagination)
{}

Container::Container(Tile* tile) : Container(ITEM_BROWSEFIELD, 30, false, true)
{
	TileItemVector* itemVector = tile->getItemList();
	if (itemVector) {
		for (Item* item : *itemVector) {
			if ((item->getContainer() || item->hasProperty(CONST_PROP_MOVEABLE)) &&
			    !item->hasAttribute(ITEM_ATTRIBUTE_UNIQUEID)) {
				itemlist.push_front(item);
				item->setParent(this);
			}
		}
	}

	setParent(tile);
}

Container::~Container()
{
	if (getID() == ITEM_BROWSEFIELD) {
		g_game.browseFields.erase(getTile());

		for (Item* item : itemlist) {
			item->setParent(parent);
		}
	} else {
		for (Item* item : itemlist) {
			item->setParent(nullptr);
			item->decrementReferenceCounter();
		}
	}
}

Item* Container::clone() const
{
	Container* clone = static_cast<Container*>(Item::clone());
	for (Item* item : itemlist) {
		clone->addItem(item->clone());
	}
	clone->totalWeight = totalWeight;
	return clone;
}

Container* Container::getParentContainer()
{
	Thing* thing = getParent();
	if (!thing) {
		return nullptr;
	}
	return thing->getContainer();
}

std::string Container::getName(bool addArticle /* = false*/) const
{
	const ItemType& it = items[id];
	return getNameDescription(it, this, -1, addArticle);
}

bool Container::hasContainerParent() const
{
	if (getID() == ITEM_BROWSEFIELD) {
		return false;
	}

	if (hasParent()) {
		if (auto creature = getParent()->getCreature()) {
			return !creature->getPlayer();
		}
	}
	return true;
}

void Container::addItem(Item* item)
{
	itemlist.push_back(item);
	item->setParent(this);
}

Attr_ReadValue Container::readAttr(AttrTypes_t attr, PropStream& propStream)
{
	if (attr == ATTR_CONTAINER_ITEMS) {
		if (!propStream.read<uint32_t>(serializationCount)) {
			return ATTR_READ_ERROR;
		}
		return ATTR_READ_END;
	}
	return Item::readAttr(attr, propStream);
}

bool Container::unserializeItemNode(OTB::Loader& loader, const OTB::Node& node, PropStream& propStream)
{
	bool ret = Item::unserializeItemNode(loader, node, propStream);
	if (!ret) {
		return false;
	}

	for (auto& itemNode : node.children) {
		// load container items
		if (itemNode.type != OTBM_ITEM) {
			// unknown type
			return false;
		}

		PropStream itemPropStream;
		if (!loader.getProps(itemNode, itemPropStream)) {
			return false;
		}

		Item* item = Item::CreateItem(itemPropStream);
		if (!item) {
			return false;
		}

		if (!item->unserializeItemNode(loader, itemNode, itemPropStream)) {
			return false;
		}

		addItem(item);
		updateItemWeight(item->getWeight());
	}
	return true;
}

void Container::updateItemWeight(int32_t diff)
{
	totalWeight += diff;
	if (Container* parentContainer = getParentContainer()) {
		parentContainer->updateItemWeight(diff);
	}
}

uint32_t Container::getWeight() const { return Item::getWeight() + totalWeight; }

Item* Container::getItemByIndex(size_t index) const
{
	if (index >= size()) {
		return nullptr;
	}
	return itemlist[index];
}

uint32_t Container::getItemHoldingCount() const
{
	uint32_t counter = 0;
	for (ContainerIterator it = iterator(); it.hasNext(); it.advance()) {
		++counter;
	}
	return counter;
}

bool Container::isHoldingItem(const Item* item) const
{
	for (ContainerIterator it = iterator(); it.hasNext(); it.advance()) {
		if (*it == item) {
			return true;
		}
	}
	return false;
}

void Container::onAddContainerItem(Item* item)
{
	SpectatorVec spectators;
	g_game.map.getSpectators(spectators, getPosition(), false, true, 1, 1, 1, 1);

	// send to client
	for (Creature* spectator : spectators) {
		assert(dynamic_cast<Player*>(spectator) != nullptr);
		static_cast<Player*>(spectator)->sendAddContainerItem(this, item);
	}

	// event methods
	for (Creature* spectator : spectators) {
		assert(dynamic_cast<Player*>(spectator) != nullptr);
		static_cast<Player*>(spectator)->onAddContainerItem(item);
	}
}

void Container::onUpdateContainerItem(uint32_t index, Item* oldItem, Item* newItem)
{
	SpectatorVec spectators;
	g_game.map.getSpectators(spectators, getPosition(), false, true, 1, 1, 1, 1);

	// send to client
	for (Creature* spectator : spectators) {
		assert(dynamic_cast<Player*>(spectator) != nullptr);
		static_cast<Player*>(spectator)->sendUpdateContainerItem(this, index, newItem);
	}

	// event methods
	for (Creature* spectator : spectators) {
		assert(dynamic_cast<Player*>(spectator) != nullptr);
		static_cast<Player*>(spectator)->onUpdateContainerItem(this, oldItem, newItem);
	}
}

void Container::onRemoveContainerItem(uint32_t index, Item* item)
{
	SpectatorVec spectators;
	g_game.map.getSpectators(spectators, getPosition(), false, true, 1, 1, 1, 1);

	// send change to client
	for (Creature* spectator : spectators) {
		assert(dynamic_cast<Player*>(spectator) != nullptr);
		static_cast<Player*>(spectator)->sendRemoveContainerItem(this, index);
	}

	// event methods
	for (Creature* spectator : spectators) {
		assert(dynamic_cast<Player*>(spectator) != nullptr);
		static_cast<Player*>(spectator)->onRemoveContainerItem(this, item);
	}
}

ReturnValue Container::queryAdd(int32_t index, const Thing& thing, uint32_t count, uint32_t flags,
                                Creature* actor /* = nullptr*/) const
{
	bool childIsOwner = hasBitSet(FLAG_CHILDISOWNER, flags);
	if (childIsOwner) {
		// a child container is querying, since we are the top container (not carried by a player) just return with no
		// error.
		return RETURNVALUE_NOERROR;
	}

	if (!unlocked) {
		return RETURNVALUE_NOTPOSSIBLE;
	}

	const Item* item = thing.getItem();
	if (!item) {
		return RETURNVALUE_NOTPOSSIBLE;
	}

	if (!item->isPickupable()) {
		return RETURNVALUE_CANNOTPICKUP;
	}

	if (item == this) {
		return RETURNVALUE_THISISIMPOSSIBLE;
	}

	// quiver: allow ammo only
	if (getWeaponType() == WEAPON_QUIVER && item->getWeaponType() != WEAPON_AMMO) {
		return RETURNVALUE_QUIVERAMMOONLY;
	}

	// store items can be only moved into depot chest or store inbox
	if (item->isStoreItem() && !dynamic_cast<const DepotChest*>(this)) {
		return RETURNVALUE_ITEMCANNOTBEMOVEDTHERE;
	}

	const Cylinder* cylinder = getParent();

	// don't allow moving items into container that is store item and is in store inbox
	if (isStoreItem() && dynamic_cast<const StoreInbox*>(cylinder)) {
		ReturnValue ret = RETURNVALUE_ITEMCANNOTBEMOVEDTHERE;
		if (!item->isStoreItem()) {
			ret = RETURNVALUE_CANNOTMOVEITEMISNOTSTOREITEM;
		}
		return ret;
	}

	if (!hasBitSet(FLAG_NOLIMIT, flags)) {
		while (cylinder) {
			if (cylinder == &thing) {
				return RETURNVALUE_THISISIMPOSSIBLE;
			}

			if (dynamic_cast<const Inbox*>(cylinder)) {
				return RETURNVALUE_CONTAINERNOTENOUGHROOM;
			}

			cylinder = cylinder->getParent();
		}

		if (index == INDEX_WHEREEVER && size() >= capacity() && !hasPagination()) {
			return RETURNVALUE_CONTAINERNOTENOUGHROOM;
		}
	} else {
		while (cylinder) {
			if (cylinder == &thing) {
				return RETURNVALUE_THISISIMPOSSIBLE;
			}

			cylinder = cylinder->getParent();
		}
	}

	const Cylinder* const topParent = getTopParent();
	if (actor && getBoolean(ConfigManager::ONLY_INVITED_CAN_MOVE_HOUSE_ITEMS)) {
		if (const HouseTile* const houseTile = dynamic_cast<const HouseTile*>(topParent->getTile())) {
			if (!topParent->getCreature() && !houseTile->getHouse()->isInvited(actor->getPlayer())) {
				return RETURNVALUE_PLAYERISNOTINVITED;
			}
		}
	}

	if (topParent != this) {
		return topParent->queryAdd(INDEX_WHEREEVER, *item, count, flags | FLAG_CHILDISOWNER, actor);
	}
	return RETURNVALUE_NOERROR;
}

ReturnValue Container::queryMaxCount(int32_t index, const Thing& thing, uint32_t count, uint32_t& maxQueryCount,
                                     uint32_t flags) const
{
	const Item* item = thing.getItem();
	if (!item) {
		maxQueryCount = 0;
		return RETURNVALUE_NOTPOSSIBLE;
	}

	if (hasBitSet(FLAG_NOLIMIT, flags) || hasPagination()) {
		maxQueryCount = std::max<uint32_t>(1, count);
		return RETURNVALUE_NOERROR;
	}

	int32_t freeSlots = std::max<int32_t>(capacity() - size(), 0);

	if (item->isStackable()) {
		uint32_t n = 0;

		if (index == INDEX_WHEREEVER) {
			// Iterate through every item and check how much free stackable slots there is.
			uint32_t slotIndex = 0;
			for (Item* containerItem : itemlist) {
				if (containerItem != item && containerItem->equals(item) &&
				    containerItem->getItemCount() < ITEM_STACK_SIZE) {
					if (queryAdd(slotIndex++, *item, count, flags) == RETURNVALUE_NOERROR) {
						n += ITEM_STACK_SIZE - containerItem->getItemCount();
					}
				}
			}
		} else {
			const Item* destItem = getItemByIndex(index);
			if (item->equals(destItem) && destItem->getItemCount() < ITEM_STACK_SIZE) {
				if (queryAdd(index, *item, count, flags) == RETURNVALUE_NOERROR) {
					n = ITEM_STACK_SIZE - destItem->getItemCount();
				}
			}
		}

		maxQueryCount = freeSlots * ITEM_STACK_SIZE + n;
		if (maxQueryCount < count) {
			return RETURNVALUE_CONTAINERNOTENOUGHROOM;
		}
	} else {
		maxQueryCount = freeSlots;
		if (maxQueryCount == 0) {
			return RETURNVALUE_CONTAINERNOTENOUGHROOM;
		}
	}
	return RETURNVALUE_NOERROR;
}

ReturnValue Container::queryRemove(const Thing& thing, uint32_t count, uint32_t flags,
                                   Creature* actor /*= nullptr */) const
{
	int32_t index = getThingIndex(&thing);
	if (index == -1) {
		return RETURNVALUE_NOTPOSSIBLE;
	}

	const Item* item = thing.getItem();
	if (!item) {
		return RETURNVALUE_NOTPOSSIBLE;
	}

	if (count == 0 || (item->isStackable() && count > item->getItemCount())) {
		return RETURNVALUE_NOTPOSSIBLE;
	}

	if (!item->isMoveable() && !hasBitSet(FLAG_IGNORENOTMOVEABLE, flags)) {
		return RETURNVALUE_NOTMOVEABLE;
	}

	if (actor && getBoolean(ConfigManager::ONLY_INVITED_CAN_MOVE_HOUSE_ITEMS)) {
		const Cylinder* const topParent = getTopParent();
		if (const HouseTile* const houseTile = dynamic_cast<const HouseTile*>(topParent->getTile())) {
			if (!topParent->getCreature() && !houseTile->getHouse()->isInvited(actor->getPlayer())) {
				return RETURNVALUE_PLAYERISNOTINVITED;
			}
		}
	}

	return RETURNVALUE_NOERROR;
}

Cylinder* Container::queryDestination(int32_t& index, const Thing& thing, Item** destItem, uint32_t& flags)
{
	if (!unlocked) {
		*destItem = nullptr;
		return this;
	}

	if (index == 254 /*move up*/) {
		index = INDEX_WHEREEVER;
		*destItem = nullptr;

		Container* parentContainer = dynamic_cast<Container*>(getParent());
		if (parentContainer) {
			return parentContainer;
		}
		return this;
	}

	if (index == 255 /*add wherever*/) {
		index = INDEX_WHEREEVER;
		*destItem = nullptr;
	} else if (index >= static_cast<int32_t>(capacity())) {
		/*
		if you have a container, maximize it to show all 20 slots
		then you open a bag that is inside the container you will have a bag
		with 8 slots and a "grey" area where the other 12 slots where from the
		container if you drop the item on that grey area the client calculates
		the slot position as if the bag has 20 slots
		*/
		index = INDEX_WHEREEVER;
		*destItem = nullptr;
	}

	const Item* item = thing.getItem();
	if (!item) {
		return this;
	}

	if (index != INDEX_WHEREEVER) {
		Item* itemFromIndex = getItemByIndex(index);
		if (itemFromIndex) {
			*destItem = itemFromIndex;
		}

		Cylinder* subCylinder = dynamic_cast<Cylinder*>(*destItem);
		if (subCylinder) {
			index = INDEX_WHEREEVER;
			*destItem = nullptr;
			return subCylinder;
		}
	}

	bool autoStack = !hasBitSet(FLAG_IGNOREAUTOSTACK, flags);
	if (autoStack && item->isStackable() && item->getParent() != this) {
		if (*destItem && (*destItem)->equals(item) && (*destItem)->getItemCount() < ITEM_STACK_SIZE) {
			return this;
		}

		// try find a suitable item to stack with
		uint32_t n = 0;
		for (Item* listItem : itemlist) {
			if (listItem != item && listItem->equals(item) && listItem->getItemCount() < ITEM_STACK_SIZE) {
				*destItem = listItem;
				index = n;
				return this;
			}
			++n;
		}
	}
	return this;
}

void Container::addThing(Thing* thing) { return addThing(0, thing); }

void Container::addThing(int32_t index, Thing* thing)
{
	if (index >= static_cast<int32_t>(capacity())) {
		return /*RETURNVALUE_NOTPOSSIBLE*/;
	}

	Item* item = thing->getItem();
	if (!item) {
		return /*RETURNVALUE_NOTPOSSIBLE*/;
	}

	item->setParent(this);
	itemlist.push_front(item);
	updateItemWeight(item->getWeight());
	ammoCount += item->getItemCount();

	// send change to client
	if (hasParent() && (getParent() != VirtualCylinder::virtualCylinder)) {
		onAddContainerItem(item);
	}
}

void Container::addItemBack(Item* item)
{
	addItem(item);
	updateItemWeight(item->getWeight());
	ammoCount += item->getItemCount();

	// send change to client
	if (hasParent() && (getParent() != VirtualCylinder::virtualCylinder)) {
		onAddContainerItem(item);
	}
}

void Container::updateThing(Thing* thing, uint16_t itemId, uint32_t count)
{
	int32_t index = getThingIndex(thing);
	if (index == -1) {
		return /*RETURNVALUE_NOTPOSSIBLE*/;
	}

	Item* item = thing->getItem();
	if (!item) {
		return /*RETURNVALUE_NOTPOSSIBLE*/;
	}

	ammoCount += count;
	ammoCount -= item->getItemCount();

	const int32_t oldWeight = item->getWeight();
	item->setID(itemId);
	item->setSubType(count);
	updateItemWeight(-oldWeight + item->getWeight());

	// send change to client
	if (hasParent()) {
		onUpdateContainerItem(index, item, item);
	}
}

void Container::replaceThing(uint32_t index, Thing* thing)
{
	Item* item = thing->getItem();
	if (!item) {
		return /*RETURNVALUE_NOTPOSSIBLE*/;
	}

	Item* replacedItem = getItemByIndex(index);
	if (!replacedItem) {
		return /*RETURNVALUE_NOTPOSSIBLE*/;
	}

	ammoCount -= replacedItem->getItemCount();

	itemlist[index] = item;
	item->setParent(this);
	updateItemWeight(-static_cast<int32_t>(replacedItem->getWeight()) + item->getWeight());

	ammoCount += item->getItemCount();

	// send change to client
	if (hasParent()) {
		onUpdateContainerItem(index, replacedItem, item);
	}

	replacedItem->setParent(nullptr);
}

void Container::removeThing(Thing* thing, uint32_t count)
{
	Item* item = thing->getItem();
	if (!item) {
		return /*RETURNVALUE_NOTPOSSIBLE*/;
	}

	int32_t index = getThingIndex(thing);
	if (index == -1) {
		return /*RETURNVALUE_NOTPOSSIBLE*/;
	}

	if (item->isStackable() && count != item->getItemCount()) {
		uint8_t newCount = static_cast<uint8_t>(std::max<int32_t>(0, item->getItemCount() - count));
		const int32_t oldWeight = item->getWeight();

		ammoCount -= (item->getItemCount() - newCount);

		item->setItemCount(newCount);
		updateItemWeight(-oldWeight + item->getWeight());

		// send change to client
		if (hasParent()) {
			onUpdateContainerItem(index, item, item);
		}
	} else {
		updateItemWeight(-static_cast<int32_t>(item->getWeight()));

		ammoCount -= item->getItemCount();

		// send change to client
		if (hasParent()) {
			onRemoveContainerItem(index, item);
		}

		item->setParent(nullptr);
		itemlist.erase(itemlist.begin() + index);
	}
}

int32_t Container::getThingIndex(const Thing* thing) const
{
	int32_t index = 0;
	for (Item* item : itemlist) {
		if (item == thing) {
			return index;
		}
		++index;
	}
	return -1;
}

size_t Container::getFirstIndex() const { return 0; }

size_t Container::getLastIndex() const { return size(); }

uint32_t Container::getItemTypeCount(uint16_t itemId, int32_t subType /* = -1*/) const
{
	uint32_t count = 0;
	for (Item* item : itemlist) {
		if (item->getID() == itemId) {
			count += countByType(item, subType);
		}
	}
	return count;
}

std::map<uint32_t, uint32_t>& Container::getAllItemTypeCount(std::map<uint32_t, uint32_t>& countMap) const
{
	for (Item* item : itemlist) {
		countMap[item->getID()] += item->getItemCount();
	}
	return countMap;
}

ItemVector Container::getItems(bool recursive /*= false*/)
{
	ItemVector containerItems;
	if (recursive) {
		for (ContainerIterator it = iterator(); it.hasNext(); it.advance()) {
			containerItems.push_back(*it);
		}
	} else {
		for (Item* item : itemlist) {
			containerItems.push_back(item);
		}
	}
	return containerItems;
}

Thing* Container::getThing(size_t index) const { return getItemByIndex(index); }

void Container::postAddNotification(Thing* thing, const Cylinder* oldParent, int32_t index, cylinderlink_t)
{
	Cylinder* topParent = getTopParent();
	if (topParent->getCreature()) {
		topParent->postAddNotification(thing, oldParent, index, LINK_TOPPARENT);
	} else if (topParent == this) {
		// let the tile class notify surrounding players
		if (topParent->hasParent()) {
			topParent->getParent()->postAddNotification(thing, oldParent, index, LINK_NEAR);
		}
	} else {
		topParent->postAddNotification(thing, oldParent, index, LINK_PARENT);
	}
}

void Container::postRemoveNotification(Thing* thing, const Cylinder* newParent, int32_t index, cylinderlink_t)
{
	Cylinder* topParent = getTopParent();
	if (topParent->getCreature()) {
		topParent->postRemoveNotification(thing, newParent, index, LINK_TOPPARENT);
	} else if (topParent == this) {
		// let the tile class notify surrounding players
		if (topParent->hasParent()) {
			topParent->getParent()->postRemoveNotification(thing, newParent, index, LINK_NEAR);
		}
	} else {
		topParent->postRemoveNotification(thing, newParent, index, LINK_PARENT);
	}
}

void Container::internalRemoveThing(Thing* thing)
{
	auto cit = std::find(itemlist.begin(), itemlist.end(), thing);
	if (cit == itemlist.end()) {
		return;
	}
	itemlist.erase(cit);
}

void Container::internalAddThing(Thing* thing) { internalAddThing(0, thing); }

void Container::internalAddThing(uint32_t, Thing* thing)
{
	Item* item = thing->getItem();
	if (!item) {
		return;
	}

	item->setParent(this);
	itemlist.push_front(item);
	updateItemWeight(item->getWeight());
	ammoCount += item->getItemCount();
}

void Container::startDecaying()
{
	Item::startDecaying();

	for (Item* item : itemlist) {
		item->startDecaying();
	}
}

ContainerIterator Container::iterator() const
{
	ContainerIterator cit;
	if (!itemlist.empty()) {
		cit.over.push_back(this);
		cit.cur = itemlist.begin();
	}
	return cit;
}

Item* ContainerIterator::operator*() { return *cur; }

void ContainerIterator::advance()
{
	if (Item* i = *cur) {
		if (Container* c = i->getContainer()) {
			if (!c->empty()) {
				over.push_back(c);
			}
		}
	}

	++cur;

	if (cur == over.front()->itemlist.end()) {
		over.pop_front();
		if (!over.empty()) {
			cur = over.front()->itemlist.begin();
		}
	}
}
