// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_TELEPORT_H
#define FS_TELEPORT_H

#include "item.h"

class Teleport final : public Item
{
public:
	explicit Teleport(uint16_t type) : Item{type} {};

	std::shared_ptr<Teleport> getTeleport() override { return std::static_pointer_cast<Teleport>(shared_from_this()); }
	std::shared_ptr<const Teleport> getTeleport() const override
	{
		return std::static_pointer_cast<const Teleport>(shared_from_this());
	}

	// serialization
	Attr_ReadValue readAttr(AttrTypes_t attr, PropStream& propStream) override;
	void serializeAttr(PropWriteStream& propWriteStream) const override;

	const Position& getDestPos() const { return destPos; }
	void setDestPos(const Position& pos) { destPos = pos; }

	std::shared_ptr<Thing> getReceiver() override final { return shared_from_this(); }
	std::shared_ptr<const Thing> getReceiver() const override final { return shared_from_this(); }

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
		return RETURNVALUE_NOERROR;
	}

	std::shared_ptr<Thing> queryDestination(int32_t&, const std::shared_ptr<const Thing>&, std::shared_ptr<Item>&,
	                                        uint32_t&) override
	{
		return shared_from_this();
	}

	void addThing(const std::shared_ptr<Thing>& thing) override { addThing(0, thing); }
	void addThing(int32_t index, const std::shared_ptr<Thing>& thing) override;

	void updateThing(const std::shared_ptr<Thing>&, uint16_t, uint32_t) override {}
	void replaceThing(uint32_t, const std::shared_ptr<Thing>&) override {}
	void removeThing(const std::shared_ptr<Thing>&, uint32_t) override {}

	void postAddNotification(const std::shared_ptr<Thing>& thing, const std::shared_ptr<const Thing>& oldParent,
	                         int32_t index, cylinderlink_t link = LINK_OWNER) override;
	void postRemoveNotification(const std::shared_ptr<Thing>& thing, const std::shared_ptr<const Thing>& newParent,
	                            int32_t index, cylinderlink_t link = LINK_OWNER) override;

private:
	Position destPos;
};

#endif // FS_TELEPORT_H
