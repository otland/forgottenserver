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

#include <boost/range/adaptor/reversed.hpp>

#include "tile.h"
#include "game.h"
#include "player.h"
#include "creature.h"
#include "teleport.h"
#include "trashholder.h"
#include "mailbox.h"
#include "combat.h"
#include "movement.h"
#include "monster.h"

extern Game g_game;
extern MoveEvents* g_moveEvents;

StaticTile real_nullptr_tile(0xFFFF, 0xFFFF, 0xFFFF);
Tile& Tile::nullptr_tile = real_nullptr_tile;

bool Tile::hasProperty(enum ITEMPROPERTY prop) const
{
	if (ground && ground->hasProperty(prop)) {
		return true;
	}

	if (const TileItemVector* items = getItemList()) {
		for (const Item* item : *items) {
			if (item->hasProperty(prop)) {
				return true;
			}
		}
	}
	return false;
}

bool Tile::hasProperty(Item* exclude, enum ITEMPROPERTY prop) const
{
	assert(exclude);

	if (ground && exclude != ground && ground->hasProperty(prop)) {
		return true;
	}

	if (const TileItemVector* items = getItemList()) {
		for (const Item* item : *items) {
			if (item != exclude && item->hasProperty(prop)) {
				return true;
			}
		}
	}

	return false;
}

bool Tile::hasHeight(uint32_t n) const
{
	uint32_t height = 0;

	if (ground) {
		if (ground->hasProperty(CONST_PROP_HASHEIGHT)) {
			++height;
		}

		if (n == height) {
			return true;
		}
	}

	if (const TileItemVector* items = getItemList()) {
		for (const Item* item : *items) {
			if (item->hasProperty(CONST_PROP_HASHEIGHT)) {
				++height;
			}

			if (n == height) {
				return true;
			}
		}
	}
	return false;
}

size_t Tile::getCreatureCount() const
{
	if (const CreatureVector* creatures = getCreatures()) {
		return creatures->size();
	}
	return 0;
}

size_t Tile::getItemCount() const
{
	if (const TileItemVector* items = getItemList()) {
		return items->size();
	}
	return 0;
}

uint32_t Tile::getTopItemCount() const
{
	if (const TileItemVector* items = getItemList()) {
		return items->getTopItemCount();
	}
	return 0;
}

uint32_t Tile::getDownItemCount() const
{
	if (const TileItemVector* items = getItemList()) {
		return items->getDownItemCount();
	}
	return 0;
}

std::string Tile::getDescription(int32_t) const
{
	return "You dont know why, but you cant see anything!";
}

Teleport* Tile::getTeleportItem() const
{
	if (!hasFlag(TILESTATE_TELEPORT)) {
		return nullptr;
	}

	if (const TileItemVector* items = getItemList()) {
		for (ItemVector::const_reverse_iterator it = items->rbegin(); it != items->rend(); ++it) {
			if ((*it)->getTeleport()) {
				return (*it)->getTeleport();
			}
		}
	}
	return nullptr;
}

MagicField* Tile::getFieldItem() const
{
	if (!hasFlag(TILESTATE_MAGICFIELD)) {
		return nullptr;
	}

	if (ground && ground->getMagicField()) {
		return ground->getMagicField();
	}

	if (const TileItemVector* items = getItemList()) {
		for (ItemVector::const_reverse_iterator it = items->rbegin(); it != items->rend(); ++it) {
			if ((*it)->getMagicField()) {
				return (*it)->getMagicField();
			}
		}
	}

	return nullptr;
}

TrashHolder* Tile::getTrashHolder() const
{
	if (!hasFlag(TILESTATE_TRASHHOLDER)) {
		return nullptr;
	}

	if (ground && ground->getTrashHolder()) {
		return ground->getTrashHolder();
	}

	if (const TileItemVector* items = getItemList()) {
		for (ItemVector::const_reverse_iterator it = items->rbegin(); it != items->rend(); ++it) {
			if ((*it)->getTrashHolder()) {
				return (*it)->getTrashHolder();
			}
		}
	}
	return nullptr;
}

Mailbox* Tile::getMailbox() const
{
	if (!hasFlag(TILESTATE_MAILBOX)) {
		return nullptr;
	}

	if (ground && ground->getMailbox()) {
		return ground->getMailbox();
	}

	if (const TileItemVector* items = getItemList()) {
		for (ItemVector::const_reverse_iterator it = items->rbegin(); it != items->rend(); ++it) {
			if ((*it)->getMailbox()) {
				return (*it)->getMailbox();
			}
		}
	}
	return nullptr;
}

BedItem* Tile::getBedItem() const
{
	if (!hasFlag(TILESTATE_BED)) {
		return nullptr;
	}

	if (ground && ground->getBed()) {
		return ground->getBed();
	}

	if (const TileItemVector* items = getItemList()) {
		for (ItemVector::const_reverse_iterator it = items->rbegin(); it != items->rend(); ++it) {
			if ((*it)->getBed()) {
				return (*it)->getBed();
			}
		}
	}
	return nullptr;
}

Creature* Tile::getTopCreature()
{
	return const_cast<Creature*>(static_cast<const Tile*>(this)->getTopCreature());
}

const Creature* Tile::getTopCreature() const
{
	if (const CreatureVector* creatures = getCreatures()) {
		if (!creatures->empty()) {
			return *creatures->begin();
		}
	}
	return nullptr;
}

const Creature* Tile::getBottomCreature() const
{
	if (const CreatureVector* creatures = getCreatures()) {
		if (!creatures->empty()) {
			return *creatures->rbegin();
		}
	}
	return nullptr;
}

Creature* Tile::getTopVisibleCreature(const Creature* creature)
{
	return const_cast<Creature*>(static_cast<const Tile*>(this)->getTopVisibleCreature(creature));
}

const Creature* Tile::getTopVisibleCreature(const Creature* creature) const
{
	if (const CreatureVector* creatures = getCreatures()) {
		if (creature) {
			const Player* player = creature->getPlayer();
			if (player && player->isAccessPlayer()) {
				return getTopCreature();
			}

			for (Creature* tileCreature : *creatures) {
				if (creature->canSeeCreature(tileCreature)) {
					return tileCreature;
				}
			}
		} else {
			for (Creature* tileCreature : *creatures) {
				if (!tileCreature->isInvisible()) {
					const Player* player = tileCreature->getPlayer();
					if (!player || !player->isInGhostMode()) {
						return tileCreature;
					}
				}
			}
		}
	}
	return nullptr;
}

