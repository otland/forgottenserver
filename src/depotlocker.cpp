// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "otpch.h"

#include "depotlocker.h"

#include "inbox.h"

DepotLocker::DepotLocker(uint16_t type) : Container(type), depotId(0) {}

void DepotLocker::readAttr(AttrTypes_t attr, OTB::iterator& first, const OTB::iterator& last)
{
	switch (attr) {
		case ATTR_DEPOT_ID:
			depotId = OTB::read<uint16_t>(first, last);
			break;

		default:
			Item::readAttr(attr, first, last);
			break;
	}
}

ReturnValue DepotLocker::queryAdd(int32_t, const Thing&, uint32_t, uint32_t, Creature*) const
{
	return RETURNVALUE_NOTENOUGHROOM;
}

void DepotLocker::postAddNotification(Thing* thing, const Cylinder* oldParent, int32_t index, cylinderlink_t)
{
	if (const auto parent = getParent()) {
		parent->postAddNotification(thing, oldParent, index, LINK_PARENT);
	}
}

void DepotLocker::postRemoveNotification(Thing* thing, const Cylinder* newParent, int32_t index, cylinderlink_t)
{
	if (const auto parent = getParent()) {
		parent->postRemoveNotification(thing, newParent, index, LINK_PARENT);
	}
}

void DepotLocker::removeInbox(Inbox* inbox)
{
	auto cit = std::find(itemlist.begin(), itemlist.end(), inbox);
	if (cit == itemlist.end()) {
		return;
	}
	itemlist.erase(cit);
}
