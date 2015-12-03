#include "otpch.h"

#include "reward.h"


Reward::Reward() :
	Container(ITEM_REWARD_CONTAINER)
{
	maxSize = 32;
	unlocked = true;
	pagination = true;
}

ReturnValue Reward::queryAdd(int32_t, const Thing& thing, uint32_t,
	uint32_t, Creature* actor/* = nullptr*/) const
{
	if (actor) {
		return RETURNVALUE_NOTPOSSIBLE;
	}

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
	return RETURNVALUE_NOERROR;
}

void Reward::postAddNotification(Thing* thing, const Cylinder* oldParent, int32_t index, cylinderlink_t)
{
	Cylinder* parent = getParent();
	if (parent != nullptr) {
		parent->postAddNotification(thing, oldParent, index, LINK_PARENT);
	}
}

void Reward::postRemoveNotification(Thing* thing, const Cylinder* newParent, int32_t index, cylinderlink_t)
{
	Cylinder* parent = getParent();
	if (parent != nullptr) {
		parent->postRemoveNotification(thing, newParent, index, LINK_PARENT);
	}
}

Cylinder* Reward::getParent() const
{
	if (parent) {
		return parent->getParent();
	}
	return nullptr;
}