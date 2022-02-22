// Copyright 2022 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "otpch.h"

#include "storeinbox.h"

StoreInbox::StoreInbox(uint16_t type) : Container(type, 20, true, true) {}

ReturnValue StoreInbox::queryAdd(int32_t, const Thing& thing, uint32_t, uint32_t flags, Creature*) const
{
	const Item* item = thing.getItem();
	if (!item) {
		return RETURNVALUE_NOTPOSSIBLE;
	}

	if (item == this) {
		return RETURNVALUE_THISISIMPOSSIBLE;
	}

	if (!item->isPickupable()) {
		return RETURNVALUE_CANNOTPICKUP;
	}

	if (!hasBitSet(FLAG_NOLIMIT, flags)) {
		if (!item->isStoreItem()) {
			return RETURNVALUE_CANNOTMOVEITEMISNOTSTOREITEM;
		}

		const Container* container = item->getContainer();
		if (container && !container->empty()) {
			return RETURNVALUE_ITEMCANNOTBEMOVEDTHERE;
		}
	}

	return RETURNVALUE_NOERROR;
}

void StoreInbox::postAddNotification(Thing* thing, const Cylinder* oldParent, int32_t index, cylinderlink_t)
{
	if (parent != nullptr) {
		parent->postAddNotification(thing, oldParent, index, LINK_TOPPARENT);
	}
}

void StoreInbox::postRemoveNotification(Thing* thing, const Cylinder* newParent, int32_t index, cylinderlink_t)
{
	if (parent != nullptr) {
		parent->postRemoveNotification(thing, newParent, index, LINK_TOPPARENT);
	}
}

