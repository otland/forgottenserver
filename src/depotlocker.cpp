// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "otpch.h"

#include "depotlocker.h"

#include "inbox.h"

DepotLocker::DepotLocker(uint16_t type) : Container(type), depotId(0) {}

Attr_ReadValue DepotLocker::readAttr(AttrTypes_t attr, PropStream& propStream)
{
	if (attr == ATTR_DEPOT_ID) {
		if (!propStream.read<uint16_t>(depotId)) {
			return ATTR_READ_ERROR;
		}
		return ATTR_READ_CONTINUE;
	}
	return Item::readAttr(attr, propStream);
}

ReturnValue DepotLocker::queryAdd(int32_t, std::shared_ptr<const Thing>, uint32_t, uint32_t,
                                  std::shared_ptr<Creature>) const
{
	return RETURNVALUE_NOTENOUGHROOM;
}

void DepotLocker::postAddNotification(std::shared_ptr<Thing> thing, std::shared_ptr<const Cylinder> oldParent,
                                      int32_t index, cylinderlink_t)
{
	if (parent) {
		parent->postAddNotification(thing, oldParent, index, LINK_PARENT);
	}
}

void DepotLocker::postRemoveNotification(std::shared_ptr<Thing> thing, std::shared_ptr<const Cylinder> newParent,
                                         int32_t index, cylinderlink_t)
{
	if (parent) {
		parent->postRemoveNotification(thing, newParent, index, LINK_PARENT);
	}
}

void DepotLocker::removeInbox(std::shared_ptr<Inbox> inbox)
{
	auto cit = std::find(itemList.begin(), itemList.end(), inbox);
	if (cit == itemList.end()) {
		return;
	}
	itemList.erase(cit);
}
