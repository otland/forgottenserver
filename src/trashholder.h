// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_TRASHHOLDER_H
#define FS_TRASHHOLDER_H

#include "item.h"

class TrashHolder final : public Item, public Cylinder
{
public:
	explicit TrashHolder(uint16_t itemId) : Item{itemId} {}

	std::shared_ptr<TrashHolder> getTrashHolder() override { return std::static_pointer_cast<TrashHolder>(getItem()); }
	std::shared_ptr<const TrashHolder> getTrashHolder() const override
	{
		return std::static_pointer_cast<const TrashHolder>(getItem());
	}

	// cylinder implementations
	ReturnValue queryAdd(int32_t index, std::shared_ptr<const Thing> thing, uint32_t count, uint32_t flags,
	                     std::shared_ptr<Creature> actor = nullptr) const override;
	ReturnValue queryMaxCount(int32_t index, std::shared_ptr<const Thing> thing, uint32_t count,
	                          uint32_t& maxQueryCount, uint32_t flags) const override;
	ReturnValue queryRemove(std::shared_ptr<const Thing> thing, uint32_t count, uint32_t flags,
	                        std::shared_ptr<Creature> actor = nullptr) const override;
	std::shared_ptr<Cylinder> queryDestination(int32_t& index, std::shared_ptr<const Thing> thing,
	                                           std::shared_ptr<Item>& destItem, uint32_t& flags) override;

	void addThing(std::shared_ptr<Thing> thing) override;
	void addThing(int32_t index, std::shared_ptr<Thing> thing) override;

	void updateThing(std::shared_ptr<Thing> thing, uint16_t itemId, uint32_t count) override;
	void replaceThing(uint32_t index, std::shared_ptr<Thing> thing) override;

	void removeThing(std::shared_ptr<Thing> thing, uint32_t count) override;

	void postAddNotification(std::shared_ptr<Thing> thing, std::shared_ptr<const Cylinder> oldParent, int32_t index,
	                         cylinderlink_t link = LINK_OWNER) override;
	void postRemoveNotification(std::shared_ptr<Thing> thing, std::shared_ptr<const Cylinder> newParent, int32_t index,
	                            cylinderlink_t link = LINK_OWNER) override;
};

#endif // FS_TRASHHOLDER_H
