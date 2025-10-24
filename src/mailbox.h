// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_MAILBOX_H
#define FS_MAILBOX_H

#include "item.h"

class Mailbox final : public Item
{
public:
	explicit Mailbox(uint16_t itemId) : Item(itemId) {}

	std::shared_ptr<Mailbox> getMailbox() override
	{
		assert(std::dynamic_pointer_cast<Mailbox>(shared_from_this()) != nullptr);
		return std::static_pointer_cast<Mailbox>(shared_from_this());
	}
	std::shared_ptr<const Mailbox> getMailbox() const override
	{
		assert(std::dynamic_pointer_cast<const Mailbox>(shared_from_this()) != nullptr);
		return std::static_pointer_cast<const Mailbox>(shared_from_this());
	}

	ReturnValue queryAdd(int32_t index, std::shared_ptr<const Thing> thing, uint32_t count, uint32_t flags,
	                     std::shared_ptr<Creature> actor = nullptr) const override;
	ReturnValue queryMaxCount(int32_t index, std::shared_ptr<const Thing> thing, uint32_t count,
	                          uint32_t& maxQueryCount, uint32_t flags) const override;
	ReturnValue queryRemove(std::shared_ptr<const Thing> thing, uint32_t count, uint32_t flags,
	                        std::shared_ptr<Creature> actor = nullptr) const override;
	std::shared_ptr<Thing> queryDestination(int32_t& index, std::shared_ptr<const Thing> thing,
	                                        std::shared_ptr<Item>& destItem, uint32_t& flags) override;

	void addThing(std::shared_ptr<Thing> thing) override;
	void addThing(int32_t index, std::shared_ptr<Thing> thing) override;

	void updateThing(std::shared_ptr<Thing> thing, uint16_t itemId, uint32_t count) override;
	void replaceThing(uint32_t index, std::shared_ptr<Thing> thing) override;

	void removeThing(std::shared_ptr<Thing> thing, uint32_t count) override;

	void postAddNotification(std::shared_ptr<Thing> thing, std::shared_ptr<const Thing> oldParent, int32_t index,
	                         cylinderlink_t link = LINK_OWNER) override;
	void postRemoveNotification(std::shared_ptr<Thing> thing, std::shared_ptr<const Thing> newParent, int32_t index,
	                            cylinderlink_t link = LINK_OWNER) override;

private:
	bool getReceiver(std::shared_ptr<Item> item, std::string& name) const;
	bool sendItem(std::shared_ptr<Item> item) const;
};

#endif // FS_MAILBOX_H