const Creature* Tile::getBottomVisibleCreature(const Creature* creature) const
{
	if (const CreatureVector* creatures = getCreatures()) {
		if (creature) {
			const Player* player = creature->getPlayer();
			if (player && player->isAccessPlayer()) {
				return getBottomCreature();
			}

			for (CreatureVector::const_reverse_iterator it = creatures->rbegin(), end = creatures->rend(); it != end; ++it) {
				if (creature->canSeeCreature(*it)) {
					return *it;
				}
			}
		} else {
			for (CreatureVector::const_reverse_iterator it = creatures->rbegin(), end = creatures->rend(); it != end; ++it) {
				if (!(*it)->isInvisible()) {
					const Player* player = (*it)->getPlayer();
					if (!player || !player->isInGhostMode()) {
						return *it;
					}
				}
			}
		}
	}
	return nullptr;
}

Item* Tile::getTopDownItem()
{
	if (TileItemVector* items = getItemList()) {
		if (items->getDownItemCount() > 0) {
			return *items->getBeginDownItem();
		}
	}
	return nullptr;
}

Item* Tile::getTopTopItem()
{
	if (TileItemVector* items = getItemList()) {
		if (items->getTopItemCount() > 0) {
			return *(items->getEndTopItem() - 1);
		}
	}
	return nullptr;
}

Item* Tile::getItemByTopOrder(int32_t topOrder)
{
	//topOrder:
	//1: borders
	//2: ladders, signs, splashes
	//3: doors etc
	//4: creatures
	if (TileItemVector* items = getItemList()) {
		ItemVector::reverse_iterator itEnd = ItemVector::reverse_iterator(items->getBeginTopItem());
		for (ItemVector::reverse_iterator it = ItemVector::reverse_iterator(items->getEndTopItem()); it != itEnd; ++it) {
			if (Item::items[(*it)->getID()].alwaysOnTopOrder == topOrder) {
				return (*it);
			}
		}
	}
	return nullptr;
}

Thing* Tile::getTopVisibleThing(const Creature* creature)
{
	Thing* thing = getTopVisibleCreature(creature);
	if (thing) {
		return thing;
	}

	TileItemVector* items = getItemList();
	if (items) {
		for (ItemVector::iterator it = items->getBeginDownItem(); it != items->getEndDownItem(); ++it) {
			const ItemType& iit = Item::items[(*it)->getID()];
			if (!iit.lookThrough) {
				return (*it);
			}
		}

		ItemVector::reverse_iterator itEnd = ItemVector::reverse_iterator(items->getBeginTopItem());
		for (ItemVector::reverse_iterator it = ItemVector::reverse_iterator(items->getEndTopItem()); it != itEnd; ++it) {
			const ItemType& iit = Item::items[(*it)->getID()];
			if (!iit.lookThrough) {
				return (*it);
			}
		}
	}

	return ground;
}

void Tile::onAddTileItem(Item* item)
{
	if (item->hasProperty(CONST_PROP_MOVEABLE) || item->getContainer()) {
		auto it = g_game.browseFields.find(this);
		if (it != g_game.browseFields.end()) {
			it->second->__addThingBack(item);
			item->setParent(this);
		}
	}

	updateTileFlags(item, false);

	const Position& cylinderMapPos = getPosition();

	const SpectatorVec& list = g_game.getSpectators(cylinderMapPos);

	//send to client
	for (Creature* spectator : list) {
		if (Player* tmpPlayer = spectator->getPlayer()) {
			tmpPlayer->sendAddTileItem(this, cylinderMapPos, item);
		}
	}

	//event methods
	for (Creature* spectator : list) {
		spectator->onAddTileItem(this, cylinderMapPos);
	}
}

void Tile::onUpdateTileItem(Item* oldItem, const ItemType& oldType, Item* newItem, const ItemType& newType)
{
	if (newItem->hasProperty(CONST_PROP_MOVEABLE) || newItem->getContainer()) {
		auto it = g_game.browseFields.find(this);
		if (it != g_game.browseFields.end()) {
			int32_t index = it->second->__getIndexOfThing(oldItem);
			if (index != -1) {
				it->second->__replaceThing(index, newItem);
				newItem->setParent(this);
			}
		}
	} else if (oldItem->hasProperty(CONST_PROP_MOVEABLE) || oldItem->getContainer()) {
		auto it = g_game.browseFields.find(this);
		if (it != g_game.browseFields.end()) {
			Cylinder* oldParent = oldItem->getParent();
			it->second->__removeThing(oldItem, oldItem->getItemCount());
			oldItem->setParent(oldParent);
		}
	}

	const Position& cylinderMapPos = getPosition();

	const SpectatorVec& list = g_game.getSpectators(cylinderMapPos);

	//send to client
	for (Creature* spectator : list) {
		if (Player* tmpPlayer = spectator->getPlayer()) {
			tmpPlayer->sendUpdateTileItem(this, cylinderMapPos, newItem);
		}
	}

	//event methods
	for (Creature* spectator : list) {
		spectator->onUpdateTileItem(this, cylinderMapPos, oldItem, oldType, newItem, newType);
	}
}

void Tile::onRemoveTileItem(const SpectatorVec& list, const std::vector<int32_t>& oldStackPosVector, Item* item)
{
	if (item->hasProperty(CONST_PROP_MOVEABLE) || item->getContainer()) {
		auto it = g_game.browseFields.find(this);
		if (it != g_game.browseFields.end()) {
			it->second->__removeThing(item, item->getItemCount());
		}
	}

	updateTileFlags(item, true);

	const Position& cylinderMapPos = getPosition();
	const ItemType& iType = Item::items[item->getID()];

	//send to client
	size_t i = 0;
	for (Creature* spectator : list) {
		if (Player* tmpPlayer = spectator->getPlayer()) {
			tmpPlayer->sendRemoveTileThing(cylinderMapPos, oldStackPosVector[i++]);
		}
	}

	//event methods
	for (Creature* spectator : list) {
		spectator->onRemoveTileItem(this, cylinderMapPos, iType, item);
	}
}

void Tile::onUpdateTile(const SpectatorVec& list)
{
	const Position& cylinderMapPos = getPosition();

	//send to clients
	for (Creature* spectator : list) {
		spectator->getPlayer()->sendUpdateTile(this, cylinderMapPos);
	}
}

