/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2018  Mark Samman <mark.samman@gmail.com>
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

#ifndef FS_TRASHHOLDER_H_BA162024D67B4D388147F5EE06F33098
#define FS_TRASHHOLDER_H_BA162024D67B4D388147F5EE06F33098

#include "item.h"
#include "cylinder.h"
#include "const.h"

class TrashHolder final : public Item, public Cylinder
{
	public:
		explicit TrashHolder(uint16_t itemId) : Item(itemId) {}

		TrashHolder* getTrashHolder() override {
			return this;
		}
		const TrashHolder* getTrashHolder() const override {
			return this;
		}

		//cylinder implementations
		ReturnValue queryAdd(int32_t index, const Thing& thing, uint32_t count, uint32_t flags, Creature* actor = nullptr) const override;
		ReturnValue queryMaxCount(int32_t index, const Thing& thing, uint32_t count, uint32_t& maxQueryCount, uint32_t flags) const override;
		ReturnValue queryRemove(const Thing& thing, uint32_t count, uint32_t flags) const override;
		Cylinder* queryDestination(int32_t& index, const Thing& thing, Item** destItem, uint32_t& flags) override;

		void addThing(Thing* thing) override;
		void addThing(int32_t index, Thing* thing) override;

		void updateThing(Thing* thing, uint16_t itemId, uint32_t count) override;
		void replaceThing(uint32_t index, Thing* thing) override;

		void removeThing(Thing* thing, uint32_t count) override;

		void postAddNotification(Thing* thing, const Cylinder* oldParent, int32_t index, cylinderlink_t link = LINK_OWNER) override;
		void postRemoveNotification(Thing* thing, const Cylinder* newParent, int32_t index, cylinderlink_t link = LINK_OWNER) override;
};

#endif
