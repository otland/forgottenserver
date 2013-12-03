/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2013  Mark Samman <mark.samman@gmail.com>
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

class DepotLocker : public Container
{
	public:
		DepotLocker(uint16_t _type);
		~DepotLocker();

		DepotLocker* getDepotLocker() {
			return this;
		}
		const DepotLocker* getDepotLocker() const {
			return this;
		}

		void removeInbox(Inbox* inbox);

		//serialization
		Attr_ReadValue readAttr(AttrTypes_t attr, PropStream& propStream);

		uint32_t getDepotId() const {
			return depotId;
		}
		void setDepotId(uint32_t id) {
			depotId = id;
		}

		//cylinder implementations
		ReturnValue __queryAdd(int32_t index, const Thing* thing, uint32_t count,
		                       uint32_t flags, Creature* actor = nullptr) const;

		void postAddNotification(Thing* thing, const Cylinder* oldParent, int32_t index, cylinderlink_t link = LINK_OWNER);
		void postRemoveNotification(Thing* thing, const Cylinder* newParent, int32_t index, bool isCompleteRemoval, cylinderlink_t link = LINK_OWNER);

		//overrides
		bool canRemove() const {
			return false;
		}

	private:
		uint32_t depotId;
};

#endif

