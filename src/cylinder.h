/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
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

#ifndef FS_CYLINDER_H_54BBCEB2A5B7415DAD837E4D58115150
#define FS_CYLINDER_H_54BBCEB2A5B7415DAD837E4D58115150

#include "enums.h"
#include "thing.h"

class Item;
class Creature;

#define INDEX_WHEREEVER -1

enum cylinderflags_t {
	FLAG_NOLIMIT = 1,		//Bypass limits like capacity/container limits, blocking items/creatures etc.
	FLAG_IGNOREBLOCKITEM = 2,	//Bypass moveable blocking item checks
	FLAG_IGNOREBLOCKCREATURE = 4,	//Bypass creature checks
	FLAG_CHILDISOWNER = 8,		//Used by containers to query capacity of the carrier (player)
	FLAG_PATHFINDING = 16,		//An additional check is done for floor changing/teleport items
	FLAG_IGNOREFIELDDAMAGE = 32,	//Bypass field damage checks
	FLAG_IGNORENOTMOVEABLE = 64,	//Bypass check for movability
	FLAG_IGNOREAUTOSTACK = 128    //__queryDestination will not try to stack items together
};

enum cylinderlink_t {
	LINK_OWNER,
	LINK_PARENT,
	LINK_TOPPARENT,
	LINK_NEAR
};

class Cylinder : virtual public Thing
{
	public:
		/**
		  * Query if the cylinder can add an object
		  * \param index points to the destination index (inventory slot/container position)
			* -1 is a internal value and means add to a empty position, with no destItem
		  * \param thing the object to move/add
		  * \param count is the amount that we want to move/add
		  * \param flags if FLAG_CHILDISOWNER if set the query is from a child-cylinder (check cap etc.)
			* if FLAG_NOLIMIT is set blocking items/container limits is ignored
		  * \param actor the creature trying to add the thing
		  * \returns ReturnValue holds the return value
		  */
		virtual ReturnValue __queryAdd(int32_t index, const Thing* thing, uint32_t count,
		                               uint32_t flags, Creature* actor = nullptr) const = 0;

		/**
		  * Query the cylinder how much it can accept
		  * \param index points to the destination index (inventory slot/container position)
			* -1 is a internal value and means add to a empty position, with no destItem
		  * \param item the object to move/add
		  * \param count is the amount that we want to move/add
		  * \param maxQueryCount is the max amount that the cylinder can accept
		  * \param flags optional flags to modifiy the default behaviour
		  * \returns ReturnValue holds the return value
		  */
		virtual ReturnValue __queryMaxCount(int32_t index, const Thing* thing, uint32_t count, uint32_t& maxQueryCount,
		                                    uint32_t flags) const = 0;

		/**
		  * Query if the cylinder can remove an object
		  * \param item the object to move/remove
		  * \param count is the amount that we want to remove
		  * \param flags optional flags to modifiy the default behaviour
		  * \returns ReturnValue holds the return value
		  */
		virtual ReturnValue __queryRemove(const Thing* thing, uint32_t count, uint32_t flags) const = 0;

		/**
		  * Query the destination cylinder
		  * \param index points to the destination index (inventory slot/container position),
			* -1 is a internal value and means add to a empty position, with no destItem
			* this method can change the index to point to the new cylinder index
		  * \param destItem is the destination object
		  * \param flags optional flags to modifiy the default behaviour
			* this method can modifiy the flags
		  * \returns Cylinder returns the destination cylinder
		  */
		virtual Cylinder* __queryDestination(int32_t& index, const Thing* thing, Item** destItem,
		                                     uint32_t& flags) = 0;

		/**
		  * Add the object to the cylinder
		  * \param item is the object to add
		  */
		virtual void __addThing(Thing* thing) = 0;

		/**
		  * Add the object to the cylinder
		  * \param index points to the destination index (inventory slot/container position)
		  * \param item is the object to add
		  */
		virtual void __addThing(int32_t index, Thing* thing) = 0;

		/**
		  * Update the item count or type for an object
		  * \param thing is the object to update
		  * \param itemId is the new item id
		  * \param count is the new count value
		  */
		virtual void __updateThing(Thing* thing, uint16_t itemId, uint32_t count) = 0;

		/**
		  * Replace an object with a new
		  * \param index is the position to change (inventory slot/container position)
		  * \param thing is the object to update
		  */
		virtual void __replaceThing(uint32_t index, Thing* thing) = 0;

		/**
		  * Remove an object
		  * \param thing is the object to delete
		  * \param count is the new count value
		  */
		virtual void __removeThing(Thing* thing, uint32_t count) = 0;

