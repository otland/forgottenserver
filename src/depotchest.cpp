// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "otpch.h"

#include "depotchest.h"

#include "tools.h"

ReturnValue DepotChest::queryAdd(int32_t index, const std::shared_ptr<const Thing>& thing, uint32_t count,
                                 uint32_t flags, const std::shared_ptr<Creature>& actor /* = nullptr*/) const
{
	const auto& item = thing->getItem();
	if (!item) {
		return RETURNVALUE_NOTPOSSIBLE;
	}

	bool skipLimit = hasBitSet(FLAG_NOLIMIT, flags);
	if (!skipLimit) {
		int32_t addCount = 0;

		if ((item->isStackable() && item->getItemCount() != count)) {
			addCount = 1;
		}

		if (item->getTopParent().get() != this) {
			if (const auto& container = item->getContainer()) {
				addCount = container->getItemHoldingCount() + 1;
			} else {
				addCount = 1;
			}
		}

		if (getItemHoldingCount() + addCount > maxDepotItems) {
			return RETURNVALUE_DEPOTISFULL;
		}
	}

	return Container::queryAdd(index, thing, count, flags, actor);
}

void DepotChest::postAddNotification(const std::shared_ptr<Thing>& thing, const std::shared_ptr<const Thing>& oldParent,
                                     int32_t index, cylinderlink_t)
{
	if (const auto& parent = getParent()) {
		parent->postAddNotification(thing, oldParent, index, LINK_PARENT);
	}
}

void DepotChest::postRemoveNotification(const std::shared_ptr<Thing>& thing,
                                        const std::shared_ptr<const Thing>& newParent, int32_t index, cylinderlink_t)
{
	if (const auto& parent = getParent()) {
		parent->postRemoveNotification(thing, newParent, index, LINK_PARENT);
	}
}

std::shared_ptr<Thing> DepotChest::getParent() const
{
	auto parent = Container::getParent();
	return parent ? parent->getParent() : nullptr;
}
