/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2016  Mark Samman <mark.samman@gmail.com>
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

#ifndef FS_DEPOTLOCKER_H_53AD8E0606A34070B87F792611F4F3F8
#define FS_DEPOTLOCKER_H_53AD8E0606A34070B87F792611F4F3F8

#include "container.h"
#include "inbox.h"

class DepotLocker final : public Container
{
	public:
		explicit DepotLocker(uint16_t type);

		DepotLocker* getDepotLocker() final {
			return this;
		}
		const DepotLocker* getDepotLocker() const final {
			return this;
		}

		void removeInbox(Inbox* inbox);

		//serialization
		Attr_ReadValue readAttr(AttrTypes_t attr, PropStream& propStream) final;

		uint16_t getDepotId() const {
			return depotId;
		}
		void setDepotId(uint16_t depotId) {
			this->depotId = depotId;
		}

		//cylinder implementations
		ReturnValue queryAdd(int32_t index, const Thing& thing, uint32_t count,
				uint32_t flags, Creature* actor = nullptr) const final;

		void postAddNotification(Thing* thing, const Cylinder* oldParent, int32_t index, cylinderlink_t link = LINK_OWNER) final;
		void postRemoveNotification(Thing* thing, const Cylinder* newParent, int32_t index, cylinderlink_t link = LINK_OWNER) final;

		bool canRemove() const final {
			return false;
		}

	private:
		uint16_t depotId;
};

#endif