void Tile::moveCreature(Creature* creature, Cylinder* toCylinder, bool forceTeleport/* = false*/)
{
	Tile* newTile = toCylinder->getTile();
	int32_t oldStackPos = __getIndexOfThing(creature);

	Position oldPos = getPosition();
	Position newPos = newTile->getPosition();

	bool teleport = forceTeleport || !newTile->ground || !Position::areInRange<1, 1, 0>(oldPos, newPos);

	SpectatorVec list;
	g_game.getSpectators(list, oldPos, true);
	g_game.getSpectators(list, newPos, true);

	std::vector<int32_t> oldStackPosVector;
	for (Creature* spectator : list) {
		if (Player* tmpPlayer = spectator->getPlayer()) {
			if (tmpPlayer->canSeeCreature(creature)) {
				oldStackPosVector.push_back(getClientIndexOfCreature(tmpPlayer, creature));
			} else {
				oldStackPosVector.push_back(-1);
			}
		}
	}

	//remove the creature
	__removeThing(creature, 0);

	// Switch the node ownership
	if (qt_node != newTile->qt_node) {
		qt_node->removeCreature(creature);
		newTile->qt_node->addCreature(creature);
	}

	//add the creature
	newTile->__addThing(creature);
	int32_t newStackPos = newTile->__getIndexOfThing(creature);

	if (!teleport) {
		if (oldPos.y > newPos.y) {
			creature->setDirection(NORTH);
		} else if (oldPos.y < newPos.y) {
			creature->setDirection(SOUTH);
		}

		if (oldPos.x < newPos.x) {
			creature->setDirection(EAST);
		} else if (oldPos.x > newPos.x) {
			creature->setDirection(WEST);
		}
	}

	//send to client
	size_t i = 0;
	for (Creature* spectator : list) {
		if (Player* tmpPlayer = spectator->getPlayer()) {
			//Use the correct stackpos
			int32_t stackpos = oldStackPosVector[i++];
			if (stackpos != -1) {
				tmpPlayer->sendCreatureMove(creature, newPos, newTile->getClientIndexOfCreature(tmpPlayer, creature), oldPos, stackpos, teleport);
			}
		}
	}

	//event method
	for (Creature* spectator : list) {
		spectator->onCreatureMove(creature, newTile, newPos, this, oldPos, teleport);
	}

	postRemoveNotification(creature, toCylinder, oldStackPos, true);
	newTile->postAddNotification(creature, this, newStackPos);
}

