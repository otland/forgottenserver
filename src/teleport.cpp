/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2014  Mark Samman <mark.samman@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "otpch.h"

#include "teleport.h"
#include "game.h"

extern Game g_game;

Teleport::Teleport(uint16_t _type) : Item(_type)
{
}

Teleport::~Teleport()
{
	//
}

Attr_ReadValue Teleport::readAttr(AttrTypes_t attr, PropStream& propStream)
{
	if (ATTR_TELE_DEST == attr) {
		if (!propStream.GET_USHORT(destPos.x) || !propStream.GET_USHORT(destPos.y) || !propStream.GET_UCHAR(destPos.z)) {
			return ATTR_READ_ERROR;
		}
		return ATTR_READ_CONTINUE;
	} else {
		return Item::readAttr(attr, propStream);
	}
}

bool Teleport::serializeAttr(PropWriteStream& propWriteStream) const
{
	bool ret = Item::serializeAttr(propWriteStream);
	propWriteStream.ADD_UCHAR(ATTR_TELE_DEST);
	propWriteStream.ADD_USHORT(destPos.x);
	propWriteStream.ADD_USHORT(destPos.y);
	propWriteStream.ADD_UCHAR(destPos.z);
	return ret;
}

ReturnValue Teleport::__queryAdd(int32_t, const Thing*, uint32_t, uint32_t, Creature*) const
{
	return RET_NOTPOSSIBLE;
}

ReturnValue Teleport::__queryMaxCount(int32_t, const Thing*, uint32_t, uint32_t&, uint32_t) const
{
	return RET_NOTPOSSIBLE;
}

ReturnValue Teleport::__queryRemove(const Thing*, uint32_t, uint32_t) const
{
	return RET_NOERROR;
}

Cylinder* Teleport::__queryDestination(int32_t&, const Thing*, Item**, uint32_t&)
{
	return this;
}

void Teleport::__addThing(Thing* thing)
{
	return __addThing(0, thing);
}

void Teleport::__addThing(int32_t, Thing* thing)
{
	Tile* destTile = g_game.getTile(destPos.x, destPos.y, destPos.z);
	if (!destTile) {
		return;
	}

	const ItemType& it = Item::items[getID()];

	if (Creature* creature = thing->getCreature()) {
		Position origPos = creature->getPosition();
		g_game.internalCreatureTurn(creature, origPos.x > destPos.x ? WEST : EAST);
		getTile()->moveCreature(creature, destTile);
		if (it.magicEffect != CONST_ME_NONE) {
			g_game.addMagicEffect(origPos, it.magicEffect);
			g_game.addMagicEffect(destTile->getPosition(), it.magicEffect);
		}
	} else if (Item* item = thing->getItem()) {
		if (it.magicEffect != CONST_ME_NONE) {
			g_game.addMagicEffect(item->getPosition(), it.magicEffect);
		}
		g_game.internalMoveItem(getTile(), destTile, INDEX_WHEREEVER, item, item->getItemCount(), nullptr);
		if (it.magicEffect != CONST_ME_NONE) {
			g_game.addMagicEffect(destTile->getPosition(), it.magicEffect);
		}
	}
}

void Teleport::__updateThing(Thing*, uint16_t, uint32_t)
{
	//
}

void Teleport::__replaceThing(uint32_t, Thing*)
{
	//
}

void Teleport::__removeThing(Thing*, uint32_t)
{
	//
}

void Teleport::postAddNotification(Thing* thing, const Cylinder* oldParent, int32_t index, cylinderlink_t)
{
	getParent()->postAddNotification(thing, oldParent, index, LINK_PARENT);
}

void Teleport::postRemoveNotification(Thing* thing, const Cylinder* newParent, int32_t index, bool isCompleteRemoval, cylinderlink_t)
{
	getParent()->postRemoveNotification(thing, newParent, index, isCompleteRemoval, LINK_PARENT);
}
