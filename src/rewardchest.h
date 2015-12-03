/**
* The Forgotten Server - a free and open-source MMORPG server emulator
* Copyright (C) 2015  Mark Samman <mark.samman@gmail.com>
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

#ifndef FS_REWARDCHEST_H
#define FS_REWARDCHEST_H

#include "container.h"

class RewardChest final : public Container
{
public:
	explicit RewardChest(uint16_t _type);

	RewardChest* getRewardChest() final {
		return this;
	}
	const RewardChest* getRewardChest() const final {
		return this;
	}
	/*
	//serialization
	Attr_ReadValue readAttr(AttrTypes_t attr, PropStream& propStream) final;*/

	//cylinder implementations
	ReturnValue queryAdd(int32_t index, const Thing& thing, uint32_t count,
		uint32_t flags, Creature* actor = nullptr) const final;

	void postAddNotification(Thing* thing, const Cylinder* oldParent, int32_t index, cylinderlink_t link = LINK_OWNER) final;
	void postRemoveNotification(Thing* thing, const Cylinder* newParent, int32_t index, cylinderlink_t link = LINK_OWNER) final;

	bool canRemove() const final {
		return false;
	}
};

#endif