ReturnValue Tile::__queryAdd(int32_t, const Thing* thing, uint32_t, uint32_t flags, Creature*) const
{
	const CreatureVector* creatures = getCreatures();
	const TileItemVector* items = getItemList();

	if (const Creature* creature = thing->getCreature()) {
		if (hasBitSet(FLAG_NOLIMIT, flags)) {
			return RET_NOERROR;
		}

		if (hasBitSet(FLAG_PATHFINDING, flags)) {
			if (floorChange() || positionChange()) {
				return RET_NOTPOSSIBLE;
			}
		}

		if (ground == nullptr) {
			return RET_NOTPOSSIBLE;
		}

		if (const Monster* monster = creature->getMonster()) {
			if (hasFlag(TILESTATE_PROTECTIONZONE)) {
				return RET_NOTPOSSIBLE;
			}

			if (floorChange() || positionChange()) {
				return RET_NOTPOSSIBLE;
			}

			if (monster->canPushCreatures() && !monster->isSummon()) {
				if (creatures) {
					for (Creature* tileCreature : *creatures) {
						if (tileCreature->getPlayer() && tileCreature->getPlayer()->isInGhostMode()) {
							continue;
						}

						const Monster* creatureMonster = tileCreature->getMonster();
						if (!creatureMonster || !tileCreature->isPushable() ||
						        (creatureMonster->isSummon() && creatureMonster->getMaster()->getPlayer())) {
							return RET_NOTPOSSIBLE;
						}
					}
				}
			} else if (creatures && !creatures->empty()) {
				for (const Creature* tileCreature : *creatures) {
					if (!tileCreature->isInGhostMode()) {
						return RET_NOTENOUGHROOM;
					}
				}
			}

			if (hasFlag(TILESTATE_IMMOVABLEBLOCKSOLID)) {
				return RET_NOTPOSSIBLE;
			}

			if (hasBitSet(FLAG_PATHFINDING, flags) && hasFlag(TILESTATE_IMMOVABLENOFIELDBLOCKPATH)) {
				return RET_NOTPOSSIBLE;
			}

			if (hasFlag(TILESTATE_BLOCKSOLID) || (hasBitSet(FLAG_PATHFINDING, flags) && hasFlag(TILESTATE_NOFIELDBLOCKPATH))) {
				if (!(monster->canPushItems() || hasBitSet(FLAG_IGNOREBLOCKITEM, flags))) {
					return RET_NOTPOSSIBLE;
				}
			}

			MagicField* field = getFieldItem();
			if (field && !field->isBlocking()) {
				CombatType_t combatType = field->getCombatType();

				//There is 3 options for a monster to enter a magic field
				//1) Monster is immune
				if (!monster->isImmune(combatType)) {
					//1) Monster is "strong" enough to handle the damage
					//2) Monster is already afflicated by this type of condition
					if (hasBitSet(FLAG_IGNOREFIELDDAMAGE, flags)) {
						if (!(monster->canPushItems() || monster->hasCondition(Combat::DamageToConditionType(combatType)))) {
							return RET_NOTPOSSIBLE;
						}
					} else {
						return RET_NOTPOSSIBLE;
					}
				}
			}

			return RET_NOERROR;
		} else if (const Player* player = creature->getPlayer()) {
			if (creatures && !creatures->empty() && !hasBitSet(FLAG_IGNOREBLOCKCREATURE, flags) && !player->isAccessPlayer()) {
				for (const Creature* tileCreature : *creatures) {
					if (!player->canWalkthrough(tileCreature)) {
						return RET_NOTPOSSIBLE;
					}
				}
			}

			if (player->getParent() == nullptr && hasFlag(TILESTATE_NOLOGOUT)) {
				//player is trying to login to a "no logout" tile
				return RET_NOTPOSSIBLE;
			}

			if (player->getTile() && player->isPzLocked()) {
				if (!player->getTile()->hasFlag(TILESTATE_PVPZONE)) {
					//player is trying to enter a pvp zone while being pz-locked
					if (hasFlag(TILESTATE_PVPZONE)) {
						return RET_PLAYERISPZLOCKEDENTERPVPZONE;
					}
				} else if (!hasFlag(TILESTATE_PVPZONE)) { //player is trying to leave a pvp zone while being pz-locked
					return RET_PLAYERISPZLOCKEDLEAVEPVPZONE;
				}
			}

			if ((hasFlag(TILESTATE_NOPVPZONE) || hasFlag(TILESTATE_PROTECTIONZONE)) && player->isPzLocked()) {
				return RET_PLAYERISPZLOCKED;
			}
		} else if (creatures && !creatures->empty() && !hasBitSet(FLAG_IGNOREBLOCKCREATURE, flags)) {
			for (const Creature* tileCreature : *creatures) {
				if (!tileCreature->isInGhostMode()) {
					return RET_NOTENOUGHROOM;
				}
			}
		}

		if (items) {
			if (!hasBitSet(FLAG_IGNOREBLOCKITEM, flags)) {
				//If the FLAG_IGNOREBLOCKITEM bit isn't set we dont have to iterate every single item
				if (hasFlag(TILESTATE_BLOCKSOLID)) {
					return RET_NOTENOUGHROOM;
				}
			} else {
				//FLAG_IGNOREBLOCKITEM is set
				if (ground) {
					const ItemType& iiType = Item::items[ground->getID()];
					if (iiType.blockSolid && (!iiType.moveable || ground->hasAttribute(ITEM_ATTRIBUTE_UNIQUEID))) {
						return RET_NOTPOSSIBLE;
					}
				}

				for (const Item* item : *items) {
					const ItemType& iiType = Item::items[item->getID()];
					if (iiType.blockSolid && (!iiType.moveable || item->hasAttribute(ITEM_ATTRIBUTE_UNIQUEID))) {
						return RET_NOTPOSSIBLE;
					}
				}
			}
		}
	} else if (const Item* item = thing->getItem()) {
		if (items && items->size() >= 0xFFFF) {
			return RET_NOTPOSSIBLE;
		}

		if (hasBitSet(FLAG_NOLIMIT, flags)) {
			return RET_NOERROR;
		}

		bool itemIsHangable = item->isHangable();
		if (ground == nullptr && !itemIsHangable) {
			return RET_NOTPOSSIBLE;
		}

		if (creatures && !creatures->empty() && item->isBlocking() && !hasBitSet(FLAG_IGNOREBLOCKCREATURE, flags)) {
			for (const Creature* tileCreature : *creatures) {
				if (!tileCreature->isInGhostMode()) {
					return RET_NOTENOUGHROOM;
				}
			}
		}

		if (itemIsHangable && hasFlag(TILESTATE_SUPPORTS_HANGABLE)) {
			if (items) {
				for (const Item* tileItem : *items) {
					if (tileItem->isHangable()) {
						return RET_NEEDEXCHANGE;
					}
				}
			}
		} else {
			if (ground) {
				const ItemType& iiType = Item::items[ground->getID()];
				if (iiType.blockSolid) {
					if (!iiType.allowPickupable || item->isMagicField() || item->isBlocking()) {
						if (!item->isPickupable()) {
							return RET_NOTENOUGHROOM;
						}

						if (!iiType.hasHeight || iiType.pickupable || iiType.isBed()) {
							return RET_NOTENOUGHROOM;
						}
					}
				}
			}

			if (items) {
				for (const Item* tileItem : *items) {
					const ItemType& iiType = Item::items[tileItem->getID()];
					if (!iiType.blockSolid) {
						continue;
					}

					if (iiType.allowPickupable && !item->isMagicField() && !item->isBlocking()) {
						continue;
					}

					if (!item->isPickupable()) {
						return RET_NOTENOUGHROOM;
					}

					if (!iiType.hasHeight || iiType.pickupable || iiType.isBed()) {
						return RET_NOTENOUGHROOM;
					}
				}
			}
		}
	}
	return RET_NOERROR;
}

ReturnValue Tile::__queryMaxCount(int32_t, const Thing*, uint32_t count, uint32_t& maxQueryCount, uint32_t) const
{
	maxQueryCount = std::max<uint32_t>(1, count);
	return RET_NOERROR;
}

ReturnValue Tile::__queryRemove(const Thing* thing, uint32_t count, uint32_t flags) const
{
	int32_t index = __getIndexOfThing(thing);
	if (index == -1) {
		return RET_NOTPOSSIBLE;
	}

	const Item* item = thing->getItem();
	if (item == nullptr) {
		return RET_NOTPOSSIBLE;
	}

	if (count == 0 || (item->isStackable() && count > item->getItemCount())) {
		return RET_NOTPOSSIBLE;
	}

	if (!item->isMoveable() && !hasBitSet(FLAG_IGNORENOTMOVEABLE, flags)) {
		return RET_NOTMOVEABLE;
	}

	return RET_NOERROR;
}

Cylinder* Tile::__queryDestination(int32_t&, const Thing*, Item** destItem, uint32_t& flags)
{
	Tile* destTile = nullptr;
	*destItem = nullptr;

	if (floorChangeDown()) {
		int32_t dx = tilePos.x;
		int32_t dy = tilePos.y;
		int32_t dz = tilePos.z + 1;

		Tile* southDownTile = g_game.getTile(dx, dy - 1, dz);
		if (southDownTile && southDownTile->floorChange(SOUTH_ALT)) {
			dy -= 2;
			destTile = g_game.getTile(dx, dy, dz);
		} else {
			Tile* eastDownTile = g_game.getTile(dx - 1, dy, dz);
			if (eastDownTile && eastDownTile->floorChange(EAST_ALT)) {
				dx -= 2;
				destTile = g_game.getTile(dx, dy, dz);
			} else {
				Tile* downTile = g_game.getTile(dx, dy, dz);
				if (downTile) {
					if (downTile->floorChange(NORTH)) {
						++dy;
					}

					if (downTile->floorChange(SOUTH)) {
						--dy;
					}

					if (downTile->floorChange(SOUTH_ALT)) {
						dy -= 2;
					}

					if (downTile->floorChange(EAST)) {
						--dx;
					}

					if (downTile->floorChange(EAST_ALT)) {
						dx -= 2;
					}

					if (downTile->floorChange(WEST)) {
						++dx;
					}

					destTile = g_game.getTile(dx, dy, dz);
				}
			}
		}
	} else if (floorChange()) {
		int32_t dx = tilePos.x;
		int32_t dy = tilePos.y;
		int32_t dz = tilePos.getZ() - 1;

		if (floorChange(NORTH)) {
			--dy;
		}

		if (floorChange(SOUTH)) {
			++dy;
		}

		if (floorChange(EAST)) {
			++dx;
		}

		if (floorChange(WEST)) {
			--dx;
		}

		if (floorChange(SOUTH_ALT)) {
			dy += 2;
		}

		if (floorChange(EAST_ALT)) {
			dx += 2;
		}

		destTile = g_game.getTile(dx, dy, dz);
	}

	if (destTile == nullptr) {
		destTile = this;
	} else {
		flags |= FLAG_NOLIMIT;    //Will ignore that there is blocking items/creatures
	}

	if (destTile) {
		Thing* destThing = destTile->getTopDownItem();
		if (destThing) {
			*destItem = destThing->getItem();
		}
	}
	return destTile;
}

