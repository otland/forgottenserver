// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "otpch.h"

#include "trashholder.h"

#include "game.h"

extern Game g_game;

ReturnValue TrashHolder::queryAdd(int32_t, std::shared_ptr<const Thing>, uint32_t, uint32_t,
                                  std::shared_ptr<Creature>) const
{
	return RETURNVALUE_NOERROR;
}

ReturnValue TrashHolder::queryMaxCount(int32_t, std::shared_ptr<const Thing>, uint32_t count, uint32_t& maxQueryCount,
                                       uint32_t) const
{
	maxQueryCount = std::max<uint32_t>(1, count);
	return RETURNVALUE_NOERROR;
}

ReturnValue TrashHolder::queryRemove(std::shared_ptr<const Thing>, uint32_t, uint32_t,
                                     std::shared_ptr<Creature> /*= nullptr*/) const
{
	return RETURNVALUE_NOTPOSSIBLE;
}

std::shared_ptr<Thing> TrashHolder::queryDestination(int32_t&, std::shared_ptr<const Thing>, std::shared_ptr<Item>&,
                                                     uint32_t&)
{
	return shared_from_this();
}

void TrashHolder::addThing(std::shared_ptr<Thing> thing) { return addThing(0, thing); }

void TrashHolder::addThing(int32_t, std::shared_ptr<Thing> thing)
{
	auto item = thing->getItem();
	if (!item) {
		return;
	}

	if (item.get() == this || !item->hasProperty(CONST_PROP_MOVEABLE)) {
		return;
	}

	const ItemType& it = Item::items[id];
	if (item->isHangable() && it.isGroundTile()) {
		auto tile = Item::getTile();
		if (tile && tile->hasFlag(TILESTATE_SUPPORTS_HANGABLE)) {
			return;
		}
	}

	g_game.internalRemoveItem(item);

	if (it.magicEffect != CONST_ME_NONE) {
		g_game.addMagicEffect(Item::getPosition(), it.magicEffect);
	}
}

void TrashHolder::updateThing(std::shared_ptr<Thing>, uint16_t, uint32_t)
{
	//
}

void TrashHolder::replaceThing(uint32_t, std::shared_ptr<Thing>)
{
	//
}

void TrashHolder::removeThing(std::shared_ptr<Thing>, uint32_t)
{
	//
}

void TrashHolder::postAddNotification(std::shared_ptr<Thing> thing, std::shared_ptr<const Thing> oldParent,
                                      int32_t index, cylinderlink_t)
{
	Item::getParent()->postAddNotification(thing, oldParent, index, LINK_PARENT);
}

void TrashHolder::postRemoveNotification(std::shared_ptr<Thing> thing, std::shared_ptr<const Thing> newParent,
                                         int32_t index, cylinderlink_t)
{
	Item::getParent()->postRemoveNotification(thing, newParent, index, LINK_PARENT);
}
