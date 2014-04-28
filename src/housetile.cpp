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

#include "housetile.h"
#include "house.h"
#include "game.h"

extern Game g_game;

HouseTile::HouseTile(int32_t x, int32_t y, int32_t z, House* _house) :
	DynamicTile(x, y, z)
{
	house = _house;
	setFlag(TILESTATE_HOUSE);
}

HouseTile::~HouseTile()
{
	//
}

void HouseTile::__addThing(int32_t index, Thing* thing)
{
	Tile::__addThing(index, thing);

	if (!thing->getParent()) {
		return;
	}

	if (Item* item = thing->getItem()) {
		updateHouse(item);
	}
}

void HouseTile::__internalAddThing(uint32_t index, Thing* thing)
{
	Tile::__internalAddThing(index, thing);

	if (!thing->getParent()) {
		return;
	}

	if (Item* item = thing->getItem()) {
		updateHouse(item);
	}
}

void HouseTile::updateHouse(Item* item)
{
	if (item->getTile() == this) {
		Door* door = item->getDoor();
		if (door && door->getDoorId() != 0) {
			house->addDoor(door);
		}

		if (!door) {
			BedItem* bed = item->getBed();
			if (bed) {
				house->addBed(bed);
			}
		}
	}
}

ReturnValue HouseTile::__queryAdd(int32_t index, const Thing* thing, uint32_t count, uint32_t flags, Creature* actor/* = nullptr*/) const
{
	if (const Creature* creature = thing->getCreature()) {
		if (const Player* player = creature->getPlayer()) {
			if (!house->isInvited(player)) {
				return RET_PLAYERISNOTINVITED;
			}
		} else {
			return RET_NOTPOSSIBLE;
		}
	} else if (thing->getItem() && actor) {
		Player* actorPlayer = actor->getPlayer();
		if (!house->isInvited(actorPlayer)) {
			return RET_CANNOTTHROW;
		}
	}
	return Tile::__queryAdd(index, thing, count, flags, actor);
}

Cylinder* HouseTile::__queryDestination(int32_t& index, const Thing* thing, Item** destItem, uint32_t& flags)
{
	if (const Creature* creature = thing->getCreature()) {
		if (const Player* player = creature->getPlayer()) {
			if (!house->isInvited(player)) {
				const Position& entryPos = house->getEntryPosition();
				Tile* destTile = g_game.getTile(entryPos.x, entryPos.y, entryPos.z);
				if (!destTile) {
					std::cout << "Error: [HouseTile::__queryDestination] House entry not correct"
					          << " - Name: " << house->getName()
					          << " - House id: " << house->getId()
					          << " - Tile not found: " << entryPos << std::endl;

					const Position& templePos = player->getTemplePosition();
					destTile = g_game.getTile(templePos.x, templePos.y, templePos.z);
					if (!destTile) {
						destTile = &(Tile::nullptr_tile);
					}
				}

				index = -1;
				*destItem = nullptr;
				return destTile;
			}
		}
	}
	return Tile::__queryDestination(index, thing, destItem, flags);
}
