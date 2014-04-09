/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2014  Mark Samman <mark.samman@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef FS_MAILBOX_H_D231C6BE8D384CAAA3AE410C1323F9DB
#define FS_MAILBOX_H_D231C6BE8D384CAAA3AE410C1323F9DB

#include "item.h"
#include "cylinder.h"
#include "const.h"


class Mailbox : public Item, public Cylinder
{
	public:
		Mailbox(uint16_t _type);
		~Mailbox();

		virtual Mailbox* getMailbox() {
			return this;
		}
		virtual const Mailbox* getMailbox() const {
			return this;
		}

		//cylinder implementations
		virtual ReturnValue __queryAdd(int32_t index, const Thing* thing, uint32_t count,
		                               uint32_t flags, Creature* actor = nullptr) const;
		virtual ReturnValue __queryMaxCount(int32_t index, const Thing* thing, uint32_t count,
		                                    uint32_t& maxQueryCount, uint32_t flags) const;
		virtual ReturnValue __queryRemove(const Thing* thing, uint32_t count, uint32_t flags) const;
		virtual Cylinder* __queryDestination(int32_t& index, const Thing* thing, Item** destItem,
		                                     uint32_t& flags);

		virtual void __addThing(Thing* thing);
		virtual void __addThing(int32_t index, Thing* thing);

		virtual void __updateThing(Thing* thing, uint16_t itemId, uint32_t count);
		virtual void __replaceThing(uint32_t index, Thing* thing);

		virtual void __removeThing(Thing* thing, uint32_t count);

		virtual void postAddNotification(Thing* thing, const Cylinder* oldParent, int32_t index, cylinderlink_t link = LINK_OWNER);
		virtual void postRemoveNotification(Thing* thing, const Cylinder* newParent, int32_t index, bool isCompleteRemoval, cylinderlink_t link = LINK_OWNER);

	private:
		bool getReceiver(Item* item, std::string& name) const;
		bool sendItem(Item* item) const;

		static bool canSend(const Item* item);
};

#endif
