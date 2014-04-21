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

#include "otpch.h"

#include "inbox.h"
#include "tools.h"

Inbox::Inbox(uint16_t _type) :
	Container(_type)
{
	maxSize = 30;
	unlocked = false;
	pagination = true;
}

Inbox::~Inbox()
{
	//
}

ReturnValue Inbox::__queryAdd(int32_t, const Thing* thing, uint32_t,
                              uint32_t flags, Creature*) const
{
	if (!hasBitSet(FLAG_NOLIMIT, flags)) {
		return RET_CONTAINERNOTENOUGHROOM;
	}

	const Item* item = thing->getItem();
	if (!item) {
		return RET_NOTPOSSIBLE;
	}

	if (item == this) {
		return RET_THISISIMPOSSIBLE;
	}

	if (!item->isPickupable()) {
		return RET_CANNOTPICKUP;
	}

	return RET_NOERROR;
}

void Inbox::postAddNotification(Thing* thing, const Cylinder* oldParent, int32_t index, cylinderlink_t)
{
	Cylinder* parent = getParent();
	if (parent != nullptr) {
		parent->postAddNotification(thing, oldParent, index, LINK_PARENT);
	}
}

void Inbox::postRemoveNotification(Thing* thing, const Cylinder* newParent, int32_t index, bool isCompleteRemoval, cylinderlink_t)
{
	Cylinder* parent = getParent();
	if (parent != nullptr) {
		parent->postRemoveNotification(thing, newParent, index, isCompleteRemoval, LINK_PARENT);
	}
}

Cylinder* Inbox::getParent() const
{
	if (parent) {
		return parent->getParent();
	}
	return nullptr;
}
