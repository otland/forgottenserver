// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "otpch.h"

#include "inbox.h"

#include "tools.h"

ReturnValue Inbox::queryAdd(int32_t, const std::shared_ptr<const Thing>& thing, uint32_t, uint32_t flags,
                            const std::shared_ptr<Creature>&) const
{
	if (!hasBitSet(FLAG_NOLIMIT, flags)) {
		return RETURNVALUE_CONTAINERNOTENOUGHROOM;
	}

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

	return RETURNVALUE_NOERROR;
}

void Inbox::postAddNotification(const std::shared_ptr<Thing>& thing, const std::shared_ptr<const Thing>& oldParent,
                                int32_t index, cylinderlink_t)
{
	if (const auto& parent = getParent()) {
		parent->postAddNotification(thing, oldParent, index, LINK_PARENT);
	}
}

void Inbox::postRemoveNotification(const std::shared_ptr<Thing>& thing, const std::shared_ptr<const Thing>& newParent,
                                   int32_t index, cylinderlink_t)
{
	if (const auto& parent = getParent()) {
		parent->postRemoveNotification(thing, newParent, index, LINK_PARENT);
	}
}

std::shared_ptr<Thing> Inbox::getParent() const
{
	auto parent = Container::getParent();
	return parent ? parent->getParent() : nullptr;
}
