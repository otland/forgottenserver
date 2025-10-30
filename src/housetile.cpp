// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "otpch.h"

#include "housetile.h"

#include "configmanager.h"
#include "game.h"
#include "house.h"

extern Game g_game;

void HouseTile::addThing(int32_t index, const std::shared_ptr<Thing>& thing)
{
	Tile::addThing(index, thing);

	if (!thing->hasParent()) {
		return;
	}

	if (const auto& item = thing->getItem()) {
		updateHouse(item);
	}
}

void HouseTile::internalAddThing(uint32_t index, const std::shared_ptr<Thing>& thing)
{
	Tile::internalAddThing(index, thing);

	if (!thing->hasParent()) {
		return;
	}

	if (const auto& item = thing->getItem()) {
		updateHouse(item);
	}
}

void HouseTile::updateHouse(const std::shared_ptr<Item>& item)
{
	if (item->getParent().get() != this) {
		return;
	}

	if (const auto& door = item->getDoor()) {
		if (door->getDoorId() != 0) {
			house->addDoor(door);
		}
	} else if (const auto& bed = item->getBed()) {
		if (bed) {
			house->addBed(bed);
		}
	}
}

ReturnValue HouseTile::queryAdd(int32_t index, const std::shared_ptr<const Thing>& thing, uint32_t count,
                                uint32_t flags, const std::shared_ptr<Creature>& actor /* = nullptr*/) const
{
	if (const auto& creature = thing->getCreature()) {
		if (const auto& player = creature->getPlayer()) {
			if (!house->isInvited(player)) {
				return RETURNVALUE_PLAYERISNOTINVITED;
			}
		} else {
			return RETURNVALUE_NOTPOSSIBLE;
		}
	} else if (const auto& item = thing->getItem()) {
		if (item->isStoreItem() && !item->hasAttribute(ITEM_ATTRIBUTE_WRAPID)) {
			return RETURNVALUE_ITEMCANNOTBEMOVEDTHERE;
		}

		if (actor && getBoolean(ConfigManager::ONLY_INVITED_CAN_MOVE_HOUSE_ITEMS)) {
			if (!house->isInvited(actor->getPlayer())) {
				return RETURNVALUE_PLAYERISNOTINVITED;
			}
		}
	}
	return Tile::queryAdd(index, thing, count, flags, actor);
}

std::shared_ptr<Thing> HouseTile::queryDestination(int32_t& index, const std::shared_ptr<const Thing>& thing,
                                                   std::shared_ptr<Item>& destItem, uint32_t& flags)
{
	if (const auto& creature = thing->getCreature()) {
		if (const auto& player = creature->getPlayer()) {
			if (!house->isInvited(player)) {
				const Position& entryPos = house->getEntryPosition();
				auto destTile = g_game.map.getTile(entryPos);
				if (!destTile) {
					std::cout << "Error: [HouseTile::queryDestination] House entry not correct"
					          << " - Name: " << house->getName() << " - House id: " << house->getId()
					          << " - Tile not found: " << entryPos << std::endl;

					destTile = g_game.map.getTile(player->getTemplePosition());
					if (!destTile) {
						destTile = Tile::nullptr_tile;
					}
				}

				index = -1;
				destItem = nullptr;
				return destTile;
			}
		}
	}

	return Tile::queryDestination(index, thing, destItem, flags);
}

ReturnValue HouseTile::queryRemove(const std::shared_ptr<const Thing>& thing, uint32_t count, uint32_t flags,
                                   const std::shared_ptr<Creature>& actor /*= nullptr*/) const
{
	const auto& item = thing->getItem();
	if (!item) {
		return RETURNVALUE_NOTPOSSIBLE;
	}

	if (actor && getBoolean(ConfigManager::ONLY_INVITED_CAN_MOVE_HOUSE_ITEMS)) {
		if (!house->isInvited(actor->getPlayer())) {
			return RETURNVALUE_PLAYERISNOTINVITED;
		}
	}
	return Tile::queryRemove(thing, count, flags);
}
