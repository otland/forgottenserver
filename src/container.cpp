// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "otpch.h"

#include "container.h"

#include "depotchest.h"
#include "game.h"
#include "housetile.h"
#include "inbox.h"
#include "iomap.h"
#include "storeinbox.h"

extern Game g_game;

Container::~Container()
{
	if (getID() == ITEM_BROWSEFIELD) {
		const auto& parent = getParent();
		for (const auto& item : itemList) {
			item->setParent(parent);
		}
	}
}

std::shared_ptr<Item> Container::clone() const
{
	const auto clone = std::static_pointer_cast<Container>(Item::clone());
	if (!clone) {
		return nullptr;
	}

	// Clone all items inside this container
	for (const auto& item : itemList) {
		clone->addItem(item->clone());
	}

	// Preserve the total weight of the container
	clone->totalWeight = totalWeight;

	return clone;
}

std::string Container::getName(bool addArticle /* = false*/) const
{
	const ItemType& it = items[id];
	return getNameDescription(it, getContainer(), -1, addArticle);
}

bool Container::hasContainerParent() const
{
	if (getID() == ITEM_BROWSEFIELD) {
		return false;
	}

	if (hasParent()) {
		if (const auto& creature = getParent()->asCreature()) {
			return !creature->asPlayer();
		}
	}
	return true;
}

void Container::addItem(std::shared_ptr<Item> item)
{
	item->setParent(getContainer());
	itemList.push_back(std::move(item));
}

void Container::readAttr(AttrTypes_t attr, OTB::iterator& first, const OTB::iterator& last)
{
	if (attr == ATTR_CONTAINER_ITEMS) {
		serializationCount = OTB::read<uint32_t>(first, last);
		return;
	}

	Item::readAttr(attr, first, last);
}

void Container::unserializeItemNode(OTB::iterator& first, const OTB::iterator& last, const OTB::Node& node)
{
	Item::unserializeItemNode(first, last, node);

	for (const auto& itemNode : node.children) {
		// load container items
		if (itemNode.type != tfs::io::map::OTBM_ITEM) [[unlikely]] {
			throw std::invalid_argument("Invalid node type");
		}

		auto it = itemNode.propsBegin;
		auto id = OTB::read<uint16_t>(it, itemNode.propsEnd);

		auto item = Item::CreateItem(Item::getPersistentId(id));
		if (!item) [[unlikely]] {
			throw std::runtime_error(std::format("[{:s}] Invalid item type: {:d}", __FUNCTION__, id));
		}

		item->unserializeItemNode(it, itemNode.propsEnd, itemNode);

		addItem(item);
		updateItemWeight(item->getWeight());
	}
}

void Container::updateItemWeight(int32_t diff)
{
	totalWeight += diff;
	if (const auto& parentContainer = std::dynamic_pointer_cast<Container>(getParent())) {
		parentContainer->updateItemWeight(diff);
	}
}

uint32_t Container::getWeight() const { return Item::getWeight() + totalWeight; }

std::shared_ptr<Item> Container::getItemByIndex(size_t index) const
{
	if (index >= size()) {
		return nullptr;
	}
	return itemList[index];
}

uint32_t Container::getItemHoldingCount() const
{
	uint32_t itemCount = 0;
	for (ContainerIterator it = iterator(); it.hasNext(); it.advance()) {
		++itemCount;
	}
	return itemCount;
}

bool Container::isHoldingItem(const std::shared_ptr<const Item>& item) const
{
	for (ContainerIterator it = iterator(); it.hasNext(); it.advance()) {
		if (*it == item) {
			return true;
		}
	}
	return false;
}

void Container::onAddContainerItem(const std::shared_ptr<Item>& item)
{
	SpectatorVec spectators;
	g_game.map.getSpectators(spectators, getPosition(), false, true, 1, 1, 1, 1);

	// send to client
	for (const auto& spectator : spectators) {
		assert(spectator->asPlayer() != nullptr);
		std::static_pointer_cast<Player>(spectator)->sendAddContainerItem(getContainer(), item);
	}

	// event methods
	for (const auto& spectator : spectators) {
		assert(spectator->asPlayer() != nullptr);
		std::static_pointer_cast<Player>(spectator)->onAddContainerItem(item);
	}
}

void Container::onUpdateContainerItem(uint32_t index, const std::shared_ptr<Item>& oldItem,
                                      const std::shared_ptr<Item>& newItem)
{
	SpectatorVec spectators;
	g_game.map.getSpectators(spectators, getPosition(), false, true, 1, 1, 1, 1);

	// send to client
	for (const auto& spectator : spectators) {
		assert(spectator->asPlayer() != nullptr);
		std::static_pointer_cast<Player>(spectator)->sendUpdateContainerItem(getContainer(), index, newItem);
	}

	// event methods
	for (const auto& spectator : spectators) {
		assert(spectator->asPlayer() != nullptr);
		std::static_pointer_cast<Player>(spectator)->onUpdateContainerItem(getContainer(), oldItem, newItem);
	}
}

