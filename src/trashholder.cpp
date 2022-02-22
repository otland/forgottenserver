// Copyright 2022 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "otpch.h"

#include "trashholder.h"
#include "game.h"

extern Game g_game;

ReturnValue TrashHolder::queryAdd(int32_t, const Thing&, uint32_t, uint32_t, Creature*) const
{
	return RETURNVALUE_NOERROR;
}

ReturnValue TrashHolder::queryMaxCount(int32_t, const Thing&, uint32_t count, uint32_t& maxQueryCount, uint32_t) const
{
	maxQueryCount = std::max<uint32_t>(1, count);
	return RETURNVALUE_NOERROR;
}

ReturnValue TrashHolder::queryRemove(const Thing&, uint32_t, uint32_t, Creature* /*= nullptr*/) const
{
	return RETURNVALUE_NOTPOSSIBLE;
}

Cylinder* TrashHolder::queryDestination(int32_t&, const Thing&, Item**, uint32_t&)
{
	return this;
}

void TrashHolder::addThing(Thing* thing)
{
	return addThing(0, thing);
}

void TrashHolder::addThing(int32_t, Thing* thing)
{
	Item* item = thing->getItem();
	if (!item) {
		return;
	}

	if (item == this || !item->hasProperty(CONST_PROP_MOVEABLE)) {
		return;
	}

	const ItemType& it = Item::items[id];
	if (item->isHangable() && it.isGroundTile()) {
		Tile* tile = dynamic_cast<Tile*>(getParent());
		if (tile && tile->hasFlag(TILESTATE_SUPPORTS_HANGABLE)) {
			return;
		}
	}

	g_game.internalRemoveItem(item);

	if (it.magicEffect != CONST_ME_NONE) {
		g_game.addMagicEffect(getPosition(), it.magicEffect);
	}
}

void TrashHolder::updateThing(Thing*, uint16_t, uint32_t)
{
	//
}

void TrashHolder::replaceThing(uint32_t, Thing*)
{
	//
}

void TrashHolder::removeThing(Thing*, uint32_t)
{
	//
}

void TrashHolder::postAddNotification(Thing* thing, const Cylinder* oldParent, int32_t index, cylinderlink_t)
{
	getParent()->postAddNotification(thing, oldParent, index, LINK_PARENT);
}

void TrashHolder::postRemoveNotification(Thing* thing, const Cylinder* newParent, int32_t index, cylinderlink_t)
{
	getParent()->postRemoveNotification(thing, newParent, index, LINK_PARENT);
}
