// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_CONTAINER_H
#define FS_CONTAINER_H

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
	std::shared_ptr<Item> operator*();

private:
	std::list<std::shared_ptr<const Container>> over;
	ItemDeque::const_iterator cur;

	friend class Container;
};

class Container : public Item
{
public:
	explicit Container(uint16_t type);
	Container(uint16_t type, uint16_t size, bool unlocked = true, bool pagination = false);
	explicit Container(std::shared_ptr<Tile> tile);
	~Container();

	// non-copyable
	Container(const Container&) = delete;
	Container& operator=(const Container&) = delete;

	std::shared_ptr<Item> clone() const override final;

	std::shared_ptr<Container> getContainer() override final
	{
		assert(std::dynamic_pointer_cast<Container>(Item::getItem()) != nullptr);
		return std::static_pointer_cast<Container>(Item::getItem());
	}
	std::shared_ptr<const Container> getContainer() const override final
	{
		assert(std::dynamic_pointer_cast<const Container>(Item::getItem()) != nullptr);
		return std::static_pointer_cast<const Container>(Item::getItem());
	}

	virtual std::shared_ptr<DepotLocker> getDepotLocker() { return nullptr; }
	virtual std::shared_ptr<const DepotLocker> getDepotLocker() const { return nullptr; }

	virtual std::shared_ptr<StoreInbox> getStoreInbox() { return nullptr; }
	virtual std::shared_ptr<const StoreInbox> getStoreInbox() const { return nullptr; }

	bool hasContainerParent() const;

	Attr_ReadValue readAttr(AttrTypes_t attr, PropStream& propStream) override;
	bool unserializeItemNode(OTB::Loader& loader, const OTB::Node& node, PropStream& propStream) override;

	size_t size() const { return itemList.size(); }
	bool empty() const { return itemList.empty(); }
	uint32_t capacity() const { return maxSize; }
	uint32_t getAmmoCount() const { return ammoCount; }

	ContainerIterator iterator() const;

	const ItemDeque& getItemList() const { return itemList; }

	ItemDeque::const_reverse_iterator getReversedItems() const { return itemList.rbegin(); }
	ItemDeque::const_reverse_iterator getReversedEnd() const { return itemList.rend(); }

	std::string getName(bool addArticle = false) const;

	void addItem(std::shared_ptr<Item> item);
	std::shared_ptr<Item> getItemByIndex(size_t index) const;
	bool isHoldingItem(std::shared_ptr<const Item> item) const;

	uint32_t getItemHoldingCount() const;
	uint32_t getWeight() const override final;

	bool isUnlocked() const { return unlocked; }
	bool hasPagination() const { return pagination; }

	// Thing implementations
	virtual ReturnValue queryAdd(int32_t index, std::shared_ptr<const Thing> thing, uint32_t count, uint32_t flags,
	                             std::shared_ptr<Creature> actor = nullptr) const override;
	ReturnValue queryMaxCount(int32_t index, std::shared_ptr<const Thing> thing, uint32_t count,
	                          uint32_t& maxQueryCount, uint32_t flags) const override final;
	ReturnValue queryRemove(std::shared_ptr<const Thing> thing, uint32_t count, uint32_t flags,
	                        std::shared_ptr<Creature> actor = nullptr) const override final;
	std::shared_ptr<Thing> queryDestination(int32_t& index, std::shared_ptr<const Thing> thing,
	                                        std::shared_ptr<Item>& destItem, uint32_t& flags) override final;

	void addThing(std::shared_ptr<Thing> thing) override final;
	void addThing(int32_t index, std::shared_ptr<Thing> thing) override final;
	void addItemBack(std::shared_ptr<Item> item);

	void updateThing(std::shared_ptr<Thing> thing, uint16_t itemId, uint32_t count) override final;
	void replaceThing(uint32_t index, std::shared_ptr<Thing> thing) override final;

	void removeThing(std::shared_ptr<Thing> thing, uint32_t count) override final;

	int32_t getThingIndex(std::shared_ptr<const Thing> thing) const override final;
	size_t getFirstIndex() const override final;
	size_t getLastIndex() const override final;
	uint32_t getItemTypeCount(uint16_t itemId, int32_t subType = -1) const override final;
	std::map<uint32_t, uint32_t>& getAllItemTypeCount(std::map<uint32_t, uint32_t>& countMap) const override final;
	std::shared_ptr<Thing> getThing(size_t index) const override final;

	ItemVector getItems(bool recursive = false);

	void postAddNotification(std::shared_ptr<Thing> thing, std::shared_ptr<const Thing> oldParent, int32_t index,
	                         cylinderlink_t link = LINK_OWNER) override;
	void postRemoveNotification(std::shared_ptr<Thing> thing, std::shared_ptr<const Thing> newParent, int32_t index,
	                            cylinderlink_t link = LINK_OWNER) override;

	void internalRemoveThing(std::shared_ptr<Thing> thing) override final;
	void internalAddThing(std::shared_ptr<Thing> thing) override final;
	void internalAddThing(uint32_t index, std::shared_ptr<Thing> thing) override final;
	void startDecaying() override final;

protected:
	ItemDeque itemList;

private:
	uint32_t maxSize;
	uint32_t totalWeight = 0;
	uint32_t serializationCount = 0;
	uint32_t ammoCount = 0;

	bool unlocked;
	bool pagination;

	void onAddContainerItem(std::shared_ptr<Item> item);
	void onUpdateContainerItem(uint32_t index, std::shared_ptr<Item> oldItem, std::shared_ptr<Item> newItem);
	void onRemoveContainerItem(uint32_t index, std::shared_ptr<Item> item);

	std::shared_ptr<Container> getParentContainer();
	void updateItemWeight(int32_t diff);

	friend class ContainerIterator;
	friend class IOMapSerialize;
};

#endif // FS_CONTAINER_H
