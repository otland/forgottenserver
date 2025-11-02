// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "otpch.h"

#include "storeinbox.h"

ReturnValue StoreInbox::queryAdd(int32_t, const std::shared_ptr<const Thing>& thing, uint32_t, uint32_t flags,
                                 const std::shared_ptr<Creature>&) const
{
	const auto& item = thing->getItem();
	if (!item) {
		return RETURNVALUE_NOTPOSSIBLE;
	}

	if (item.get() == this) {
		return RETURNVALUE_THISISIMPOSSIBLE;
	}

	if (!item->isPickupable()) {
		return RETURNVALUE_CANNOTPICKUP;
	}

	if (!hasBitSet(FLAG_NOLIMIT, flags)) {
		if (!item->isStoreItem()) {
			return RETURNVALUE_CANNOTMOVEITEMISNOTSTOREITEM;
		}

		if (const auto& container = item->getContainer()) {
			if (!container->empty()) {
				return RETURNVALUE_ITEMCANNOTBEMOVEDTHERE;
			}
		}
	}

	return RETURNVALUE_NOERROR;
}

void StoreInbox::postAddNotification(const std::shared_ptr<Thing>& thing, const std::shared_ptr<const Thing>& oldParent,
                                     int32_t index, cylinderlink_t)
{
	if (const auto& parent = getParent()) {
		parent->postAddNotification(thing, oldParent, index, LINK_TOPPARENT);
	}
}

void StoreInbox::postRemoveNotification(const std::shared_ptr<Thing>& thing,
                                        const std::shared_ptr<const Thing>& newParent, int32_t index, cylinderlink_t)
{
	if (const auto& parent = getParent()) {
		parent->postRemoveNotification(thing, newParent, index, LINK_TOPPARENT);
	}
}
