// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_CONTAINER_H
#define FS_CONTAINER_H

#include "cylinder.h"
#include "item.h"
#include "tile.h"

class Container;
class DepotLocker;
class StoreInbox;

class ContainerIterator
{
public:
	bool hasNext() const { return !over.empty(); }

	void advance();
	Item* operator*();

private:
	std::list<const Container*> over;
	ItemDeque::const_iterator cur;

	friend class Container;
};

class Container : public Item, public Cylinder
{
public:
	explicit Container(uint16_t type);
	Container(uint16_t type, uint16_t size, bool unlocked = true, bool pagination = false);
	explicit Container(Tile* tile);
	~Container();

	// non-copyable
	Container(const Container&) = delete;
	Container& operator=(const Container&) = delete;

	Item* clone() const override final;

	Container* getContainer() override final { return this; }
	const Container* getContainer() const override final { return this; }

	virtual DepotLocker* getDepotLocker() { return nullptr; }
	virtual const DepotLocker* getDepotLocker() const { return nullptr; }

	virtual StoreInbox* getStoreInbox() { return nullptr; }
	virtual const StoreInbox* getStoreInbox() const { return nullptr; }

	bool hasContainerParent() const;

	Attr_ReadValue readAttr(AttrTypes_t attr, PropStream& propStream) override;
	bool unserializeItemNode(OTB::Loader& loader, const OTB::Node& node, PropStream& propStream) override;

	size_t size() const { return itemlist.size(); }
	bool empty() const { return itemlist.empty(); }
	uint32_t capacity() const { return maxSize; }
	uint32_t getAmmoCount() const { return ammoCount; }

	ContainerIterator iterator() const;

	const ItemDeque& getItemList() const { return itemlist; }

	ItemDeque::const_reverse_iterator getReversedItems() const { return itemlist.rbegin(); }
	ItemDeque::const_reverse_iterator getReversedEnd() const { return itemlist.rend(); }

	std::string getName(bool addArticle = false) const;

	void addItem(Item* item);
	Item* getItemByIndex(size_t index) const;
	bool isHoldingItem(const Item* item) const;

	uint32_t getItemHoldingCount() const;
	uint16_t getFreeSlots();
	uint32_t getWeight() const override final;
	StashContainerList getStowableItems() const;

	bool isUnlocked() const { return unlocked; }
	bool hasPagination() const { return pagination; }

	// cylinder implementations
	virtual ReturnValue queryAdd(int32_t index, const Thing& thing, uint32_t count, uint32_t flags,
	                             Creature* actor = nullptr) const override;
	ReturnValue queryMaxCount(int32_t index, const Thing& thing, uint32_t count, uint32_t& maxQueryCount,
	                          uint32_t flags) const override final;
	ReturnValue queryRemove(const Thing& thing, uint32_t count, uint32_t flags,
	                        Creature* actor = nullptr) const override final;
	Cylinder* queryDestination(int32_t& index, const Thing& thing, Item** destItem, uint32_t& flags) override final;

	void addThing(Thing* thing) override final;
	void addThing(int32_t index, Thing* thing) override final;
	void addItemBack(Item* item);

	void updateThing(Thing* thing, uint16_t itemId, uint32_t count) override final;
	void replaceThing(uint32_t index, Thing* thing) override final;

	void removeThing(Thing* thing, uint32_t count) override final;

	int32_t getThingIndex(const Thing* thing) const override final;
	size_t getFirstIndex() const override final;
	size_t getLastIndex() const override final;
	uint32_t getItemTypeCount(uint16_t itemId, int32_t subType = -1) const override final;
	std::map<uint32_t, uint32_t>& getAllItemTypeCount(std::map<uint32_t, uint32_t>& countMap) const override final;
	Thing* getThing(size_t index) const override final;

	ItemVector getItems(bool recursive = false);

	void postAddNotification(Thing* thing, const Cylinder* oldParent, int32_t index,
	                         cylinderlink_t link = LINK_OWNER) override;
	void postRemoveNotification(Thing* thing, const Cylinder* newParent, int32_t index,
	                            cylinderlink_t link = LINK_OWNER) override;

	void internalRemoveThing(Thing* thing) override final;
	void internalAddThing(Thing* thing) override final;
	void internalAddThing(uint32_t index, Thing* thing) override final;
	void startDecaying() override final;

protected:
	ItemDeque itemlist;

private:
	uint32_t maxSize;
	uint32_t totalWeight = 0;
	uint32_t serializationCount = 0;
	uint32_t ammoCount = 0;

	bool unlocked;
	bool pagination;

	void onAddContainerItem(Item* item);
	void onUpdateContainerItem(uint32_t index, Item* oldItem, Item* newItem);
	void onRemoveContainerItem(uint32_t index, Item* item);

	Container* getParentContainer();
	void updateItemWeight(int32_t diff);

	friend class ContainerIterator;
	friend class IOMapSerialize;
};

#endif // FS_CONTAINER_H
