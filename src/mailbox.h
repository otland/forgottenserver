// Copyright 2022 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_MAILBOX_H_D231C6BE8D384CAAA3AE410C1323F9DB
#define FS_MAILBOX_H_D231C6BE8D384CAAA3AE410C1323F9DB

#include "item.h"
#include "cylinder.h"
#include "const.h"

class Mailbox final : public Item, public Cylinder
{
	public:
		explicit Mailbox(uint16_t itemId) : Item(itemId) {}

		Mailbox* getMailbox() override {
			return this;
		}
		const Mailbox* getMailbox() const override {
			return this;
		}

		//cylinder implementations
		ReturnValue queryAdd(int32_t index, const Thing& thing, uint32_t count,
				uint32_t flags, Creature* actor = nullptr) const override;
		ReturnValue queryMaxCount(int32_t index, const Thing& thing, uint32_t count,
				uint32_t& maxQueryCount, uint32_t flags) const override;
		ReturnValue queryRemove(const Thing& thing, uint32_t count, uint32_t flags, Creature* actor = nullptr) const override;
		Cylinder* queryDestination(int32_t& index, const Thing& thing, Item** destItem,
				uint32_t& flags) override;

		void addThing(Thing* thing) override;
		void addThing(int32_t index, Thing* thing) override;

		void updateThing(Thing* thing, uint16_t itemId, uint32_t count) override;
		void replaceThing(uint32_t index, Thing* thing) override;

		void removeThing(Thing* thing, uint32_t count) override;

		void postAddNotification(Thing* thing, const Cylinder* oldParent, int32_t index, cylinderlink_t link = LINK_OWNER) override;
		void postRemoveNotification(Thing* thing, const Cylinder* newParent, int32_t index, cylinderlink_t link = LINK_OWNER) override;

	private:
		bool getReceiver(Item* item, std::string& name) const;
		bool sendItem(Item* item) const;

		static bool canSend(const Item* item);
};

#endif