void Tile::__addThing(Thing* thing)
{
	__addThing(0, thing);
}

void Tile::__addThing(int32_t, Thing* thing)
{
	Creature* creature = thing->getCreature();
	if (creature) {
		g_game.clearSpectatorCache();
		creature->setParent(this);
		CreatureVector* creatures = makeCreatures();
		creatures->insert(creatures->begin(), creature);
	} else {
		Item* item = thing->getItem();
		if (item == nullptr) {
			return /*RET_NOTPOSSIBLE*/;
		}

		TileItemVector* items = getItemList();
		if (items && items->size() > 0xFFFF) {
			return /*RET_NOTPOSSIBLE*/;
		}

		item->setParent(this);

		if (item->isGroundTile()) {
			if (ground == nullptr) {
				ground = item;
				onAddTileItem(item);
			} else {
				const ItemType& oldType = Item::items[ground->getID()];
				const ItemType& newType = Item::items[item->getID()];

				int32_t oldGroundIndex = __getIndexOfThing(ground);
				Item* oldGround = ground;
				ground->setParent(nullptr);
				g_game.ReleaseItem(ground);
				ground = item;
				updateTileFlags(oldGround, true);
				updateTileFlags(item, false);
				onUpdateTileItem(oldGround, oldType, item, newType);
				postRemoveNotification(oldGround, nullptr, oldGroundIndex, true);
			}
		} else if (item->isAlwaysOnTop()) {
			if (item->isSplash()) {
				//remove old splash if exists
				if (items) {
					for (ItemVector::iterator it = items->getBeginTopItem(); it != items->getEndTopItem(); ++it) {
						if ((*it)->isSplash()) {
							int32_t oldSplashIndex = __getIndexOfThing(*it);
							Item* oldSplash = *it;
							__removeThing(oldSplash, 1);
							oldSplash->setParent(nullptr);
							g_game.ReleaseItem(oldSplash);
							postRemoveNotification(oldSplash, nullptr, oldSplashIndex, true);
							break;
						}
					}
				}
			}

			bool isInserted = false;

			if (items) {
				for (ItemVector::iterator it = items->getBeginTopItem(); it != items->getEndTopItem(); ++it) {
					//Note: this is different from internalAddThing
					if (Item::items[item->getID()].alwaysOnTopOrder <= Item::items[(*it)->getID()].alwaysOnTopOrder) {
						items->insert(it, item);
						isInserted = true;
						break;
					}
				}
			} else {
				items = makeItemList();
			}

			if (!isInserted) {
				items->push_back(item);
			}

			onAddTileItem(item);
		} else {
			if (item->isMagicField()) {
				//remove old field item if exists
				if (items) {
					for (ItemVector::iterator it = items->getBeginDownItem(); it != items->getEndDownItem(); ++it) {
						MagicField* oldField = (*it)->getMagicField();
						if (oldField) {
							if (oldField->isReplaceable()) {
								int32_t oldFieldIndex = __getIndexOfThing(*it);
								__removeThing(oldField, 1);

								oldField->setParent(nullptr);
								g_game.ReleaseItem(oldField);
								postRemoveNotification(oldField, nullptr, oldFieldIndex, true);
								break;
							} else {
								//This magic field cannot be replaced.
								item->setParent(nullptr);
								g_game.ReleaseItem(item);
								return;
							}
						}
					}
				}
			}

			items = makeItemList();
			items->insert(items->getBeginDownItem(), item);
			++items->downItemCount;
			onAddTileItem(item);
		}
	}
}

void Tile::__updateThing(Thing* thing, uint16_t itemId, uint32_t count)
{
	int32_t index = __getIndexOfThing(thing);
	if (index == -1) {
		return /*RET_NOTPOSSIBLE*/;
	}

	Item* item = thing->getItem();
	if (item == nullptr) {
		return /*RET_NOTPOSSIBLE*/;
	}

	const ItemType& oldType = Item::items[item->getID()];
	const ItemType& newType = Item::items[itemId];
	updateTileFlags(item, true);
	item->setID(itemId);
	item->setSubType(count);
	updateTileFlags(item, false);
	onUpdateTileItem(item, oldType, item, newType);
}

void Tile::__replaceThing(uint32_t index, Thing* thing)
{
	int32_t pos = index;

	Item* item = thing->getItem();
	if (item == nullptr) {
		return /*RET_NOTPOSSIBLE*/;
	}

	Item* oldItem = nullptr;
	bool isInserted = false;

	if (ground) {
		if (pos == 0) {
			oldItem = ground;
			ground = item;
			isInserted = true;
		}

		--pos;
	}

	TileItemVector* items = getItemList();
	if (items && !isInserted) {
		int32_t topItemSize = getTopItemCount();
		if (pos < topItemSize) {
			ItemVector::iterator it = items->getBeginTopItem();
			it += pos;

			oldItem = (*it);
			it = items->erase(it);
			items->insert(it, item);
			isInserted = true;
		}

		pos -= topItemSize;
	}

	CreatureVector* creatures = getCreatures();
	if (creatures) {
		if (!isInserted && pos < (int32_t)creatures->size()) {
			return /*RET_NOTPOSSIBLE*/;
		}

		pos -= (uint32_t)creatures->size();
	}

	if (items && !isInserted) {
		int32_t downItemSize = getDownItemCount();
		if (pos < downItemSize) {
			ItemVector::iterator it = items->begin() + pos;
			oldItem = *it;
			it = items->erase(it);
			items->insert(it, item);
			isInserted = true;
		}
	}

	if (isInserted) {
		item->setParent(this);

		updateTileFlags(oldItem, true);
		updateTileFlags(item, false);
		const ItemType& oldType = Item::items[oldItem->getID()];
		const ItemType& newType = Item::items[item->getID()];
		onUpdateTileItem(oldItem, oldType, item, newType);

		oldItem->setParent(nullptr);
		return /*RET_NOERROR*/;
	}
}

