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

class VirtualCylinder final : public Thing
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
	virtual std::shared_ptr<Thing> queryDestination(int32_t&, std::shared_ptr<const Thing>, std::shared_ptr<Item>&,
	                                                uint32_t&) override
	{
		return nullptr;
	}

	virtual void addThing(std::shared_ptr<Thing>) override {}
	virtual void addThing(int32_t, std::shared_ptr<Thing>) override {}
	virtual void updateThing(std::shared_ptr<Thing>, uint16_t, uint32_t) override {}
	virtual void replaceThing(uint32_t, std::shared_ptr<Thing>) override {}
	virtual void removeThing(std::shared_ptr<Thing>, uint32_t) override {}

	virtual void postAddNotification(std::shared_ptr<Thing>, std::shared_ptr<const Thing>, int32_t,
	                                 cylinderlink_t = LINK_OWNER) override
	{}
	virtual void postRemoveNotification(std::shared_ptr<Thing>, std::shared_ptr<const Thing>, int32_t,
	                                    cylinderlink_t = LINK_OWNER) override
	{}
};

#endif // FS_CYLINDER_H
