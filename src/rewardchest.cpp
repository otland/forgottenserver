#include "otpch.h"

#include "rewardchest.h"

RewardChest::RewardChest(uint16_t _type) :
	Container(_type)
{
	maxSize = 32;
	unlocked = true;
	pagination = true;
}

/*
Attr_ReadValue RewardChest::readAttr(AttrTypes_t attr, PropStream& propStream)
{
	if (attr == ATTR_DEPOT_ID) {
		if (!propStream.read<uint16_t>(depotId)) {
			return ATTR_READ_ERROR;
		}
		return ATTR_READ_CONTINUE;
	}
	return Item::readAttr(attr, propStream);
}*/

ReturnValue RewardChest::queryAdd(int32_t index, const Thing& thing, uint32_t count,
	uint32_t flags, Creature* actor/* = nullptr*/) const
{
	if (actor) {
		return RETURNVALUE_NOTPOSSIBLE;
	}
		
	return RETURNVALUE_NOERROR;
}

void RewardChest::postAddNotification(Thing* thing, const Cylinder* oldParent, int32_t index, cylinderlink_t)
{
	if (parent != nullptr) {
		parent->postAddNotification(thing, oldParent, index, LINK_PARENT);
	}
}

void RewardChest::postRemoveNotification(Thing* thing, const Cylinder* newParent, int32_t index, cylinderlink_t)
{
	if (parent != nullptr) {
		parent->postRemoveNotification(thing, newParent, index, LINK_PARENT);
	}
}

/*
void RewardChest::removeInbox(Inbox* inbox)
{
	auto cit = std::find(itemlist.begin(), itemlist.end(), inbox);
	if (cit == itemlist.end()) {
		return;
	}
	itemlist.erase(cit);
}*/
