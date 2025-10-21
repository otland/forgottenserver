// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "otpch.h"

#include "teleport.h"

#include "game.h"

extern Game g_game;

Attr_ReadValue Teleport::readAttr(AttrTypes_t attr, PropStream& propStream)
{
	if (attr == ATTR_TELE_DEST) {
		if (!propStream.read<uint16_t>(destPos.x) || !propStream.read<uint16_t>(destPos.y) ||
		    !propStream.read<uint8_t>(destPos.z)) {
			return ATTR_READ_ERROR;
		}
		return ATTR_READ_CONTINUE;
	}
	return Item::readAttr(attr, propStream);
}

void Teleport::serializeAttr(PropWriteStream& propWriteStream) const
{
	Item::serializeAttr(propWriteStream);

	propWriteStream.write<uint8_t>(ATTR_TELE_DEST);
	propWriteStream.write<uint16_t>(destPos.x);
	propWriteStream.write<uint16_t>(destPos.y);
	propWriteStream.write<uint8_t>(destPos.z);
}

ReturnValue Teleport::queryAdd(int32_t, std::shared_ptr<const Thing>, uint32_t, uint32_t,
                               std::shared_ptr<Creature>) const
{
	return RETURNVALUE_NOTPOSSIBLE;
}

ReturnValue Teleport::queryMaxCount(int32_t, std::shared_ptr<const Thing>, uint32_t, uint32_t&, uint32_t) const
{
	return RETURNVALUE_NOTPOSSIBLE;
}

ReturnValue Teleport::queryRemove(std::shared_ptr<const Thing>, uint32_t, uint32_t,
                                  std::shared_ptr<Creature> /*= nullptr */) const
{
	return RETURNVALUE_NOERROR;
}

std::shared_ptr<Cylinder> Teleport::queryDestination(int32_t&, std::shared_ptr<const Thing>, std::shared_ptr<Item>&,
                                                     uint32_t&)
{
	return shared_from_this();
}

void Teleport::addThing(std::shared_ptr<Thing> thing) { return addThing(0, thing); }

void Teleport::addThing(int32_t, std::shared_ptr<Thing> thing)
{
	auto destTile = g_game.map.getTile(destPos);
	if (!destTile) {
		return;
	}

	// Prevent infinite loop
	auto destTeleport = destTile->getTeleportItem();
	if (destTeleport) {
		std::vector<Position> lastPositions = {getPosition()};

		while (true) {
			const Position& nextPos = destTeleport->getDestPos();
			if (std::find(lastPositions.begin(), lastPositions.end(), nextPos) != lastPositions.end()) {
				std::cout << "Warning: possible infinite loop teleport. " << nextPos << std::endl;
				return;
			}

			auto tile = g_game.map.getTile(nextPos);
			if (!tile) {
				break;
			}

			destTeleport = tile->getTeleportItem();
			if (!destTeleport) {
				break;
			}

			lastPositions.push_back(nextPos);
		}
	}

	const MagicEffectClasses effect = Item::items[id].magicEffect;

	if (auto creature = thing->getCreature()) {
		Position origPos = creature->getPosition();
		g_game.internalCreatureTurn(creature, origPos.x > destPos.x ? DIRECTION_WEST : DIRECTION_EAST);
		g_game.map.moveCreature(creature, destTile);
		if (effect != CONST_ME_NONE) {
			g_game.addMagicEffect(origPos, effect);
			g_game.addMagicEffect(destTile->getPosition(), effect);
		}
	} else if (auto item = thing->getItem()) {
		if (effect != CONST_ME_NONE) {
			g_game.addMagicEffect(destTile->getPosition(), effect);
			g_game.addMagicEffect(item->getPosition(), effect);
		}
		g_game.internalMoveItem(getTile(), destTile, INDEX_WHEREEVER, item, item->getItemCount(), nullptr,
		                        FLAG_NOLIMIT);
	}
}

void Teleport::updateThing(std::shared_ptr<Thing>, uint16_t, uint32_t)
{
	//
}

void Teleport::replaceThing(uint32_t, std::shared_ptr<Thing>)
{
	//
}

void Teleport::removeThing(std::shared_ptr<Thing>, uint32_t)
{
	//
}

void Teleport::postAddNotification(std::shared_ptr<Thing> thing, std::shared_ptr<const Cylinder> oldParent,
                                   int32_t index, cylinderlink_t)
{
	getParent()->postAddNotification(thing, oldParent, index, LINK_PARENT);
}

void Teleport::postRemoveNotification(std::shared_ptr<Thing> thing, std::shared_ptr<const Cylinder> newParent,
                                      int32_t index, cylinderlink_t)
{
	getParent()->postRemoveNotification(thing, newParent, index, LINK_PARENT);
}