void Tile::__removeThing(Thing* thing, uint32_t count)
{
	Creature* creature = thing->getCreature();
	if (creature) {
		CreatureVector* creatures = getCreatures();
		if (creatures) {
			CreatureVector::iterator it = std::find(creatures->begin(), creatures->end(), thing);
			if (it != creatures->end()) {
				g_game.clearSpectatorCache();
				creatures->erase(it);
			}
		}

		return;
	}

	Item* item = thing->getItem();
	if (!item) {
		return;
	}

	int32_t index = __getIndexOfThing(item);
	if (index == -1) {
		return;
	}

	if (item == ground) {
		ground->setParent(nullptr);
		ground = nullptr;
		const SpectatorVec& list = g_game.getSpectators(getPosition());
		onRemoveTileItem(list, std::vector<int32_t>(list.size(), 0), item);
		return;
	}

	TileItemVector* items = getItemList();
	if (!items) {
		return;
	}

	if (item->isAlwaysOnTop()) {
		auto it = std::find(items->getBeginTopItem(), items->getEndTopItem(), item);
		if (it == items->getEndTopItem()) {
			return;
		}

		std::vector<int32_t> oldStackPosVector;

		const SpectatorVec& list = g_game.getSpectators(getPosition());
		for (Creature* spectator : list) {
			if (Player* tmpPlayer = spectator->getPlayer()) {
				oldStackPosVector.push_back(getStackposOfThing(tmpPlayer, item));
			}
		}

		item->setParent(nullptr);
		items->erase(it);
		onRemoveTileItem(list, oldStackPosVector, item);
	} else {
		auto it = std::find(items->getBeginDownItem(), items->getEndDownItem(), item);
		if (it == items->getEndDownItem()) {
			return;
		}

		if (item->isStackable() && count != item->getItemCount()) {
			uint8_t newCount = (uint8_t)std::max<int32_t>(0, (int32_t)(item->getItemCount() - count));

			updateTileFlags(item, true);
			item->setItemCount(newCount);
			updateTileFlags(item, false);

			const ItemType& itemType = Item::items[item->getID()];
			onUpdateTileItem(item, itemType, item, itemType);
		} else {
			std::vector<int32_t> oldStackPosVector;

			const SpectatorVec& list = g_game.getSpectators(getPosition());
			for (Creature* spectator : list) {
				if (Player* tmpPlayer = spectator->getPlayer()) {
					oldStackPosVector.push_back(getStackposOfThing(tmpPlayer, item));
				}
			}

			item->setParent(nullptr);
			items->erase(it);
			--items->downItemCount;
			onRemoveTileItem(list, oldStackPosVector, item);
		}
	}
}

int32_t Tile::__getIndexOfThing(const Thing* thing) const
{
	int32_t n = -1;
	if (ground) {
		if (ground == thing) {
			return 0;
		}
		++n;
	}

	const TileItemVector* items = getItemList();
	if (items) {
		const Item* item = thing->getItem();
		if (item && item->isAlwaysOnTop()) {
			for (ItemVector::const_iterator it = items->getBeginTopItem(); it != items->getEndTopItem(); ++it) {
				++n;
				if (*it == item) {
					return n;
				}
			}
		} else {
			n += items->getTopItemCount();
		}
	}

	if (const CreatureVector* creatures = getCreatures()) {
		if (thing->getCreature()) {
			for (Creature* creature : *creatures) {
				++n;
				if (creature == thing) {
					return n;
				}
			}
		} else {
			n += creatures->size();
		}
	}

	if (items) {
		const Item* item = thing->getItem();
		if (item && !item->isAlwaysOnTop()) {
			for (ItemVector::const_iterator it = items->getBeginDownItem(); it != items->getEndDownItem(); ++it) {
				++n;
				if (*it == item) {
					return n;
				}
			}
		}
	}
	return -1;
}

int32_t Tile::getClientIndexOfCreature(const Player* player, const Creature* creature) const
{
	int32_t n;
	if (ground) {
		n = 1;
	} else {
		n = 0;
	}

	const TileItemVector* items = getItemList();
	if (items) {
		n += items->getTopItemCount();
	}

	if (const CreatureVector* creatures = getCreatures()) {
		for (const Creature* c : boost::adaptors::reverse(*creatures)) {
			if (c == creature) {
				return n;
			} else if (player->canSeeCreature(c)) {
				++n;
			}
		}
	}
	return -1;
}

int32_t Tile::getStackposOfCreature(const Player* player, const Creature* creature) const
{
	int32_t n;
	if (ground) {
		n = 1;
	} else {
		n = 0;
	}

	const TileItemVector* items = getItemList();
	if (items) {
		n += items->getTopItemCount();
		if (n >= 10) {
			return -1;
		}
	}

	if (const CreatureVector* creatures = getCreatures()) {
		for (const Creature* c : boost::adaptors::reverse(*creatures)) {
			if (c == creature) {
				return n;
			} else if (player->canSeeCreature(c)) {
				if (++n >= 10) {
					return -1;
				}
			}
		}
	}
	return -1;
}

int32_t Tile::getStackposOfThing(const Player* player, const Thing* thing) const
{
	int32_t n = 0;
	if (ground) {
		if (ground == thing) {
			return n;
		}
		++n;
	}

	const TileItemVector* items = getItemList();
	if (items) {
		const Item* item = thing->getItem();
		if (item && item->isAlwaysOnTop()) {
			for (ItemVector::const_iterator it = items->getBeginTopItem(); it != items->getEndTopItem(); ++it) {
				if (*it == item) {
					return n;
				} else if (++n == 10) {
					return -1;
				}
			}
		} else {
			n += items->getTopItemCount();
			if (n >= 10) {
				return -1;
			}
		}
	}

	if (const CreatureVector* creatures = getCreatures()) {
		for (const Creature* creature : boost::adaptors::reverse(*creatures)) {
			if (creature == thing) {
				return n;
			} else if (player->canSeeCreature(creature)) {
				if (++n >= 10) {
					return -1;
				}
			}
		}
	}

	if (items) {
		const Item* item = thing->getItem();
		if (item && !item->isAlwaysOnTop()) {
			for (ItemVector::const_iterator it = items->getBeginDownItem(); it != items->getEndDownItem(); ++it) {
				if (*it == item) {
					return n;
				} else if (++n >= 10) {
					return -1;
				}
			}
		}
	}
	return -1;
}

