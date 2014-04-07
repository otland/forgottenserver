/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2014  Mark Samman <mark.samman@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "otpch.h"

#include "container.h"
#include "iomap.h"
#include "game.h"
#include "player.h"

extern Game g_game;

Container::Container(uint16_t _type) : Item(_type)
{
	maxSize = items[_type].maxItems;
	totalWeight = 0.0;
	serializationCount = 0;
	unlocked = true;
	pagination = false;
}

Container::Container(Tile* tile) : Item(ITEM_BROWSEFIELD)
{
	TileItemVector* itemVector = tile->getItemList();
	if (itemVector) {
		for (Item* item : *itemVector) {
			if (item->getContainer() || item->hasProperty(CONST_PROP_MOVEABLE)) {
				itemlist.push_front(item);
				item->setParent(this);
			}
		}
	}

	maxSize = 30;
	totalWeight = 0.0;
	serializationCount = 0;
	unlocked = false;
	pagination = true;
	setParent(tile);
}

Container::~Container()
{
	//std::cout << "Container destructor " << this << std::endl;
	if (getID() == ITEM_BROWSEFIELD) {
		g_game.browseFields.erase(getTile());

		for (Item* item : itemlist) {
			item->setParent(parent);
		}
	} else {
		for (Item* item : itemlist) {
			item->setParent(nullptr);
			item->releaseThing2();
		}
	}
}

