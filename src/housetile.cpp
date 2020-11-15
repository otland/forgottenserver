/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2019  Mark Samman <mark.samman@gmail.com>
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
#include "configmanager.h"

extern Game g_game;
extern ConfigManager g_config;

HouseTile::HouseTile(int32_t x, int32_t y, int32_t z, House* house) :
	DynamicTile(x, y, z), house(house) {}

void HouseTile::addThing(int32_t index, Thing* thing)
{
	Tile::addThing(index, thing);

	if (!thing->getParent()) {
		return;
	}

	if (Item* item = thing->getItem()) {
		updateHouse(item);
	}
}

void HouseTile::internalAddThing(uint32_t index, Thing* thing)
{
	Tile::internalAddThing(index, thing);

	if (!thing->getParent()) {
		return;
	}

	if (Item* item = thing->getItem()) {
		updateHouse(item);
	}
}

void HouseTile::updateHouse(Item* item)
{
	if (item->getParent() != this) {
		return;
	}

	Door* door = item->getDoor();
	if (door) {
		if (door->getDoorId() != 0) {
			house->addDoor(door);
		}
	} else {
		BedItem* bed = item->getBed();
		if (bed) {
			house->addBed(bed);
		}
	}
}

ReturnValue HouseTile::queryAdd(int32_t index, const Thing& thing, uint32_t count, uint32_t flags, Creature* actor/* = nullptr*/) const
{
	if (const Creature* creature = thing.getCreature()) {
		if (const Player* player = creature->getPlayer()) {
			if (!house->isInvited(player)) {
				return RETURNVALUE_PLAYERISNOTINVITED;
			}
		} else {
			return RETURNVALUE_NOTPOSSIBLE;
		}
	} else if (const Item* item = thing.getItem()) {
		if (item->isStoreItem() && !item->hasAttribute(ITEM_ATTRIBUTE_WRAPID)) {
			return RETURNVALUE_ITEMCANNOTBEMOVEDTHERE;
		}

		if (actor) {
			Player* actorPlayer = actor->getPlayer();
			if (!house->isInvited(actorPlayer)) {
				return RETURNVALUE_CANNOTTHROW;
			}
		}
	}
	return Tile::queryAdd(index, thing, count, flags, actor);
}

Tile* HouseTile::queryDestination(int32_t& index, const Thing& thing, Item** destItem, uint32_t& flags)
{
	if (const Creature* creature = thing.getCreature()) {
		if (const Player* player = creature->getPlayer()) {
			if (!house->isInvited(player)) {
				const Position& entryPos = house->getEntryPosition();
				Tile* destTile = g_game.map.getTile(entryPos);
				if (!destTile) {
					std::cout << "Error: [HouseTile::queryDestination] House entry not correct"
					          << " - Name: " << house->getName()
					          << " - House id: " << house->getId()
					          << " - Tile not found: " << entryPos << std::endl;

					destTile = g_game.map.getTile(player->getTemplePosition());
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

	return Tile::queryDestination(index, thing, destItem, flags);
}

ReturnValue HouseTile::queryRemove(const Thing& thing, uint32_t count, uint32_t flags, Creature* actor /*= nullptr*/) const
{
	const Item* item = thing.getItem();
	if (!item) {
		return RETURNVALUE_NOTPOSSIBLE;
	}

	if (actor && g_config.getBoolean(ConfigManager::ONLY_INVITED_CAN_MOVE_HOUSE_ITEMS)) {
		Player* actorPlayer = actor->getPlayer();
		if (!house->isInvited(actorPlayer)) {
			return RETURNVALUE_NOTPOSSIBLE;
		}
	}
	return Tile::queryRemove(thing, count, flags);
}
