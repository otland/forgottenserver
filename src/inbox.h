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
	ReturnValue queryAdd(int32_t index, const Thing& thing, uint32_t count, uint32_t flags,
	                     Creature* actor = nullptr) const override;

	void postAddNotification(Thing* thing, const Cylinder* oldParent, int32_t index,
	                         cylinderlink_t link = LINK_OWNER) override;
	void postRemoveNotification(Thing* thing, const Cylinder* newParent, int32_t index,
	                            cylinderlink_t link = LINK_OWNER) override;

	// Item implementations
	bool canRemove() const override { return false; }

	// Thing implementations
	bool hasParent() const override { return getParent(); }
	Cylinder* getParent() const override;
	Cylinder* getRealParent() const override { return parent; }
};

#endif // FS_INBOX_H
