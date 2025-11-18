// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_THING_H
#define FS_THING_H

#include "enums.h"

class Container;
class Creature;
class Item;
class Tile;
struct Position;

inline constexpr int32_t INDEX_WHEREEVER = -1;

enum ReceiverFlag_t
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

enum ReceiverLink_t
{
	LINK_OWNER,
	LINK_PARENT,
	LINK_TOPPARENT,
	LINK_NEAR,
};

class Thing
{
public:
	constexpr Thing() = default;
	virtual ~Thing() = default;

	// non-copyable
	Thing(const Thing&) = delete;
	Thing& operator=(const Thing&) = delete;

	virtual bool hasParent() const { return false; }
	virtual Thing* getParent() const { return nullptr; }
	virtual Thing* getRealParent() const { return getParent(); }

	virtual void setParent(Thing*)
	{
		//
	}

	virtual const Position& getPosition() const;
	virtual int32_t getThrowRange() const = 0;
	virtual bool isPushable() const = 0;

	virtual Tile* getTile() { return nullptr; }
	virtual const Tile* getTile() const { return nullptr; }
	virtual Item* getItem() { return nullptr; }
	virtual const Item* getItem() const { return nullptr; }
	virtual Creature* getCreature() { return nullptr; }
	virtual const Creature* getCreature() const { return nullptr; }

	virtual bool isRemoved() const { return true; }

	virtual Thing* getReceiver() { return nullptr; }
	virtual const Thing* getReceiver() const { return nullptr; }

	/**
	 * Query if the thing can add an object
	 * \param index points to the destination index (inventory slot/container
	 * position) -1 is a internal value and means add to a empty position, with
	 * no destItem \param thing the object to move/add \param count is the
	 * amount that we want to move/add \param flags if FLAG_CHILDISOWNER if set
	 * the query is from a child-thing (check cap etc.) if FLAG_NOLIMIT is
	 * set blocking items/container limits is ignored \param actor the creature
	 * trying to add the thing \returns ReturnValue holds the return value
	 */
	virtual ReturnValue queryAdd(int32_t, const Thing&, uint32_t, uint32_t, Creature* = nullptr) const
	{
		return RETURNVALUE_NOTPOSSIBLE;
	}

	/**
	 * Query the thing how much it can accept
	 * \param index points to the destination index (inventory slot/container
	 * position) -1 is a internal value and means add to a empty position, with
	 * no destItem \param thing the object to move/add \param count is the
	 * amount that we want to move/add \param maxQueryCount is the max amount
	 * that the thing can accept \param flags optional flags to modify the
	 * default behaviour \returns ReturnValue holds the return value
	 */
	virtual ReturnValue queryMaxCount(int32_t, const Thing&, uint32_t, uint32_t&, uint32_t) const
	{
		return RETURNVALUE_NOTPOSSIBLE;
	}

	/**
	 * Query if the thing can remove an object
	 * \param thing the object to move/remove
	 * \param count is the amount that we want to remove
	 * \param flags optional flags to modify the default behaviour
	 * \returns ReturnValue holds the return value
	 */
	virtual ReturnValue queryRemove(const Thing&, uint32_t, uint32_t, Creature* = nullptr) const
	{
		return RETURNVALUE_NOTPOSSIBLE;
	}

	/**
	 * Query the destination thing
	 * \param index points to the destination index (inventory slot/container
	 * position), -1 is a internal value and means add to a empty position, with
	 * no destItem this method can change the index to point to the new thing
	 * index \param destItem is the destination object \param flags optional
	 * flags to modify the default behaviour this method can modify the flags
	 * \returns Thing returns the destination thing
	 */
	virtual Thing* queryDestination(int32_t&, const Thing&, Item**, uint32_t&) { return nullptr; }

	/**
	 * Add the object to the thing
	 * \param thing is the object to add
	 */
	virtual void addThing(Thing*) {}

	/**
	 * Add the object to the thing
	 * \param index points to the destination index (inventory slot/container
	 * position) \param thing is the object to add
	 */
	virtual void addThing(int32_t, Thing*) {}

	/**
	 * Update the item count or type for an object
	 * \param thing is the object to update
	 * \param itemId is the new item id
	 * \param count is the new count value
	 */
	virtual void updateThing(Thing*, uint16_t, uint32_t) {}

	/**
	 * Replace an object with a new
	 * \param index is the position to change (inventory slot/container
	 * position) \param thing is the object to update
	 */
	virtual void replaceThing(uint32_t, Thing*) {}

	/**
	 * Remove an object
	 * \param thing is the object to delete
	 * \param count is the new count value
	 */
	virtual void removeThing(Thing*, uint32_t) {}

	/**
	 * Is sent after an operation (move/add) to update internal values
	 * \param thing is the object that has been added
	 * \param index is the objects new index value
	 * \param link holds the relation the object has to the thing
	 */
	virtual void postAddNotification(Thing*, const Thing*, int32_t, ReceiverLink_t = LINK_OWNER) {}

	/**
	 * Is sent after an operation (move/remove) to update internal values
	 * \param thing is the object that has been removed
	 * \param index is the previous index of the removed object
	 * \param link holds the relation the object has to the thing
	 */
	virtual void postRemoveNotification(Thing*, const Thing*, int32_t, ReceiverLink_t = LINK_OWNER) {}

	/**
	 * Gets the index of an object
	 * \param thing the object to get the index value from
	 * \returns the index of the object, returns -1 if not found
	 */
	virtual int32_t getThingIndex(const Thing*) const { return -1; }

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
	virtual Thing* getThing(size_t) const { return nullptr; }

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
	 * Removes an object from the thing without sending to the client(s)
	 * \param thing is the object to add
	 */
	virtual void internalRemoveThing(Thing*) {}

	/**
	 * Adds an object to the thing without sending to the client(s)
	 * \param thing is the object to add
	 */
	virtual void internalAddThing(Thing*) {}

	/**
	 * Adds an object to the thing without sending to the client(s)
	 * \param thing is the object to add
	 * \param index points to the destination index (inventory slot/container
	 * position)
	 */
	virtual void internalAddThing(uint32_t, Thing*) {}
};

#endif // FS_THING_H
