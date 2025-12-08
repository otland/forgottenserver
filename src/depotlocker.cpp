// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "otpch.h"

#include "depotlocker.h"

#include "inbox.h"

void DepotLocker::readAttr(AttrTypes_t attr, OTB::iterator& first, const OTB::iterator& last)
{
	if (attr == ATTR_DEPOT_ID) {
		depotId = OTB::read<uint16_t>(first, last);
		return;
	}

	Item::readAttr(attr, first, last);
}

void DepotLocker::postAddNotification(const std::shared_ptr<Thing>& thing,
                                      const std::shared_ptr<const Thing>& oldParent, int32_t index, ReceiverLink_t)
{
	if (const auto& parent = getParent()) {
		parent->postAddNotification(thing, oldParent, index, LINK_PARENT);
	}
}

void DepotLocker::postRemoveNotification(const std::shared_ptr<Thing>& thing,
                                         const std::shared_ptr<const Thing>& newParent, int32_t index, ReceiverLink_t)
{
	if (const auto& parent = getParent()) {
		parent->postRemoveNotification(thing, newParent, index, LINK_PARENT);
	}
}

void DepotLocker::removeInbox(const std::shared_ptr<Inbox>& inbox)
{
	auto cit = std::find(itemList.begin(), itemList.end(), inbox);
	if (cit == itemList.end()) {
		return;
	}
	itemList.erase(cit);
}