void Container::onRemoveContainerItem(uint32_t index, const std::shared_ptr<Item>& item)
{
	SpectatorVec spectators;
	g_game.map.getSpectators(spectators, getPosition(), false, true, 1, 1, 1, 1);

	// send change to client
	for (const auto& spectator : spectators) {
		assert(spectator->asPlayer() != nullptr);
		std::static_pointer_cast<Player>(spectator)->sendRemoveContainerItem(getContainer(), index);
	}

	// event methods
	for (const auto& spectator : spectators) {
		assert(spectator->asPlayer() != nullptr);
		std::static_pointer_cast<Player>(spectator)->onRemoveContainerItem(getContainer(), item);
	}
}

ReturnValue Container::queryAdd(int32_t index, const std::shared_ptr<const Thing>& thing, uint32_t count,
                                uint32_t flags, const std::shared_ptr<Creature>& actor /* = nullptr*/) const
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

	const auto& item = thing->asItem();
	if (!item) {
		return RETURNVALUE_NOTPOSSIBLE;
	}

	if (!item->isPickupable()) {
		return RETURNVALUE_CANNOTPICKUP;
	}

	if (item.get() == this) {
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

	if (auto parent = getParent()) {
		// don't allow moving items into container that is store item and is in store inbox
		if (isStoreItem() && std::dynamic_pointer_cast<const StoreInbox>(parent)) {
			ReturnValue ret = RETURNVALUE_ITEMCANNOTBEMOVEDTHERE;
			if (!item->isStoreItem()) {
				ret = RETURNVALUE_CANNOTMOVEITEMISNOTSTOREITEM;
			}
			return ret;
		}

		if (hasBitSet(FLAG_NOLIMIT, flags)) {
			while (parent) {
				if (parent == thing) {
					return RETURNVALUE_THISISIMPOSSIBLE;
				}

				parent = parent->getParent();
			}
		} else {
			while (parent) {
				if (parent == thing) {
					return RETURNVALUE_THISISIMPOSSIBLE;
				}

				if (std::dynamic_pointer_cast<const Inbox>(parent)) {
					return RETURNVALUE_CONTAINERNOTENOUGHROOM;
				}

				parent = parent->getParent();
			}

			if (index == INDEX_WHEREEVER && size() >= capacity() && !hasPagination()) {
				return RETURNVALUE_CONTAINERNOTENOUGHROOM;
			}
		}
	}

	const auto& topParent = getTopParent();
	if (actor && getBoolean(ConfigManager::ONLY_INVITED_CAN_MOVE_HOUSE_ITEMS)) {
		if (const auto& tile = topParent->getTile()) {
			if (const auto& houseTile = tile->getHouseTile()) {
				if (!topParent->asCreature() && !houseTile->getHouse()->isInvited(actor->asPlayer())) {
					return RETURNVALUE_PLAYERISNOTINVITED;
				}
			}
		}
	}

	if (topParent.get() != this) {
		return topParent->queryAdd(INDEX_WHEREEVER, item, count, flags | FLAG_CHILDISOWNER, actor);
	}

	return RETURNVALUE_NOERROR;
}

ReturnValue Container::queryMaxCount(int32_t index, const std::shared_ptr<const Thing>& thing, uint32_t count,
                                     uint32_t& maxQueryCount, uint32_t flags) const
{
	const auto& item = thing->asItem();
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
		uint32_t availableStackSpace = 0;

		if (index == INDEX_WHEREEVER) {
			// Iterate through every item and check how much free stackable slots there is.
			uint32_t slotIndex = 0;
			for (const auto& containerItem : itemList) {
				if (containerItem && containerItem != item && *containerItem == *item &&
				    containerItem->getItemCount() < ITEM_STACK_SIZE) {
					if (queryAdd(slotIndex++, item, count, flags) == RETURNVALUE_NOERROR) {
						availableStackSpace += ITEM_STACK_SIZE - containerItem->getItemCount();
					}
				}
			}
		} else if (const auto& destItem = getItemByIndex(index);
		           destItem && *destItem == *item && destItem->getItemCount() < ITEM_STACK_SIZE) {
			if (queryAdd(index, item, count, flags) == RETURNVALUE_NOERROR) {
				availableStackSpace = ITEM_STACK_SIZE - destItem->getItemCount();
			}
		}

		maxQueryCount = freeSlots * ITEM_STACK_SIZE + availableStackSpace;
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

ReturnValue Container::queryRemove(const std::shared_ptr<const Thing>& thing, uint32_t count, uint32_t flags,
                                   const std::shared_ptr<Creature>& actor /*= nullptr */) const
{
	int32_t index = getThingIndex(thing);
	if (index == -1) {
		return RETURNVALUE_NOTPOSSIBLE;
	}

	const auto& item = thing->asItem();
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
		const auto& topParent = getTopParent();
		if (const auto& tile = topParent->getTile()) {
			if (const auto& houseTile = tile->getHouseTile()) {
				if (!topParent->asCreature() && !houseTile->getHouse()->isInvited(actor->asPlayer())) {
					return RETURNVALUE_PLAYERISNOTINVITED;
				}
			}
		}
	}

	return RETURNVALUE_NOERROR;
}

