// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_INBOX_H
#define FS_INBOX_H

#include "container.h"

class Inbox;
using Inbox_ptr = std::shared_ptr<Inbox>;

class Inbox final : public Container
{
public:
	explicit Inbox(uint16_t type);

	// Cylinder implementations
	ReturnValue queryAdd(int32_t index, std::shared_ptr<const Thing> thing, uint32_t count, uint32_t flags,
	                     std::shared_ptr<Creature> actor = nullptr) const override;

	void postAddNotification(std::shared_ptr<Thing> thing, std::shared_ptr<const Cylinder> oldParent, int32_t index,
	                         cylinderlink_t link = LINK_OWNER) override;
	void postRemoveNotification(std::shared_ptr<Thing> thing, std::shared_ptr<const Cylinder> newParent, int32_t index,
	                            cylinderlink_t link = LINK_OWNER) override;

	// Item implementations
	bool canRemove() const override { return false; }
	std::shared_ptr<Cylinder> getParent() const override;
	std::shared_ptr<Cylinder> getRealParent() const override { return parent; }
};

#endif // FS_INBOX_H