Item* Container::clone() const
{
	Container* clone = static_cast<Container*>(Item::clone());
	for (Item* item : itemlist) {
		clone->addItem(item->clone());
	}
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

bool Container::hasParent() const
{
	return getID() != ITEM_BROWSEFIELD && dynamic_cast<const Player*>(getParent()) == nullptr;
}

void Container::addItem(Item* item)
{
	itemlist.push_back(item);
	item->setParent(this);
}

Attr_ReadValue Container::readAttr(AttrTypes_t attr, PropStream& propStream)
{
	if (attr == ATTR_CONTAINER_ITEMS) {
		uint32_t count;
		if (!propStream.GET_ULONG(count)) {
			return ATTR_READ_ERROR;
		}

		serializationCount = count;
		return ATTR_READ_END;
	}
	return Item::readAttr(attr, propStream);
}

bool Container::unserializeItemNode(FileLoader& f, NODE node, PropStream& propStream)
{
	bool ret = Item::unserializeItemNode(f, node, propStream);
	if (!ret) {
		return false;
	}

	uint32_t type;
	NODE nodeItem = f.getChildNode(node, type);
	while (nodeItem) {
		//load container items
		if (type != OTBM_ITEM) {
			// unknown type
			return false;
		}

		PropStream itemPropStream;
		if (!f.getProps(nodeItem, itemPropStream)) {
			return false;
		}

		Item* item = Item::CreateItem(itemPropStream);
		if (!item) {
			return false;
		}

		if (!item->unserializeItemNode(f, nodeItem, itemPropStream)) {
			return false;
		}

		addItem(item);
		totalWeight += item->getWeight();

		if (Container* parent_container = getParentContainer()) {
			parent_container->updateItemWeight(item->getWeight());
		}

		nodeItem = f.getNextNode(nodeItem, type);
	}
	return true;
}

void Container::updateItemWeight(double diff)
{
	totalWeight += diff;

	if (Container* parentContainer = getParentContainer()) {
		parentContainer->updateItemWeight(diff);
	}
}

double Container::getWeight() const
{
	return Item::getWeight() + totalWeight;
}

std::string Container::getContentDescription() const
{
	std::ostringstream os;
	return getContentDescription(os).str();
}

std::ostringstream& Container::getContentDescription(std::ostringstream& os) const
{
	bool firstitem = true;
	Container* evil = const_cast<Container*>(this);

	for (ContainerIterator cit = evil->begin(); cit != evil->end(); ++cit) {
		Item* item = (*cit);

		Container* container = item->getContainer();
		if (container && !container->empty()) {
			continue;
		}

		if (firstitem) {
			firstitem = false;
		} else {
			os << ", ";
		}

		os << item->getNameDescription();
	}

	if (firstitem) {
		os << "nothing";
	}
	return os;
}

Item* Container::getItemByIndex(uint32_t index) const
{
	if (index >= size()) {
		return nullptr;
	}
	return itemlist[index];
}

uint32_t Container::getItemHoldingCount() const
{
	uint32_t counter = 0;
	for (ContainerIterator iter = begin(); iter != end(); ++iter) {
		++counter;
	}
	return counter;
}

bool Container::isHoldingItem(const Item* item) const
{
	for (ContainerIterator cit = begin(); cit != end(); ++cit) {
		if (*cit == item) {
			return true;
		}
	}
	return false;
}

void Container::onAddContainerItem(Item* item)
{
	SpectatorVec list;
	g_game.getSpectators(list, getPosition(), false, true, 2, 2, 2, 2);

	//send to client
	for (Creature* spectator : list) {
		spectator->getPlayer()->sendAddContainerItem(this, item);
	}

	//event methods
	for (Creature* spectator : list) {
		spectator->getPlayer()->onAddContainerItem(item);
	}
}

void Container::onUpdateContainerItem(uint32_t index, Item* oldItem, Item* newItem)
{
	SpectatorVec list;
	g_game.getSpectators(list, getPosition(), false, true, 2, 2, 2, 2);

	//send to client
	for (Creature* spectator : list) {
		spectator->getPlayer()->sendUpdateContainerItem(this, index, newItem);
	}

	//event methods
	for (Creature* spectator : list) {
		spectator->getPlayer()->onUpdateContainerItem(this, oldItem, newItem);
	}
}

void Container::onRemoveContainerItem(uint32_t index, Item* item)
{
	SpectatorVec list;
	g_game.getSpectators(list, getPosition(), false, true, 2, 2, 2, 2);

	//send change to client
	for (Creature* spectator : list) {
		spectator->getPlayer()->sendRemoveContainerItem(this, index);
	}

	//event methods
	for (Creature* spectator : list) {
		spectator->getPlayer()->onRemoveContainerItem(this, item);
	}
}

ReturnValue Container::__queryAdd(int32_t index, const Thing* thing, uint32_t count,
                                  uint32_t flags, Creature* actor/* = nullptr*/) const
{
	bool childIsOwner = hasBitSet(FLAG_CHILDISOWNER, flags);
	if (childIsOwner) {
		//a child container is querying, since we are the top container (not carried by a player)
		//just return with no error.
		return RET_NOERROR;
	}

	if (!unlocked) {
		return RET_NOTPOSSIBLE;
	}

	const Item* item = thing->getItem();
	if (item == nullptr) {
		return RET_NOTPOSSIBLE;
	}

	if (!item->isPickupable()) {
		return RET_CANNOTPICKUP;
	}

	if (item == this) {
		return RET_THISISIMPOSSIBLE;
	}

	const Cylinder* cylinder = getParent();
	if (!hasBitSet(FLAG_NOLIMIT, flags)) {
		while (cylinder) {
			if (cylinder == thing) {
				return RET_THISISIMPOSSIBLE;
			}

			if (dynamic_cast<const Inbox*>(cylinder)) {
				return RET_CONTAINERNOTENOUGHROOM;
			}

			cylinder = cylinder->getParent();
		}

		if (index == INDEX_WHEREEVER && size() >= capacity()) {
			return RET_CONTAINERNOTENOUGHROOM;
		}
	} else {
		while (cylinder) {
			if (cylinder == thing) {
				return RET_THISISIMPOSSIBLE;
			}

			cylinder = cylinder->getParent();
		}
	}

	const Cylinder* topParent = getTopParent();
	if (topParent != this) {
		return topParent->__queryAdd(INDEX_WHEREEVER, item, count, flags | FLAG_CHILDISOWNER, actor);
	} else {
		return RET_NOERROR;
	}
}

ReturnValue Container::__queryMaxCount(int32_t index, const Thing* thing, uint32_t count,
                                       uint32_t& maxQueryCount, uint32_t flags) const
{
	const Item* item = thing->getItem();
	if (item == nullptr) {
		maxQueryCount = 0;
		return RET_NOTPOSSIBLE;
	}

	if (hasBitSet(FLAG_NOLIMIT, flags)) {
		maxQueryCount = std::max<uint32_t>(1, count);
		return RET_NOERROR;
	}

	int32_t freeSlots = std::max<int32_t>(capacity() - size(), 0);

	if (item->isStackable()) {
		uint32_t n = 0;

		if (index == INDEX_WHEREEVER) {
			//Iterate through every item and check how much free stackable slots there is.
			uint32_t slotIndex = 0;
			for (Item* containerItem : itemlist) {
				if (containerItem != item && containerItem->getID() == item->getID() && containerItem->getItemCount() < 100) {
					uint32_t remainder = (100 - containerItem->getItemCount());
					if (__queryAdd(slotIndex++, item, remainder, flags) == RET_NOERROR) {
						n += remainder;
					}
				}
			}
		} else {
			const Item* destItem = getItemByIndex(index);
			if (destItem && destItem->getID() == item->getID() && destItem->getItemCount() < 100) {
				uint32_t remainder = 100 - destItem->getItemCount();
				if (__queryAdd(index, item, remainder, flags) == RET_NOERROR) {
					n = remainder;
				}
			}
		}

		maxQueryCount = freeSlots * 100 + n;
		if (maxQueryCount < count) {
			return RET_CONTAINERNOTENOUGHROOM;
		}
	} else {
		maxQueryCount = freeSlots;
		if (maxQueryCount == 0) {
			return RET_CONTAINERNOTENOUGHROOM;
		}
	}
	return RET_NOERROR;
}

ReturnValue Container::__queryRemove(const Thing* thing, uint32_t count, uint32_t flags) const
{
	int32_t index = __getIndexOfThing(thing);
	if (index == -1) {
		return RET_NOTPOSSIBLE;
	}

	const Item* item = thing->getItem();
	if (item == nullptr) {
		return RET_NOTPOSSIBLE;
	}

	if (count == 0 || (item->isStackable() && count > item->getItemCount())) {
		return RET_NOTPOSSIBLE;
	}

	if (!item->isMoveable() && !hasBitSet(FLAG_IGNORENOTMOVEABLE, flags)) {
		return RET_NOTMOVEABLE;
	}
	return RET_NOERROR;
}

Cylinder* Container::__queryDestination(int32_t& index, const Thing* thing, Item** destItem,
                                        uint32_t& flags)
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
	} else if (index >= (int32_t)capacity()) {
		/*
		if you have a container, maximize it to show all 20 slots
		then you open a bag that is inside the container you will have a bag with 8 slots
		and a "grey" area where the other 12 slots where from the container
		if you drop the item on that grey area
		the client calculates the slot position as if the bag has 20 slots
		*/
		index = INDEX_WHEREEVER;
		*destItem = nullptr;
	}

	const Item* item = thing->getItem();
	if (!item) {
		return this;
	}

	bool autoStack = !hasBitSet(FLAG_IGNOREAUTOSTACK, flags);
	if (autoStack && item->isStackable() && item->getParent() != this) {
		//try find a suitable item to stack with
		uint32_t n = 0;
		for (Item* listItem : itemlist) {
			if (listItem != item && listItem->getID() == item->getID() && listItem->getItemCount() < 100) {
				*destItem = listItem;
				index = n;
				return this;
			}
			++n;
		}
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
	return this;
}