int32_t Tile::__getFirstIndex() const
{
	return 0;
}

int32_t Tile::__getLastIndex() const
{
	return getThingCount();
}

uint32_t Tile::__getItemTypeCount(uint16_t itemId, int32_t subType /*= -1*/) const
{
	uint32_t count = 0;
	if (ground && ground->getID() == itemId) {
		count += Item::countByType(ground, subType);
	}

	const TileItemVector* items = getItemList();
	if (items) {
		for (const Item* item : *items) {
			if (item->getID() == itemId) {
				count += Item::countByType(item, subType);
			}
		}
	}
	return count;
}

Thing* Tile::__getThing(size_t index) const
{
	if (ground) {
		if (index == 0) {
			return ground;
		}

		--index;
	}

	const TileItemVector* items = getItemList();
	if (items) {
		uint32_t topItemSize = items->getTopItemCount();
		if (index < topItemSize) {
			return items->at(items->downItemCount + index);
		}
		index -= topItemSize;
	}

	if (const CreatureVector* creatures = getCreatures()) {
		if (index < creatures->size()) {
			return (*creatures)[index];
		}
		index -= creatures->size();
	}

	if (items) {
		if (index < items->getDownItemCount()) {
			return items->at(index);
		}
	}
	return nullptr;
}

void Tile::postAddNotification(Thing* thing, const Cylinder* oldParent, int32_t index, cylinderlink_t link /*= LINK_OWNER*/)
{
	SpectatorVec list;
	g_game.getSpectators(list, getPosition(), true, true);
	for (Creature* spectator : list) {
		spectator->getPlayer()->postAddNotification(thing, oldParent, index, LINK_NEAR);
	}

	//add a reference to this item, it may be deleted after being added (mailbox for example)
	Creature* creature = thing->getCreature();
	Item* item;
	if (creature) {
		creature->useThing2();
		item = nullptr;
	} else {
		item = thing->getItem();
		if (item) {
			item->useThing2();
		}
	}

	if (link == LINK_OWNER) {
		//calling movement scripts
		Creature* creature = thing->getCreature();
		if (creature) {
			g_moveEvents->onCreatureMove(creature, this, true);
		} else if (item) {
			g_moveEvents->onItemMove(item, this, true);
		}

		if (hasFlag(TILESTATE_TELEPORT)) {
			Teleport* teleport = getTeleportItem();
			if (teleport) {
				teleport->__addThing(thing);
			}
		} else if (hasFlag(TILESTATE_TRASHHOLDER)) {
			TrashHolder* trashholder = getTrashHolder();
			if (trashholder) {
				trashholder->__addThing(thing);
			}
		} else if (hasFlag(TILESTATE_MAILBOX)) {
			Mailbox* mailbox = getMailbox();
			if (mailbox) {
				mailbox->__addThing(thing);
			}
		}
	}

	//release the reference to this item onces we are finished
	if (creature) {
		g_game.ReleaseCreature(creature);
	} else if (item) {
		g_game.ReleaseItem(item);
	}
}

void Tile::postRemoveNotification(Thing* thing, const Cylinder* newParent, int32_t index, bool isCompleteRemoval, cylinderlink_t)
{
	SpectatorVec list;
	g_game.getSpectators(list, getPosition(), true, true);

	if (/*isCompleteRemoval &&*/ getThingCount() > 8) {
		onUpdateTile(list);
	}

	for (Creature* spectator : list) {
		spectator->getPlayer()->postRemoveNotification(thing, newParent, index, isCompleteRemoval, LINK_NEAR);
	}

	//calling movement scripts
	Creature* creature = thing->getCreature();
	if (creature) {
		g_moveEvents->onCreatureMove(creature, this, false);
	} else {
		Item* item = thing->getItem();
		if (item) {
			g_moveEvents->onItemMove(item, this, false);
		}
	}
}

void Tile::__internalAddThing(Thing* thing)
{
	__internalAddThing(0, thing);
}

void Tile::__internalAddThing(uint32_t, Thing* thing)
{
	thing->setParent(this);

	Creature* creature = thing->getCreature();
	if (creature) {
		g_game.clearSpectatorCache();
		CreatureVector* creatures = makeCreatures();
		creatures->insert(creatures->begin(), creature);
	} else {
		Item* item = thing->getItem();
		if (item == nullptr) {
			return;
		}

		TileItemVector* items = makeItemList();
		if (items->size() >= 0xFFFF) {
			return /*RET_NOTPOSSIBLE*/;
		}

		if (item->isGroundTile()) {
			if (ground == nullptr) {
				ground = item;
			}
		} else if (item->isAlwaysOnTop()) {
			bool isInserted = false;
			for (ItemVector::iterator it = items->getBeginTopItem(); it != items->getEndTopItem(); ++it) {
				if (Item::items[(*it)->getID()].alwaysOnTopOrder > Item::items[item->getID()].alwaysOnTopOrder) {
					items->insert(it, item);
					isInserted = true;
					break;
				}
			}

			if (!isInserted) {
				items->push_back(item);
			}
		} else {
			items->insert(items->getBeginDownItem(), item);
			++items->downItemCount;
		}

		updateTileFlags(item, false);
	}
}

