/**
 * The Forgotten Server - a server application for the MMORPG Tibia
 * Copyright (C) 2013  Mark Samman <mark.samman@gmail.com>
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

#ifndef __OTSERV_CONTAINER_H__
#define __OTSERV_CONTAINER_H__

#include <queue>

#include "definitions.h"
#include "cylinder.h"
#include "item.h"

class Container;
class DepotChest;
class DepotLocker;

class ContainerIterator
{
	public:
		ContainerIterator();
		ContainerIterator(const ContainerIterator& rhs);
		~ContainerIterator();

		ContainerIterator& operator=(const ContainerIterator& rhs);
		bool operator==(const ContainerIterator& rhs);
		bool operator!=(const ContainerIterator& rhs);
		ContainerIterator& operator++();
		ContainerIterator operator++(int);
		Item* operator*();
		Item* operator->();

	protected:
		ContainerIterator(Container* super);

		Container* super;
		std::queue<Container*> over;
		ItemDeque::iterator cur;

		friend class Container;
};

class Container : public Item, public Cylinder
{
	public:
		Container(uint16_t _type);
		Container(Tile* tile);
		virtual ~Container();
		virtual Item* clone() const;

		virtual Container* getContainer() {
			return this;
		}
		virtual const Container* getContainer() const {
			return this;
		}

		virtual DepotChest* getDepotChest() {
			return nullptr;
		}
		virtual const DepotChest* getDepotChest() const {
			return nullptr;
		}

		virtual DepotLocker* getDepotLocker() {
			return nullptr;
		}
		virtual const DepotLocker* getDepotLocker() const {
			return nullptr;
		}

		Attr_ReadValue readAttr(AttrTypes_t attr, PropStream& propStream);
		bool unserializeItemNode(FileLoader& f, NODE node, PropStream& propStream);
		std::string getContentDescription() const;

		uint32_t size() const {
			return (uint32_t)itemlist.size();
		}
		bool empty() const {
			return itemlist.empty();
		}
		uint32_t capacity() const {
			return maxSize;
		}

		ContainerIterator begin();
		ContainerIterator end();
		ContainerIterator begin() const;
		ContainerIterator end() const;

		const ItemDeque& getItemList() const {
			return itemlist;
		}

		ItemDeque::const_reverse_iterator getReversedItems() const {
			return itemlist.rbegin();
		}
		ItemDeque::const_reverse_iterator getReversedEnd() const {
			return itemlist.rend();
		}

		bool hasParent() const;
		void addItem(Item* item);
		Item* getItemByIndex(uint32_t index) const;
		bool isHoldingItem(const Item* item) const;

		uint32_t getItemHoldingCount() const;
		virtual double getWeight() const;

		bool isUnlocked() const {
			return unlocked;
		}
		bool hasPages() const {
			return pages;
		}

		//cylinder implementations
		virtual ReturnValue __queryAdd(int32_t index, const Thing* thing, uint32_t count,
		                               uint32_t flags, Creature* actor = nullptr) const;
		virtual ReturnValue __queryMaxCount(int32_t index, const Thing* thing, uint32_t count, uint32_t& maxQueryCount,
		                                    uint32_t flags) const;
		virtual ReturnValue __queryRemove(const Thing* thing, uint32_t count, uint32_t flags) const;
		virtual Cylinder* __queryDestination(int32_t& index, const Thing* thing, Item** destItem,
		                                     uint32_t& flags);

		virtual void __addThing(Thing* thing);
		virtual void __addThing(int32_t index, Thing* thing);
		virtual void __addThingBack(Thing* thing);

		virtual void __updateThing(Thing* thing, uint16_t itemId, uint32_t count);
		virtual void __replaceThing(uint32_t index, Thing* thing);

		virtual void __removeThing(Thing* thing, uint32_t count);

		virtual int32_t __getIndexOfThing(const Thing* thing) const;
		virtual int32_t __getFirstIndex() const;
		virtual int32_t __getLastIndex() const;
		virtual uint32_t __getItemTypeCount(uint16_t itemId, int32_t subType = -1) const;
		virtual std::map<uint32_t, uint32_t>& __getAllItemTypeCount(std::map<uint32_t, uint32_t>& countMap) const;
		virtual Thing* __getThing(uint32_t index) const;

		virtual void postAddNotification(Thing* thing, const Cylinder* oldParent, int32_t index, cylinderlink_t link = LINK_OWNER);
		virtual void postRemoveNotification(Thing* thing, const Cylinder* newParent, int32_t index, bool isCompleteRemoval, cylinderlink_t link = LINK_OWNER);

		virtual void __internalAddThing(Thing* thing);
		virtual void __internalAddThing(uint32_t index, Thing* thing);
		virtual void __startDecaying();

	private:
		void onAddContainerItem(Item* item);
		void onUpdateContainerItem(uint32_t index, Item* oldItem, const ItemType& oldType,
		                           Item* newItem, const ItemType& newType);
		void onRemoveContainerItem(uint32_t index, Item* item);

		Container* getParentContainer();
		void updateItemWeight(double diff);

	protected:
		std::ostringstream& getContentDescription(std::ostringstream& os) const;

		uint32_t maxSize;
		double totalWeight;
		ItemDeque itemlist;
		uint32_t serializationCount;

		bool unlocked;
		bool pages;

		friend class ContainerIterator;
		friend class IOMapSerialize;
};

#endif
