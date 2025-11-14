// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_DEPOTLOCKER_H
#define FS_DEPOTLOCKER_H

#include "container.h"

class Inbox;

class DepotLocker final : public Container
{
public:
	explicit DepotLocker(uint16_t type) : Container{type}, depotId{0} {}

	void removeInbox(const std::shared_ptr<Inbox>& inbox);
	uint16_t getDepotId() const { return depotId; }
	void setDepotId(uint16_t depotId) { this->depotId = depotId; }

	// Serialization
	Attr_ReadValue readAttr(AttrTypes_t attr, PropStream& propStream) override;

	std::shared_ptr<DepotLocker> getDepotLocker() override
	{
		return std::static_pointer_cast<DepotLocker>(shared_from_this());
	}
	std::shared_ptr<const DepotLocker> getDepotLocker() const override
	{
		return std::static_pointer_cast<const DepotLocker>(shared_from_this());
	}

	ReturnValue queryAdd(int32_t, const std::shared_ptr<const Thing>&, uint32_t, uint32_t,
	                     const std::shared_ptr<Creature>& = nullptr) const override
	{
		return RETURNVALUE_NOTENOUGHROOM;
	}

	void postAddNotification(const std::shared_ptr<Thing>& thing, const std::shared_ptr<const Thing>& oldParent,
	                         int32_t index, ReceiverLink_t link = LINK_OWNER) override;
	void postRemoveNotification(const std::shared_ptr<Thing>& thing, const std::shared_ptr<const Thing>& newParent,
	                            int32_t index, ReceiverLink_t link = LINK_OWNER) override;

	// Item implementations
	bool canRemove() const override { return false; }

private:
	uint16_t depotId;
};

#endif // FS_DEPOTLOCKER_H