void Tile::updateTileFlags(Item* item, bool removing)
{
	if (!removing) {
		//!removing is adding an item to the tile
		if (!hasFlag(TILESTATE_FLOORCHANGE)) {
			if (item->floorChangeDown()) {
				setFlag(TILESTATE_FLOORCHANGE);
				setFlag(TILESTATE_FLOORCHANGE_DOWN);
			}

			if (item->floorChangeNorth()) {
				setFlag(TILESTATE_FLOORCHANGE);
				setFlag(TILESTATE_FLOORCHANGE_NORTH);
			}

			if (item->floorChangeSouth()) {
				setFlag(TILESTATE_FLOORCHANGE);
				setFlag(TILESTATE_FLOORCHANGE_SOUTH);
			}

			if (item->floorChangeEast()) {
				setFlag(TILESTATE_FLOORCHANGE);
				setFlag(TILESTATE_FLOORCHANGE_EAST);
			}

			if (item->floorChangeWest()) {
				setFlag(TILESTATE_FLOORCHANGE);
				setFlag(TILESTATE_FLOORCHANGE_WEST);
			}

			if (item->floorChangeSouthAlt()) {
				setFlag(TILESTATE_FLOORCHANGE);
				setFlag(TILESTATE_FLOORCHANGE_SOUTH_ALT);
			}

			if (item->floorChangeEastAlt()) {
				setFlag(TILESTATE_FLOORCHANGE);
				setFlag(TILESTATE_FLOORCHANGE_EAST_ALT);
			}
		}

		if (item->hasProperty(CONST_PROP_IMMOVABLEBLOCKSOLID)) {
			setFlag(TILESTATE_IMMOVABLEBLOCKSOLID);
		}

		if (item->hasProperty(CONST_PROP_BLOCKPATH)) {
			setFlag(TILESTATE_BLOCKPATH);
		}

		if (item->hasProperty(CONST_PROP_NOFIELDBLOCKPATH)) {
			setFlag(TILESTATE_NOFIELDBLOCKPATH);
		}

		if (item->hasProperty(CONST_PROP_IMMOVABLENOFIELDBLOCKPATH)) {
			setFlag(TILESTATE_IMMOVABLENOFIELDBLOCKPATH);
		}

		if (item->getTeleport()) {
			setFlag(TILESTATE_TELEPORT);
		}

		if (item->getMagicField()) {
			setFlag(TILESTATE_MAGICFIELD);
		}

		if (item->getMailbox()) {
			setFlag(TILESTATE_MAILBOX);
		}

		if (item->getTrashHolder()) {
			setFlag(TILESTATE_TRASHHOLDER);
		}

		if (item->hasProperty(CONST_PROP_BLOCKSOLID)) {
			setFlag(TILESTATE_BLOCKSOLID);
		}

		if (item->getBed()) {
			setFlag(TILESTATE_BED);
		}

		Container* container = item->getContainer();
		if (container && container->getDepotLocker()) {
			setFlag(TILESTATE_DEPOT);
		}

		if (item->hasProperty(CONST_PROP_SUPPORTHANGABLE)) {
			setFlag(TILESTATE_SUPPORTS_HANGABLE);
		}
	} else {
		if (item->floorChangeDown()) {
			resetFlag(TILESTATE_FLOORCHANGE);
			resetFlag(TILESTATE_FLOORCHANGE_DOWN);
		}

		if (item->floorChangeNorth()) {
			resetFlag(TILESTATE_FLOORCHANGE);
			resetFlag(TILESTATE_FLOORCHANGE_NORTH);
		}

		if (item->floorChangeSouth()) {
			resetFlag(TILESTATE_FLOORCHANGE);
			resetFlag(TILESTATE_FLOORCHANGE_SOUTH);
		}

		if (item->floorChangeEast()) {
			resetFlag(TILESTATE_FLOORCHANGE);
			resetFlag(TILESTATE_FLOORCHANGE_EAST);
		}

		if (item->floorChangeWest()) {
			resetFlag(TILESTATE_FLOORCHANGE);
			resetFlag(TILESTATE_FLOORCHANGE_WEST);
		}

		if (item->floorChangeSouthAlt()) {
			resetFlag(TILESTATE_FLOORCHANGE);
			resetFlag(TILESTATE_FLOORCHANGE_SOUTH_ALT);
		}

		if (item->floorChangeEastAlt()) {
			resetFlag(TILESTATE_FLOORCHANGE);
			resetFlag(TILESTATE_FLOORCHANGE_EAST_ALT);
		}

		if (item->hasProperty(CONST_PROP_BLOCKSOLID) && !hasProperty(item, CONST_PROP_BLOCKSOLID)) {
			resetFlag(TILESTATE_BLOCKSOLID);
		}

		if (item->hasProperty(CONST_PROP_IMMOVABLEBLOCKSOLID) && !hasProperty(item, CONST_PROP_IMMOVABLEBLOCKSOLID)) {
			resetFlag(TILESTATE_IMMOVABLEBLOCKSOLID);
		}

		if (item->hasProperty(CONST_PROP_BLOCKPATH) && !hasProperty(item, CONST_PROP_BLOCKPATH)) {
			resetFlag(TILESTATE_BLOCKPATH);
		}

		if (item->hasProperty(CONST_PROP_NOFIELDBLOCKPATH) && !hasProperty(item, CONST_PROP_NOFIELDBLOCKPATH)) {
			resetFlag(TILESTATE_NOFIELDBLOCKPATH);
		}

		if (item->hasProperty(CONST_PROP_IMMOVABLEBLOCKPATH) && !hasProperty(item, CONST_PROP_IMMOVABLEBLOCKPATH)) {
			resetFlag(TILESTATE_IMMOVABLEBLOCKPATH);
		}

		if (item->hasProperty(CONST_PROP_IMMOVABLENOFIELDBLOCKPATH) && !hasProperty(item, CONST_PROP_IMMOVABLENOFIELDBLOCKPATH)) {
			resetFlag(TILESTATE_IMMOVABLENOFIELDBLOCKPATH);
		}

		if (item->getTeleport()) {
			resetFlag(TILESTATE_TELEPORT);
		}

		if (item->getMagicField()) {
			resetFlag(TILESTATE_MAGICFIELD);
		}

		if (item->getMailbox()) {
			resetFlag(TILESTATE_MAILBOX);
		}

		if (item->getTrashHolder()) {
			resetFlag(TILESTATE_TRASHHOLDER);
		}

		if (item->getBed()) {
			resetFlag(TILESTATE_BED);
		}

		Container* container = item->getContainer();
		if (container && container->getDepotLocker()) {
			resetFlag(TILESTATE_DEPOT);
		}

		if (item->hasProperty(CONST_PROP_SUPPORTHANGABLE)) {
			resetFlag(TILESTATE_SUPPORTS_HANGABLE);
		}
	}
}

bool Tile::isMoveableBlocking() const
{
	return !ground || hasFlag(TILESTATE_BLOCKSOLID);
}
