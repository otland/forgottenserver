// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "otpch.h"

#include "trashholder.h"

#include "game.h"

extern Game g_game;

ReturnValue TrashHolder::queryMaxCount(int32_t, const std::shared_ptr<const Thing>&, uint32_t count,
                                       uint32_t& maxQueryCount, uint32_t) const
{
	maxQueryCount = std::max<uint32_t>(1, count);
	return RETURNVALUE_NOERROR;
}

void TrashHolder::addThing(int32_t, const std::shared_ptr<Thing>& thing)
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
		auto tile = getTile();
		if (tile && tile->hasFlag(TILESTATE_SUPPORTS_HANGABLE)) {
			return;
		}
	}

	g_game.internalRemoveItem(item);

	if (it.magicEffect != CONST_ME_NONE) {
		g_game.addMagicEffect(getPosition(), it.magicEffect);
	}
}

void TrashHolder::postAddNotification(const std::shared_ptr<Thing>& thing,
                                      const std::shared_ptr<const Thing>& oldParent, int32_t index, cylinderlink_t)
{
	getParent()->postAddNotification(thing, oldParent, index, LINK_PARENT);
}

void TrashHolder::postRemoveNotification(const std::shared_ptr<Thing>& thing,
                                         const std::shared_ptr<const Thing>& newParent, int32_t index, cylinderlink_t)
{
	getParent()->postRemoveNotification(thing, newParent, index, LINK_PARENT);
}
