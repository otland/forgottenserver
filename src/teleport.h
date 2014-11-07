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

#ifndef FS_TELEPORT_H_873B7F7F1DB24101A7ACFB54B25E0ABC
#define FS_TELEPORT_H_873B7F7F1DB24101A7ACFB54B25E0ABC

#include "tile.h"

class Teleport : public Item, public Cylinder
{
	public:
		Teleport(uint16_t _type);
		~Teleport();

		Teleport* getTeleport() final {
			return this;
		}
		const Teleport* getTeleport() const final {
			return this;
		}

		//serialization
		Attr_ReadValue readAttr(AttrTypes_t attr, PropStream& propStream) final;
		bool serializeAttr(PropWriteStream& propWriteStream) const final;

		const Position& getDestPos() const {
			return destPos;
		}
		void setDestPos(const Position& pos) {
			destPos = pos;
		}

		//cylinder implementations
		ReturnValue __queryAdd(int32_t index, const Thing* thing, uint32_t count,
			uint32_t flags, Creature* actor = nullptr) const final;
		ReturnValue __queryMaxCount(int32_t index, const Thing* thing, uint32_t count,
			uint32_t& maxQueryCount, uint32_t flags) const final;
		ReturnValue __queryRemove(const Thing* thing, uint32_t count, uint32_t flags) const final;
		Cylinder* __queryDestination(int32_t& index, const Thing* thing, Item** destItem,
			uint32_t& flags) final;

		void __addThing(Thing* thing) final;
		void __addThing(int32_t index, Thing* thing) final;

		void __updateThing(Thing* thing, uint16_t itemId, uint32_t count) final;
		void __replaceThing(uint32_t index, Thing* thing) final;

		void __removeThing(Thing* thing, uint32_t count) final;

		void postAddNotification(Thing* thing, const Cylinder* oldParent, int32_t index, cylinderlink_t link = LINK_OWNER) final;
		void postRemoveNotification(Thing* thing, const Cylinder* newParent, int32_t index, bool isCompleteRemoval, cylinderlink_t link = LINK_OWNER) final;

	private:
		Position destPos;
};

#endif
