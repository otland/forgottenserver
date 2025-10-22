// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_CYLINDER_H
#define FS_CYLINDER_H

#include "enums.h"
#include "thing.h"

class Creature;
class Item;

static constexpr int32_t INDEX_WHEREEVER = -1;

enum cylinderflags_t
{
	FLAG_NOLIMIT = 1 << 0,             // Bypass limits like capacity/container limits, blocking items/creatures etc.
	FLAG_IGNOREBLOCKITEM = 1 << 1,     // Bypass movable blocking item checks
	FLAG_IGNOREBLOCKCREATURE = 1 << 2, // Bypass creature checks
	FLAG_CHILDISOWNER = 1 << 3,        // Used by containers to query capacity of the carrier (player)
	FLAG_PATHFINDING = 1 << 4,         // An additional check is done for floor changing/teleport items
	FLAG_IGNOREFIELDDAMAGE = 1 << 5,   // Bypass field damage checks
	FLAG_IGNORENOTMOVEABLE = 1 << 6,   // Bypass check for mobility
	FLAG_IGNOREAUTOSTACK = 1 << 7,     // queryDestination will not try to stack items together
};

enum cylinderlink_t
{
	LINK_OWNER,
	LINK_PARENT,
	LINK_TOPPARENT,
	LINK_NEAR,
};

class Cylinder : virtual public Thing, private std::enable_shared_from_this<Cylinder>
{
public:
	virtual ~Cylinder() = default;

	std::shared_ptr<Cylinder> getCylinder() { return std::static_pointer_cast<Cylinder>(shared_from_this()); }
	std::shared_ptr<const Cylinder> getCylinder() const
	{
		return std::static_pointer_cast<const Cylinder>(shared_from_this());
	}

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
	virtual ReturnValue queryAdd(int32_t index, std::shared_ptr<const Thing> thing, uint32_t count, uint32_t flags,
	                             std::shared_ptr<Creature> actor = nullptr) const = 0;

	/**
	 * Query the cylinder how much it can accept
	 * \param index points to the destination index (inventory slot/container
	 * position) -1 is a internal value and means add to a empty position, with
	 * no destItem \param thing the object to move/add \param count is the
	 * amount that we want to move/add \param maxQueryCount is the max amount
	 * that the cylinder can accept \param flags optional flags to modify the
	 * default behaviour \returns ReturnValue holds the return value
	 */
	virtual ReturnValue queryMaxCount(int32_t index, std::shared_ptr<const Thing> thing, uint32_t count,
	                                  uint32_t& maxQueryCount, uint32_t flags) const = 0;

	/**
	 * Query if the cylinder can remove an object
	 * \param thing the object to move/remove
	 * \param count is the amount that we want to remove
	 * \param flags optional flags to modify the default behaviour
	 * \returns ReturnValue holds the return value
	 */
	virtual ReturnValue queryRemove(std::shared_ptr<const Thing> thing, uint32_t count, uint32_t flags,
	                                std::shared_ptr<Creature> = nullptr) const = 0;

	/**
	 * Query the destination cylinder
	 * \param index points to the destination index (inventory slot/container
	 * position), -1 is a internal value and means add to a empty position, with
	 * no destItem this method can change the index to point to the new cylinder
	 * index \param destItem is the destination object \param flags optional
	 * flags to modify the default behaviour this method can modify the flags
	 * \returns Cylinder returns the destination cylinder
	 */
	virtual std::shared_ptr<Cylinder> queryDestination(int32_t& index, std::shared_ptr<const Thing> thing,
	                                                   std::shared_ptr<Item>& destItem, uint32_t& flags) = 0;

	/**
	 * Add the object to the cylinder
	 * \param thing is the object to add
	 */
	virtual void addThing(std::shared_ptr<Thing> thing) = 0;

	/**
	 * Add the object to the cylinder
	 * \param index points to the destination index (inventory slot/container
	 * position) \param thing is the object to add
	 */
	virtual void addThing(int32_t index, std::shared_ptr<Thing> thing) = 0;

	/**
	 * Update the item count or type for an object
	 * \param thing is the object to update
	 * \param itemId is the new item id
	 * \param count is the new count value
	 */
	virtual void updateThing(std::shared_ptr<Thing> thing, uint16_t itemId, uint32_t count) = 0;

	/**
	 * Replace an object with a new
	 * \param index is the position to change (inventory slot/container
	 * position) \param thing is the object to update
	 */
	virtual void replaceThing(uint32_t index, std::shared_ptr<Thing> thing) = 0;

	/**
	 * Remove an object
	 * \param thing is the object to delete
	 * \param count is the new count value
	 */
	virtual void removeThing(std::shared_ptr<Thing> thing, uint32_t count) = 0;

