// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_STOREINBOX_H
#define FS_STOREINBOX_H

#include "container.h"

class StoreInbox final : public Container
{
public:
	explicit StoreInbox(uint16_t type) : Container{type, 20, true, true} {}

	std::shared_ptr<StoreInbox> getStoreInbox() override
	{
		return std::static_pointer_cast<StoreInbox>(shared_from_this());
	}
	std::shared_ptr<const StoreInbox> getStoreInbox() const override
	{
		return std::static_pointer_cast<const StoreInbox>(shared_from_this());
	}

	ReturnValue queryAdd(int32_t index, const std::shared_ptr<const Thing>& thing, uint32_t count, uint32_t flags,
	                     const std::shared_ptr<Creature>& actor = nullptr) const override;

	void postAddNotification(const std::shared_ptr<Thing>& thing, const std::shared_ptr<const Thing>& oldParent,
	                         int32_t index, ReceiverLink_t link = LINK_OWNER) override;
	void postRemoveNotification(const std::shared_ptr<Thing>& thing, const std::shared_ptr<const Thing>& newParent,
	                            int32_t index, ReceiverLink_t link = LINK_OWNER) override;

	// Item implementations
	bool canRemove() const override { return false; }
};

#endif // FS_STOREINBOX_H
