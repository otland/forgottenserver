// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_MAILBOX_H
#define FS_MAILBOX_H

#include "item.h"

class Mailbox final : public Item
{
public:
	explicit Mailbox(uint16_t itemId) : Item(itemId) {}

	Thing* getReceiver() override final { return this; }
	const Thing* getReceiver() const override final { return this; }

	Mailbox* getMailbox() override { return this; }
	const Mailbox* getMailbox() const override { return this; }

	ReturnValue queryAdd(int32_t index, const Thing& thing, uint32_t count, uint32_t flags,
	                     Creature* actor = nullptr) const override;
	ReturnValue queryMaxCount(int32_t index, const Thing& thing, uint32_t count, uint32_t& maxQueryCount,
	                          uint32_t flags) const override;
	Thing* queryDestination(int32_t&, const Thing&, Item**, uint32_t&) override { return this; }

	void addThing(Thing* thing) override { return addThing(0, thing); }
	void addThing(int32_t index, Thing* thing) override;

	void postAddNotification(Thing* thing, const Thing* oldParent, int32_t index,
	                         ReceiverLink_t link = LINK_OWNER) override;
	void postRemoveNotification(Thing* thing, const Thing* newParent, int32_t index,
	                            ReceiverLink_t link = LINK_OWNER) override;

private:
	bool getReceiver(Item* item, std::string& name) const;
	bool sendItem(Item* item) const;

	static bool canSend(const Item* item);
};

#endif // FS_MAILBOX_H