	/**
	 * Is sent after an operation (move/add) to update internal values
	 * \param thing is the object that has been added
	 * \param index is the objects new index value
	 * \param link holds the relation the object has to the cylinder
	 */
	virtual void postAddNotification(std::shared_ptr<Thing> thing, std::shared_ptr<const Cylinder> oldParent,
	                                 int32_t index, cylinderlink_t link = LINK_OWNER) = 0;

	/**
	 * Is sent after an operation (move/remove) to update internal values
	 * \param thing is the object that has been removed
	 * \param index is the previous index of the removed object
	 * \param link holds the relation the object has to the cylinder
	 */
	virtual void postRemoveNotification(std::shared_ptr<Thing> thing, std::shared_ptr<const Cylinder> newParent,
	                                    int32_t index, cylinderlink_t link = LINK_OWNER) = 0;

	/**
	 * Gets the index of an object
	 * \param thing the object to get the index value from
	 * \returns the index of the object, returns -1 if not found
	 */
	virtual int32_t getThingIndex(std::shared_ptr<const Thing> thing) const;

	/**
	 * Returns the first index
	 * \returns the first index, if not implemented 0 is returned
	 */
	virtual size_t getFirstIndex() const;

	/**
	 * Returns the last index
	 * \returns the last index, if not implemented 0 is returned
	 */
	virtual size_t getLastIndex() const;

	/**
	 * Gets the object based on index
	 * \returns the object, returns nullptr if not found
	 */
	virtual std::shared_ptr<Thing> getThing(size_t index) const;

	/**
	 * Get the amount of items of a certain type
	 * \param itemId is the item type to the get the count of
	 * \param subType is the extra type an item can have such as
	 * charges/fluidtype, -1 means not used \returns the amount of items of the
	 * asked item type
	 */
	virtual uint32_t getItemTypeCount(uint16_t itemId, int32_t subType = -1) const;

	/**
	 * Get the amount of items of a all types
	 * \param countMap a map to put the itemID:count mapping in
	 * \returns a map mapping item id to count (same as first argument)
	 */
	virtual std::map<uint32_t, uint32_t>& getAllItemTypeCount(std::map<uint32_t, uint32_t>& countMap) const;

	/**
	 * Removes an object from the cylinder without sending to the client(s)
	 * \param thing is the object to add
	 */
	virtual void internalRemoveThing(std::shared_ptr<Thing> thing);

	/**
	 * Adds an object to the cylinder without sending to the client(s)
	 * \param thing is the object to add
	 */
	virtual void internalAddThing(std::shared_ptr<Thing> thing);

	/**
	 * Adds an object to the cylinder without sending to the client(s)
	 * \param thing is the object to add
	 * \param index points to the destination index (inventory slot/container
	 * position)
	 */
	virtual void internalAddThing(uint32_t index, std::shared_ptr<Thing> thing);

	virtual void startDecaying();
};

class VirtualCylinder final : public Cylinder
{
public:
	static std::shared_ptr<VirtualCylinder> virtualCylinder;

	virtual ReturnValue queryAdd(int32_t, std::shared_ptr<const Thing>, uint32_t, uint32_t,
	                             std::shared_ptr<Creature> = nullptr) const override
	{
		return RETURNVALUE_NOTPOSSIBLE;
	}
	virtual ReturnValue queryMaxCount(int32_t, std::shared_ptr<const Thing>, uint32_t, uint32_t&,
	                                  uint32_t) const override
	{
		return RETURNVALUE_NOTPOSSIBLE;
	}
	virtual ReturnValue queryRemove(std::shared_ptr<const Thing>, uint32_t, uint32_t,
	                                std::shared_ptr<Creature> = nullptr) const override
	{
		return RETURNVALUE_NOTPOSSIBLE;
	}
	virtual std::shared_ptr<Cylinder> queryDestination(int32_t&, std::shared_ptr<const Thing>, std::shared_ptr<Item>&,
	                                                   uint32_t&) override
	{
		return nullptr;
	}

	virtual void addThing(std::shared_ptr<Thing>) override {}
	virtual void addThing(int32_t, std::shared_ptr<Thing>) override {}
	virtual void updateThing(std::shared_ptr<Thing>, uint16_t, uint32_t) override {}
	virtual void replaceThing(uint32_t, std::shared_ptr<Thing>) override {}
	virtual void removeThing(std::shared_ptr<Thing>, uint32_t) override {}

	virtual void postAddNotification(std::shared_ptr<Thing>, std::shared_ptr<const Cylinder>, int32_t,
	                                 cylinderlink_t = LINK_OWNER) override
	{}
	virtual void postRemoveNotification(std::shared_ptr<Thing>, std::shared_ptr<const Cylinder>, int32_t,
	                                    cylinderlink_t = LINK_OWNER) override
	{}
};

#endif // FS_CYLINDER_H
