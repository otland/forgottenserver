// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_TRASHHOLDER_H
#define FS_TRASHHOLDER_H

#include "item.h"

class TrashHolder final : public Item
{
public:
	explicit TrashHolder(uint16_t itemId) : Item{itemId} {}

	std::shared_ptr<TrashHolder> getTrashHolder() override
	{
		return std::static_pointer_cast<TrashHolder>(shared_from_this());
	}
	std::shared_ptr<const TrashHolder> getTrashHolder() const override
	{
		return std::static_pointer_cast<const TrashHolder>(shared_from_this());
	}

	std::shared_ptr<Thing> getReceiver() override final { return shared_from_this(); }
	std::shared_ptr<const Thing> getReceiver() const override final { return shared_from_this(); }

	ReturnValue queryAdd(int32_t, const std::shared_ptr<const Thing>&, uint32_t, uint32_t,
	                     const std::shared_ptr<Creature>& = nullptr) const override
	{
		return RETURNVALUE_NOERROR;
	}

	ReturnValue queryMaxCount(int32_t index, const std::shared_ptr<const Thing>& thing, uint32_t count,
	                          uint32_t& maxQueryCount, uint32_t flags) const override;

	std::shared_ptr<Thing> queryDestination(int32_t&, const std::shared_ptr<const Thing>&, std::shared_ptr<Item>&,
	                                        uint32_t&) override
	{
		return shared_from_this();
	}

	void addThing(const std::shared_ptr<Thing>& thing) override { return addThing(0, thing); }
	void addThing(int32_t index, const std::shared_ptr<Thing>& thing) override;

	void postAddNotification(const std::shared_ptr<Thing>& thing, const std::shared_ptr<const Thing>& oldParent,
	                         int32_t index, ReceiverLink_t link = LINK_OWNER) override;
	void postRemoveNotification(const std::shared_ptr<Thing>& thing, const std::shared_ptr<const Thing>& newParent,
	                            int32_t index, ReceiverLink_t link = LINK_OWNER) override;
};

#endif // FS_TRASHHOLDER_H
