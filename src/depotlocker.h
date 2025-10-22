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

	void removeInbox(std::shared_ptr<Inbox> inbox);
	uint16_t getDepotId() const { return depotId; }
	void setDepotId(uint16_t depotId) { this->depotId = depotId; }

	// Serialization
	Attr_ReadValue readAttr(AttrTypes_t attr, PropStream& propStream) override;

	std::shared_ptr<DepotLocker> getDepotLocker() override
	{
		return std::static_pointer_cast<DepotLocker>(Item::getItem());
	}
	std::shared_ptr<const DepotLocker> getDepotLocker() const override
	{
		return std::static_pointer_cast<const DepotLocker>(Item::getItem());
	}

	// Cylinder implementations
	ReturnValue queryAdd(int32_t index, std::shared_ptr<const Thing> thing, uint32_t count, uint32_t flags,
	                     std::shared_ptr<Creature> actor = nullptr) const override;

	void postAddNotification(std::shared_ptr<Thing> thing, std::shared_ptr<const Cylinder> oldParent, int32_t index,
	                         cylinderlink_t link = LINK_OWNER) override;
	void postRemoveNotification(std::shared_ptr<Thing> thing, std::shared_ptr<const Cylinder> newParent, int32_t index,
	                            cylinderlink_t link = LINK_OWNER) override;

	// Item implementations
	bool canRemove() const override { return false; }

private:
	uint16_t depotId;
};

#endif // FS_DEPOTLOCKER_H
