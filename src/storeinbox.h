// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_STOREINBOX_H
#define FS_STOREINBOX_H

#include "container.h"

class StoreInbox final : public Container, public std::enable_shared_from_this<StoreInbox>
{
public:
	explicit StoreInbox(uint16_t type);

	using std::enable_shared_from_this<StoreInbox>::shared_from_this;
	std::shared_ptr<StoreInbox> getStoreInbox() override { return shared_from_this(); }
	std::shared_ptr<const StoreInbox> getStoreInbox() const override { return shared_from_this(); }

	// Cylinder implementations
	ReturnValue queryAdd(int32_t index, std::shared_ptr<const Thing> thing, uint32_t count, uint32_t flags,
	                     std::shared_ptr<Creature> actor = nullptr) const override;

	void postAddNotification(std::shared_ptr<Thing> thing, std::shared_ptr<const Cylinder> oldParent, int32_t index,
	                         cylinderlink_t link = LINK_OWNER) override;
	void postRemoveNotification(std::shared_ptr<Thing> thing, std::shared_ptr<const Cylinder> newParent, int32_t index,
	                            cylinderlink_t link = LINK_OWNER) override;

	// Item implementations
	bool canRemove() const override { return false; }
};

#endif // FS_STOREINBOX_H
