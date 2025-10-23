// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_THING_H
#define FS_THING_H

class Container;
class Creature;
class Cylinder;
class Item;
class Tile;
struct Position;

enum cylinderlink_t
{
	LINK_OWNER,
	LINK_PARENT,
	LINK_TOPPARENT,
	LINK_NEAR,
};

class Thing : public std::enable_shared_from_this<Thing>
{
public:
	constexpr Thing() = default;
	virtual ~Thing() = default;

	// non-copyable
	Thing(const Thing&) = delete;
	Thing& operator=(const Thing&) = delete;

	virtual bool hasParent() const { return false; }
	virtual std::shared_ptr<Thing> getParent() const { return nullptr; }
	virtual std::shared_ptr<Thing> getRealParent() const { return getParent(); }

	virtual void setParent(std::shared_ptr<Thing>) { throw std::runtime_error("Not implemented"); };

	virtual std::shared_ptr<Tile> getTile() { return nullptr; }
	virtual std::shared_ptr<const Tile> getTile() const { return nullptr; }

	virtual const Position& getPosition() const;
	virtual int32_t getThrowRange() const { throw std::runtime_error("Not implemented"); };
	virtual bool isPushable() const { throw std::runtime_error("Not implemented"); };

	virtual std::shared_ptr<Container> getContainer() { return nullptr; }
	virtual std::shared_ptr<const Container> getContainer() const { return nullptr; }
	virtual std::shared_ptr<Item> getItem() { return nullptr; }
	virtual std::shared_ptr<const Item> getItem() const { return nullptr; }
	virtual std::shared_ptr<Creature> getCreature() { return nullptr; }
	virtual std::shared_ptr<const Creature> getCreature() const { return nullptr; }

	virtual bool isRemoved() const { return true; }

	/**
	 * Query if the cylinder can add an object
	 * \param index points to the destination index (inventory slot/container
	 * position) -1 is a internal value and means add to a empty position, with
	 * no destItem \param thing the object to move/add \param count is the
	 * amount that we want to move/add \param flags if FLAG_CHILDISOWNER if set
	 * the query is from a child-cylinder (check cap etc.) if FLAG_NOLIMIT is
	 * set blocking items/container limits is ignored \param actor the creature
	 * trying to add the thing \returns ReturnValue holds the return value
	 */
	virtual ReturnValue queryAdd(int32_t, std::shared_ptr<const Thing>, uint32_t, uint32_t,
	                             std::shared_ptr<Creature> = nullptr) const
	{
		throw std::runtime_error("Not implemented");
	};

	/**
	 * Query the cylinder how much it can accept
	 * \param index points to the destination index (inventory slot/container
	 * position) -1 is a internal value and means add to a empty position, with
	 * no destItem \param thing the object to move/add \param count is the
	 * amount that we want to move/add \param maxQueryCount is the max amount
	 * that the cylinder can accept \param flags optional flags to modify the
	 * default behaviour \returns ReturnValue holds the return value
	 */
	virtual ReturnValue queryMaxCount(int32_t, std::shared_ptr<const Thing>, uint32_t, uint32_t&, uint32_t) const
	{
		throw std::runtime_error("Not implemented");
	};

	/**
	 * Query if the cylinder can remove an object
	 * \param thing the object to move/remove
	 * \param count is the amount that we want to remove
	 * \param flags optional flags to modify the default behaviour
	 * \returns ReturnValue holds the return value
	 */
	virtual ReturnValue queryRemove(std::shared_ptr<const Thing>, uint32_t, uint32_t,
	                                std::shared_ptr<Creature> = nullptr) const
	{
		throw std::runtime_error("Not implemented");
	};

	/**
	 * Query the destination cylinder
	 * \param index points to the destination index (inventory slot/container
	 * position), -1 is a internal value and means add to a empty position, with
	 * no destItem this method can change the index to point to the new cylinder
	 * index \param destItem is the destination object \param flags optional
	 * flags to modify the default behaviour this method can modify the flags
	 * \returns Cylinder returns the destination cylinder
	 */
	virtual std::shared_ptr<Thing> queryDestination(int32_t&, std::shared_ptr<const Thing>, std::shared_ptr<Item>&,
	                                                uint32_t&)
	{
		throw std::runtime_error("Not implemented");
	};

