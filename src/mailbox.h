// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_MAILBOX_H
#define FS_MAILBOX_H

#include "item.h"

class Mailbox final : public Item
{
public:
	explicit Mailbox(uint16_t itemId) : Item{itemId} {}

	std::shared_ptr<Mailbox> getMailbox() override { return std::static_pointer_cast<Mailbox>(shared_from_this()); }
	std::shared_ptr<const Mailbox> getMailbox() const override
	{
		return std::static_pointer_cast<const Mailbox>(shared_from_this());
	}

	ReturnValue queryAdd(int32_t index, const std::shared_ptr<const Thing>& thing, uint32_t count, uint32_t flags,
	                     const std::shared_ptr<Creature>& actor = nullptr) const override;
	ReturnValue queryMaxCount(int32_t index, const std::shared_ptr<const Thing>& thing, uint32_t count,
	                          uint32_t& maxQueryCount, uint32_t flags) const override;

	ReturnValue queryRemove(const std::shared_ptr<const Thing>&, uint32_t, uint32_t,
	                        const std::shared_ptr<Creature>& = nullptr) const override
	{
		return RETURNVALUE_NOTPOSSIBLE;
	}

	std::shared_ptr<Thing> queryDestination(int32_t&, const std::shared_ptr<const Thing>&, std::shared_ptr<Item>&,
	                                        uint32_t&) override
	{
		return shared_from_this();
	}

	void addThing(const std::shared_ptr<Thing>& thing) override { return addThing(0, thing); }
	void addThing(int32_t index, const std::shared_ptr<Thing>& thing) override;

	void updateThing(const std::shared_ptr<Thing>&, uint16_t, uint32_t) override {}
	void replaceThing(uint32_t, const std::shared_ptr<Thing>&) override {}
	void removeThing(const std::shared_ptr<Thing>&, uint32_t) override {}

	void postAddNotification(const std::shared_ptr<Thing>& thing, const std::shared_ptr<const Thing>& oldParent,
	                         int32_t index, cylinderlink_t link = LINK_OWNER) override;
	void postRemoveNotification(const std::shared_ptr<Thing>& thing, const std::shared_ptr<const Thing>& newParent,
	                            int32_t index, cylinderlink_t link = LINK_OWNER) override;
};

#endif // FS_MAILBOX_H