		/**
		  * Is sent after an operation (move/add) to update internal values
		  * \param thing is the object that has been added
		  * \param index is the objects new index value
		  * \param link holds the relation the object has to the cylinder
		  */
		virtual void postAddNotification(Thing* thing, const Cylinder* oldParent, int32_t index, cylinderlink_t link = LINK_OWNER) = 0;

		/**
		  * Is sent after an operation (move/remove) to update internal values
		  * \param thing is the object that has been removed
		  * \param index is the previous index of the removed object
		  * \param isCompleteRemoval indicates if the item was completely removed or just partially (stackables)
		  * \param link holds the relation the object has to the cylinder
		  */
		virtual void postRemoveNotification(Thing* thing, const Cylinder* newParent, int32_t index, bool isCompleteRemoval, cylinderlink_t link = LINK_OWNER) = 0;

		/**
		  * Gets the index of an object
		  * \param thing the object to get the index value from
		  * \returns the index of the object, returns -1 if not found
		  */
		virtual int32_t __getIndexOfThing(const Thing* thing) const;

		/**
		  * Returns the first index
		  * \returns the first index, if not implemented -1 is returned
		  */
		virtual int32_t __getFirstIndex() const;

		/**
		  * Returns the last index
		  * \returns the last index, if not implemented -1 is returned
		  */
		virtual int32_t __getLastIndex() const;

		/**
		  * Gets the object based on index
		  * \returns the object, returns nullptr if not found
		  */
		virtual Thing* __getThing(size_t index) const;

		/**
		  * Get the amount of items of a certain type
		  * \param itemId is the item type to the get the count of
		  * \param subType is the extra type an item can have such as charges/fluidtype, -1 means not used
		  * \returns the amount of items of the asked item type
		  */
		virtual uint32_t __getItemTypeCount(uint16_t itemId, int32_t subType = -1) const;

		/**
		  * Get the amount of items of a all types
		  * \param countMap a map to put the itemID:count mapping in
		  * \param returns a map mapping item id to count (same as first argument)
		  */
		virtual std::map<uint32_t, uint32_t>& __getAllItemTypeCount(std::map<uint32_t, uint32_t>& countMap) const;

		/**
		  * Adds an object to the cylinder without sending to the client(s)
		  * \param thing is the object to add
		  */
		virtual void __internalAddThing(Thing* thing);

		/**
		  * Adds an object to the cylinder without sending to the client(s)
		  * \param thing is the object to add
		  * \param index points to the destination index (inventory slot/container position)
		  */
		virtual void __internalAddThing(uint32_t index, Thing* thing);

		virtual void __startDecaying();
};

class VirtualCylinder : public Cylinder
{
	public:
		static VirtualCylinder* virtualCylinder;

		virtual ReturnValue __queryAdd(int32_t index, const Thing* thing, uint32_t count,
		                               uint32_t flags, Creature* actor = nullptr) const {
			return RET_NOTPOSSIBLE;
		}
		virtual ReturnValue __queryMaxCount(int32_t index, const Thing* thing, uint32_t count,
		                                    uint32_t& maxQueryCount, uint32_t flags) const {
			return RET_NOTPOSSIBLE;
		}
		virtual ReturnValue __queryRemove(const Thing* thing, uint32_t count, uint32_t flags) const {
			return RET_NOTPOSSIBLE;
		}
		virtual Cylinder* __queryDestination(int32_t& index, const Thing* thing, Item** destItem,
		                                     uint32_t& flags) {
			return nullptr;
		}

		virtual void __addThing(Thing* thing) {}
		virtual void __addThing(int32_t index, Thing* thing) {}
		virtual void __updateThing(Thing* thing, uint16_t itemId, uint32_t count) {}
		virtual void __replaceThing(uint32_t index, Thing* thing) {}
		virtual void __removeThing(Thing* thing, uint32_t count) {}

		virtual void postAddNotification(Thing* thing, const Cylinder* oldParent, int32_t index, cylinderlink_t link = LINK_OWNER) {}
		virtual void postRemoveNotification(Thing* thing, const Cylinder* newParent, int32_t index, bool isCompleteRemoval,
		                                    cylinderlink_t link = LINK_OWNER) {}

		virtual bool isPushable() const {
			return false;
		}
		virtual int getThrowRange() const {
			return 1;
		}
		virtual std::string getDescription(int32_t lookDistance) const {
			return std::string();
		}

		virtual bool isRemoved() const {
			return false;
		}
};

#endif