std::shared_ptr<Thing> Container::queryDestination(int32_t& index, const std::shared_ptr<const Thing>& thing,
                                                   std::shared_ptr<Item>& destItem, uint32_t& flags)
{
	if (!unlocked) {
		destItem = nullptr;
		return shared_from_this();
	}

	if (index == 254 /*move up*/) {
		index = INDEX_WHEREEVER;
		destItem = nullptr;

		if (const auto& parentContainer = std::dynamic_pointer_cast<Container>(getParent())) {
			return parentContainer;
		}
		return shared_from_this();
	}

	if (index == 255 /*add wherever*/) {
		index = INDEX_WHEREEVER;
		destItem = nullptr;
	} else if (index >= static_cast<int32_t>(capacity())) {
		/*
		if you have a container, maximize it to show all 20 slots
		then you open a bag that is inside the container you will have a bag
		with 8 slots and a "grey" area where the other 12 slots where from the
		container if you drop the item on that grey area the client calculates
		the slot position as if the bag has 20 slots
		*/
		index = INDEX_WHEREEVER;
		destItem = nullptr;
	}

	const auto& item = thing->asItem();
	if (!item) {
		return shared_from_this();
	}

	if (index != INDEX_WHEREEVER) {
		if (const auto& itemFromIndex = getItemByIndex(index)) {
			if (const auto& receiver = itemFromIndex->getReceiver()) {
				index = INDEX_WHEREEVER;
				destItem = nullptr;
				return receiver;
			}

			destItem = itemFromIndex;
		}
	}

	bool autoStack = !hasBitSet(FLAG_IGNOREAUTOSTACK, flags);
	if (autoStack && item->isStackable() && item->getParent().get() != this) {
		if (destItem && *destItem == *item && destItem->getItemCount() < ITEM_STACK_SIZE) {
			return shared_from_this();
		}

		// try find a suitable item to stack with
		uint32_t slotIndex = 0;
		for (const auto& listItem : itemList) {
			if (listItem != item && *listItem == *item && listItem->getItemCount() < ITEM_STACK_SIZE) {
				destItem = listItem;
				index = slotIndex;
				return shared_from_this();
			}
			++slotIndex;
		}
	}
	return shared_from_this();
}

void Container::addThing(int32_t index, const std::shared_ptr<Thing>& thing)
{
	if (index >= static_cast<int32_t>(capacity())) {
		return /*RETURNVALUE_NOTPOSSIBLE*/;
	}

	const auto& item = thing->asItem();
	if (!item) {
		return /*RETURNVALUE_NOTPOSSIBLE*/;
	}

	item->setParent(getContainer());
	itemList.push_front(item);
	updateItemWeight(item->getWeight());
	ammoCount += item->getItemCount();

	// send change to client
	if (hasParent()) {
		onAddContainerItem(item);
	}
}

void Container::addItemBack(const std::shared_ptr<Item>& item)
{
	addItem(item);
	updateItemWeight(item->getWeight());
	ammoCount += item->getItemCount();

	// send change to client
	if (hasParent()) {
		onAddContainerItem(item);
	}
}

void Container::updateThing(const std::shared_ptr<Thing>& thing, uint16_t itemId, uint32_t count)
{
	int32_t index = getThingIndex(thing);
	if (index == -1) {
		return /*RETURNVALUE_NOTPOSSIBLE*/;
	}

	const auto& item = thing->asItem();
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

void Container::replaceThing(uint32_t index, const std::shared_ptr<Thing>& thing)
{
	if (!thing) {
		return;
	}

	const auto& item = thing->asItem();
	if (!item) {
		return /*RETURNVALUE_NOTPOSSIBLE*/;
	}

	const auto& replacedItem = getItemByIndex(index);
	if (!replacedItem) {
		return /*RETURNVALUE_NOTPOSSIBLE*/;
	}

	ammoCount -= replacedItem->getItemCount();

	itemList[index] = item;
	item->setParent(getContainer());
	updateItemWeight(-static_cast<int32_t>(replacedItem->getWeight()) + item->getWeight());

	ammoCount += item->getItemCount();

	// send change to client
	if (hasParent()) {
		onUpdateContainerItem(index, replacedItem, item);
	}

	replacedItem->setParent(nullptr);
}

void Container::removeThing(const std::shared_ptr<Thing>& thing, uint32_t count)
{
	if (!thing) {
		return;
	}

	const auto& item = thing->asItem();
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
		itemList.erase(itemList.begin() + index);
	}
}