	/**
	 * Add the object to the cylinder
	 * \param thing is the object to add
	 */
	virtual void addThing(std::shared_ptr<Thing>) { throw std::runtime_error("Not implemented"); };

	/**
	 * Add the object to the cylinder
	 * \param index points to the destination index (inventory slot/container
	 * position) \param thing is the object to add
	 */
	virtual void addThing(int32_t, std::shared_ptr<Thing>) { throw std::runtime_error("Not implemented"); };

	/**
	 * Update the item count or type for an object
	 * \param thing is the object to update
	 * \param itemId is the new item id
	 * \param count is the new count value
	 */
	virtual void updateThing(std::shared_ptr<Thing>, uint16_t, uint32_t)
	{
		throw std::runtime_error("Not implemented");
	};

	/**
	 * Replace an object with a new
	 * \param index is the position to change (inventory slot/container
	 * position) \param thing is the object to update
	 */
	virtual void replaceThing(uint32_t, std::shared_ptr<Thing>) { throw std::runtime_error("Not implemented"); };

	/**
	 * Remove an object
	 * \param thing is the object to delete
	 * \param count is the new count value
	 */
	virtual void removeThing(std::shared_ptr<Thing>, uint32_t) { throw std::runtime_error("Not implemented"); };

	/**
	 * Is sent after an operation (move/add) to update internal values
	 * \param thing is the object that has been added
	 * \param index is the objects new index value
	 * \param link holds the relation the object has to the cylinder
	 */
	virtual void postAddNotification(std::shared_ptr<Thing>, std::shared_ptr<const Thing>, int32_t,
	                                 cylinderlink_t = LINK_OWNER)
	{
		throw std::runtime_error("Not implemented");
	};

	/**
	 * Is sent after an operation (move/remove) to update internal values
	 * \param thing is the object that has been removed
	 * \param index is the previous index of the removed object
	 * \param link holds the relation the object has to the cylinder
	 */
	virtual void postRemoveNotification(std::shared_ptr<Thing>, std::shared_ptr<const Thing>, int32_t,
	                                    cylinderlink_t = LINK_OWNER)
	{
		throw std::runtime_error("Not implemented");
	};

	/**
	 * Gets the index of an object
	 * \param thing the object to get the index value from
	 * \returns the index of the object, returns -1 if not found
	 */
	virtual int32_t getThingIndex(std::shared_ptr<const Thing>) const { return -1; }

	/**
	 * Returns the first index
	 * \returns the first index, if not implemented 0 is returned
	 */
	virtual size_t getFirstIndex() const { return 0; }

	/**
	 * Returns the last index
	 * \returns the last index, if not implemented 0 is returned
	 */
	virtual size_t getLastIndex() const { return 0; }

	/**
	 * Gets the object based on index
	 * \returns the object, returns nullptr if not found
	 */
	virtual std::shared_ptr<Thing> getThing(size_t) const { return nullptr; }

	/**
	 * Get the amount of items of a certain type
	 * \param itemId is the item type to the get the count of
	 * \param subType is the extra type an item can have such as
	 * charges/fluidtype, -1 means not used \returns the amount of items of the
	 * asked item type
	 */
	virtual uint32_t getItemTypeCount(uint16_t, int32_t = -1) const { return 0; }

	/**
	 * Get the amount of items of a all types
	 * \param countMap a map to put the itemID:count mapping in
	 * \returns a map mapping item id to count (same as first argument)
	 */
	virtual std::map<uint32_t, uint32_t>& getAllItemTypeCount(std::map<uint32_t, uint32_t>& countMap) const
	{
		return countMap;
	}

	/**
	 * Removes an object from the cylinder without sending to the client(s)
	 * \param thing is the object to add
	 */
	virtual void internalRemoveThing(std::shared_ptr<Thing>) {}

	/**
	 * Adds an object to the cylinder without sending to the client(s)
	 * \param thing is the object to add
	 */
	virtual void internalAddThing(std::shared_ptr<Thing>) {}

	/**
	 * Adds an object to the cylinder without sending to the client(s)
	 * \param thing is the object to add
	 * \param index points to the destination index (inventory slot/container
	 * position)
	 */
	virtual void internalAddThing(uint32_t, std::shared_ptr<Thing>) {}

	virtual void startDecaying() {}
};

#endif // FS_THING_H
