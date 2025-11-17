// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_TELEPORT_H
#define FS_TELEPORT_H

#include "item.h"

class Teleport final : public Item
{
public:
	explicit Teleport(uint16_t type) : Item(type) {};

	Thing* getReceiver() override final { return this; }
	const Thing* getReceiver() const override final { return this; }

	Teleport* getTeleport() override { return this; }
	const Teleport* getTeleport() const override { return this; }

	Attr_ReadValue readAttr(AttrTypes_t attr, PropStream& propStream) override;
	void serializeAttr(PropWriteStream& propWriteStream) const override;

	const Position& getDestPos() const { return destPos; }
	void setDestPos(const Position& pos) { destPos = pos; }

	ReturnValue queryRemove(const Thing&, uint32_t, uint32_t, Creature* = nullptr) const override
	{
		return RETURNVALUE_NOERROR;
	}
	Thing* queryDestination(int32_t&, const Thing&, Item**, uint32_t&) override { return this; }

	void addThing(Thing* thing) override { return addThing(0, thing); }
	void addThing(int32_t index, Thing* thing) override;

	void postAddNotification(Thing* thing, const Thing* oldParent, int32_t index,
	                         ReceiverLink_t link = LINK_OWNER) override;
	void postRemoveNotification(Thing* thing, const Thing* newParent, int32_t index,
	                            ReceiverLink_t link = LINK_OWNER) override;

private:
	Position destPos;
};

#endif // FS_TELEPORT_H