int32_t Container::getThingIndex(const std::shared_ptr<const Thing>& thing) const
{
	int32_t itemIndex = 0;
	for (const auto& item : itemList) {
		if (item == thing) {
			return itemIndex;
		}
		++itemIndex;
	}
	return -1;
}

uint32_t Container::getItemTypeCount(uint16_t itemId, int32_t subType /* = -1*/) const
{
	uint32_t count = 0;
	for (const auto& item : itemList) {
		if (item->getID() == itemId) {
			count += countByType(item, subType);
		}
	}
	return count;
}

std::map<uint32_t, uint32_t>& Container::getAllItemTypeCount(std::map<uint32_t, uint32_t>& countMap) const
{
	for (const auto& item : itemList) {
		countMap[item->getID()] += item->getItemCount();
	}
	return countMap;
}

std::vector<std::shared_ptr<Item>> Container::getItems(bool recursive /*= false*/)
{
	std::vector<std::shared_ptr<Item>> containerItems;
	if (recursive) {
		for (ContainerIterator it = iterator(); it.hasNext(); it.advance()) {
			containerItems.push_back(*it);
		}
	} else {
		for (const auto& item : itemList) {
			containerItems.push_back(item);
		}
	}
	return containerItems;
}

void Container::postAddNotification(const std::shared_ptr<Thing>& thing, const std::shared_ptr<const Thing>& oldParent,
                                    int32_t index, ReceiverLink_t)
{
	const auto& topParent = getTopParent();
	if (topParent.get() == this) {
		if (const auto& tile = topParent->getTile()) {
			// Container is at the top level, on the ground
			tile->postAddNotification(thing, oldParent, index, LINK_NEAR);
		}
	} else if (const auto& creature = topParent->asCreature()) {
		if (const auto& player = creature->asPlayer()) {
			// Container is inside a player's inventory
			player->postAddNotification(thing, oldParent, index, LINK_TOPPARENT);
		}
	} else {
		// Container is inside another container
		topParent->postAddNotification(thing, oldParent, index, LINK_PARENT);
	}
}

void Container::postRemoveNotification(const std::shared_ptr<Thing>& thing,
                                       const std::shared_ptr<const Thing>& newParent, int32_t index, ReceiverLink_t)
{
	const auto& topParent = getTopParent();
	if (topParent.get() == this) {
		if (const auto& tile = topParent->getTile()) {
			// Container is at the top level, on the ground
			tile->postRemoveNotification(thing, newParent, index, LINK_NEAR);
		}
	} else if (const auto& creature = topParent->asCreature()) {
		if (const auto& player = creature->asPlayer()) {
			// Container is inside a player's inventory
			player->postRemoveNotification(thing, newParent, index, LINK_TOPPARENT);
		}
	} else {
		// Container is inside another container
		topParent->postRemoveNotification(thing, newParent, index, LINK_PARENT);
	}
}

void Container::internalRemoveThing(const std::shared_ptr<Thing>& thing)
{
	auto cit = std::find(itemList.begin(), itemList.end(), thing);
	if (cit == itemList.end()) {
		return;
	}
	itemList.erase(cit);
}

void Container::internalAddThing(uint32_t, const std::shared_ptr<Thing>& thing)
{
	const auto& item = thing->asItem();
	if (!item) {
		return;
	}

	item->setParent(getContainer());
	itemList.push_front(item);
	updateItemWeight(item->getWeight());
	ammoCount += item->getItemCount();
}

void Container::startDecaying()
{
	Item::startDecaying();

	for (const auto& item : itemList) {
		item->startDecaying();
	}
}

ContainerIterator Container::iterator() const
{
	ContainerIterator cit;
	if (!itemList.empty()) {
		cit.over.push_back(getContainer());
		cit.cur = itemList.begin();
	}
	return cit;
}

void ContainerIterator::advance()
{
	if (const auto currentItem = cur->get()) {
		if (const auto& c = currentItem->getContainer()) {
			if (!c->empty()) {
				over.push_back(c);
			}
		}
	}

	++cur;

	if (cur == over.front()->itemList.end()) {
		over.pop_front();
		if (!over.empty()) {
			cur = over.front()->itemList.begin();
		}
	}
}
