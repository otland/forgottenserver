// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_DEPOTLOCKER_H
#define FS_DEPOTLOCKER_H

#include "container.h"

class Inbox;

using DepotLocker_ptr = std::shared_ptr<DepotLocker>;

class DepotLocker final : public Container
{
public:
	explicit DepotLocker(uint16_t type);

	void removeInbox(Inbox* inbox);
	uint16_t getDepotId() const { return depotId; }
	void setDepotId(uint16_t depotId) { this->depotId = depotId; }

	// Serialization
	Attr_ReadValue readAttr(AttrTypes_t attr, PropStream& propStream) override;

	// Container implementations
	DepotLocker* getDepotLocker() override { return this; }
	const DepotLocker* getDepotLocker() const override { return this; }

	// Cylinder implementations
	ReturnValue queryAdd(int32_t index, const Thing& thing, uint32_t count, uint32_t flags,
	                     Creature* actor = nullptr) const override;

	void postAddNotification(Thing* thing, const Cylinder* oldParent, int32_t index,
	                         cylinderlink_t link = LINK_OWNER) override;
	void postRemoveNotification(Thing* thing, const Cylinder* newParent, int32_t index,
	                            cylinderlink_t link = LINK_OWNER) override;

	// Item implementations
	bool canRemove() const override { return false; }

private:
	uint16_t depotId;
};

#endif // FS_DEPOTLOCKER_H
