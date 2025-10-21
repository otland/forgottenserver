// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "otpch.h"

#include "inbox.h"

#include "tools.h"

Inbox::Inbox(uint16_t type) : Container(type, 30, false, true) {}

ReturnValue Inbox::queryAdd(int32_t, std::shared_ptr<const Thing> thing, uint32_t, uint32_t flags,
                            std::shared_ptr<Creature>) const
{
	if (!hasBitSet(FLAG_NOLIMIT, flags)) {
		return RETURNVALUE_CONTAINERNOTENOUGHROOM;
	}

	auto item = thing->getItem();
	if (!item) {
		return RETURNVALUE_NOTPOSSIBLE;
	}

	if (item.get() == this) {
		return RETURNVALUE_THISISIMPOSSIBLE;
	}

	if (!item->isPickupable()) {
		return RETURNVALUE_CANNOTPICKUP;
	}

	return RETURNVALUE_NOERROR;
}

void Inbox::postAddNotification(std::shared_ptr<Thing> thing, std::shared_ptr<const Cylinder> oldParent, int32_t index,
                                cylinderlink_t)
{
	auto parent = getParent();
	if (parent) {
		parent->postAddNotification(thing, oldParent, index, LINK_PARENT);
	}
}

void Inbox::postRemoveNotification(std::shared_ptr<Thing> thing, std::shared_ptr<const Cylinder> newParent,
                                   int32_t index, cylinderlink_t)
{
	auto parent = getParent();
	if (parent) {
		parent->postRemoveNotification(thing, newParent, index, LINK_PARENT);
	}
}

std::shared_ptr<Cylinder> Inbox::getParent() const
{
	if (parent) {
		return parent->getParent();
	}
	return nullptr;
}
