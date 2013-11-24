/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2013  Mark Samman <mark.samman@gmail.com>
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
	destPos.x = 0;
	destPos.y = 0;
	destPos.z = 0;
}

Teleport::~Teleport()
{
	//
}

Attr_ReadValue Teleport::readAttr(AttrTypes_t attr, PropStream& propStream)
{
	if (ATTR_TELE_DEST == attr) {
		TeleportDest* tele_dest;
		if (!propStream.GET_STRUCT(tele_dest)) {
			return ATTR_READ_ERROR;
		}

		setDestPos(Position(tele_dest->_x, tele_dest->_y, tele_dest->_z));
		return ATTR_READ_CONTINUE;
	} else {
		return Item::readAttr(attr, propStream);
	}
}

bool Teleport::serializeAttr(PropWriteStream& propWriteStream) const
{
	bool ret = Item::serializeAttr(propWriteStream);

	propWriteStream.ADD_UCHAR(ATTR_TELE_DEST);

	TeleportDest tele_dest;
	tele_dest._x = destPos.x;
	tele_dest._y = destPos.y;
	tele_dest._z = (uint8_t)destPos.z;
	propWriteStream.ADD_VALUE(tele_dest);

	return ret;
}

ReturnValue Teleport::__queryAdd(int32_t index, const Thing* thing, uint32_t count,
                                 uint32_t flags, Creature* actor/* = nullptr*/) const
{
	return RET_NOTPOSSIBLE;
}

ReturnValue Teleport::__queryMaxCount(int32_t index, const Thing* thing, uint32_t count,
                                      uint32_t& maxQueryCount, uint32_t flags) const
{
	return RET_NOTPOSSIBLE;
}

ReturnValue Teleport::__queryRemove(const Thing* thing, uint32_t count, uint32_t flags) const
{
	return RET_NOERROR;
}

Cylinder* Teleport::__queryDestination(int32_t& index, const Thing* thing, Item** destItem,
                                       uint32_t& flags)
{
	return this;
}

void Teleport::__addThing(Thing* thing)
{
	return __addThing(0, thing);
}

void Teleport::__addThing(int32_t index, Thing* thing)
{
	Tile* destTile = g_game.getTile(destPos.x, destPos.y, destPos.z);
	if (!destTile) {
		return;
	}

	if (Creature* creature = thing->getCreature()) {
		Position origPos = creature->getPosition();
		g_game.internalCreatureTurn(creature, origPos.x > destPos.x ? WEST : EAST);
		getTile()->moveCreature(creature, destTile);
		g_game.addMagicEffect(origPos, NM_ME_TELEPORT);
		g_game.addMagicEffect(destTile->getPosition(), NM_ME_TELEPORT);
	} else if (Item* item = thing->getItem()) {
		g_game.addMagicEffect(item->getPosition(), NM_ME_TELEPORT);
		g_game.internalMoveItem(getTile(), destTile, INDEX_WHEREEVER, item, item->getItemCount(), nullptr);
		g_game.addMagicEffect(destTile->getPosition(), NM_ME_TELEPORT);
	}
}

void Teleport::__updateThing(Thing* thing, uint16_t itemId, uint32_t count)
{
	//
}

void Teleport::__replaceThing(uint32_t index, Thing* thing)
{
	//
}

void Teleport::__removeThing(Thing* thing, uint32_t count)
{
	//
}

void Teleport::postAddNotification(Thing* thing, const Cylinder* oldParent, int32_t index, cylinderlink_t link /*= LINK_OWNER*/)
{
	getParent()->postAddNotification(thing, oldParent, index, LINK_PARENT);
}

void Teleport::postRemoveNotification(Thing* thing, const Cylinder* newParent, int32_t index, bool isCompleteRemoval, cylinderlink_t link /*= LINK_OWNER*/)
{
	getParent()->postRemoveNotification(thing, newParent, index, isCompleteRemoval, LINK_PARENT);
}
