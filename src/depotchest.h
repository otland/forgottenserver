// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_DEPOTCHEST_H
#define FS_DEPOTCHEST_H

#include "container.h"

class DepotChest;
using DepotChest_ptr = std::shared_ptr<DepotChest>;

class DepotChest final : public Container
{
public:
	explicit DepotChest(uint16_t type, bool paginated = true);

	void setMaxDepotItems(uint32_t maxitems) { maxDepotItems = maxitems; }

	ReturnValue queryAdd(int32_t index, const Thing& thing, uint32_t count, uint32_t flags,
	                     Creature* actor = nullptr) const override;

	void postAddNotification(Thing* thing, const Thing* oldParent, int32_t index,
	                         ReceiverLink_t link = LINK_OWNER) override;
	void postRemoveNotification(Thing* thing, const Thing* newParent, int32_t index,
	                            ReceiverLink_t link = LINK_OWNER) override;

	bool canRemove() const override { return false; }
	Cylinder* getParent() const override;

private:
	uint32_t maxDepotItems = 0;
};

#endif // FS_DEPOTCHEST_H