void Container::__addThing(Thing* thing)
{
	return __addThing(0, thing);
}

void Container::__addThing(int32_t index, Thing* thing)
{
	if (index >= (int32_t)capacity()) {
		return /*RET_NOTPOSSIBLE*/;
	}

	Item* item = thing->getItem();
	if (item == nullptr) {
		return /*RET_NOTPOSSIBLE*/;
	}

	item->setParent(this);
	itemlist.push_front(item);
	totalWeight += item->getWeight();

	if (Container* parentContainer = getParentContainer()) {
		parentContainer->updateItemWeight(item->getWeight());
	}

	//send change to client
	if (getParent() && (getParent() != VirtualCylinder::virtualCylinder)) {
		onAddContainerItem(item);
	}
}

void Container::__addThingBack(Thing* thing)
{
	Item* item = thing->getItem();
	if (item == nullptr) {
		return /*RET_NOTPOSSIBLE*/;
	}

	item->setParent(this);
	itemlist.push_back(item);
	totalWeight += item->getWeight();

	if (Container* parentContainer = getParentContainer()) {
		parentContainer->updateItemWeight(item->getWeight());
	}

	//send change to client
	if (getParent() && (getParent() != VirtualCylinder::virtualCylinder)) {
		onAddContainerItem(item);
	}
}

