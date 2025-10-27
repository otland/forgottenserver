// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_CYLINDER_H
#define FS_CYLINDER_H

#include "enums.h"
#include "thing.h"

class Creature;
class Item;

class VirtualCylinder final : public Thing
{
public:
	static std::shared_ptr<VirtualCylinder> virtualCylinder;

	ReturnValue queryAdd(int32_t, const std::shared_ptr<const Thing>&, uint32_t, uint32_t,
	                     const std::shared_ptr<Creature>& = nullptr) const override
	{
		return RETURNVALUE_NOTPOSSIBLE;
	}
	ReturnValue queryMaxCount(int32_t, const std::shared_ptr<const Thing>&, uint32_t, uint32_t&,
	                          uint32_t) const override
	{
		return RETURNVALUE_NOTPOSSIBLE;
	}
	ReturnValue queryRemove(const std::shared_ptr<const Thing>&, uint32_t, uint32_t,
	                        const std::shared_ptr<Creature>& = nullptr) const override
	{
		return RETURNVALUE_NOTPOSSIBLE;
	}
	std::shared_ptr<Thing> queryDestination(int32_t&, const std::shared_ptr<const Thing>&, std::shared_ptr<Item>&,
	                                        uint32_t&) override
	{
		return nullptr;
	}

	void addThing(const std::shared_ptr<Thing>&) override {}
	void addThing(int32_t, const std::shared_ptr<Thing>&) override {}
	void updateThing(const std::shared_ptr<Thing>&, uint16_t, uint32_t) override {}
	void replaceThing(uint32_t, const std::shared_ptr<Thing>&) override {}
	void removeThing(const std::shared_ptr<Thing>&, uint32_t) override {}

	void postAddNotification(const std::shared_ptr<Thing>&, const std::shared_ptr<const Thing>&, int32_t,
	                         cylinderlink_t = LINK_OWNER) override
	{}
	void postRemoveNotification(const std::shared_ptr<Thing>&, const std::shared_ptr<const Thing>&, int32_t,
	                            cylinderlink_t = LINK_OWNER) override
	{}
};

#endif // FS_CYLINDER_H
