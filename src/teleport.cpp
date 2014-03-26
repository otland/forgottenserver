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

Teleport::Teleport(uint16_t _type, MagicEffectClasses _effect /* = CONST_ME_TELEPORT */) : Item(_type)
{
	destPos.x = 0;
	destPos.y = 0;
	destPos.z = 0;
	effect = _effect;
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

	if (Creature* creature = thing->getCreature()) {
		Position origPos = creature->getPosition();
		g_game.internalCreatureTurn(creature, origPos.x > destPos.x ? WEST : EAST);
		getTile()->moveCreature(creature, destTile);
		if (effect != CONST_ME_NONE) {
			g_game.addMagicEffect(origPos, effect);
			g_game.addMagicEffect(destTile->getPosition(), effect);
		}
	} else if (Item* item = thing->getItem()) {
		if (effect != CONST_ME_NONE) {
			g_game.addMagicEffect(item->getPosition(), effect);
		}
		g_game.internalMoveItem(getTile(), destTile, INDEX_WHEREEVER, item, item->getItemCount(), nullptr);
		if (effect != CONST_ME_NONE) {
			g_game.addMagicEffect(destTile->getPosition(), effect);
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