void Container::__updateThing(Thing* thing, uint16_t itemId, uint32_t count)
{
	int32_t index = __getIndexOfThing(thing);
	if (index == -1) {
		return /*RET_NOTPOSSIBLE*/;
	}

	Item* item = thing->getItem();
	if (item == nullptr) {
		return /*RET_NOTPOSSIBLE*/;
	}

	const double oldWeight = item->getWeight();

	item->setID(itemId);
	item->setSubType(count);

	const double diffWeight = -oldWeight + item->getWeight();

	totalWeight += diffWeight;

	if (Container* parentContainer = getParentContainer()) {
		parentContainer->updateItemWeight(diffWeight);
	}

	//send change to client
	if (getParent()) {
		onUpdateContainerItem(index, item, item);
	}
}

void Container::__replaceThing(uint32_t index, Thing* thing)
{
	Item* item = thing->getItem();
	if (!item) {
		return /*RET_NOTPOSSIBLE*/;
	}

	Item* replacedItem = getItemByIndex(index);
	if (!replacedItem) {
		return /*RET_NOTPOSSIBLE*/;
	}

	totalWeight -= replacedItem->getWeight();
	totalWeight += item->getWeight();

	if (Container* parentContainer = getParentContainer()) {
		parentContainer->updateItemWeight(-replacedItem->getWeight() + item->getWeight());
	}

	itemlist[index] = item;
	item->setParent(this);

	//send change to client
	if (getParent()) {
		onUpdateContainerItem(index, replacedItem, item);
	}

	replacedItem->setParent(nullptr);
}

void Container::__removeThing(Thing* thing, uint32_t count)
{
	Item* item = thing->getItem();
	if (item == nullptr) {
		return /*RET_NOTPOSSIBLE*/;
	}

	int32_t index = __getIndexOfThing(thing);
	if (index == -1) {
		return /*RET_NOTPOSSIBLE*/;
	}

	if (item->isStackable() && count != item->getItemCount()) {
		uint8_t newCount = (uint8_t)std::max<int32_t>(0, item->getItemCount() - count);
		const double oldWeight = -item->getWeight();
		item->setItemCount(newCount);
		const double diffWeight = oldWeight + item->getWeight();
		totalWeight += diffWeight;

		//send change to client
		if (getParent()) {
			if (Container* parentContainer = getParentContainer()) {
				parentContainer->updateItemWeight(diffWeight);
			}

			onUpdateContainerItem(index, item, item);
		}
	} else {
		//send change to client
		if (getParent()) {
			if (Container* parentContainer = getParentContainer()) {
				parentContainer->updateItemWeight(-item->getWeight());
			}

			onRemoveContainerItem(index, item);
		}

		totalWeight -= item->getWeight();
		item->setParent(nullptr);
		itemlist.erase(itemlist.begin() + index);
	}
}

int32_t Container::__getIndexOfThing(const Thing* thing) const
{
	uint32_t index = 0;
	for (Item* item : itemlist) {
		if (item == thing) {
			return index;
		}
		++index;
	}
	return -1;
}

int32_t Container::__getFirstIndex() const
{
	return 0;
}

int32_t Container::__getLastIndex() const
{
	return size();
}

uint32_t Container::__getItemTypeCount(uint16_t itemId, int32_t subType /*= -1*/) const
{
	uint32_t count = 0;
	for (Item* item : itemlist) {
		if (item->getID() == itemId) {
			count += countByType(item, subType);
		}
	}
	return count;
}

std::map<uint32_t, uint32_t>& Container::__getAllItemTypeCount(std::map<uint32_t, uint32_t>& countMap) const
{
	for (Item* item : itemlist) {
		countMap[item->getID()] += item->getItemCount();
	}
	return countMap;
}

Thing* Container::__getThing(size_t index) const
{
	return getItemByIndex(index);
}

