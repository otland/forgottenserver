/**
 * The Forgotten Server - a server application for the MMORPG Tibia
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

#include "otpch.h"

#include "inbox.h"
#include "tools.h"

Inbox::Inbox(uint16_t _type) :
	Container(_type)
{
	maxSize = 30;
}

Inbox::~Inbox()
{
	//
}

ReturnValue Inbox::__queryAdd(int32_t index, const Thing* thing, uint32_t count,
                              uint32_t flags, Creature* actor/* = NULL*/) const
{
	bool skipLimit = hasBitSet(FLAG_NOLIMIT, flags);

	if (!skipLimit) {
		return RET_CONTAINERNOTENOUGHROOM;
	}

	return Container::__queryAdd(index, thing, count, flags, actor);
}

void Inbox::postAddNotification(Thing* thing, const Cylinder* oldParent, int32_t index, cylinderlink_t link /*= LINK_OWNER*/)
{
	if (getParent() != NULL) {
		getParent()->postAddNotification(thing, oldParent, index, LINK_PARENT);
	}
}

void Inbox::postRemoveNotification(Thing* thing, const Cylinder* newParent, int32_t index, bool isCompleteRemoval, cylinderlink_t link /*= LINK_OWNER*/)
{
	if (getParent() != NULL) {
		getParent()->postRemoveNotification(thing, newParent, index, isCompleteRemoval, LINK_PARENT);
	}
}