void Container::postAddNotification(Thing* thing, const Cylinder* oldParent, int32_t index, cylinderlink_t)
{
	Cylinder* topParent = getTopParent();
	if (topParent->getCreature()) {
		topParent->postAddNotification(thing, oldParent, index, LINK_TOPPARENT);
	} else if (topParent == this) {
		//let the tile class notify surrounding players
		if (topParent->getParent()) {
			topParent->getParent()->postAddNotification(thing, oldParent, index, LINK_NEAR);
		}
	} else {
		topParent->postAddNotification(thing, oldParent, index, LINK_PARENT);
	}
}

void Container::postRemoveNotification(Thing* thing, const Cylinder* newParent, int32_t index, bool isCompleteRemoval, cylinderlink_t)
{
	Cylinder* topParent = getTopParent();
	if (topParent->getCreature()) {
		topParent->postRemoveNotification(thing, newParent, index, isCompleteRemoval, LINK_TOPPARENT);
	} else if (topParent == this) {
		//let the tile class notify surrounding players
		if (topParent->getParent()) {
			topParent->getParent()->postRemoveNotification(thing, newParent, index, isCompleteRemoval, LINK_NEAR);
		}
	} else {
		topParent->postRemoveNotification(thing, newParent, index, isCompleteRemoval, LINK_PARENT);
	}
}

void Container::__internalAddThing(Thing* thing)
{
	__internalAddThing(0, thing);
}

void Container::__internalAddThing(uint32_t, Thing* thing)
{
	Item* item = thing->getItem();
	if (item == nullptr) {
		return;
	}

	item->setParent(this);
	itemlist.push_front(item);
	totalWeight += item->getWeight();

	if (Container* parentContainer = getParentContainer()) {
		parentContainer->updateItemWeight(item->getWeight());
	}
}

void Container::__startDecaying()
{
	for (Item* item : itemlist) {
		item->__startDecaying();
	}
}

ContainerIterator Container::begin()
{
	ContainerIterator cit(this);

	if (!itemlist.empty()) {
		cit.over.push(this);
		cit.cur = itemlist.begin();
	}

	return cit;
}

ContainerIterator Container::end()
{
	ContainerIterator cit(this);
	return cit;
}

// Very evil constructors, look away if you are sensitive!
ContainerIterator Container::begin() const
{
	Container* evil = const_cast<Container*>(this);
	return evil->begin();
}

ContainerIterator Container::end() const
{
	Container* evil = const_cast<Container*>(this);
	return evil->end();
}

ContainerIterator::ContainerIterator():
	super(nullptr) {}

ContainerIterator::ContainerIterator(Container* super):
	super(super) {}

ContainerIterator::~ContainerIterator() {}

ContainerIterator::ContainerIterator(const ContainerIterator& rhs):
	super(rhs.super), over(rhs.over), cur(rhs.cur) {}

bool ContainerIterator::operator==(const ContainerIterator& rhs) const
{
	return !(*this != rhs);
}

bool ContainerIterator::operator!=(const ContainerIterator& rhs) const
{
	assert(super);

	if (super != rhs.super) {
		return true;
	}

	if (over.empty() && rhs.over.empty()) {
		return false;
	}

	if (over.empty()) {
		return true;
	}

	if (rhs.over.empty()) {
		return true;
	}

	if (over.front() != rhs.over.front()) {
		return true;
	}

	return cur != rhs.cur;
}

ContainerIterator& ContainerIterator::operator=(const ContainerIterator& rhs)
{
	this->super = rhs.super;
	this->cur = rhs.cur;
	this->over = rhs.over;
	return *this;
}

Item* ContainerIterator::operator*()
{
	assert(super);
	return *cur;
}

Item* ContainerIterator::operator->()
{
	return *(*this);
}

ContainerIterator& ContainerIterator::operator++()
{
	assert(super);

	if (Item* i = *cur) {
		if (Container* c = i->getContainer()) {
			if (!c->empty()) {
				over.push(c);
			}
		}
	}

	++cur;

	if (cur == over.front()->itemlist.end()) {
		over.pop();

		if (over.empty()) {
			return *this;
		}

		cur = over.front()->itemlist.begin();
	}

	return *this;
}

ContainerIterator ContainerIterator::operator++(int)
{
	ContainerIterator tmp(*this);
	++*this;
	return tmp;
}
