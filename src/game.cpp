// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "otpch.h"

#include "game.h"

#include "actions.h"
#include "bed.h"
#include "configmanager.h"
#include "creature.h"
#include "creatureevent.h"
#include "databasetasks.h"
#include "events.h"
#include "globalevent.h"
#include "housetile.h"
#include "http/http.h"
#include "iologindata.h"
#include "iomarket.h"
#include "items.h"
#include "monster.h"
#include "movement.h"
#include "npc.h"
#include "outfit.h"
#include "party.h"
#include "podium.h"
#include "scheduler.h"
#include "script.h"
#include "server.h"
#include "spectators.h"
#include "spells.h"
#include "storeinbox.h"
#include "talkaction.h"
#include "weapons.h"

#include <fstream>

extern Actions* g_actions;
extern Chat* g_chat;
extern TalkActions* g_talkActions;
extern Spells* g_spells;
extern Vocations g_vocations;
extern GlobalEvents* g_globalEvents;
extern CreatureEvents* g_creatureEvents;
extern Monsters g_monsters;
extern MoveEvents* g_moveEvents;
extern Weapons* g_weapons;
extern Scripts* g_scripts;

Game::Game()
{
	offlineTrainingWindow.defaultEnterButton = 0;
	offlineTrainingWindow.defaultEscapeButton = 1;
	offlineTrainingWindow.choices.emplace_back("Sword Fighting and Shielding", SKILL_SWORD);
	offlineTrainingWindow.choices.emplace_back("Axe Fighting and Shielding", SKILL_AXE);
	offlineTrainingWindow.choices.emplace_back("Club Fighting and Shielding", SKILL_CLUB);
	offlineTrainingWindow.choices.emplace_back("Distance Fighting and Shielding", SKILL_DISTANCE);
	offlineTrainingWindow.choices.emplace_back("Magic Level and Shielding", SKILL_MAGLEVEL);
	offlineTrainingWindow.buttons.emplace_back("Okay", offlineTrainingWindow.defaultEnterButton);
	offlineTrainingWindow.buttons.emplace_back("Cancel", offlineTrainingWindow.defaultEscapeButton);
	offlineTrainingWindow.priority = true;
}

void Game::start(ServiceManager* manager)
{
	serviceManager = manager;

	g_scheduler.addEvent(createSchedulerTask(EVENT_CREATURE_THINK_INTERVAL, [this]() { checkCreatures(0); }));
	g_scheduler.addEvent(
	    createSchedulerTask(getNumber(ConfigManager::PATHFINDING_INTERVAL), [this]() { updateCreaturesPath(0); }));
	g_scheduler.addEvent(createSchedulerTask(EVENT_DECAYINTERVAL, [this]() { checkDecay(); }));
}

GameState_t Game::getGameState() const { return gameState; }

void Game::setWorldType(WorldType_t type) { worldType = type; }

void Game::setGameState(GameState_t newState)
{
	if (gameState == GAME_STATE_SHUTDOWN) {
		return; // this cannot be stopped
	}

	if (gameState == newState) {
		return;
	}

	gameState = newState;
	switch (newState) {
		case GAME_STATE_INIT: {
			groups.load();
			g_chat->load();

			map.spawns.startup();

			mounts.loadFromXml();

			loadPlayersRecord();

			g_globalEvents->startup();
			break;
		}

		case GAME_STATE_SHUTDOWN: {
			g_globalEvents->save();
			g_globalEvents->shutdown();

			// kick all players that are still online
			auto it = players.begin();
			while (it != players.end()) {
				it->second->kickPlayer(true);
				it = players.begin();
			}

			saveGameState();

			g_dispatcher.addTask([this]() { shutdown(); });

			g_scheduler.stop();
			g_databaseTasks.stop();
			g_dispatcher.stop();
#ifdef HTTP
			tfs::http::stop();
#endif
			break;
		}

		case GAME_STATE_CLOSED: {
			g_globalEvents->save();

			/* kick all players without the CanAlwaysLogin flag */
			auto it = players.begin();
			while (it != players.end()) {
				if (!it->second->hasFlag(PlayerFlag_CanAlwaysLogin)) {
					it->second->kickPlayer(true);
					it = players.begin();
				} else {
					++it;
				}
			}

			saveGameState();
			break;
		}

		default:
			break;
	}
}

void Game::saveGameState()
{
	if (gameState == GAME_STATE_NORMAL) {
		setGameState(GAME_STATE_MAINTAIN);
	}

	std::cout << "Saving server..." << std::endl;

	for (const auto& player : players | std::views::values) {
		player->setLoginPosition(player->getPosition());
		IOLoginData::savePlayer(player);
	}

	Map::save();

	g_databaseTasks.flush();

	if (gameState == GAME_STATE_MAINTAIN) {
		setGameState(GAME_STATE_NORMAL);
	}
}

bool Game::loadMainMap(const std::string& filename)
{
	return map.loadMap("data/world/" + filename + ".otbm", true, false);
}

void Game::loadMap(const std::string& path, bool isCalledByLua) { map.loadMap(path, false, isCalledByLua); }

std::shared_ptr<Thing> Game::internalGetThing(const std::shared_ptr<Player>& player, const Position& pos) const
{
	if (pos.x != 0xFFFF) {
		return map.getTile(pos);
	}

	// container
	if (pos.y & 0x40) {
		uint8_t from_cid = pos.y & 0x0F;
		return player->getContainerByID(from_cid);
	}

	// inventory
	return player;
}

std::shared_ptr<Thing> Game::internalGetThing(const std::shared_ptr<Player>& player, const Position& pos, int32_t index,
                                              uint32_t spriteId, stackPosType_t type) const
{
	if (pos.x != 0xFFFF) {
		const auto& tile = map.getTile(pos);
		if (!tile) {
			return nullptr;
		}

		std::shared_ptr<Thing> thing = nullptr;
		switch (type) {
			case STACKPOS_LOOK: {
				return tile->getTopVisibleThing(player);
			}

			case STACKPOS_MOVE: {
				const auto& item = tile->getTopDownItem();
				if (item && item->isMoveable()) {
					thing = item;
				} else {
					thing = tile->getTopVisibleCreature(player);
				}

				if (player && tile->hasFlag(TILESTATE_SUPPORTS_HANGABLE)) {
					// do extra checks here if the thing is accessible
					if (thing && thing->getItem()) {
						if (tile->hasProperty(CONST_PROP_ISVERTICAL)) {
							if (player->getPosition().x + 1 == tile->getPosition().x) {
								thing = nullptr;
							}
						} else { // horizontal
							if (player->getPosition().y + 1 == tile->getPosition().y) {
								thing = nullptr;
							}
						}
					}
				}
				break;
			}

			case STACKPOS_USEITEM: {
				thing = tile->getUseItem(index);
				break;
			}

			case STACKPOS_TOPDOWN_ITEM: {
				thing = tile->getTopDownItem();
				break;
			}

			case STACKPOS_USETARGET: {
				thing = tile->getTopVisibleCreature(player);
				if (!thing) {
					thing = tile->getUseItem(index);
				}
				break;
			}

			default: {
				thing = nullptr;
				break;
			}
		}

		return thing;
	}

	// container
	if (pos.y & 0x40) {
		uint8_t fromCid = pos.y & 0x0F;

		const auto& parentContainer = player->getContainerByID(fromCid);
		if (!parentContainer) {
			return nullptr;
		}

		if (parentContainer->getID() == ITEM_BROWSEFIELD) {
			const auto& tile = parentContainer->getTile();
			if (tile && tile->hasFlag(TILESTATE_SUPPORTS_HANGABLE)) {
				if (tile->hasProperty(CONST_PROP_ISVERTICAL)) {
					if (player->getPosition().x + 1 == tile->getPosition().x) {
						return nullptr;
					}
				} else { // horizontal
					if (player->getPosition().y + 1 == tile->getPosition().y) {
						return nullptr;
					}
				}
			}
		}

		uint8_t slot = pos.z;
		return parentContainer->getItemByIndex(player->getContainerIndex(fromCid) + slot);
	} else if (pos.y == 0 && pos.z == 0) {
		const ItemType& it = Item::items.getItemIdByClientId(spriteId);
		if (it.id == 0) {
			return nullptr;
		}

		int32_t subType;
		if (it.isFluidContainer() && index < static_cast<int32_t>(sizeof(reverseFluidMap) / sizeof(uint8_t))) {
			subType = reverseFluidMap[index];
		} else {
			subType = -1;
		}

		return findItemOfType(player, it.id, true, subType);
	}

	// inventory
	slots_t slot = static_cast<slots_t>(pos.y);
	if (slot == CONST_SLOT_STORE_INBOX) {
		return player->getStoreInbox();
	}

	return player->getInventoryItem(slot);
}

static std::pair<Position, uint8_t> internalGetPosition(const std::shared_ptr<Item>& item)
{
	if (const auto& topParent = item->getTopParent()) {
		if (const auto& creature = topParent->getCreature()) {
			if (const auto& player = creature->getPlayer()) {
				const uint16_t x = 0xFFFF;

				if (const auto& parent = item->getParent()) {
					if (const auto& container = parent->getContainer()) {
						const uint16_t y =
						    static_cast<uint16_t>(0x40) | static_cast<uint16_t>(player->getContainerID(container));
						const uint8_t z = container->getThingIndex(item);

						return std::make_pair(Position{x, y, z}, z);
					}
				}

				const uint16_t y = player->getThingIndex(item);
				return std::make_pair(Position{x, y, 0}, y);
			}
		}

		if (const auto& tile = topParent->getTile()) {
			return std::make_pair(tile->getPosition(), tile->getThingIndex(item));
		}
	}

	return std::make_pair(Position{0, 0, 0}, 0);
}

std::shared_ptr<Creature> Game::getCreatureByID(uint32_t id)
{
	if (id <= Player::playerIDLimit) {
		return getPlayerByID(id);
	} else if (id <= Npc::npcAutoID) {
		return getNpcByID(id);
	} else if (id <= Monster::monsterAutoID) {
		return getMonsterByID(id);
	}
	return nullptr;
}

std::shared_ptr<Monster> Game::getMonsterByID(uint32_t id)
{
	if (id == 0) {
		return nullptr;
	}

	auto it = monsters.find(id);
	if (it == monsters.end()) {
		return nullptr;
	}
	return it->second;
}

std::shared_ptr<Npc> Game::getNpcByID(uint32_t id)
{
	if (id == 0) {
		return nullptr;
	}

	auto it = npcs.find(id);
	if (it == npcs.end()) {
		return nullptr;
	}
	return it->second;
}

std::shared_ptr<Player> Game::getPlayerByID(uint32_t id)
{
	if (id == 0) {
		return nullptr;
	}

	auto it = players.find(id);
	if (it == players.end()) {
		return nullptr;
	}
	return it->second;
}

std::shared_ptr<Creature> Game::getCreatureByName(const std::string& s)
{
	if (s.empty()) {
		return nullptr;
	}

	const std::string& lowerCaseName = boost::algorithm::to_lower_copy(s);

	if (auto it = mappedPlayerNames.find(lowerCaseName); it != mappedPlayerNames.end()) {
		return it->second;
	}

	auto equalCreatureName = [&](const auto& it) {
		const auto& name = it.second->getName();
		return lowerCaseName.size() == name.size() &&
		       std::equal(lowerCaseName.begin(), lowerCaseName.end(), name.begin(),
		                  [](char a, char b) { return a == std::tolower(b); });
	};

	if (auto it = std::find_if(npcs.begin(), npcs.end(), equalCreatureName); it != npcs.end()) {
		return it->second;
	}

	if (auto it = std::find_if(monsters.begin(), monsters.end(), equalCreatureName); it != monsters.end()) {
		return it->second;
	}

	return nullptr;
}

std::shared_ptr<Npc> Game::getNpcByName(const std::string& name)
{
	if (name.empty()) {
		return nullptr;
	}

	for (const auto& it : npcs) {
		if (caseInsensitiveEqual(name, it.second->getName())) {
			return it.second;
		}
	}
	return nullptr;
}

std::shared_ptr<Player> Game::getPlayerByName(const std::string& s)
{
	if (s.empty()) {
		return nullptr;
	}

	auto it = mappedPlayerNames.find(boost::algorithm::to_lower_copy(s));
	if (it == mappedPlayerNames.end()) {
		return nullptr;
	}
	return it->second;
}

std::shared_ptr<Player> Game::getPlayerByGUID(const uint32_t& guid)
{
	if (guid == 0) {
		return nullptr;
	}

	auto it = mappedPlayerGuids.find(guid);
	if (it == mappedPlayerGuids.end()) {
		return nullptr;
	}
	return it->second;
}

ReturnValue Game::getPlayerByNameWildcard(const std::string& s, std::shared_ptr<Player>& player)
{
	size_t strlen = s.length();
	if (strlen == 0 || strlen > PLAYER_NAME_LENGTH) {
		return RETURNVALUE_PLAYERWITHTHISNAMEISNOTONLINE;
	}

	if (s.back() == '~') {
		const std::string& query = boost::algorithm::to_lower_copy(s.substr(0, strlen - 1));
		std::string result;
		ReturnValue ret = wildcardTree.findOne(query, result);
		if (ret != RETURNVALUE_NOERROR) {
			return ret;
		}

		player = getPlayerByName(result);
	} else {
		player = getPlayerByName(s);
	}

	if (!player) {
		return RETURNVALUE_PLAYERWITHTHISNAMEISNOTONLINE;
	}

	return RETURNVALUE_NOERROR;
}

std::shared_ptr<Player> Game::getPlayerByAccount(uint32_t acc)
{
	for (const auto& it : players) {
		if (it.second->getAccount() == acc) {
			return it.second;
		}
	}
	return nullptr;
}

bool Game::internalPlaceCreature(const std::shared_ptr<Creature>& creature, const Position& pos,
                                 bool extendedPos /*=false*/, bool forced /*= false*/)
{
	if (creature->hasParent()) {
		return false;
	}

	if (!map.placeCreature(pos, creature, extendedPos, forced)) {
		return false;
	}

	creature->setID();
	creature->addList();
	return true;
}

bool Game::placeCreature(const std::shared_ptr<Creature>& creature, const Position& pos, bool extendedPos /*=false*/,
                         bool forced /*= false*/, MagicEffectClasses magicEffect /*= CONST_ME_TELEPORT*/)
{
	if (!internalPlaceCreature(creature, pos, extendedPos, forced)) {
		return false;
	}

	SpectatorVec spectators;
	map.getSpectators(spectators, creature->getPosition(), true);
	for (const auto& spectator : spectators) {
		if (const auto& tmpPlayer = spectator->getPlayer()) {
			tmpPlayer->sendCreatureAppear(creature, creature->getPosition(), magicEffect);
		}
	}

	for (const auto& spectator : spectators) {
		spectator->onCreatureAppear(creature, true);
	}

	creature->getParent()->postAddNotification(creature, nullptr, 0);

	addCreatureCheck(creature);
	creature->onPlacedCreature();
	return true;
}

bool Game::removeCreature(const std::shared_ptr<Creature>& creature, bool isLogout /* = true*/)
{
	if (creature->isRemoved()) {
		return false;
	}

	creature->releaseFollowers();

	const auto& tile = creature->getTile();

	std::vector<int32_t> oldStackPosVector;

	SpectatorVec spectators;
	map.getSpectators(spectators, tile->getPosition(), true);
	for (const auto& spectator : spectators) {
		if (const auto& player = spectator->getPlayer()) {
			oldStackPosVector.push_back(
			    player->canSeeCreature(creature) ? tile->getClientIndexOfCreature(player, creature) : -1);
		}
	}

	tile->removeCreature(creature);

	const Position& tilePosition = tile->getPosition();

	// send to client
	size_t i = 0;
	for (const auto& spectator : spectators) {
		if (const auto& player = spectator->getPlayer()) {
			player->sendRemoveTileCreature(creature, tilePosition, oldStackPosVector[i++]);
		}
	}

	// event method
	for (const auto& spectator : spectators) {
		spectator->onRemoveCreature(creature, isLogout);
	}

	const auto& master = creature->getMaster();
	if (master && !master->isRemoved()) {
		creature->setMaster(nullptr);
	}

	creature->getParent()->postRemoveNotification(creature, nullptr, 0);

	creature->removeList();
	creature->setRemoved();

	removeCreatureCheck(creature);

	for (const auto& summon : creature->summons) {
		summon->setSkillLoss(false);
		removeCreature(summon);
	}
	return true;
}

void Game::executeDeath(uint32_t creatureId)
{
	const auto& creature = getCreatureByID(creatureId);
	if (creature && !creature->isRemoved()) {
		creature->onDeath();
	}
}

void Game::playerMoveThing(uint32_t playerId, const Position& fromPos, uint16_t spriteId, uint8_t fromStackPos,
                           const Position& toPos, uint8_t count)
{
	const auto& player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	uint8_t fromIndex = 0;
	if (fromPos.x == 0xFFFF) {
		if (fromPos.y & 0x40) {
			fromIndex = fromPos.z;
		} else {
			fromIndex = static_cast<uint8_t>(fromPos.y);
		}
	} else {
		fromIndex = fromStackPos;
	}

	const auto& thing = internalGetThing(player, fromPos, fromIndex, 0, STACKPOS_MOVE);
	if (!thing) {
		player->sendCancelMessage(RETURNVALUE_NOTPOSSIBLE);
		return;
	}

	if (const auto& movingCreature = thing->getCreature()) {
		const auto& tile = map.getTile(toPos);
		if (!tile) {
			player->sendCancelMessage(RETURNVALUE_NOTPOSSIBLE);
			return;
		}

		if (movingCreature->getPosition().isInRange(player->getPosition(), 1, 1, 0)) {
			SchedulerTask* task = createSchedulerTask(
			    MOVE_CREATURE_INTERVAL, [=, this, playerID = player->getID(), creatureID = movingCreature->getID()]() {
				    playerMoveCreatureByID(playerID, creatureID, fromPos, toPos);
			    });
			player->setNextActionTask(task);
		} else {
			playerMoveCreature(player, movingCreature, movingCreature->getPosition(), tile);
		}
	} else if (thing->getItem()) {
		const auto& toThing = internalGetThing(player, toPos);
		if (!toThing) {
			player->sendCancelMessage(RETURNVALUE_NOTPOSSIBLE);
			return;
		}

		playerMoveItem(player, fromPos, spriteId, fromStackPos, toPos, count, thing->getItem(), toThing);
	}
}

void Game::playerMoveCreatureByID(uint32_t playerId, uint32_t movingCreatureId, const Position& movingCreatureOrigPos,
                                  const Position& toPos)
{
	const auto& player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	const auto& movingCreature = getCreatureByID(movingCreatureId);
	if (!movingCreature) {
		return;
	}

	const auto& toTile = map.getTile(toPos);
	if (!toTile) {
		player->sendCancelMessage(RETURNVALUE_NOTPOSSIBLE);
		return;
	}

	playerMoveCreature(player, movingCreature, movingCreatureOrigPos, toTile);
}

void Game::playerMoveCreature(const std::shared_ptr<Player>& player, const std::shared_ptr<Creature>& movingCreature,
                              const Position& movingCreatureOrigPos, const std::shared_ptr<Tile>& toTile)
{
	if (!player->canDoAction()) {
		uint32_t delay = player->getNextActionTime();
		SchedulerTask* task =
		    createSchedulerTask(delay, [=, this, playerID = player->getID(), movingCreatureID = movingCreature->getID(),
		                                toPos = toTile->getPosition()]() {
			    playerMoveCreatureByID(playerID, movingCreatureID, movingCreatureOrigPos, toPos);
		    });
		player->setNextActionTask(task);
		return;
	}

	if (movingCreature->isMovementBlocked()) {
		player->sendCancelMessage(RETURNVALUE_NOTMOVEABLE);
		return;
	}

	player->setNextActionTask(nullptr);

	if (!movingCreatureOrigPos.isInRange(player->getPosition(), 1, 1, 0)) {
		// need to walk to the creature first before moving it
		std::vector<Direction> listDir;
		if (player->getPathTo(movingCreatureOrigPos, listDir, 0, 1, true, true)) {
			g_dispatcher.addTask([this, playerID = player->getID(), listDir = std::move(listDir)]() {
				playerAutoWalk(playerID, listDir);
			});
			SchedulerTask* task =
			    createSchedulerTask(RANGE_MOVE_CREATURE_INTERVAL, [=, this, playerID = player->getID(),
			                                                       movingCreatureID = movingCreature->getID(),
			                                                       toPos = toTile->getPosition()] {
				    playerMoveCreatureByID(playerID, movingCreatureID, movingCreatureOrigPos, toPos);
			    });
			player->setNextWalkActionTask(task);
		} else {
			player->sendCancelMessage(RETURNVALUE_THEREISNOWAY);
		}
		return;
	}

	if ((!movingCreature->isPushable() && !player->hasFlag(PlayerFlag_CanPushAllCreatures)) ||
	    (movingCreature->isInGhostMode() && !player->canSeeGhostMode(movingCreature))) {
		player->sendCancelMessage(RETURNVALUE_NOTMOVEABLE);
		return;
	}

	// check throw distance
	const Position& movingCreaturePos = movingCreature->getPosition();
	const Position& toPos = toTile->getPosition();
	if ((movingCreaturePos.getDistanceX(toPos) > movingCreature->getThrowRange()) ||
	    (movingCreaturePos.getDistanceY(toPos) > movingCreature->getThrowRange()) ||
	    (movingCreaturePos.getDistanceZ(toPos) * 4 > movingCreature->getThrowRange())) {
		player->sendCancelMessage(RETURNVALUE_DESTINATIONOUTOFREACH);
		return;
	}

	if (!movingCreaturePos.isInRange(player->getPosition(), 1, 1, 0)) {
		player->sendCancelMessage(RETURNVALUE_NOTPOSSIBLE);
		return;
	}

	if (player != movingCreature) {
		if (toTile->hasFlag(TILESTATE_BLOCKPATH)) {
			player->sendCancelMessage(RETURNVALUE_NOTENOUGHROOM);
			return;
		} else if ((movingCreature->getZone() == ZONE_PROTECTION && !toTile->hasFlag(TILESTATE_PROTECTIONZONE)) ||
		           (movingCreature->getZone() == ZONE_NOPVP && !toTile->hasFlag(TILESTATE_NOPVPZONE))) {
			player->sendCancelMessage(RETURNVALUE_NOTPOSSIBLE);
			return;
		} else {
			if (CreatureVector* tileCreatures = toTile->getCreatures()) {
				for (const auto& tileCreature : *tileCreatures) {
					if (!tileCreature->isInGhostMode()) {
						player->sendCancelMessage(RETURNVALUE_NOTENOUGHROOM);
						return;
					}
				}
			}

			if (const auto& movingNpc = movingCreature->getNpc()) {
				if (!Spawns::isInZone(movingNpc->getMasterPos(), movingNpc->getMasterRadius(), toPos)) {
					player->sendCancelMessage(RETURNVALUE_NOTENOUGHROOM);
					return;
				}
			}
		}
	}

	if (!tfs::events::player::onMoveCreature(player, movingCreature, movingCreaturePos, toPos)) {
		return;
	}

	ReturnValue ret = internalMoveCreature(movingCreature, toTile);
	if (ret != RETURNVALUE_NOERROR) {
		player->sendCancelMessage(ret);
	}
}

ReturnValue Game::internalMoveCreature(const std::shared_ptr<Creature>& creature, Direction direction,
                                       uint32_t flags /*= 0*/)
{
	creature->setLastPosition(creature->getPosition());
	const Position& currentPos = creature->getPosition();
	Position destPos = getNextPosition(direction, currentPos);
	const auto& player = creature->getPlayer();

	bool diagonalMovement = (direction & DIRECTION_DIAGONAL_MASK) != 0;
	if (player && !diagonalMovement) {
		// try to go up
		if (currentPos.z != 8 && creature->getTile()->hasHeight(3)) {
			const auto& aboveTile = map.getTile(currentPos.x, currentPos.y, currentPos.getZ() - 1);
			if (!aboveTile || (!aboveTile->getGround() && !aboveTile->hasFlag(TILESTATE_BLOCKSOLID))) {
				if (const auto& destAboveTile = map.getTile(destPos.x, destPos.y, destPos.getZ() - 1)) {
					if (destAboveTile->getGround() && !destAboveTile->hasFlag(TILESTATE_IMMOVABLEBLOCKSOLID)) {
						flags |= FLAG_IGNOREBLOCKITEM | FLAG_IGNOREBLOCKCREATURE;

						if (!destAboveTile->hasFlag(TILESTATE_FLOORCHANGE)) {
							player->setDirection(direction);
							destPos.z--;
						}
					}
				}
			}
		}

		// try to go down
		if (currentPos.z != 7 && currentPos.z == destPos.z) {
			const auto& destTile = map.getTile(destPos.x, destPos.y, destPos.z);
			if (!destTile || (!destTile->getGround() && !destTile->hasFlag(TILESTATE_BLOCKSOLID))) {
				if (const auto& destBelowTile = map.getTile(destPos.x, destPos.y, destPos.z + 1)) {
					if (destBelowTile->hasHeight(3) && !destBelowTile->hasFlag(TILESTATE_IMMOVABLEBLOCKSOLID)) {
						flags |= FLAG_IGNOREBLOCKITEM | FLAG_IGNOREBLOCKCREATURE;
						player->setDirection(direction);
						destPos.z++;
					}
				}
			}
		}
	}

	if (const auto& toTile = map.getTile(destPos)) {
		return internalMoveCreature(creature, toTile, flags);
	}
	return RETURNVALUE_NOTPOSSIBLE;
}

ReturnValue Game::internalMoveCreature(const std::shared_ptr<Creature>& creature, const std::shared_ptr<Tile>& toTile,
                                       uint32_t flags /*= 0*/)
{
	if (creature->hasCondition(CONDITION_ROOT)) {
		return RETURNVALUE_NOTPOSSIBLE;
	}

	// check if we can move the creature to the destination
	ReturnValue ret = toTile->queryAdd(0, creature, 1, flags);
	if (ret != RETURNVALUE_NOERROR) {
		return ret;
	}

	map.moveCreature(creature, toTile);
	if (creature->getParent() != toTile) {
		return RETURNVALUE_NOERROR;
	}

	int32_t index = 0;
	std::shared_ptr<Item> toItem = nullptr;
	std::shared_ptr<Thing> subThing = nullptr;
	std::shared_ptr<Thing> toThing = toTile;
	std::shared_ptr<Thing> fromThing = nullptr;
	uint32_t n = 0;

	while ((subThing = toThing->queryDestination(index, creature, toItem, flags)) != toThing) {
		map.moveCreature(creature, subThing->getTile());

		if (creature->getParent() != subThing) {
			// could happen if a script move the creature
			fromThing = nullptr;
			break;
		}

		fromThing = toThing;
		toThing = subThing;
		flags = 0;

		// to prevent infinite loop
		if (++n >= MAP_MAX_LAYERS) {
			break;
		}
	}

	if (fromThing) {
		const Position& fromPosition = fromThing->getPosition();
		const Position& toPosition = toThing->getPosition();
		if (fromPosition.z != toPosition.z && (fromPosition.x != toPosition.x || fromPosition.y != toPosition.y)) {
			Direction dir = getDirectionTo(fromPosition, toPosition);
			if ((dir & DIRECTION_DIAGONAL_MASK) == 0) {
				internalCreatureTurn(creature, dir);
			}
		}
	}

	return RETURNVALUE_NOERROR;
}

void Game::playerMoveItemByPlayerID(uint32_t playerId, const Position& fromPos, uint16_t spriteId, uint8_t fromStackPos,
                                    const Position& toPos, uint8_t count)
{
	const auto& player = getPlayerByID(playerId);
	if (!player) {
		return;
	}
	playerMoveItem(player, fromPos, spriteId, fromStackPos, toPos, count, nullptr, nullptr);
}

void Game::playerMoveItem(const std::shared_ptr<Player>& player, const Position& fromPos, uint16_t spriteId,
                          uint8_t fromStackPos, const Position& toPos, uint8_t count,
                          const std::shared_ptr<Item>& itemHint, const std::shared_ptr<Thing>& toThingHint)
{
	if (!player->canDoAction()) {
		uint32_t delay = player->getNextActionTime();
		SchedulerTask* task = createSchedulerTask(delay, [=, this, playerID = player->getID()]() {
			playerMoveItemByPlayerID(playerID, fromPos, spriteId, fromStackPos, toPos, count);
		});
		player->setNextActionTask(task);
		return;
	}

	player->setNextActionTask(nullptr);

	auto item = itemHint;
	if (!item) {
		uint8_t fromIndex = 0;
		if (fromPos.x == 0xFFFF) {
			if (fromPos.y & 0x40) {
				fromIndex = fromPos.z;
			} else {
				fromIndex = static_cast<uint8_t>(fromPos.y);
			}
		} else {
			fromIndex = fromStackPos;
		}

		const auto& thing = internalGetThing(player, fromPos, fromIndex, 0, STACKPOS_MOVE);
		if (!thing || !thing->getItem()) {
			player->sendCancelMessage(RETURNVALUE_NOTPOSSIBLE);
			return;
		}

		item = thing->getItem();
	}

	if (item->getClientID() != spriteId) {
		player->sendCancelMessage(RETURNVALUE_NOTPOSSIBLE);
		return;
	}

	const auto& fromThing = internalGetThing(player, fromPos);
	if (!fromThing) {
		player->sendCancelMessage(RETURNVALUE_NOTPOSSIBLE);
		return;
	}

	auto toThing = toThingHint;
	if (!toThing) {
		toThing = internalGetThing(player, toPos);
		if (!toThing) {
			player->sendCancelMessage(RETURNVALUE_NOTPOSSIBLE);
			return;
		}
	}

	if (!item->isPushable() || item->hasAttribute(ITEM_ATTRIBUTE_UNIQUEID)) {
		player->sendCancelMessage(RETURNVALUE_NOTMOVEABLE);
		return;
	}

	const Position& playerPos = player->getPosition();
	const Position& mapFromPos = fromThing->getTile()->getPosition();
	if (playerPos.z != mapFromPos.z) {
		player->sendCancelMessage(playerPos.z > mapFromPos.z ? RETURNVALUE_FIRSTGOUPSTAIRS
		                                                     : RETURNVALUE_FIRSTGODOWNSTAIRS);
		return;
	}

	if (!playerPos.isInRange(mapFromPos, 1, 1)) {
		// need to walk to the item first before using it
		std::vector<Direction> listDir;
		if (player->getPathTo(item->getPosition(), listDir, 0, 1, true, true)) {
			g_dispatcher.addTask([this, playerID = player->getID(), listDir = std::move(listDir)]() {
				playerAutoWalk(playerID, listDir);
			});
			SchedulerTask* task =
			    createSchedulerTask(RANGE_MOVE_ITEM_INTERVAL, [=, this, playerID = player->getID()]() {
				    playerMoveItemByPlayerID(playerID, fromPos, spriteId, fromStackPos, toPos, count);
			    });
			player->setNextWalkActionTask(task);
		} else {
			player->sendCancelMessage(RETURNVALUE_THEREISNOWAY);
		}
		return;
	}

	const auto& toThingTile = toThing->getTile();
	const Position& mapToPos = toThingTile->getPosition();

	// hangable item specific code
	if (item->isHangable() && toThingTile->hasFlag(TILESTATE_SUPPORTS_HANGABLE)) {
		// destination supports hangable objects so need to move there first
		bool vertical = toThingTile->hasProperty(CONST_PROP_ISVERTICAL);
		if (vertical) {
			if (playerPos.x + 1 == mapToPos.x) {
				player->sendCancelMessage(RETURNVALUE_NOTPOSSIBLE);
				return;
			}
		} else { // horizontal
			if (playerPos.y + 1 == mapToPos.y) {
				player->sendCancelMessage(RETURNVALUE_NOTPOSSIBLE);
				return;
			}
		}

		if (!playerPos.isInRange(mapToPos, 1, 1, 0)) {
			Position walkPos = mapToPos;
			if (vertical) {
				walkPos.x++;
			} else {
				walkPos.y++;
			}

			Position itemPos = fromPos;
			uint8_t itemStackPos = fromStackPos;

			if (fromPos.x != 0xFFFF && mapFromPos.isInRange(playerPos, 1, 1) &&
			    !mapFromPos.isInRange(walkPos, 1, 1, 0)) {
				// need to pickup the item first
				std::shared_ptr<Item> moveItem = nullptr;

				ReturnValue ret = internalMoveItem(fromThing, player, INDEX_WHEREEVER, item, count, &moveItem, 0,
				                                   player, nullptr, &fromPos, &toPos);
				if (ret != RETURNVALUE_NOERROR) {
					player->sendCancelMessage(ret);
					return;
				}

				// changing the position since its now in the inventory of the player
				std::tie(itemPos, itemStackPos) = internalGetPosition(moveItem);
			}

			std::vector<Direction> listDir;
			if (player->getPathTo(walkPos, listDir, 0, 0, true, true)) {
				g_dispatcher.addTask([this, playerID = player->getID(), listDir = std::move(listDir)]() {
					playerAutoWalk(playerID, listDir);
				});
				SchedulerTask* task = createSchedulerTask(
				    RANGE_MOVE_ITEM_INTERVAL,
				    [this, playerID = player->getID(), itemPos, spriteId, itemStackPos, toPos, count]() {
					    playerMoveItemByPlayerID(playerID, itemPos, spriteId, itemStackPos, toPos, count);
				    });
				player->setNextWalkActionTask(task);
			} else {
				player->sendCancelMessage(RETURNVALUE_THEREISNOWAY);
			}
			return;
		}
	}

	if (!item->isPickupable() && playerPos.z != mapToPos.z) {
		player->sendCancelMessage(RETURNVALUE_DESTINATIONOUTOFREACH);
		return;
	}

	int32_t throwRange = item->getThrowRange();
	if ((playerPos.getDistanceX(mapToPos) > throwRange) || (playerPos.getDistanceY(mapToPos) > throwRange)) {
		player->sendCancelMessage(RETURNVALUE_DESTINATIONOUTOFREACH);
		return;
	}

	if (!canThrowObjectTo(mapFromPos, mapToPos, true, false, throwRange, throwRange)) {
		player->sendCancelMessage(RETURNVALUE_CANNOTTHROW);
		return;
	}

	uint8_t toIndex = 0;
	if (toPos.x == 0xFFFF) {
		if (toPos.y & 0x40) {
			toIndex = toPos.z;
		} else {
			toIndex = static_cast<uint8_t>(toPos.y);
		}
	}

	ReturnValue ret =
	    internalMoveItem(fromThing, toThing, toIndex, item, count, nullptr, 0, player, nullptr, &fromPos, &toPos);
	if (ret != RETURNVALUE_NOERROR) {
		player->sendCancelMessage(ret);
	}
}

ReturnValue Game::internalMoveItem(const std::shared_ptr<Thing>& fromThingHint,
                                   const std::shared_ptr<Thing>& toThingHint, int32_t index,
                                   const std::shared_ptr<Item>& item, uint32_t count, std::shared_ptr<Item>* _moveItem,
                                   uint32_t flags /*= 0*/, const std::shared_ptr<Creature>& actor /* = nullptr*/,
                                   const std::shared_ptr<Item>& tradeItem /* = nullptr*/,
                                   const Position* fromPos /*= nullptr*/, const Position* toPos /*= nullptr*/)
{
	const auto& actorPlayer = actor ? actor->getPlayer() : nullptr;
	if (actorPlayer && fromPos && toPos) {
		const ReturnValue ret =
		    tfs::events::player::onMoveItem(actorPlayer, item, count, *fromPos, *toPos, fromThingHint, toThingHint);
		if (ret != RETURNVALUE_NOERROR) {
			return ret;
		}
	}

	auto fromThing = fromThingHint;
	if (const auto& fromTile = fromThing->getTile()) {
		auto it = browseFields.find(fromTile.get());
		if (it != browseFields.end() && it->second == fromThing) {
			fromThing = fromTile;
		}
	}

	std::shared_ptr<Item> toItem = nullptr;

	std::shared_ptr<Thing> subThing;
	int floorN = 0;

	auto toThing = toThingHint;
	while ((subThing = toThing->queryDestination(index, item, toItem, flags)) != toThing) {
		toThing = subThing;

		// to prevent infinite loop
		if (++floorN >= MAP_MAX_LAYERS) {
			break;
		}
	}

	// destination is the same as the source?
	if (item == toItem) {
		return RETURNVALUE_NOERROR; // silently ignore move
	}

	// check if we can add this item
	ReturnValue ret = toThing->queryAdd(index, item, count, flags, actor);
	if (ret == RETURNVALUE_NEEDEXCHANGE) {
		// check if we can add it to source thing
		ret = fromThing->queryAdd(fromThing->getThingIndex(item), toItem, toItem->getItemCount(), 0);
		if (ret == RETURNVALUE_NOERROR) {
			if (actorPlayer && fromPos && toPos) {
				const ReturnValue eventRet = tfs::events::player::onMoveItem(
				    actorPlayer, toItem, toItem->getItemCount(), *toPos, *fromPos, toThing, fromThing);
				if (eventRet != RETURNVALUE_NOERROR) {
					return eventRet;
				}
			}

			// check how much we can move
			uint32_t maxExchangeQueryCount = 0;
			ReturnValue retExchangeMaxCount =
			    fromThing->queryMaxCount(INDEX_WHEREEVER, toItem, toItem->getItemCount(), maxExchangeQueryCount, 0);

			if (retExchangeMaxCount != RETURNVALUE_NOERROR && maxExchangeQueryCount == 0) {
				return retExchangeMaxCount;
			}

			if (toThing->queryRemove(toItem, toItem->getItemCount(), flags, actor) == RETURNVALUE_NOERROR) {
				int32_t oldToItemIndex = toThing->getThingIndex(toItem);
				toThing->removeThing(toItem, toItem->getItemCount());
				fromThing->addThing(toItem);

				if (oldToItemIndex != -1) {
					toThing->postRemoveNotification(toItem, fromThing, oldToItemIndex);
				}

				int32_t newToItemIndex = fromThing->getThingIndex(toItem);
				if (newToItemIndex != -1) {
					fromThing->postAddNotification(toItem, toThing, newToItemIndex);
				}

				ret = toThing->queryAdd(index, item, count, flags);

				if (actorPlayer && fromPos && toPos && !toItem->isRemoved()) {
					tfs::events::player::onItemMoved(actorPlayer, toItem, toItem->getItemCount(), *toPos, *fromPos,
					                                 toThing, fromThing);
				}

				toItem = nullptr;
			}
		}
	}

	if (ret != RETURNVALUE_NOERROR) {
		return ret;
	}

	// check how much we can move
	uint32_t maxQueryCount = 0;
	ReturnValue retMaxCount = toThing->queryMaxCount(index, item, count, maxQueryCount, flags);
	if (retMaxCount != RETURNVALUE_NOERROR && maxQueryCount == 0) {
		return retMaxCount;
	}

	uint32_t moveCount;
	if (item->isStackable()) {
		moveCount = std::min<uint32_t>(count, maxQueryCount);
	} else {
		moveCount = maxQueryCount;
	}

	// check if we can remove this item
	ret = fromThing->queryRemove(item, moveCount, flags, actor);
	if (ret != RETURNVALUE_NOERROR) {
		return ret;
	}

	if (tradeItem) {
		if (toThing->getItem() == tradeItem) {
			return RETURNVALUE_NOTENOUGHROOM;
		}

		auto parent = toThing->getParent();
		while (parent) {
			if (parent->getItem() == tradeItem) {
				return RETURNVALUE_NOTENOUGHROOM;
			}

			parent = parent->getParent();
		}
	}

	auto moveItem = item;
	int32_t itemIndex = fromThing->getThingIndex(item);
	std::shared_ptr<Item> updateItem = nullptr;

	if (item->isStackable()) {
		// lets find out how much we need to move
		uint32_t allowedCount = 0;

		// when item is moved onto another equal item
		if (toItem && *item == *toItem && moveCount != ITEM_STACK_SIZE) {
			allowedCount = std::min<uint32_t>(ITEM_STACK_SIZE - toItem->getItemCount(), moveCount);
			if (allowedCount > 0) {
				fromThing->removeThing(item, allowedCount);
				toThing->updateThing(toItem, toItem->getID(), toItem->getItemCount() + allowedCount);
				updateItem = toItem;
				moveCount -= allowedCount;

				// we fully merged two stacks, so we have nothing to move
				if (moveCount == 0) {
					moveItem = nullptr;
				}
			}
		} else {
			int32_t newCount = moveCount - allowedCount;
			if (newCount != item->getItemCount() && newCount > 0) {
				// we get part of the source, clone the item and remove moved count from source
				moveItem = item->clone();
				moveItem->setItemCount(newCount);
			}
		}
	}

	// add item
	if (moveItem) {
		fromThing->removeThing(item, moveCount);
		toThing->addThing(index, moveItem);
	}

	if (itemIndex != -1) {
		fromThing->postRemoveNotification(item, toThing, itemIndex);
	}

	if (moveItem) {
		int32_t moveItemIndex = toThing->getThingIndex(moveItem);
		if (moveItemIndex != -1) {
			toThing->postAddNotification(moveItem, fromThing, moveItemIndex);
		}
	}

	if (updateItem) {
		int32_t updateItemIndex = toThing->getThingIndex(updateItem);
		if (updateItemIndex != -1) {
			toThing->postAddNotification(updateItem, fromThing, updateItemIndex);
		}
	}

	if (_moveItem) {
		if (moveItem) {
			*_moveItem = moveItem;
		} else {
			*_moveItem = item;
		}
	}

	// update quiver
	if (actorPlayer) {
		actorPlayer->sendQuiverUpdate(getBoolean(ConfigManager::CLASSIC_EQUIPMENT_SLOTS));
	}

	// we could not move all, inform the player
	if (item->isStackable() && maxQueryCount < count) {
		return retMaxCount;
	}

	if (moveItem && moveItem->getDuration() > 0) {
		if (moveItem->getDecaying() != DECAYING_TRUE) {
			moveItem->setDecaying(DECAYING_TRUE);
			toDecayItems.push_back(moveItem);
		}
	}

	if (actorPlayer && fromPos && toPos) {
		if (updateItem && !updateItem->isRemoved()) {
			tfs::events::player::onItemMoved(actorPlayer, updateItem, count, *fromPos, *toPos, fromThing, toThing);
		} else if (moveItem && !moveItem->isRemoved()) {
			tfs::events::player::onItemMoved(actorPlayer, moveItem, count, *fromPos, *toPos, fromThing, toThing);
		} else if (item && !item->isRemoved()) {
			tfs::events::player::onItemMoved(actorPlayer, item, count, *fromPos, *toPos, fromThing, toThing);
		}
	}

	return ret;
}

ReturnValue Game::internalAddItem(const std::shared_ptr<Thing>& toThing, const std::shared_ptr<Item>& item,
                                  int32_t index /*= INDEX_WHEREEVER*/, uint32_t flags /* = 0*/, bool test /* = false*/)
{
	uint32_t remainderCount = 0;
	return internalAddItem(toThing, item, index, flags, test, remainderCount);
}

ReturnValue Game::internalAddItem(const std::shared_ptr<Thing>& toThing, const std::shared_ptr<Item>& item,
                                  int32_t index, uint32_t flags, bool test, uint32_t& remainderCount)
{
	if (!toThing || !item) {
		return RETURNVALUE_NOTPOSSIBLE;
	}

	std::shared_ptr<Item> toItem = nullptr;
	const auto& destThing = toThing->queryDestination(index, item, toItem, flags);

	// check if we can add this item
	ReturnValue ret = destThing->queryAdd(index, item, item->getItemCount(), flags);
	if (ret != RETURNVALUE_NOERROR) {
		return ret;
	}

	/*
	Check if we can move add the whole amount, we do this by checking against the original thing,
	since the queryDestination can return a thing that might only hold a part of the full amount.
	*/
	uint32_t maxQueryCount = 0;
	ret = toThing->queryMaxCount(INDEX_WHEREEVER, item, item->getItemCount(), maxQueryCount, flags);

	if (ret != RETURNVALUE_NOERROR) {
		return ret;
	}

	if (test) {
		return RETURNVALUE_NOERROR;
	}

	if (item->isStackable() && toItem && *item == *toItem) {
		uint32_t m = std::min<uint32_t>(item->getItemCount(), maxQueryCount);
		uint32_t n = std::min<uint32_t>(ITEM_STACK_SIZE - toItem->getItemCount(), m);

		destThing->updateThing(toItem, toItem->getID(), toItem->getItemCount() + n);

		int32_t count = m - n;
		if (count > 0) {
			if (item->getItemCount() != count) {
				const auto& remainderItem = item->clone();
				remainderItem->setItemCount(count);
				if (internalAddItem(toThing, remainderItem, INDEX_WHEREEVER, flags, false) != RETURNVALUE_NOERROR) {
					remainderCount = count;
				}
			} else {
				destThing->addThing(index, item);

				int32_t itemIndex = destThing->getThingIndex(item);
				if (itemIndex != -1) {
					destThing->postAddNotification(item, nullptr, itemIndex);
				}
			}
		} else {
			// fully merged with toItem, item will be destroyed
			item->onRemoved();

			int32_t itemIndex = destThing->getThingIndex(toItem);
			if (itemIndex != -1) {
				destThing->postAddNotification(toItem, nullptr, itemIndex);
			}
		}
	} else {
		destThing->addThing(index, item);

		int32_t itemIndex = destThing->getThingIndex(item);
		if (itemIndex != -1) {
			destThing->postAddNotification(item, nullptr, itemIndex);
		}
	}

	if (!item->isRemoved() && item->getDuration() > 0) {
		if (item->getDecaying() != DECAYING_TRUE) {
			item->setDecaying(DECAYING_TRUE);
			toDecayItems.push_back(item);
		}
	}

	return RETURNVALUE_NOERROR;
}

ReturnValue Game::internalRemoveItem(const std::shared_ptr<Item>& item, int32_t count /*= -1*/, bool test /*= false*/,
                                     uint32_t flags /*= 0*/)
{
	auto parent = item->getParent();
	if (!parent) {
		return RETURNVALUE_NOTPOSSIBLE;
	}

	const auto& fromTile = parent->getTile();
	if (fromTile) {
		auto it = browseFields.find(fromTile.get());
		if (it != browseFields.end() && it->second == parent) {
			parent = fromTile;
		}
	}

	if (count == -1) {
		count = item->getItemCount();
	}

	// check if we can remove this item
	ReturnValue ret = parent->queryRemove(item, count, flags | FLAG_IGNORENOTMOVEABLE);
	if (ret != RETURNVALUE_NOERROR) {
		return ret;
	}

	if (!item->canRemove()) {
		return RETURNVALUE_NOTPOSSIBLE;
	}

	if (!test) {
		int32_t index = parent->getThingIndex(item);

		// remove the item
		parent->removeThing(item, count);

		if (item->isRemoved()) {
			item->onRemoved();
			if (item->canDecay()) {
				decayItems->remove(item);
			}
		}

		parent->postRemoveNotification(item, nullptr, index);
	}

	return RETURNVALUE_NOERROR;
}

ReturnValue Game::internalPlayerAddItem(const std::shared_ptr<Player>& player, const std::shared_ptr<Item>& item,
                                        bool dropOnMap /*= true*/, slots_t slot /*= CONST_SLOT_WHEREEVER*/)
{
	uint32_t remainderCount = 0;
	ReturnValue ret = internalAddItem(player, item, static_cast<int32_t>(slot), 0, false, remainderCount);
	if (remainderCount != 0) {
		const auto& remainderItem = Item::CreateItem(item->getID(), remainderCount);
		internalAddItem(player->getTile(), remainderItem, INDEX_WHEREEVER, FLAG_NOLIMIT);
	}

	if (ret != RETURNVALUE_NOERROR && dropOnMap) {
		ret = internalAddItem(player->getTile(), item, INDEX_WHEREEVER, FLAG_NOLIMIT);
	}

	return ret;
}

std::shared_ptr<Item> Game::findItemOfType(const std::shared_ptr<Thing>& fromThing, uint16_t itemId,
                                           bool depthSearch /*= true*/, int32_t subType /*= -1*/) const
{
	if (!fromThing) {
		return nullptr;
	}

	std::vector<std::shared_ptr<Container>> containers;
	for (size_t i = fromThing->getFirstIndex(), j = fromThing->getLastIndex(); i < j; ++i) {
		const auto& thing = fromThing->getThing(i);
		if (!thing) {
			continue;
		}

		const auto& item = thing->getItem();
		if (!item) {
			continue;
		}

		if (item->getID() == itemId && (subType == -1 || subType == item->getSubType())) {
			return item;
		}

		if (depthSearch) {
			if (const auto& container = item->getContainer()) {
				containers.push_back(container);
			}
		}
	}

	size_t i = 0;
	while (i < containers.size()) {
		const auto& container = containers[i++];
		for (const auto& item : container->getItemList()) {
			if (item->getID() == itemId && (subType == -1 || subType == item->getSubType())) {
				return item;
			}

			if (const auto& subContainer = item->getContainer()) {
				containers.push_back(subContainer);
			}
		}
	}
	return nullptr;
}

bool Game::removeMoney(const std::shared_ptr<Thing>& fromThing, uint64_t money, uint32_t flags /*= 0*/)
{
	if (!fromThing) {
		return false;
	}

	if (money == 0) {
		return true;
	}

	std::vector<std::shared_ptr<Container>> containers;

	std::multimap<uint32_t, std::shared_ptr<Item>> moneyMap;
	uint64_t moneyCount = 0;

	for (size_t i = fromThing->getFirstIndex(), j = fromThing->getLastIndex(); i < j; ++i) {
		const auto& thing = fromThing->getThing(i);
		if (!thing) {
			continue;
		}

		const auto& item = thing->getItem();
		if (!item) {
			continue;
		}

		if (const auto& container = item->getContainer()) {
			containers.push_back(container);
		} else {
			const uint32_t worth = item->getWorth();
			if (worth != 0) {
				moneyCount += worth;
				moneyMap.emplace(worth, item);
			}
		}
	}

	size_t i = 0;
	while (i < containers.size()) {
		const auto& container = containers[i++];
		for (const auto& item : container->getItemList()) {
			if (const auto& tmpContainer = item->getContainer()) {
				containers.push_back(tmpContainer);
			} else {
				const uint32_t worth = item->getWorth();
				if (worth != 0) {
					moneyCount += worth;
					moneyMap.emplace(worth, item);
				}
			}
		}
	}

	if (moneyCount < money) {
		return false;
	}

	for (const auto& [worth, item] : moneyMap) {
		if (worth < money) {
			internalRemoveItem(item);
			money -= worth;
		} else if (worth > money) {
			const uint32_t worthPerItem = worth / item->getItemCount();
			const uint32_t removeCount = std::ceil(money / static_cast<double>(worthPerItem));

			addMoney(fromThing, (worthPerItem * removeCount) - money, flags);
			internalRemoveItem(item, removeCount);
			break;
		} else {
			internalRemoveItem(item);
			break;
		}
	}
	return true;
}

void Game::addMoney(const std::shared_ptr<Thing>& thing, uint64_t money, uint32_t flags /*= 0*/)
{
	if (money == 0) {
		return;
	}

	for (const auto& it : Item::items.currencyItems) {
		const uint64_t worth = it.first;

		uint32_t currencyCoins = money / worth;
		if (currencyCoins <= 0) {
			continue;
		}

		money -= currencyCoins * worth;
		while (currencyCoins > 0) {
			const uint16_t count = std::min<uint32_t>(ITEM_STACK_SIZE, currencyCoins);

			const auto& remaindItem = Item::CreateItem(it.second, count);

			ReturnValue ret = internalAddItem(thing, remaindItem, INDEX_WHEREEVER, flags);
			if (ret != RETURNVALUE_NOERROR) {
				internalAddItem(thing->getTile(), remaindItem, INDEX_WHEREEVER, FLAG_NOLIMIT);
			}

			currencyCoins -= count;
		}
	}
}

std::shared_ptr<Item> Game::transformItem(const std::shared_ptr<Item>& item, uint16_t newId, int32_t newCount /*= -1*/)
{
	if (item->getID() == newId && (newCount == -1 || (newCount == item->getSubType() &&
	                                                  newCount != 0))) { // chargeless item placed on map = infinite
		return item;
	}

	auto parent = item->getParent();
	if (!parent) {
		return nullptr;
	}

	const auto& fromTile = parent->getTile();
	if (fromTile) {
		auto it = browseFields.find(fromTile.get());
		if (it != browseFields.end() && it->second == parent) {
			parent = fromTile;
		}
	}

	int32_t itemIndex = parent->getThingIndex(item);
	if (itemIndex == -1) {
		return item;
	}

	if (!item->canTransform()) {
		return item;
	}

	const ItemType& newType = Item::items[newId];
	if (newType.id == 0) {
		return item;
	}

	const ItemType& curType = Item::items[item->getID()];
	if (curType.alwaysOnTop != newType.alwaysOnTop) {
		// This only occurs when you transform items on tiles from a downItem to a topItem (or vice versa)
		// Remove the old, and add the new
		parent->removeThing(item, item->getItemCount());
		parent->postRemoveNotification(item, parent, itemIndex);

		item->setID(newId);
		if (newCount != -1) {
			item->setSubType(newCount);
		}
		parent->addThing(item);

		const auto& newParent = item->getParent();
		if (!newParent) {
			return nullptr;
		}

		newParent->postAddNotification(item, parent, newParent->getThingIndex(item));
		return item;
	}

	if (curType.type == newType.type) {
		// Both items has the same type so we can safely change id/subtype
		if (newCount == 0 && (item->isStackable() || item->hasAttribute(ITEM_ATTRIBUTE_CHARGES))) {
			if (item->isStackable()) {
				internalRemoveItem(item);
				return nullptr;
			} else {
				int32_t newItemId = newId;
				if (curType.id == newType.id) {
					newItemId = item->getDecayTo();
				}

				if (newItemId < 0) {
					internalRemoveItem(item);
					return nullptr;
				} else if (newItemId != newId) {
					// Replacing the the old item with the new while maintaining the old position
					const auto& newItem = Item::CreateItem(newItemId, 1);
					if (!newItem) {
						return nullptr;
					}

					parent->replaceThing(itemIndex, newItem);
					parent->postAddNotification(newItem, parent, itemIndex);

					item->setParent(nullptr);
					parent->postRemoveNotification(item, parent, itemIndex);
					return newItem;
				}
				return transformItem(item, newItemId);
			}
		} else {
			parent->postRemoveNotification(item, parent, itemIndex);
			uint16_t itemId = item->getID();
			int32_t count = item->getSubType();

			if (curType.id != newType.id) {
				if (newType.group != curType.group) {
					item->setDefaultSubtype();
				}

				itemId = newId;
			}

			if (newCount != -1 && newType.hasSubType()) {
				count = newCount;
			}

			parent->updateThing(item, itemId, count);
			parent->postAddNotification(item, parent, itemIndex);
			return item;
		}
	}

	// Replacing the old item with the new while maintaining the old position
	std::shared_ptr<Item> newItem;
	if (newCount == -1) {
		newItem = Item::CreateItem(newId);
	} else {
		newItem = Item::CreateItem(newId, newCount);
	}

	if (!newItem) {
		return nullptr;
	}

	parent->replaceThing(itemIndex, newItem);
	parent->postAddNotification(newItem, parent, itemIndex);

	item->setParent(nullptr);
	parent->postRemoveNotification(item, parent, itemIndex);

	if (newItem->getDuration() > 0) {
		if (newItem->getDecaying() != DECAYING_TRUE) {
			newItem->setDecaying(DECAYING_TRUE);
			toDecayItems.push_back(newItem);
		}
	}

	return newItem;
}

ReturnValue Game::internalTeleport(const std::shared_ptr<Thing>& thing, const Position& newPos,
                                   bool pushMove /* = true*/, uint32_t flags /*= 0*/)
{
	if (newPos == thing->getPosition()) {
		return RETURNVALUE_NOERROR;
	} else if (thing->isRemoved()) {
		return RETURNVALUE_NOTPOSSIBLE;
	}

	const auto& toTile = map.getTile(newPos);
	if (!toTile) {
		return RETURNVALUE_NOTPOSSIBLE;
	}

	if (const auto& creature = thing->getCreature()) {
		ReturnValue ret = toTile->queryAdd(0, creature, 1, FLAG_NOLIMIT);
		if (ret != RETURNVALUE_NOERROR) {
			return ret;
		}

		map.moveCreature(creature, toTile, !pushMove);
		return RETURNVALUE_NOERROR;
	} else if (const auto& item = thing->getItem()) {
		return internalMoveItem(item->getParent(), toTile, INDEX_WHEREEVER, item, item->getItemCount(), nullptr, flags);
	}
	return RETURNVALUE_NOTPOSSIBLE;
}

namespace {

std::shared_ptr<Item> searchForItem(const std::shared_ptr<Container>& container, uint16_t itemId)
{
	for (ContainerIterator it = container->iterator(); it.hasNext(); it.advance()) {
		if ((*it)->getID() == itemId) {
			return *it;
		}
	}

	return nullptr;
}

slots_t getSlotType(const ItemType& it)
{
	slots_t slot = CONST_SLOT_RIGHT;
	if (it.weaponType != WeaponType_t::WEAPON_SHIELD && it.weaponType != WeaponType_t::WEAPON_QUIVER) {
		int32_t slotPosition = it.slotPosition;

		if (slotPosition & SLOTP_HEAD) {
			slot = CONST_SLOT_HEAD;
		} else if (slotPosition & SLOTP_NECKLACE) {
			slot = CONST_SLOT_NECKLACE;
		} else if (slotPosition & SLOTP_ARMOR) {
			slot = CONST_SLOT_ARMOR;
		} else if (slotPosition & SLOTP_LEGS) {
			slot = CONST_SLOT_LEGS;
		} else if (slotPosition & SLOTP_FEET) {
			slot = CONST_SLOT_FEET;
		} else if (slotPosition & SLOTP_RING) {
			slot = CONST_SLOT_RING;
		} else if (slotPosition & SLOTP_AMMO) {
			slot = CONST_SLOT_AMMO;
		} else if (slotPosition & SLOTP_TWO_HAND || slotPosition & SLOTP_LEFT) {
			slot = CONST_SLOT_LEFT;
		}
	}

	return slot;
}

} // namespace

// Implementation of player invoked events
void Game::playerEquipItem(uint32_t playerId, uint16_t spriteId)
{
	const auto& player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	const auto& item = player->getInventoryItem(CONST_SLOT_BACKPACK);
	if (!item) {
		return;
	}

	const auto& backpack = item->getContainer();
	if (!backpack) {
		return;
	}

	const ItemType& it = Item::items.getItemIdByClientId(spriteId);
	slots_t slot = getSlotType(it);

	const auto& slotItem = player->getInventoryItem(slot);
	Position fromPos, toPos;
	uint8_t fromStackPos, toStackPos;
	if (slotItem) {
		std::tie(toPos, toStackPos) = internalGetPosition(slotItem);
	}

	const auto& equipItem = searchForItem(backpack, it.id);
	if (equipItem) {
		std::tie(fromPos, fromStackPos) = internalGetPosition(equipItem);
	}

	if (slotItem && slotItem->getID() == it.id &&
	    (!it.stackable || slotItem->getItemCount() == ITEM_STACK_SIZE || !equipItem)) {
		internalMoveItem(slotItem->getParent(), player, CONST_SLOT_WHEREEVER, slotItem, slotItem->getItemCount(),
		                 nullptr, 0, player, nullptr, &fromPos, &toPos);
	} else if (equipItem) {
		internalMoveItem(equipItem->getParent(), player, slot, equipItem, equipItem->getItemCount(), nullptr, 0, player,
		                 nullptr, &fromPos, &toPos);
	}
}

void Game::playerMove(uint32_t playerId, Direction direction)
{
	const auto& player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	if (player->isMovementBlocked()) {
		player->sendCancelWalk();
		return;
	}

	player->resetIdleTime();
	player->setNextWalkActionTask(nullptr);

	player->startAutoWalk(direction);
}

bool Game::playerBroadcastMessage(const std::shared_ptr<Player>& player, const std::string& text) const
{
	if (!player->hasFlag(PlayerFlag_CanBroadcast)) {
		return false;
	}

	std::cout << "> " << player->getName() << " broadcasted: \"" << text << "\"." << std::endl;

	for (const auto& it : players) {
		it.second->sendPrivateMessage(player, TALKTYPE_BROADCAST, text);
	}

	return true;
}

void Game::playerCreatePrivateChannel(uint32_t playerId)
{
	const auto& player = getPlayerByID(playerId);
	if (!player || !player->isPremium()) {
		return;
	}

	ChatChannel* channel = g_chat->createChannel(player, CHANNEL_PRIVATE);
	if (!channel || !channel->addUser(player)) {
		return;
	}

	player->sendCreatePrivateChannel(channel->getId(), channel->getName());
}

void Game::playerChannelInvite(uint32_t playerId, const std::string& name)
{
	const auto& player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	PrivateChatChannel* channel = g_chat->getPrivateChannel(player);
	if (!channel) {
		return;
	}

	if (const auto& invitePlayer = getPlayerByName(name)) {
		if (player != invitePlayer) {
			channel->invitePlayer(player, invitePlayer);
		}
	}
}

void Game::playerChannelExclude(uint32_t playerId, const std::string& name)
{
	const auto& player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	PrivateChatChannel* channel = g_chat->getPrivateChannel(player);
	if (!channel) {
		return;
	}

	if (const auto& excludePlayer = getPlayerByName(name)) {
		if (player != excludePlayer) {
			channel->excludePlayer(player, excludePlayer);
		}
	}
}

void Game::playerRequestChannels(uint32_t playerId)
{
	if (const auto& player = getPlayerByID(playerId)) {
		player->sendChannelsDialog();
	}
}

void Game::playerOpenChannel(uint32_t playerId, uint16_t channelId)
{
	const auto& player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	ChatChannel* channel = g_chat->addUserToChannel(player, channelId);
	if (!channel) {
		return;
	}

	const InvitedMap* invitedUsers = channel->getInvitedUsers();
	const UsersMap* users;
	if (!channel->isPublicChannel()) {
		users = &channel->getUsers();
	} else {
		users = nullptr;
	}

	player->sendChannel(channel->getId(), channel->getName(), users, invitedUsers);
}

void Game::playerCloseChannel(uint32_t playerId, uint16_t channelId)
{
	if (const auto& player = getPlayerByID(playerId)) {
		g_chat->removeUserFromChannel(player, channelId);
	}
}

void Game::playerOpenPrivateChannel(uint32_t playerId, std::string receiver)
{
	const auto& player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	if (!IOLoginData::formatPlayerName(receiver)) {
		player->sendCancelMessage("A player with this name does not exist.");
		return;
	}

	if (player->getName() == receiver) {
		player->sendCancelMessage("You cannot set up a private message channel with yourself.");
		return;
	}

	player->sendOpenPrivateChannel(receiver);
}

void Game::playerCloseNpcChannel(uint32_t playerId)
{
	const auto& player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	SpectatorVec spectators;
	map.getSpectators(spectators, player->getPosition());
	for (const auto& spectator : spectators) {
		if (const auto& npc = spectator->getNpc()) {
			npc->onPlayerCloseChannel(player);
		}
	}
}

void Game::playerReceivePing(uint32_t playerId)
{
	if (const auto& player = getPlayerByID(playerId)) {
		player->receivePing();
	}
}

void Game::playerReceivePingBack(uint32_t playerId)
{
	if (const auto& player = getPlayerByID(playerId)) {
		player->sendPingBack();
	}
}

void Game::playerAutoWalk(uint32_t playerId, const std::vector<Direction>& listDir)
{
	if (const auto& player = getPlayerByID(playerId)) {
		player->resetIdleTime();
		player->startAutoWalk(listDir);
	}
}

void Game::playerStopAutoWalk(uint32_t playerId)
{
	const auto& player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	player->stopWalk();
}

void Game::playerUseItemEx(uint32_t playerId, const Position& fromPos, uint8_t fromStackPos, uint16_t fromSpriteId,
                           const Position& toPos, uint8_t toStackPos, uint16_t toSpriteId)
{
	const auto& player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	bool isHotkey = (fromPos.x == 0xFFFF && fromPos.y == 0 && fromPos.z == 0);
	if (isHotkey && !getBoolean(ConfigManager::AIMBOT_HOTKEY_ENABLED)) {
		return;
	}

	const auto& thing = internalGetThing(player, fromPos, fromStackPos, fromSpriteId, STACKPOS_USEITEM);
	if (!thing) {
		player->sendCancelMessage(RETURNVALUE_NOTPOSSIBLE);
		return;
	}

	const auto& item = thing->getItem();
	if (!item || !item->isUseable() || item->getClientID() != fromSpriteId) {
		player->sendCancelMessage(RETURNVALUE_CANNOTUSETHISOBJECT);
		return;
	}

	Position walkToPos = fromPos;
	ReturnValue ret = g_actions->canUse(player, fromPos);
	if (ret == RETURNVALUE_NOERROR) {
		ret = g_actions->canUse(player, toPos, item);
		if (ret == RETURNVALUE_TOOFARAWAY) {
			walkToPos = toPos;
		}
	}

	if (ret != RETURNVALUE_NOERROR) {
		if (ret == RETURNVALUE_TOOFARAWAY) {
			Position itemPos = fromPos;
			uint8_t itemStackPos = fromStackPos;

			if (fromPos.x != 0xFFFF && toPos.x != 0xFFFF && fromPos.isInRange(player->getPosition(), 1, 1, 0) &&
			    !fromPos.isInRange(toPos, 1, 1, 0)) {
				std::shared_ptr<Item> moveItem = nullptr;

				ret = internalMoveItem(item->getParent(), player, INDEX_WHEREEVER, item, item->getItemCount(),
				                       &moveItem, 0, player, nullptr, &fromPos, &toPos);
				if (ret != RETURNVALUE_NOERROR) {
					player->sendCancelMessage(ret);
					return;
				}

				// changing the position since its now in the inventory of the player
				std::tie(itemPos, itemStackPos) = internalGetPosition(moveItem);
			}

			std::vector<Direction> listDir;
			if (player->getPathTo(walkToPos, listDir, 0, 1, true, true)) {
				g_dispatcher.addTask([this, playerID = player->getID(), listDir = std::move(listDir)]() {
					playerAutoWalk(playerID, listDir);
				});
				SchedulerTask* task = createSchedulerTask(RANGE_USE_ITEM_EX_INTERVAL, [=, this]() {
					playerUseItemEx(playerId, itemPos, itemStackPos, fromSpriteId, toPos, toStackPos, toSpriteId);
				});
				player->setNextWalkActionTask(task);
			} else {
				player->sendCancelMessage(RETURNVALUE_THEREISNOWAY);
			}
			return;
		}

		player->sendCancelMessage(ret);
		return;
	}

	if (!player->canDoAction()) {
		uint32_t delay = player->getNextActionTime();
		SchedulerTask* task = createSchedulerTask(delay, [=, this]() {
			playerUseItemEx(playerId, fromPos, fromStackPos, fromSpriteId, toPos, toStackPos, toSpriteId);
		});
		player->setNextActionTask(task);
		return;
	}

	player->resetIdleTime();
	player->setNextActionTask(nullptr);

	g_actions->useItemEx(player, fromPos, toPos, toStackPos, item, isHotkey);
}

void Game::playerUseItem(uint32_t playerId, const Position& pos, uint8_t stackPos, uint8_t index, uint16_t spriteId)
{
	const auto& player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	bool isHotkey = (pos.x == 0xFFFF && pos.y == 0 && pos.z == 0);
	if (isHotkey && !getBoolean(ConfigManager::AIMBOT_HOTKEY_ENABLED)) {
		return;
	}

	const auto& thing = internalGetThing(player, pos, stackPos, spriteId, STACKPOS_USEITEM);
	if (!thing) {
		player->sendCancelMessage(RETURNVALUE_NOTPOSSIBLE);
		return;
	}

	const auto& item = thing->getItem();
	if (!item || item->isUseable() || item->getClientID() != spriteId) {
		player->sendCancelMessage(RETURNVALUE_CANNOTUSETHISOBJECT);
		return;
	}

	ReturnValue ret = g_actions->canUse(player, pos);
	if (ret != RETURNVALUE_NOERROR) {
		if (ret == RETURNVALUE_TOOFARAWAY) {
			std::vector<Direction> listDir;
			if (player->getPathTo(pos, listDir, 0, 1, true, true)) {
				g_dispatcher.addTask([this, playerID = player->getID(), listDir = std::move(listDir)]() {
					playerAutoWalk(playerID, listDir);
				});
				SchedulerTask* task = createSchedulerTask(
				    RANGE_USE_ITEM_INTERVAL, [=, this]() { playerUseItem(playerId, pos, stackPos, index, spriteId); });
				player->setNextWalkActionTask(task);
				return;
			}

			ret = RETURNVALUE_THEREISNOWAY;
		}

		player->sendCancelMessage(ret);
		return;
	}

	if (!player->canDoAction()) {
		uint32_t delay = player->getNextActionTime();
		SchedulerTask* task =
		    createSchedulerTask(delay, [=, this]() { playerUseItem(playerId, pos, stackPos, index, spriteId); });
		player->setNextActionTask(task);
		return;
	}

	player->resetIdleTime();
	player->setNextActionTask(nullptr);

	g_actions->useItem(player, pos, index, item, isHotkey);
}

void Game::playerUseWithCreature(uint32_t playerId, const Position& fromPos, uint8_t fromStackPos, uint32_t creatureId,
                                 uint16_t spriteId)
{
	const auto& player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	const auto& creature = getCreatureByID(creatureId);
	if (!creature) {
		return;
	}

	if (!creature->getPosition().isInRange(player->getPosition(), Map::maxClientViewportX - 1,
	                                       Map::maxClientViewportY - 1, 0)) {
		return;
	}

	bool isHotkey = (fromPos.x == 0xFFFF && fromPos.y == 0 && fromPos.z == 0);
	if (!getBoolean(ConfigManager::AIMBOT_HOTKEY_ENABLED)) {
		if (creature->getPlayer() || isHotkey) {
			player->sendCancelMessage(RETURNVALUE_DIRECTPLAYERSHOOT);
			return;
		}
	}

	const auto& thing = internalGetThing(player, fromPos, fromStackPos, spriteId, STACKPOS_USEITEM);
	if (!thing) {
		player->sendCancelMessage(RETURNVALUE_NOTPOSSIBLE);
		return;
	}

	const auto& item = thing->getItem();
	if (!item || !item->isUseable() || item->getClientID() != spriteId) {
		player->sendCancelMessage(RETURNVALUE_CANNOTUSETHISOBJECT);
		return;
	}

	Position toPos = creature->getPosition();
	Position walkToPos = fromPos;
	ReturnValue ret = g_actions->canUse(player, fromPos);
	if (ret == RETURNVALUE_NOERROR) {
		ret = g_actions->canUse(player, toPos, item);
		if (ret == RETURNVALUE_TOOFARAWAY) {
			walkToPos = toPos;
		}
	}

	if (ret != RETURNVALUE_NOERROR) {
		if (ret == RETURNVALUE_TOOFARAWAY) {
			Position itemPos = fromPos;
			uint8_t itemStackPos = fromStackPos;

			if (fromPos.x != 0xFFFF && fromPos.isInRange(player->getPosition(), 1, 1, 0) &&
			    !fromPos.isInRange(toPos, 1, 1, 0)) {
				std::shared_ptr<Item> moveItem = nullptr;
				ret = internalMoveItem(item->getParent(), player, INDEX_WHEREEVER, item, item->getItemCount(),
				                       &moveItem, 0, player, nullptr, &fromPos, &toPos);
				if (ret != RETURNVALUE_NOERROR) {
					player->sendCancelMessage(ret);
					return;
				}

				// changing the position since its now in the inventory of the player
				std::tie(itemPos, itemStackPos) = internalGetPosition(moveItem);
			}

			std::vector<Direction> listDir;
			if (player->getPathTo(walkToPos, listDir, 0, 1, true, true)) {
				g_dispatcher.addTask([this, playerID = player->getID(), listDir = std::move(listDir)]() {
					playerAutoWalk(playerID, listDir);
				});
				SchedulerTask* task = createSchedulerTask(RANGE_USE_WITH_CREATURE_INTERVAL, [=, this]() {
					playerUseWithCreature(playerId, itemPos, itemStackPos, creatureId, spriteId);
				});
				player->setNextWalkActionTask(task);
			} else {
				player->sendCancelMessage(RETURNVALUE_THEREISNOWAY);
			}
			return;
		}

		player->sendCancelMessage(ret);
		return;
	}

	if (!player->canDoAction()) {
		uint32_t delay = player->getNextActionTime();
		SchedulerTask* task = createSchedulerTask(
		    delay, [=, this]() { playerUseWithCreature(playerId, fromPos, fromStackPos, creatureId, spriteId); });
		player->setNextActionTask(task);
		return;
	}

	player->resetIdleTime();
	player->setNextActionTask(nullptr);

	g_actions->useItemEx(player, fromPos, creature->getPosition(), creature->getParent()->getThingIndex(creature), item,
	                     isHotkey, creature);
}

void Game::playerCloseContainer(uint32_t playerId, uint8_t cid)
{
	const auto& player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	player->closeContainer(cid);
	player->sendCloseContainer(cid);
}

static auto createBrowseField(const std::shared_ptr<Tile>& tile)
{
	auto browseField = std::make_shared<Container>(ITEM_BROWSEFIELD);

	TileItemVector* itemVector = tile->getItemList();
	if (itemVector) {
		for (const auto& item : *itemVector) {
			if ((item->getContainer() || item->hasProperty(CONST_PROP_MOVEABLE)) &&
			    !item->hasAttribute(ITEM_ATTRIBUTE_UNIQUEID)) {
				browseField->addItem(item);
			}
		}
	}

	browseField->setParent(tile);
	return browseField;
}

void Game::playerMoveUpContainer(uint32_t playerId, uint8_t cid)
{
	const auto& player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	const auto& container = player->getContainerByID(cid);
	if (!container) {
		return;
	}

	const auto& realParent = container->getRealParent();
	auto parentContainer = realParent->getContainer();
	if (!parentContainer) {
		const auto& tile = container->getTile();
		if (!tile) {
			return;
		}

		if (!tfs::events::player::onBrowseField(player, tile->getPosition())) {
			return;
		}

		auto it = browseFields.find(tile.get());
		if (it == browseFields.end()) {
			parentContainer = createBrowseField(tile);
			browseFields[tile.get()] = parentContainer;
			g_scheduler.addEvent(createSchedulerTask(30000, [this, tile]() { browseFields.erase(tile.get()); }));
		} else {
			parentContainer = it->second;
		}
	}

	player->addContainer(cid, parentContainer);
	player->sendContainer(cid, parentContainer, player->getContainerIndex(cid));
}

void Game::playerUpdateContainer(uint32_t playerId, uint8_t cid)
{
	if (const auto& player = getPlayerByID(playerId)) {
		if (const auto& container = player->getContainerByID(cid)) {
			player->sendContainer(cid, container, player->getContainerIndex(cid));
		}
	}
}

void Game::playerRotateItem(uint32_t playerId, const Position& pos, uint8_t stackPos, const uint16_t spriteId)
{
	const auto& player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	const auto& thing = internalGetThing(player, pos, stackPos, 0, STACKPOS_TOPDOWN_ITEM);
	if (!thing) {
		return;
	}

	const auto& item = thing->getItem();
	if (!item || item->getClientID() != spriteId || (!item->isRotatable() && !item->isPodium()) ||
	    item->hasAttribute(ITEM_ATTRIBUTE_UNIQUEID)) {
		player->sendCancelMessage(RETURNVALUE_NOTPOSSIBLE);
		return;
	}

	if (pos.x != 0xFFFF && !pos.isInRange(player->getPosition(), 1, 1, 0)) {
		std::vector<Direction> listDir;
		if (player->getPathTo(pos, listDir, 0, 1, true, true)) {
			g_dispatcher.addTask([this, playerID = player->getID(), listDir = std::move(listDir)]() {
				playerAutoWalk(playerID, listDir);
			});
			SchedulerTask* task = createSchedulerTask(
			    RANGE_ROTATE_ITEM_INTERVAL, [=, this]() { playerRotateItem(playerId, pos, stackPos, spriteId); });
			player->setNextWalkActionTask(task);
		} else {
			player->sendCancelMessage(RETURNVALUE_THEREISNOWAY);
		}
		return;
	}

	if (const auto& podium = item->getPodium()) {
		podium->setDirection(static_cast<Direction>((podium->getDirection() + 1) % 4));
		updatePodium(item);
	} else {
		tfs::events::player::onRotateItem(player, item);
	}
}

void Game::playerWriteItem(uint32_t playerId, uint32_t windowTextId, std::string_view text)
{
	const auto& player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	uint16_t maxTextLength = 0;
	uint32_t internalWindowTextId = 0;

	const auto& writeItem = player->getWriteItem(internalWindowTextId, maxTextLength);
	if (text.length() > maxTextLength || windowTextId != internalWindowTextId) {
		return;
	}

	if (!writeItem || writeItem->isRemoved()) {
		player->sendCancelMessage(RETURNVALUE_NOTPOSSIBLE);
		return;
	}

	const auto& topParent = writeItem->getTopParent();
	if (topParent != player) {
		player->sendCancelMessage(RETURNVALUE_NOTPOSSIBLE);
		return;
	}

	if (const auto& creature = topParent->getCreature()) {
		if (creature->getPlayer()) {
			player->sendCancelMessage(RETURNVALUE_NOTPOSSIBLE);
			return;
		}
	}

	if (!writeItem->getPosition().isInRange(player->getPosition(), 1, 1, 0)) {
		player->sendCancelMessage(RETURNVALUE_NOTPOSSIBLE);
		return;
	}

	for (auto creatureEvent : player->getCreatureEvents(CREATURE_EVENT_TEXTEDIT)) {
		if (!creatureEvent->executeTextEdit(player, writeItem, text, windowTextId)) {
			player->setWriteItem(nullptr);
			return;
		}
	}

	if (!text.empty()) {
		if (writeItem->getText() != text) {
			writeItem->setText(text);
			writeItem->setWriter(player->getName());
			writeItem->setDate(time(nullptr));
		}
	} else {
		writeItem->resetText();
		writeItem->resetWriter();
		writeItem->resetDate();
	}

	uint16_t newId = Item::items[writeItem->getID()].writeOnceItemId;
	if (newId != 0) {
		transformItem(writeItem, newId);
	}

	player->setWriteItem(nullptr);
}

void Game::playerBrowseField(uint32_t playerId, const Position& pos)
{
	const auto& player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	const Position& playerPos = player->getPosition();
	if (playerPos.z != pos.z) {
		player->sendCancelMessage(playerPos.z > pos.z ? RETURNVALUE_FIRSTGOUPSTAIRS : RETURNVALUE_FIRSTGODOWNSTAIRS);
		return;
	}

	if (!playerPos.isInRange(pos, 1, 1)) {
		std::vector<Direction> listDir;
		if (player->getPathTo(pos, listDir, 0, 1, true, true)) {
			g_dispatcher.addTask([this, playerID = player->getID(), listDir = std::move(listDir)]() {
				playerAutoWalk(playerID, listDir);
			});
			SchedulerTask* task =
			    createSchedulerTask(RANGE_BROWSE_FIELD_INTERVAL, [=, this]() { playerBrowseField(playerId, pos); });
			player->setNextWalkActionTask(task);
		} else {
			player->sendCancelMessage(RETURNVALUE_THEREISNOWAY);
		}
		return;
	}

	const auto& tile = map.getTile(pos);
	if (!tile) {
		return;
	}

	if (!tfs::events::player::onBrowseField(player, pos)) {
		return;
	}

	std::shared_ptr<Container> container;

	auto it = browseFields.find(tile.get());
	if (it == browseFields.end()) {
		container = createBrowseField(tile);
		browseFields[tile.get()] = container;
		g_scheduler.addEvent(createSchedulerTask(30000, [this, tile]() { browseFields.erase(tile.get()); }));
	} else {
		container = it->second;
	}

	uint8_t dummyContainerId = 0xF - ((pos.x % 3) * 3 + (pos.y % 3));
	if (const auto& openContainer = player->getContainerByID(dummyContainerId)) {
		player->onCloseContainer(openContainer);
		player->closeContainer(dummyContainerId);
	} else {
		player->addContainer(dummyContainerId, container);
		player->sendContainer(dummyContainerId, container, 0);
	}
}

void Game::playerSeekInContainer(uint32_t playerId, uint8_t containerId, uint16_t index)
{
	const auto& player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	const auto& container = player->getContainerByID(containerId);
	if (!container || !container->hasPagination()) {
		return;
	}

	if ((index % container->capacity()) != 0 || index >= container->size()) {
		return;
	}

	player->setContainerIndex(containerId, index);
	player->sendContainer(containerId, container, index);
}

void Game::playerUpdateHouseWindow(uint32_t playerId, uint8_t listId, uint32_t windowTextId, const std::string& text)
{
	const auto& player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	uint32_t internalWindowTextId;
	uint32_t internalListId;

	House* house = player->getEditHouse(internalWindowTextId, internalListId);
	if (house && house->canEditAccessList(internalListId, player) && internalWindowTextId == windowTextId &&
	    listId == 0) {
		house->setAccessList(internalListId, text);
	}

	player->setEditHouse(nullptr);
}

void Game::playerWrapItem(uint32_t playerId, const Position& position, uint8_t stackPos, const uint16_t spriteId)
{
	const auto& player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	const auto& thing = internalGetThing(player, position, stackPos, 0, STACKPOS_TOPDOWN_ITEM);
	if (!thing) {
		return;
	}

	const auto& item = thing->getItem();
	if (!item || item->getClientID() != spriteId || !item->hasAttribute(ITEM_ATTRIBUTE_WRAPID) ||
	    item->hasAttribute(ITEM_ATTRIBUTE_UNIQUEID)) {
		player->sendCancelMessage(RETURNVALUE_NOTPOSSIBLE);
		return;
	}

	if (position.x != 0xFFFF && !position.isInRange(player->getPosition(), 1, 1, 0)) {
		std::vector<Direction> listDir;
		if (player->getPathTo(position, listDir, 0, 1, true, true)) {
			g_dispatcher.addTask([this, playerID = player->getID(), listDir = std::move(listDir)]() {
				playerAutoWalk(playerID, listDir);
			});
			SchedulerTask* task = createSchedulerTask(
			    RANGE_WRAP_ITEM_INTERVAL, [=, this]() { playerWrapItem(playerId, position, stackPos, spriteId); });
			player->setNextWalkActionTask(task);
		} else {
			player->sendCancelMessage(RETURNVALUE_THEREISNOWAY);
		}
		return;
	}

	tfs::events::player::onWrapItem(player, item);
}

void Game::playerRequestTrade(uint32_t playerId, const Position& pos, uint8_t stackPos, uint32_t tradePlayerId,
                              uint16_t spriteId)
{
	const auto& player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	const auto& tradePartner = getPlayerByID(tradePlayerId);
	if (!tradePartner || tradePartner == player) {
		player->sendCancelMessage("Select a player to trade with.");
		return;
	}

	if (!tradePartner->getPosition().isInRange(player->getPosition(), 2, 2, 0)) {
		player->sendCancelMessage(RETURNVALUE_DESTINATIONOUTOFREACH);
		return;
	}

	if (!canThrowObjectTo(tradePartner->getPosition(), player->getPosition(), true, true)) {
		player->sendCancelMessage(RETURNVALUE_CANNOTTHROW);
		return;
	}

	const auto& tradeThing = internalGetThing(player, pos, stackPos, 0, STACKPOS_TOPDOWN_ITEM);
	if (!tradeThing) {
		player->sendCancelMessage(RETURNVALUE_NOTPOSSIBLE);
		return;
	}

	const auto& tradeItem = tradeThing->getItem();
	if (tradeItem->getClientID() != spriteId || !tradeItem->isPickupable() ||
	    tradeItem->hasAttribute(ITEM_ATTRIBUTE_UNIQUEID)) {
		player->sendCancelMessage(RETURNVALUE_NOTPOSSIBLE);
		return;
	}

	if (getBoolean(ConfigManager::ONLY_INVITED_CAN_MOVE_HOUSE_ITEMS)) {
		if (const auto& tile = tradeItem->getTile()) {
			if (const auto& houseTile = tile->getHouseTile()) {
				if (!tradeItem->getTopParent()->getCreature() && !houseTile->getHouse()->isInvited(player)) {
					player->sendCancelMessage(RETURNVALUE_PLAYERISNOTINVITED);
					return;
				}
			}
		}
	}

	const Position& playerPosition = player->getPosition();
	const Position& tradeItemPosition = tradeItem->getPosition();
	if (playerPosition.z != tradeItemPosition.z) {
		player->sendCancelMessage(playerPosition.z > tradeItemPosition.z ? RETURNVALUE_FIRSTGOUPSTAIRS
		                                                                 : RETURNVALUE_FIRSTGODOWNSTAIRS);
		return;
	}

	if (!tradeItemPosition.isInRange(playerPosition, 1, 1)) {
		std::vector<Direction> listDir;
		if (player->getPathTo(pos, listDir, 0, 1, true, true)) {
			g_dispatcher.addTask([this, playerID = player->getID(), listDir = std::move(listDir)]() {
				playerAutoWalk(playerID, listDir);
			});
			SchedulerTask* task = createSchedulerTask(RANGE_REQUEST_TRADE_INTERVAL, [=, this]() {
				playerRequestTrade(playerId, pos, stackPos, tradePlayerId, spriteId);
			});
			player->setNextWalkActionTask(task);
		} else {
			player->sendCancelMessage(RETURNVALUE_THEREISNOWAY);
		}
		return;
	}

	const auto& tradeItemContainer = tradeItem->getContainer();
	if (tradeItemContainer) {
		for (const auto& item : tradeItems | std::views::keys) {
			if (tradeItem == item) {
				player->sendCancelMessage("This item is already being traded.");
				return;
			}

			if (tradeItemContainer->isHoldingItem(item)) {
				player->sendCancelMessage("This item is already being traded.");
				return;
			}

			const auto& container = item->getContainer();
			if (container && container->isHoldingItem(tradeItem)) {
				player->sendCancelMessage("This item is already being traded.");
				return;
			}
		}
	} else {
		for (const auto& item : tradeItems | std::views::keys) {
			if (tradeItem == item) {
				player->sendCancelMessage("This item is already being traded.");
				return;
			}

			const auto& container = item->getContainer();
			if (container && container->isHoldingItem(tradeItem)) {
				player->sendCancelMessage("This item is already being traded.");
				return;
			}
		}
	}

	if (const auto& tradeContainer = tradeItem->getContainer()) {
		if (tradeContainer->getItemHoldingCount() + 1 > ITEM_STACK_SIZE) {
			player->sendCancelMessage(std::format("You can only trade up to {} objects at once.", ITEM_STACK_SIZE));
			return;
		}
	}

	if (!tfs::events::player::onTradeRequest(player, tradePartner, tradeItem)) {
		return;
	}

	internalStartTrade(player, tradePartner, tradeItem);
}

bool Game::internalStartTrade(const std::shared_ptr<Player>& player, const std::shared_ptr<Player>& tradePartner,
                              const std::shared_ptr<Item>& tradeItem)
{
	if (player->getTradeState() != TRADE_NONE &&
	    !(player->getTradeState() == TRADE_ACKNOWLEDGE && player->getTradePartner() == tradePartner)) {
		player->sendCancelMessage(RETURNVALUE_YOUAREALREADYTRADING);
		return false;
	} else if (tradePartner->getTradeState() != TRADE_NONE && tradePartner->getTradePartner() != player) {
		player->sendCancelMessage(RETURNVALUE_THISPLAYERISALREADYTRADING);
		return false;
	}

	player->setTradePartner(tradePartner);
	player->setTradeItem(tradeItem);
	player->setTradeState(TRADE_INITIATED);
	tradeItems[tradeItem] = player->getID();

	player->sendTradeItemRequest(player->getName(), tradeItem, true);

	if (tradePartner->getTradeState() == TRADE_NONE) {
		tradePartner->sendTextMessage(MESSAGE_TRADE, fmt::format("{:s} wants to trade with you.", player->getName()));
		tradePartner->setTradeState(TRADE_ACKNOWLEDGE);
		tradePartner->setTradePartner(player);
	} else {
		const auto& counterOfferItem = tradePartner->getTradeItem();
		player->sendTradeItemRequest(tradePartner->getName(), counterOfferItem, false);
		tradePartner->sendTradeItemRequest(player->getName(), tradeItem, false);
	}

	return true;
}

void Game::playerAcceptTrade(uint32_t playerId)
{
	const auto& player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	if (!(player->getTradeState() == TRADE_ACKNOWLEDGE || player->getTradeState() == TRADE_INITIATED)) {
		return;
	}

	const auto& tradePartner = player->getTradePartner();
	if (!tradePartner) {
		return;
	}

	player->setTradeState(TRADE_ACCEPT);

	if (tradePartner->getTradeState() == TRADE_ACCEPT) {
		if (!canThrowObjectTo(tradePartner->getPosition(), player->getPosition(), true, true)) {
			internalCloseTrade(player, false);
			player->sendCancelMessage(RETURNVALUE_CANNOTTHROW);
			tradePartner->sendCancelMessage(RETURNVALUE_CANNOTTHROW);
			return;
		}

		const auto& playerTradeItem = player->getTradeItem();
		const auto& partnerTradeItem = tradePartner->getTradeItem();

		if (!tfs::events::player::onTradeAccept(player, tradePartner, playerTradeItem, partnerTradeItem)) {
			internalCloseTrade(player, false);
			return;
		}

		player->setTradeState(TRADE_TRANSFER);
		tradePartner->setTradeState(TRADE_TRANSFER);

		auto it = tradeItems.find(playerTradeItem);
		if (it != tradeItems.end()) {
			tradeItems.erase(it);
		}

		it = tradeItems.find(partnerTradeItem);
		if (it != tradeItems.end()) {
			tradeItems.erase(it);
		}

		bool isSuccess = false;

		ReturnValue tradePartnerRet = RETURNVALUE_NOERROR;
		ReturnValue playerRet = RETURNVALUE_NOERROR;

		// if player is trying to trade its own backpack
		if (tradePartner->getInventoryItem(CONST_SLOT_BACKPACK) == partnerTradeItem) {
			tradePartnerRet = (tradePartner->getInventoryItem(getSlotType(Item::items[playerTradeItem->getID()]))
			                       ? RETURNVALUE_NOTENOUGHROOM
			                       : RETURNVALUE_NOERROR);
		}

		if (player->getInventoryItem(CONST_SLOT_BACKPACK) == playerTradeItem) {
			playerRet = (player->getInventoryItem(getSlotType(Item::items[partnerTradeItem->getID()]))
			                 ? RETURNVALUE_NOTENOUGHROOM
			                 : RETURNVALUE_NOERROR);
		}

		// both players try to trade equipped backpacks
		if (player->getInventoryItem(CONST_SLOT_BACKPACK) == playerTradeItem &&
		    tradePartner->getInventoryItem(CONST_SLOT_BACKPACK) == partnerTradeItem) {
			playerRet = RETURNVALUE_NOTENOUGHROOM;
		}

		if (tradePartnerRet == RETURNVALUE_NOERROR && playerRet == RETURNVALUE_NOERROR) {
			tradePartnerRet = internalAddItem(tradePartner, playerTradeItem, INDEX_WHEREEVER, 0, true);
			playerRet = internalAddItem(player, partnerTradeItem, INDEX_WHEREEVER, 0, true);
			if (tradePartnerRet == RETURNVALUE_NOERROR && playerRet == RETURNVALUE_NOERROR) {
				playerRet = internalRemoveItem(playerTradeItem, playerTradeItem->getItemCount(), true);
				tradePartnerRet = internalRemoveItem(partnerTradeItem, partnerTradeItem->getItemCount(), true);
				if (tradePartnerRet == RETURNVALUE_NOERROR && playerRet == RETURNVALUE_NOERROR) {
					tradePartnerRet = internalMoveItem(playerTradeItem->getParent(), tradePartner, INDEX_WHEREEVER,
					                                   playerTradeItem, playerTradeItem->getItemCount(), nullptr,
					                                   FLAG_IGNOREAUTOSTACK, nullptr, partnerTradeItem);
					if (tradePartnerRet == RETURNVALUE_NOERROR) {
						internalMoveItem(partnerTradeItem->getParent(), player, INDEX_WHEREEVER, partnerTradeItem,
						                 partnerTradeItem->getItemCount(), nullptr, FLAG_IGNOREAUTOSTACK);
						playerTradeItem->onTradeEvent(ON_TRADE_TRANSFER, tradePartner);
						partnerTradeItem->onTradeEvent(ON_TRADE_TRANSFER, player);
						isSuccess = true;
					}
				}
			}
		}

		if (!isSuccess) {
			std::string errorDescription;

			if (const auto& tradeItem = tradePartner->getTradeItem()) {
				errorDescription = getTradeErrorDescription(tradePartnerRet, playerTradeItem);
				tradePartner->sendTextMessage(MESSAGE_EVENT_ADVANCE, errorDescription);
				tradeItem->onTradeEvent(ON_TRADE_CANCEL, tradePartner);
			}

			if (const auto& tradeItem = player->getTradeItem()) {
				errorDescription = getTradeErrorDescription(playerRet, partnerTradeItem);
				player->sendTextMessage(MESSAGE_EVENT_ADVANCE, errorDescription);
				tradeItem->onTradeEvent(ON_TRADE_CANCEL, player);
			}
		}

		tfs::events::player::onTradeCompleted(player, tradePartner, playerTradeItem, partnerTradeItem, isSuccess);

		player->setTradeState(TRADE_NONE);
		player->setTradeItem(nullptr);
		player->setTradePartner(nullptr);
		player->sendTradeClose();

		tradePartner->setTradeState(TRADE_NONE);
		tradePartner->setTradeItem(nullptr);
		tradePartner->setTradePartner(nullptr);
		tradePartner->sendTradeClose();
	}
}

std::string Game::getTradeErrorDescription(ReturnValue ret, const std::shared_ptr<Item>& item)
{
	if (item) {
		if (ret == RETURNVALUE_NOTENOUGHCAPACITY) {
			return fmt::format("You do not have enough capacity to carry {:s}.\n {:s}",
			                   item->isStackable() && item->getItemCount() > 1 ? "these objects" : "this object",
			                   item->getWeightDescription());
		} else if (ret == RETURNVALUE_NOTENOUGHROOM || ret == RETURNVALUE_CONTAINERNOTENOUGHROOM) {
			return fmt::format("You do not have enough room to carry {:s}.",
			                   item->isStackable() && item->getItemCount() > 1 ? "these objects" : "this object");
		}
	}
	return "Trade could not be completed.";
}

void Game::playerLookInTrade(uint32_t playerId, bool lookAtCounterOffer, uint8_t index)
{
	const auto& player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	const auto& tradePartner = player->getTradePartner();
	if (!tradePartner) {
		return;
	}

	const auto& tradeItem = lookAtCounterOffer ? tradePartner->getTradeItem() : player->getTradeItem();
	if (!tradeItem) {
		return;
	}

	const Position& playerPosition = player->getPosition();
	const Position& tradeItemPosition = tradeItem->getPosition();

	int32_t lookDistance =
	    std::max(playerPosition.getDistanceX(tradeItemPosition), playerPosition.getDistanceY(tradeItemPosition));
	if (index == 0) {
		tfs::events::player::onLookInTrade(player, tradePartner, tradeItem, lookDistance);
		return;
	}

	const auto& tradeContainer = tradeItem->getContainer();
	if (!tradeContainer) {
		return;
	}

	auto containers = std::vector{tradeContainer};
	size_t i = 0;
	while (i < containers.size()) {
		const auto& container = containers[i++];
		for (const auto& item : container->getItemList()) {
			if (const auto& childContainer = item->getContainer()) {
				containers.push_back(childContainer);
			}

			if (--index == 0) {
				tfs::events::player::onLookInTrade(player, tradePartner, item, lookDistance);
				return;
			}
		}
	}
}

void Game::playerCloseTrade(uint32_t playerId)
{
	if (const auto& player = getPlayerByID(playerId)) {
		internalCloseTrade(player);
	}
}

void Game::internalCloseTrade(const std::shared_ptr<Player>& player, bool sendCancel /* = true*/)
{
	const auto& tradePartner = player->getTradePartner();
	if ((tradePartner && tradePartner->getTradeState() == TRADE_TRANSFER) ||
	    player->getTradeState() == TRADE_TRANSFER) {
		return;
	}

	if (const auto& tradeItem = player->getTradeItem()) {
		tradeItems.erase(tradeItem);

		tradeItem->onTradeEvent(ON_TRADE_CANCEL, player);
		player->setTradeItem(nullptr);
	}

	player->setTradeState(TRADE_NONE);
	player->setTradePartner(nullptr);

	if (sendCancel) {
		player->sendTextMessage(MESSAGE_STATUS_SMALL, "Trade cancelled.");
	}
	player->sendTradeClose();

	if (tradePartner) {
		if (const auto& tradeItem = tradePartner->getTradeItem()) {
			tradeItems.erase(tradeItem);

			tradeItem->onTradeEvent(ON_TRADE_CANCEL, tradePartner);
			tradePartner->setTradeItem(nullptr);
		}

		tradePartner->setTradeState(TRADE_NONE);
		tradePartner->setTradePartner(nullptr);

		if (sendCancel) {
			tradePartner->sendTextMessage(MESSAGE_STATUS_SMALL, "Trade cancelled.");
		}
		tradePartner->sendTradeClose();
	}
}

void Game::playerPurchaseItem(uint32_t playerId, uint16_t spriteId, uint8_t count, uint16_t amount,
                              bool ignoreCap /* = false*/, bool inBackpacks /* = false*/)
{
	if (amount == 0 || amount > ITEM_STACK_SIZE) {
		return;
	}

	const auto& player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	int32_t onBuy, onSell;

	const auto& merchant = player->getShopOwner(onBuy, onSell);
	if (!merchant) {
		return;
	}

	const ItemType& it = Item::items.getItemIdByClientId(spriteId);
	if (it.id == 0) {
		return;
	}

	uint8_t subType;
	if (it.isSplash() || it.isFluidContainer()) {
		subType = clientFluidToServer(count);
	} else {
		subType = count;
	}

	if (!player->hasShopItemForSale(it.id, subType)) {
		return;
	}

	merchant->onPlayerTrade(player, onBuy, it.id, subType, amount, ignoreCap, inBackpacks);
}

void Game::playerSellItem(uint32_t playerId, uint16_t spriteId, uint8_t count, uint16_t amount, bool ignoreEquipped)
{
	if (amount == 0 || amount > ITEM_STACK_SIZE) {
		return;
	}

	const auto& player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	int32_t onBuy, onSell;

	const auto& merchant = player->getShopOwner(onBuy, onSell);
	if (!merchant) {
		return;
	}

	const ItemType& it = Item::items.getItemIdByClientId(spriteId);
	if (it.id == 0) {
		return;
	}

	uint8_t subType;
	if (it.isSplash() || it.isFluidContainer()) {
		subType = clientFluidToServer(count);
	} else {
		subType = count;
	}

	merchant->onPlayerTrade(player, onSell, it.id, subType, amount, ignoreEquipped);
}

void Game::playerCloseShop(uint32_t playerId)
{
	if (const auto& player = getPlayerByID(playerId)) {
		player->closeShopWindow();
	}
}

void Game::playerLookInShop(uint32_t playerId, uint16_t spriteId, uint8_t count)
{
	const auto& player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	int32_t onBuy, onSell;

	const auto& merchant = player->getShopOwner(onBuy, onSell);
	if (!merchant) {
		return;
	}

	const ItemType& it = Item::items.getItemIdByClientId(spriteId);
	if (it.id == 0) {
		return;
	}

	int32_t subType;
	if (it.isFluidContainer() || it.isSplash()) {
		subType = clientFluidToServer(count);
	} else {
		subType = count;
	}

	if (!player->hasShopItemForSale(it.id, subType)) {
		return;
	}

	tfs::events::player::onLookInShop(player, &it, subType);
}

void Game::playerLookAt(uint32_t playerId, const Position& pos, uint8_t stackPos)
{
	const auto& player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	const auto& thing = internalGetThing(player, pos, stackPos, 0, STACKPOS_LOOK);
	if (!thing) {
		player->sendCancelMessage(RETURNVALUE_NOTPOSSIBLE);
		return;
	}

	Position thingPos = thing->getPosition();
	if (!player->canSee(thingPos)) {
		player->sendCancelMessage(RETURNVALUE_NOTPOSSIBLE);
		return;
	}

	Position playerPos = player->getPosition();

	int32_t lookDistance = -1;
	if (thing != player) {
		lookDistance = std::max(playerPos.getDistanceX(thingPos), playerPos.getDistanceY(thingPos));
		if (playerPos.z != thingPos.z) {
			lookDistance += 15;
		}
	}

	tfs::events::player::onLook(player, pos, thing, stackPos, lookDistance);
}

void Game::playerLookInBattleList(uint32_t playerId, uint32_t creatureId)
{
	const auto& player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	const auto& creature = getCreatureByID(creatureId);
	if (!creature) {
		return;
	}

	if (!player->canSeeCreature(creature)) {
		return;
	}

	const Position& creaturePos = creature->getPosition();
	if (!player->canSee(creaturePos)) {
		return;
	}

	int32_t lookDistance = -1;
	if (creature != player) {
		const Position& playerPos = player->getPosition();
		lookDistance = std::max(playerPos.getDistanceX(creaturePos), playerPos.getDistanceY(creaturePos));
		if (playerPos.z != creaturePos.z) {
			lookDistance += 15;
		}
	}

	tfs::events::player::onLookInBattleList(player, creature, lookDistance);
}

void Game::playerCancelAttackAndFollow(uint32_t playerId)
{
	if (const auto& player = getPlayerByID(playerId)) {
		playerSetAttackedCreature(playerId, 0);
		playerFollowCreature(playerId, 0);
		player->stopWalk();
	}
}

void Game::playerSetAttackedCreature(uint32_t playerId, uint32_t creatureId)
{
	const auto& player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	if (player->getAttackedCreature() && creatureId == 0) {
		player->removeAttackedCreature();
		player->sendCancelTarget();
		return;
	}

	const auto& attackCreature = getCreatureByID(creatureId);
	if (!attackCreature) {
		player->removeAttackedCreature();
		player->sendCancelTarget();
		return;
	}

	ReturnValue ret = Combat::canTargetCreature(player, attackCreature);
	if (ret != RETURNVALUE_NOERROR) {
		player->sendCancelMessage(ret);
		player->sendCancelTarget();
		player->removeAttackedCreature();
		return;
	}

	player->setAttackedCreature(attackCreature);

	g_dispatcher.addTask([this, id = player->getID()]() { updateCreatureWalk(id); });
}

void Game::playerFollowCreature(uint32_t playerId, uint32_t creatureId)
{
	const auto& player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	player->removeAttackedCreature();

	if (const auto& followCreature = getCreatureByID(creatureId)) {
		player->setFollowCreature(followCreature);
	} else {
		player->removeFollowCreature();
	}

	g_dispatcher.addTask([this, id = player->getID()]() { updateCreatureWalk(id); });
}

void Game::playerSetFightModes(uint32_t playerId, fightMode_t fightMode, bool chaseMode, bool secureMode)
{
	if (const auto& player = getPlayerByID(playerId)) {
		player->setFightMode(fightMode);
		player->setChaseMode(chaseMode);
		player->setSecureMode(secureMode);
	}
}

void Game::playerRequestAddVip(uint32_t playerId, const std::string& name)
{
	if (name.length() > PLAYER_NAME_LENGTH) {
		return;
	}

	const auto& player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	const auto& vipPlayer = getPlayerByName(name);
	if (!vipPlayer) {
		uint32_t guid;
		bool specialVip;
		std::string formattedName = name;
		if (!IOLoginData::getGuidByNameEx(guid, specialVip, formattedName)) {
			player->sendTextMessage(MESSAGE_STATUS_SMALL, "A player with this name does not exist.");
			return;
		}

		if (specialVip && !player->hasFlag(PlayerFlag_SpecialVIP)) {
			player->sendTextMessage(MESSAGE_STATUS_SMALL, "You can not add this player.");
			return;
		}

		player->addVIP(guid, formattedName, VIPSTATUS_OFFLINE);
	} else {
		if (vipPlayer->hasFlag(PlayerFlag_SpecialVIP) && !player->hasFlag(PlayerFlag_SpecialVIP)) {
			player->sendTextMessage(MESSAGE_STATUS_SMALL, "You can not add this player.");
			return;
		}

		if (!vipPlayer->isInGhostMode() || player->canSeeGhostMode(vipPlayer)) {
			player->addVIP(vipPlayer->getGUID(), vipPlayer->getName(), VIPSTATUS_ONLINE);
		} else {
			player->addVIP(vipPlayer->getGUID(), vipPlayer->getName(), VIPSTATUS_OFFLINE);
		}
	}
}

void Game::playerRequestRemoveVip(uint32_t playerId, uint32_t guid)
{
	if (const auto& player = getPlayerByID(playerId)) {
		player->removeVIP(guid);
	}
}

void Game::playerRequestEditVip(uint32_t playerId, uint32_t guid, const std::string& description, uint32_t icon,
                                bool notify)
{
	if (const auto& player = getPlayerByID(playerId)) {
		player->editVIP(guid, description, icon, notify);
	}
}

void Game::playerTurn(uint32_t playerId, Direction dir)
{
	if (const auto& player = getPlayerByID(playerId)) {
		if (tfs::events::player::onTurn(player, dir)) {
			player->resetIdleTime();
			internalCreatureTurn(player, dir);
		}
	}
}

void Game::playerRequestOutfit(uint32_t playerId)
{
	if (!getBoolean(ConfigManager::ALLOW_CHANGEOUTFIT)) {
		return;
	}

	if (const auto& player = getPlayerByID(playerId)) {
		player->sendOutfitWindow();
	}
}

void Game::playerRequestEditPodium(uint32_t playerId, const Position& position, uint8_t stackPos,
                                   const uint16_t spriteId)
{
	const auto& player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	const auto& thing = internalGetThing(player, position, stackPos, 0, STACKPOS_TOPDOWN_ITEM);
	if (!thing) {
		return;
	}

	const ItemType& it = Item::items.getItemIdByClientId(spriteId);
	if (it.id == 0) {
		return;
	}

	const auto& item = thing->getItem();
	if (!item || item->getClientID() != spriteId || it.type != ITEM_TYPE_PODIUM) {
		player->sendCancelMessage(RETURNVALUE_NOTPOSSIBLE);
		return;
	}

	// player has to walk to podium
	// gm/god can edit instantly
	if (!player->isAccessPlayer()) {
		if (position.x != 0xFFFF && !position.isInRange(player->getPosition(), 1, 1, 0)) {
			std::vector<Direction> listDir;
			if (player->getPathTo(position, listDir, 0, 1, true, true)) {
				g_dispatcher.addTask([this, playerID = player->getID(), listDir = std::move(listDir)]() {
					playerAutoWalk(playerID, listDir);
				});
				SchedulerTask* task = createSchedulerTask(
				    400, [=, this]() { playerRequestEditPodium(playerId, position, stackPos, spriteId); });
				player->setNextWalkActionTask(task);
			} else {
				player->sendCancelMessage(RETURNVALUE_THEREISNOWAY);
			}
			return;
		}
	}

	tfs::events::player::onPodiumRequest(player, item);
}

void Game::playerEditPodium(uint32_t playerId, Outfit_t outfit, const Position& position, uint8_t stackPos,
                            const uint16_t spriteId, bool podiumVisible, Direction direction)
{
	const auto& player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	const auto& thing = internalGetThing(player, position, stackPos, 0, STACKPOS_TOPDOWN_ITEM);
	if (!thing) {
		return;
	}

	const auto& item = thing->getItem();
	if (!item) {
		return;
	}

	const ItemType& it = Item::items.getItemIdByClientId(spriteId);
	if (it.id == 0) {
		return;
	}

	tfs::events::player::onPodiumEdit(player, item, outfit, podiumVisible, direction);
}

void Game::playerToggleMount(uint32_t playerId, bool mount)
{
	const auto& player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	player->toggleMount(mount);
}

void Game::playerChangeOutfit(uint32_t playerId, Outfit_t outfit, bool randomizeMount /* = false*/)
{
	if (!getBoolean(ConfigManager::ALLOW_CHANGEOUTFIT)) {
		return;
	}

	const auto& player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	player->setRandomizeMount(randomizeMount);

	const Outfit* playerOutfit = Outfits::getInstance().getOutfitByLookType(player->getSex(), outfit.lookType);
	if (!playerOutfit) {
		outfit.lookMount = 0;
	}

	if (outfit.lookMount != 0) {
		Mount* mount = mounts.getMountByClientID(outfit.lookMount);
		if (!mount) {
			return;
		}

		if (!player->hasMount(mount)) {
			return;
		}

		int32_t speedChange = mount->speed;
		if (player->isMounted()) {
			Mount* prevMount = mounts.getMountByID(player->getCurrentMount());
			if (prevMount) {
				speedChange -= prevMount->speed;
			}
		}

		changeSpeed(player, speedChange);
		player->setCurrentMount(mount->id);
	} else {
		if (player->isMounted()) {
			player->dismount();
		}

		player->setWasMounted(false);
	}

	if (player->canWear(outfit.lookType, outfit.lookAddons)) {
		player->defaultOutfit = outfit;

		if (player->hasCondition(CONDITION_OUTFIT)) {
			return;
		}

		if (player->getRandomizeMount() && player->hasMounts()) {
			const Mount* mount = mounts.getMountByID(player->getRandomMount());
			outfit.lookMount = mount->clientId;
		}

		internalCreatureChangeOutfit(player, outfit);
	}

	if (player->isMounted()) {
		player->onChangeZone(player->getZone());
	}
}

void Game::playerSay(uint32_t playerId, uint16_t channelId, SpeakClasses type, const std::string& receiver,
                     const std::string& text)
{
	const auto& player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	player->resetIdleTime();

	if (playerSaySpell(player, type, text)) {
		return;
	}

	if (type == TALKTYPE_PRIVATE_PN) {
		playerSpeakToNpc(player, text);
		return;
	}

	uint32_t muteTime = player->isMuted();
	if (muteTime > 0) {
		player->sendTextMessage(MESSAGE_STATUS_SMALL, fmt::format("You are still muted for {:d} seconds.", muteTime));
		return;
	}

	if (!text.empty() && text.front() == '/' && player->isAccessPlayer()) {
		return;
	}

	player->removeMessageBuffer();

	switch (type) {
		case TALKTYPE_SAY:
			internalCreatureSay(player, TALKTYPE_SAY, text, false);
			break;

		case TALKTYPE_WHISPER:
			playerWhisper(player, text);
			break;

		case TALKTYPE_YELL:
			playerYell(player, text);
			break;

		case TALKTYPE_PRIVATE_TO:
		case TALKTYPE_PRIVATE_RED_TO:
			playerSpeakTo(player, type, receiver, text);
			break;

		case TALKTYPE_CHANNEL_O:
		case TALKTYPE_CHANNEL_Y:
		case TALKTYPE_CHANNEL_R1:
			g_chat->talkToChannel(player, type, text, channelId);
			break;

		case TALKTYPE_BROADCAST:
			playerBroadcastMessage(player, text);
			break;

		default:
			break;
	}
}

bool Game::playerSaySpell(const std::shared_ptr<Player>& player, SpeakClasses type, const std::string& text)
{
	std::string words = text;

	TalkActionResult_t result = g_talkActions->playerSaySpell(player, type, words);
	if (result == TALKACTION_BREAK) {
		return true;
	}

	result = g_spells->playerSaySpell(player, words);
	if (result == TALKACTION_BREAK) {
		if (!getBoolean(ConfigManager::EMOTE_SPELLS)) {
			return internalCreatureSay(player, TALKTYPE_SPELL, words, false);
		}
		return internalCreatureSay(player, TALKTYPE_MONSTER_SAY, words, false);
	} else if (result == TALKACTION_FAILED) {
		return true;
	}

	return false;
}

void Game::playerWhisper(const std::shared_ptr<Player>& player, const std::string& text)
{
	SpectatorVec spectators;
	map.getSpectators(spectators, player->getPosition(), false, false, Map::maxClientViewportX, Map::maxClientViewportX,
	                  Map::maxClientViewportY, Map::maxClientViewportY);

	// send to client
	for (const auto& spectator : spectators) {
		if (const auto& spectatorPlayer = spectator->getPlayer()) {
			if (!player->getPosition().isInRange(spectatorPlayer->getPosition(), 1, 1)) {
				spectatorPlayer->sendCreatureSay(player, TALKTYPE_WHISPER, "pspsps");
			} else {
				spectatorPlayer->sendCreatureSay(player, TALKTYPE_WHISPER, text);
			}
		}
	}

	// event method
	for (const auto& spectator : spectators) {
		spectator->onCreatureSay(player, TALKTYPE_WHISPER, text);
	}
}

bool Game::playerYell(const std::shared_ptr<Player>& player, const std::string& text)
{
	if (player->hasCondition(CONDITION_YELLTICKS)) {
		player->sendCancelMessage(RETURNVALUE_YOUAREEXHAUSTED);
		return false;
	}

	if (!player->isAccessPlayer() && !player->hasFlag(PlayerFlag_IgnoreYellCheck)) {
		uint32_t minimumLevel = getNumber(ConfigManager::YELL_MINIMUM_LEVEL);
		if (player->getLevel() < minimumLevel) {
			if (getBoolean(ConfigManager::YELL_ALLOW_PREMIUM)) {
				if (!player->isPremium()) {
					player->sendTextMessage(
					    MESSAGE_STATUS_SMALL,
					    fmt::format("You may not yell unless you have reached level {:d} or have a premium account.",
					                minimumLevel));
					return false;
				}
			} else {
				player->sendTextMessage(
				    MESSAGE_STATUS_SMALL,
				    fmt::format("You may not yell unless you have reached level {:d}.", minimumLevel));
				return false;
			}
		}

		Condition* condition = Condition::createCondition(CONDITIONID_DEFAULT, CONDITION_YELLTICKS, 30000, 0);
		player->addCondition(condition);
	}

	internalCreatureSay(player, TALKTYPE_YELL, boost::algorithm::to_upper_copy(text), false);
	return true;
}

bool Game::playerSpeakTo(const std::shared_ptr<Player>& player, SpeakClasses type, const std::string& receiver,
                         const std::string& text)
{
	const auto& toPlayer = getPlayerByName(receiver);
	if (!toPlayer) {
		player->sendTextMessage(MESSAGE_STATUS_SMALL, "A player with this name is not online.");
		return false;
	}

	if (type == TALKTYPE_PRIVATE_RED_TO &&
	    (player->hasFlag(PlayerFlag_CanTalkRedPrivate) || player->getAccountType() >= ACCOUNT_TYPE_GAMEMASTER)) {
		type = TALKTYPE_PRIVATE_RED_FROM;
	} else {
		type = TALKTYPE_PRIVATE_FROM;
	}

	if (!player->isAccessPlayer() && !player->hasFlag(PlayerFlag_IgnoreSendPrivateCheck)) {
		uint32_t minimumLevel = getNumber(ConfigManager::MINIMUM_LEVEL_TO_SEND_PRIVATE);
		if (player->getLevel() < minimumLevel) {
			if (getBoolean(ConfigManager::PREMIUM_TO_SEND_PRIVATE)) {
				if (!player->isPremium()) {
					player->sendTextMessage(
					    MESSAGE_STATUS_SMALL,
					    fmt::format(
					        "You may not send private messages unless you have reached level {:d} or have a premium account.",
					        minimumLevel));
					return false;
				}
			} else {
				player->sendTextMessage(
				    MESSAGE_STATUS_SMALL,
				    fmt::format("You may not send private messages unless you have reached level {:d}.", minimumLevel));
				return false;
			}
		}
	}

	toPlayer->sendPrivateMessage(player, type, text);
	toPlayer->onCreatureSay(player, type, text);

	if (toPlayer->isInGhostMode() && !player->canSeeGhostMode(toPlayer)) {
		player->sendTextMessage(MESSAGE_STATUS_SMALL, "A player with this name is not online.");
	} else {
		player->sendTextMessage(MESSAGE_STATUS_SMALL, fmt::format("Message sent to {:s}.", toPlayer->getName()));
	}
	return true;
}

void Game::playerSpeakToNpc(const std::shared_ptr<Player>& player, const std::string& text)
{
	SpectatorVec spectators;
	map.getSpectators(spectators, player->getPosition());
	for (const auto& spectator : spectators) {
		if (spectator->getNpc()) {
			spectator->onCreatureSay(player, TALKTYPE_PRIVATE_PN, text);
		}
	}
}

//--
bool Game::canThrowObjectTo(const Position& fromPos, const Position& toPos, bool checkLineOfSight /*= true*/,
                            bool sameFloor /*= false*/, int32_t rangex /*= Map::maxClientViewportX*/,
                            int32_t rangey /*= Map::maxClientViewportY*/) const
{
	return map.canThrowObjectTo(fromPos, toPos, checkLineOfSight, sameFloor, rangex, rangey);
}

bool Game::isSightClear(const Position& fromPos, const Position& toPos, bool sameFloor /*= false*/) const
{
	return map.isSightClear(fromPos, toPos, sameFloor);
}

bool Game::internalCreatureTurn(const std::shared_ptr<Creature>& creature, Direction dir)
{
	if (creature->getDirection() == dir) {
		return false;
	}

	creature->setDirection(dir);

	// send to client
	SpectatorVec spectators;
	map.getSpectators(spectators, creature->getPosition(), true, true);
	for (const auto& spectator : spectators) {
		assert(spectator->getPlayer() != nullptr);
		std::static_pointer_cast<Player>(spectator)->sendCreatureTurn(creature);
	}
	return true;
}

bool Game::internalCreatureSay(const std::shared_ptr<Creature>& creature, SpeakClasses type, const std::string& text,
                               bool ghostMode, SpectatorVec* spectatorsPtr /* = nullptr*/,
                               const Position* pos /* = nullptr*/, bool echo /* = false*/)
{
	if (text.empty()) {
		return false;
	}

	if (!pos) {
		pos = &creature->getPosition();
	}

	SpectatorVec spectators;

	if (!spectatorsPtr || spectatorsPtr->empty()) {
		// This somewhat complex construct ensures that the cached SpectatorVec
		// is used if available and if it can be used, else a local vector is
		// used (hopefully the compiler will optimize away the construction of
		// the temporary when it's not used).
		if (type != TALKTYPE_YELL && type != TALKTYPE_MONSTER_YELL) {
			map.getSpectators(spectators, *pos, false, false, Map::maxClientViewportX, Map::maxClientViewportX,
			                  Map::maxClientViewportY, Map::maxClientViewportY);
		} else {
			map.getSpectators(spectators, *pos, true, false, (Map::maxClientViewportX * 2) + 2,
			                  (Map::maxClientViewportX * 2) + 2, (Map::maxClientViewportY * 2) + 2,
			                  (Map::maxClientViewportY * 2) + 2);
		}
	} else {
		spectators = (*spectatorsPtr);
	}

	// send to client
	for (const auto& spectator : spectators) {
		if (const auto& tmpPlayer = spectator->getPlayer()) {
			if (!ghostMode || tmpPlayer->canSeeCreature(creature)) {
				tmpPlayer->sendCreatureSay(creature, type, text, pos);
			}
		}
	}

	// event method
	if (!echo) {
		for (const auto& spectator : spectators) {
			spectator->onCreatureSay(creature, type, text);
			if (creature != spectator) {
				tfs::events::creature::onHear(spectator, creature, text, type);
			}
		}
	}
	return true;
}

void Game::checkCreatureWalk(uint32_t creatureId)
{
	if (const auto& creature = getCreatureByID(creatureId)) {
		if (!creature->isDead()) {
			creature->onWalk();
		}
	}
}

void Game::updateCreatureWalk(uint32_t creatureId)
{
	if (const auto& creature = getCreatureByID(creatureId)) {
		if (!creature->isDead()) {
			creature->goToFollowCreature();
		}
	}
}

void Game::checkCreatureAttack(uint32_t creatureId)
{
	if (const auto& creature = getCreatureByID(creatureId)) {
		if (!creature->isDead()) {
			creature->onAttacking(0);
		}
	}
}

void Game::addCreatureCheck(const std::shared_ptr<Creature>& creature)
{
	creature->creatureCheck = true;

	if (creature->inCheckCreaturesVector) {
		// already in a vector
		return;
	}

	creature->inCheckCreaturesVector = true;
	checkCreatureLists[uniform_random(0, EVENT_CREATURECOUNT - 1)].push_back(creature);
}

void Game::removeCreatureCheck(const std::shared_ptr<Creature>& creature)
{
	if (creature->inCheckCreaturesVector) {
		creature->creatureCheck = false;
	}
}

void Game::checkCreatures(size_t index)
{
	g_scheduler.addEvent(createSchedulerTask(EVENT_CHECK_CREATURE_INTERVAL,
	                                         [=, this]() { checkCreatures((index + 1) % EVENT_CREATURECOUNT); }));

	auto& checkCreatureList = checkCreatureLists[index];
	auto it = checkCreatureList.begin(), end = checkCreatureList.end();
	while (it != end) {
		const auto& creature = *it;
		if (creature->creatureCheck) {
			if (!creature->isDead()) {
				creature->onThink(EVENT_CREATURE_THINK_INTERVAL);
				creature->onAttacking(EVENT_CREATURE_THINK_INTERVAL);
				creature->executeConditions(EVENT_CREATURE_THINK_INTERVAL);
			}
			++it;
		} else {
			creature->inCheckCreaturesVector = false;
			it = checkCreatureList.erase(it);
		}
	}

	cleanup();
}

void Game::updateCreaturesPath(size_t index)
{
	g_scheduler.addEvent(createSchedulerTask(getNumber(ConfigManager::PATHFINDING_INTERVAL),
	                                         [=, this]() { updateCreaturesPath((index + 1) % EVENT_CREATURECOUNT); }));

	for (const auto& creature : checkCreatureLists[index]) {
		if (!creature->isDead()) {
			creature->forceUpdatePath();
		}
	}
}

void Game::changeSpeed(const std::shared_ptr<Creature>& creature, int32_t varSpeedDelta)
{
	int32_t varSpeed = creature->getSpeed() - creature->getBaseSpeed();
	varSpeed += varSpeedDelta;

	creature->setSpeed(varSpeed);

	// send to clients
	SpectatorVec spectators;
	map.getSpectators(spectators, creature->getPosition(), false, true);
	for (const auto& spectator : spectators) {
		assert(spectator->getPlayer() != nullptr);
		std::static_pointer_cast<Player>(spectator)->sendChangeSpeed(creature, creature->getStepSpeed());
	}
}

void Game::internalCreatureChangeOutfit(const std::shared_ptr<Creature>& creature, const Outfit_t& outfit)
{
	if (!tfs::events::creature::onChangeOutfit(creature, outfit)) {
		return;
	}

	creature->setCurrentOutfit(outfit);

	if (creature->isInvisible()) {
		return;
	}

	// send to clients
	SpectatorVec spectators;
	map.getSpectators(spectators, creature->getPosition(), true, true);
	for (const auto& spectator : spectators) {
		assert(spectator->getPlayer() != nullptr);
		std::static_pointer_cast<Player>(spectator)->sendCreatureChangeOutfit(creature, outfit);
	}
}

void Game::internalCreatureChangeVisible(const std::shared_ptr<Creature>& creature, bool visible)
{
	// send to clients
	SpectatorVec spectators;
	map.getSpectators(spectators, creature->getPosition(), true, true);
	for (const auto& spectator : spectators) {
		assert(spectator->getPlayer() != nullptr);
		std::static_pointer_cast<Player>(spectator)->sendCreatureChangeVisible(creature, visible);
	}
}

void Game::changeLight(const std::shared_ptr<const Creature>& creature)
{
	// send to clients
	SpectatorVec spectators;
	map.getSpectators(spectators, creature->getPosition(), true, true);
	for (const auto& spectator : spectators) {
		assert(spectator->getPlayer() != nullptr);
		std::static_pointer_cast<Player>(spectator)->sendCreatureLight(creature);
	}
}

bool Game::combatBlockHit(CombatDamage& damage, const std::shared_ptr<Creature>& attacker,
                          const std::shared_ptr<Creature>& target, bool checkDefense, bool checkArmor, bool field,
                          bool ignoreResistances /*= false */)
{
	if (damage.primary.type == COMBAT_NONE && damage.secondary.type == COMBAT_NONE) {
		return true;
	}

	if (target->getPlayer() && target->isInGhostMode()) {
		return true;
	}

	static const auto sendBlockEffect = [this](BlockType_t blockType, CombatType_t combatType,
	                                           const Position& targetPos) {
		if (blockType == BLOCK_DEFENSE) {
			addMagicEffect(targetPos, CONST_ME_POFF);
		} else if (blockType == BLOCK_ARMOR) {
			addMagicEffect(targetPos, CONST_ME_BLOCKHIT);
		} else if (blockType == BLOCK_IMMUNITY) {
			uint8_t hitEffect = 0;
			switch (combatType) {
				case COMBAT_UNDEFINEDDAMAGE: {
					return;
				}
				case COMBAT_ENERGYDAMAGE:
				case COMBAT_FIREDAMAGE:
				case COMBAT_PHYSICALDAMAGE:
				case COMBAT_ICEDAMAGE:
				case COMBAT_DEATHDAMAGE: {
					hitEffect = CONST_ME_BLOCKHIT;
					break;
				}
				case COMBAT_EARTHDAMAGE: {
					hitEffect = CONST_ME_GREEN_RINGS;
					break;
				}
				case COMBAT_HOLYDAMAGE: {
					hitEffect = CONST_ME_HOLYDAMAGE;
					break;
				}
				default: {
					hitEffect = CONST_ME_POFF;
					break;
				}
			}
			addMagicEffect(targetPos, hitEffect);
		}
	};

	BlockType_t primaryBlockType, secondaryBlockType;
	if (damage.primary.type != COMBAT_NONE) {
		damage.primary.value = std::abs(damage.primary.value);
		primaryBlockType = target->blockHit(attacker, damage.primary.type, damage.primary.value, checkDefense,
		                                    checkArmor, field, ignoreResistances);

		if (damage.primary.type != COMBAT_HEALING) {
			damage.primary.value = -damage.primary.value;
		}

		sendBlockEffect(primaryBlockType, damage.primary.type, target->getPosition());
	} else {
		primaryBlockType = BLOCK_NONE;
	}

	if (damage.secondary.type != COMBAT_NONE) {
		damage.secondary.value = std::abs(damage.secondary.value);
		secondaryBlockType = target->blockHit(attacker, damage.secondary.type, damage.secondary.value, false, false,
		                                      field, ignoreResistances);

		if (damage.secondary.type != COMBAT_HEALING) {
			damage.secondary.value = -damage.secondary.value;
		}

		sendBlockEffect(secondaryBlockType, damage.secondary.type, target->getPosition());
	} else {
		secondaryBlockType = BLOCK_NONE;
	}

	damage.blockType = primaryBlockType;

	return (primaryBlockType != BLOCK_NONE) && (secondaryBlockType != BLOCK_NONE);
}

void Game::combatGetTypeInfo(CombatType_t combatType, const std::shared_ptr<Creature>& target, TextColor_t& color,
                             uint8_t& effect)
{
	switch (combatType) {
		case COMBAT_PHYSICALDAMAGE: {
			std::shared_ptr<Item> splash = nullptr;
			switch (target->getRace()) {
				case RACE_VENOM:
					color = TEXTCOLOR_LIGHTGREEN;
					effect = CONST_ME_HITBYPOISON;
					splash = Item::CreateItem(ITEM_SMALLSPLASH, FLUID_SLIME);
					break;
				case RACE_BLOOD:
					color = TEXTCOLOR_RED;
					effect = CONST_ME_DRAWBLOOD;
					if (const auto& tile = target->getTile()) {
						if (!tile->hasFlag(TILESTATE_PROTECTIONZONE)) {
							splash = Item::CreateItem(ITEM_SMALLSPLASH, FLUID_BLOOD);
						}
					}
					break;
				case RACE_UNDEAD:
					color = TEXTCOLOR_LIGHTGREY;
					effect = CONST_ME_HITAREA;
					break;
				case RACE_FIRE:
					color = TEXTCOLOR_ORANGE;
					effect = CONST_ME_DRAWBLOOD;
					break;
				case RACE_ENERGY:
					color = TEXTCOLOR_ELECTRICPURPLE;
					effect = CONST_ME_ENERGYHIT;
					break;
				case RACE_INK:
					color = TEXTCOLOR_DARKGREY;
					effect = CONST_ME_DRAWINK;
					if (const auto& tile = target->getTile()) {
						if (tile && !tile->hasFlag(TILESTATE_PROTECTIONZONE)) {
							splash = Item::CreateItem(ITEM_SMALLSPLASH, FLUID_INK);
						}
					}
					break;
				default:
					color = TEXTCOLOR_NONE;
					effect = CONST_ME_NONE;
					break;
			}

			if (splash) {
				internalAddItem(target->getTile(), splash, INDEX_WHEREEVER, FLAG_NOLIMIT);
				startDecay(splash);
			}

			break;
		}

		case COMBAT_ENERGYDAMAGE: {
			color = TEXTCOLOR_ELECTRICPURPLE;
			effect = CONST_ME_ENERGYHIT;
			break;
		}

		case COMBAT_EARTHDAMAGE: {
			color = TEXTCOLOR_LIGHTGREEN;
			effect = CONST_ME_GREEN_RINGS;
			break;
		}

		case COMBAT_DROWNDAMAGE: {
			color = TEXTCOLOR_LIGHTBLUE;
			effect = CONST_ME_LOSEENERGY;
			break;
		}
		case COMBAT_FIREDAMAGE: {
			color = TEXTCOLOR_ORANGE;
			effect = CONST_ME_HITBYFIRE;
			break;
		}
		case COMBAT_ICEDAMAGE: {
			color = TEXTCOLOR_SKYBLUE;
			effect = CONST_ME_ICEATTACK;
			break;
		}
		case COMBAT_HOLYDAMAGE: {
			color = TEXTCOLOR_YELLOW;
			effect = CONST_ME_HOLYDAMAGE;
			break;
		}
		case COMBAT_DEATHDAMAGE: {
			color = TEXTCOLOR_DARKRED;
			effect = CONST_ME_SMALLCLOUDS;
			break;
		}
		case COMBAT_LIFEDRAIN: {
			color = TEXTCOLOR_RED;
			effect = CONST_ME_MAGIC_RED;
			break;
		}
		default: {
			color = TEXTCOLOR_NONE;
			effect = CONST_ME_NONE;
			break;
		}
	}
}

bool Game::combatChangeHealth(const std::shared_ptr<Creature>& attacker, const std::shared_ptr<Creature>& target,
                              CombatDamage& damage)
{
	const Position& targetPos = target->getPosition();
	if (damage.primary.value > 0) {
		if (target->isDead()) {
			return false;
		}

		const auto& attackerPlayer = attacker ? attacker->getPlayer() : nullptr;
		const auto& targetPlayer = target->getPlayer();
		if (attackerPlayer && targetPlayer && attackerPlayer->getSkull() == SKULL_BLACK &&
		    attackerPlayer->getSkullClient(targetPlayer) == SKULL_NONE) {
			return false;
		}

		if (damage.origin != ORIGIN_NONE) {
			const auto& events = target->getCreatureEvents(CREATURE_EVENT_HEALTHCHANGE);
			if (!events.empty()) {
				for (CreatureEvent* creatureEvent : events) {
					creatureEvent->executeHealthChange(target, attacker, damage);
				}
				damage.origin = ORIGIN_NONE;
				return combatChangeHealth(attacker, target, damage);
			}
		}

		int32_t realHealthChange = target->getHealth();
		target->gainHealth(attacker, damage.primary.value);
		realHealthChange = target->getHealth() - realHealthChange;

		if (attackerPlayer && attackerPlayer != targetPlayer) {
			attackerPlayer->sendCombatAnalyzer(damage.primary.type, damage.primary.value,
			                                   DamageAnalyzerImpactType::DEALT, target ? target->getName() : "(other)");
			if (damage.secondary.type == COMBAT_HEALING) {
				attackerPlayer->sendCombatAnalyzer(damage.secondary.type, damage.secondary.value,
				                                   DamageAnalyzerImpactType::DEALT,
				                                   target ? target->getName() : "(other)");
			}
		} else if (targetPlayer) {
			targetPlayer->sendCombatAnalyzer(damage.primary.type, damage.primary.value,
			                                 DamageAnalyzerImpactType::HEALING,
			                                 attacker ? attacker->getName() : "(other)");
			if (damage.secondary.type == COMBAT_HEALING) {
				targetPlayer->sendCombatAnalyzer(damage.secondary.type, damage.secondary.value,
				                                 DamageAnalyzerImpactType::HEALING,
				                                 attacker ? attacker->getName() : "(other)");
			}
		}

		if (realHealthChange > 0 && !target->isInGhostMode()) {
			auto damageString = fmt::format("{:d} hitpoint{:s}", realHealthChange, realHealthChange != 1 ? "s" : "");

			std::string spectatorMessage;

			TextMessage message;
			message.position = targetPos;
			message.primary.value = realHealthChange;
			message.primary.color = TEXTCOLOR_PASTELRED;

			SpectatorVec spectators;
			map.getSpectators(spectators, targetPos, false, true);
			for (const auto& spectator : spectators) {
				assert(spectator->getPlayer() != nullptr);

				const auto& spectatorPlayer = std::static_pointer_cast<Player>(spectator);
				if (spectatorPlayer == attackerPlayer && attackerPlayer != targetPlayer) {
					message.type = MESSAGE_HEALED;
					message.text = fmt::format("You heal {:s} for {:s}.", target->getNameDescription(), damageString);
				} else if (spectatorPlayer == targetPlayer) {
					message.type = MESSAGE_HEALED;
					if (!attacker) {
						message.text = fmt::format("You were healed for {:s}.", damageString);
					} else if (targetPlayer == attackerPlayer) {
						message.text = fmt::format("You healed yourself for {:s}.", damageString);
					} else {
						message.text = fmt::format("You were healed by {:s} for {:s}.", attacker->getNameDescription(),
						                           damageString);
					}
				} else {
					message.type = MESSAGE_HEALED_OTHERS;
					if (spectatorMessage.empty()) {
						if (!attacker) {
							spectatorMessage =
							    fmt::format("{:s} was healed for {:s}.", target->getNameDescription(), damageString);
						} else if (attacker == target) {
							spectatorMessage = fmt::format(
							    "{:s} healed {:s}self for {:s}.", attacker->getNameDescription(),
							    targetPlayer ? (targetPlayer->getSex() == PLAYERSEX_FEMALE ? "her" : "him") : "it",
							    damageString);
						} else {
							spectatorMessage = fmt::format("{:s} healed {:s} for {:s}.", attacker->getNameDescription(),
							                               target->getNameDescription(), damageString);
						}
						spectatorMessage[0] = std::toupper(spectatorMessage[0]);
					}
					message.text = spectatorMessage;
				}
				spectatorPlayer->sendTextMessage(message);
			}
		}
	} else {
		if (!target->isAttackable()) {
			if (!target->isInGhostMode()) {
				addMagicEffect(targetPos, CONST_ME_POFF);
			}
			return true;
		}

		const auto& attackerPlayer = attacker ? attacker->getPlayer() : nullptr;
		const auto& targetPlayer = target->getPlayer();
		if (attackerPlayer && targetPlayer && attackerPlayer->getSkull() == SKULL_BLACK &&
		    attackerPlayer->getSkullClient(targetPlayer) == SKULL_NONE) {
			return false;
		}

		damage.primary.value = std::abs(damage.primary.value);
		damage.secondary.value = std::abs(damage.secondary.value);

		int32_t healthChange = damage.primary.value + damage.secondary.value;
		if (healthChange == 0) {
			return true;
		}

		TextMessage message;
		message.position = targetPos;

		SpectatorVec spectators;
		if (targetPlayer &&
		    (target->hasCondition(CONDITION_MANASHIELD) || target->hasCondition(CONDITION_MANASHIELD_BREAKABLE)) &&
		    damage.primary.type != COMBAT_UNDEFINEDDAMAGE) {
			int32_t manaDamage = std::min<int32_t>(targetPlayer->getMana(), healthChange);
			if (damage.origin != ORIGIN_NONE) {
				const auto& events = target->getCreatureEvents(CREATURE_EVENT_MANACHANGE);
				if (!events.empty()) {
					for (CreatureEvent* creatureEvent : events) {
						creatureEvent->executeManaChange(target, attacker, damage);
					}
					healthChange = damage.primary.value + damage.secondary.value;
					if (healthChange == 0) {
						return true;
					}
					manaDamage = std::min<int32_t>(targetPlayer->getMana(), healthChange);
				}
			}

			if (getBoolean(ConfigManager::MANASHIELD_BREAKABLE) && targetPlayer) {
				if (ConditionManaShield* conditionManaShield = dynamic_cast<ConditionManaShield*>(
				        targetPlayer->getCondition(CONDITION_MANASHIELD_BREAKABLE))) {
					if (int32_t remainingManaDamage =
					        conditionManaShield->onDamageTaken(targetPlayer, manaDamage) != 0) {
						manaDamage -= remainingManaDamage;
						targetPlayer->removeCondition(CONDITION_MANASHIELD_BREAKABLE);
					}
				}
			}

			if (manaDamage != 0) {
				targetPlayer->drainMana(attacker, manaDamage);
				if (targetPlayer->getMana() == 0) {
					targetPlayer->removeCondition(CONDITION_MANASHIELD_BREAKABLE);
				}

				map.getSpectators(spectators, targetPos, true, true);
				addMagicEffect(spectators, targetPos, CONST_ME_LOSEENERGY);

				std::string spectatorMessage;

				message.primary.value = manaDamage;
				message.primary.color = TEXTCOLOR_BLUE;

				for (const auto& spectator : spectators) {
					assert(spectator->getPlayer() != nullptr);

					const auto& spectatorPlayer = std::static_pointer_cast<Player>(spectator);
					if (spectatorPlayer->getPosition().z != targetPos.z) {
						continue;
					}

					if (spectatorPlayer == attackerPlayer && attackerPlayer != targetPlayer) {
						message.type = MESSAGE_DAMAGE_DEALT;
						message.text = fmt::format("{:s} loses {:d} mana due to your attack.",
						                           target->getNameDescription(), manaDamage);
						message.text[0] = std::toupper(message.text[0]);
					} else if (spectatorPlayer == targetPlayer) {
						message.type = MESSAGE_DAMAGE_RECEIVED;
						if (!attacker) {
							message.text = fmt::format("You lose {:d} mana.", manaDamage);
						} else if (targetPlayer == attackerPlayer) {
							message.text = fmt::format("You lose {:d} mana due to your own attack.", manaDamage);
						} else {
							message.text = fmt::format("You lose {:d} mana due to an attack by {:s}.", manaDamage,
							                           attacker->getNameDescription());
						}
					} else {
						message.type = MESSAGE_DAMAGE_OTHERS;
						if (spectatorMessage.empty()) {
							if (!attacker) {
								spectatorMessage =
								    fmt::format("{:s} loses {:d} mana.", target->getNameDescription(), manaDamage);
							} else if (attacker == target) {
								spectatorMessage = fmt::format(
								    "{:s} loses {:d} mana due to {:s} own attack.", target->getNameDescription(),
								    manaDamage, targetPlayer->getSex() == PLAYERSEX_FEMALE ? "her" : "his");
							} else {
								spectatorMessage = fmt::format("{:s} loses {:d} mana due to an attack by {:s}.",
								                               target->getNameDescription(), manaDamage,
								                               attacker->getNameDescription());
							}
							spectatorMessage[0] = std::toupper(spectatorMessage[0]);
						}
						message.text = spectatorMessage;
					}
					spectatorPlayer->sendTextMessage(message);
				}

				damage.primary.value -= manaDamage;
				if (damage.primary.value < 0) {
					damage.secondary.value = std::max<int32_t>(0, damage.secondary.value + damage.primary.value);
					damage.primary.value = 0;
				}

				if (attackerPlayer) {
					attackerPlayer->sendCombatAnalyzer(damage.primary.type, damage.primary.value,
					                                   DamageAnalyzerImpactType::DEALT, target->getName());
					if (damage.secondary.type != COMBAT_NONE) {
						attackerPlayer->sendCombatAnalyzer(damage.secondary.type, damage.secondary.value,
						                                   DamageAnalyzerImpactType::DEALT, target->getName());
					}
				}

				if (targetPlayer) {
					targetPlayer->sendCombatAnalyzer(damage.primary.type, manaDamage,
					                                 DamageAnalyzerImpactType::RECEIVED,
					                                 attacker ? attacker->getName() : "(other)");
					if (damage.secondary.type != COMBAT_NONE) {
						targetPlayer->sendCombatAnalyzer(damage.secondary.type, damage.secondary.value,
						                                 DamageAnalyzerImpactType::RECEIVED,
						                                 attacker ? attacker->getName() : "(other)");
					}
				}
			}
		}

		int32_t realDamage = damage.primary.value + damage.secondary.value;
		if (realDamage == 0) {
			return true;
		}

		if (damage.origin != ORIGIN_NONE) {
			const auto& events = target->getCreatureEvents(CREATURE_EVENT_HEALTHCHANGE);
			if (!events.empty()) {
				for (CreatureEvent* creatureEvent : events) {
					creatureEvent->executeHealthChange(target, attacker, damage);
				}
				damage.origin = ORIGIN_NONE;
				return combatChangeHealth(attacker, target, damage);
			}
		}

		int32_t targetHealth = target->getHealth();
		if (damage.primary.value >= targetHealth) {
			damage.primary.value = targetHealth;
			damage.secondary.value = 0;
		} else if (damage.secondary.value) {
			damage.secondary.value = std::min<int32_t>(damage.secondary.value, targetHealth - damage.primary.value);
		}

		realDamage = damage.primary.value + damage.secondary.value;
		if (realDamage == 0) {
			return true;
		}

		if (spectators.empty()) {
			map.getSpectators(spectators, targetPos, true, true);
		}

		message.primary.value = damage.primary.value;
		message.secondary.value = damage.secondary.value;

		uint8_t hitEffect;
		if (message.primary.value) {
			combatGetTypeInfo(damage.primary.type, target, message.primary.color, hitEffect);
			if (hitEffect != CONST_ME_NONE) {
				addMagicEffect(spectators, targetPos, hitEffect);
			}
		}

		if (message.secondary.value) {
			combatGetTypeInfo(damage.secondary.type, target, message.secondary.color, hitEffect);
			if (hitEffect != CONST_ME_NONE) {
				addMagicEffect(spectators, targetPos, hitEffect);
			}
		}

		if (attackerPlayer) {
			attackerPlayer->sendCombatAnalyzer(damage.primary.type, damage.primary.value,
			                                   DamageAnalyzerImpactType::DEALT, target->getName());
			if (damage.secondary.type != COMBAT_NONE) {
				attackerPlayer->sendCombatAnalyzer(damage.secondary.type, damage.secondary.value,
				                                   DamageAnalyzerImpactType::DEALT, target->getName());
			}
		}

		if (targetPlayer) {
			targetPlayer->sendCombatAnalyzer(damage.primary.type, damage.primary.value,
			                                 DamageAnalyzerImpactType::RECEIVED,
			                                 attacker ? attacker->getName() : "(other)");
			if (damage.secondary.type != COMBAT_NONE) {
				targetPlayer->sendCombatAnalyzer(damage.secondary.type, damage.secondary.value,
				                                 DamageAnalyzerImpactType::RECEIVED,
				                                 attacker ? attacker->getName() : "(other)");
			}
		}

		if (message.primary.color != TEXTCOLOR_NONE || message.secondary.color != TEXTCOLOR_NONE) {
			auto damageString = fmt::format("{:d} hitpoint{:s}", realDamage, realDamage != 1 ? "s" : "");

			std::string spectatorMessage;

			for (const auto& spectator : spectators) {
				assert(spectator->getPlayer() != nullptr);

				const auto& spectatorPlayer = std::static_pointer_cast<Player>(spectator);
				if (spectatorPlayer->getPosition().z != targetPos.z) {
					continue;
				}

				if (spectatorPlayer == attackerPlayer && attackerPlayer != targetPlayer) {
					message.type = MESSAGE_DAMAGE_DEALT;
					message.text =
					    fmt::format("{:s} loses {:s} due to your attack.", target->getNameDescription(), damageString);
					message.text[0] = std::toupper(message.text[0]);
				} else if (spectatorPlayer == targetPlayer) {
					message.type = MESSAGE_DAMAGE_RECEIVED;
					if (!attacker) {
						message.text = fmt::format("You lose {:s}.", damageString);
					} else if (targetPlayer == attackerPlayer) {
						message.text = fmt::format("You lose {:s} due to your own attack.", damageString);
					} else {
						message.text = fmt::format("You lose {:s} due to an attack by {:s}.", damageString,
						                           attacker->getNameDescription());
					}
				} else {
					message.type = MESSAGE_DAMAGE_OTHERS;
					if (spectatorMessage.empty()) {
						if (!attacker) {
							spectatorMessage =
							    fmt::format("{:s} loses {:s}.", target->getNameDescription(), damageString);
						} else if (attacker == target) {
							spectatorMessage = fmt::format(
							    "{:s} loses {:s} due to {:s} own attack.", target->getNameDescription(), damageString,
							    targetPlayer ? (targetPlayer->getSex() == PLAYERSEX_FEMALE ? "her" : "his") : "its");
						} else {
							spectatorMessage =
							    fmt::format("{:s} loses {:s} due to an attack by {:s}.", target->getNameDescription(),
							                damageString, attacker->getNameDescription());
						}
						spectatorMessage[0] = std::toupper(spectatorMessage[0]);
					}
					message.text = spectatorMessage;
				}
				spectatorPlayer->sendTextMessage(message);
			}
		}

		if (realDamage >= targetHealth) {
			for (CreatureEvent* creatureEvent : target->getCreatureEvents(CREATURE_EVENT_PREPAREDEATH)) {
				if (!creatureEvent->executeOnPrepareDeath(target, attacker)) {
					return false;
				}
			}
		}

		target->drainHealth(attacker, realDamage);
		addCreatureHealth(spectators, target);
	}

	return true;
}

bool Game::combatChangeMana(const std::shared_ptr<Creature>& attacker, const std::shared_ptr<Creature>& target,
                            CombatDamage& damage)
{
	const auto& targetPlayer = target->getPlayer();
	if (!targetPlayer) {
		return true;
	}

	int32_t manaChange = damage.primary.value + damage.secondary.value;
	if (manaChange > 0) {
		if (attacker) {
			const auto& attackerPlayer = attacker->getPlayer();
			if (attackerPlayer && attackerPlayer->getSkull() == SKULL_BLACK &&
			    attackerPlayer->getSkullClient(target) == SKULL_NONE) {
				return false;
			}
		}

		if (damage.origin != ORIGIN_NONE) {
			const auto& events = target->getCreatureEvents(CREATURE_EVENT_MANACHANGE);
			if (!events.empty()) {
				for (CreatureEvent* creatureEvent : events) {
					creatureEvent->executeManaChange(target, attacker, damage);
				}
				damage.origin = ORIGIN_NONE;
				return combatChangeMana(attacker, target, damage);
			}
		}

		int32_t realManaChange = targetPlayer->getMana();
		targetPlayer->changeMana(manaChange);
		realManaChange = targetPlayer->getMana() - realManaChange;

		if (realManaChange > 0 && !targetPlayer->isInGhostMode()) {
			TextMessage message(MESSAGE_HEALED, "You gained " + std::to_string(realManaChange) + " mana.");
			message.position = target->getPosition();
			message.primary.value = realManaChange;
			message.primary.color = TEXTCOLOR_MAYABLUE;
			targetPlayer->sendTextMessage(message);
		}
	} else {
		const Position& targetPos = target->getPosition();
		if (!target->isAttackable()) {
			if (!target->isInGhostMode()) {
				addMagicEffect(targetPos, CONST_ME_POFF);
			}
			return false;
		}

		const auto& attackerPlayer = attacker ? attacker->getPlayer() : nullptr;
		if (attackerPlayer && attackerPlayer->getSkull() == SKULL_BLACK &&
		    attackerPlayer->getSkullClient(targetPlayer) == SKULL_NONE) {
			return false;
		}

		int32_t manaLoss = std::min<int32_t>(targetPlayer->getMana(), -manaChange);
		BlockType_t blockType = target->blockHit(attacker, COMBAT_MANADRAIN, manaLoss);
		if (blockType != BLOCK_NONE) {
			addMagicEffect(targetPos, CONST_ME_POFF);
			return false;
		}

		if (manaLoss <= 0) {
			return true;
		}

		if (damage.origin != ORIGIN_NONE) {
			const auto& events = target->getCreatureEvents(CREATURE_EVENT_MANACHANGE);
			if (!events.empty()) {
				for (CreatureEvent* creatureEvent : events) {
					creatureEvent->executeManaChange(target, attacker, damage);
				}
				damage.origin = ORIGIN_NONE;
				return combatChangeMana(attacker, target, damage);
			}
		}

		targetPlayer->drainMana(attacker, manaLoss);

		std::string spectatorMessage;

		TextMessage message;
		message.position = targetPos;
		message.primary.value = manaLoss;
		message.primary.color = TEXTCOLOR_BLUE;

		SpectatorVec spectators;
		map.getSpectators(spectators, targetPos, false, true);
		for (const auto& spectator : spectators) {
			assert(spectator->getPlayer() != nullptr);

			const auto& spectatorPlayer = std::static_pointer_cast<Player>(spectator);
			if (spectatorPlayer == attackerPlayer && attackerPlayer != targetPlayer) {
				message.type = MESSAGE_DAMAGE_DEALT;
				message.text =
				    fmt::format("{:s} loses {:d} mana due to your attack.", target->getNameDescription(), manaLoss);
				message.text[0] = std::toupper(message.text[0]);
			} else if (spectatorPlayer == targetPlayer) {
				message.type = MESSAGE_DAMAGE_RECEIVED;
				if (!attacker) {
					message.text = fmt::format("You lose {:d} mana.", manaLoss);
				} else if (targetPlayer == attackerPlayer) {
					message.text = fmt::format("You lose {:d} mana due to your own attack.", manaLoss);
				} else {
					message.text = fmt::format("You lose {:d} mana due to an attack by {:s}.", manaLoss,
					                           attacker->getNameDescription());
				}
			} else {
				message.type = MESSAGE_DAMAGE_OTHERS;
				if (spectatorMessage.empty()) {
					if (!attacker) {
						spectatorMessage = fmt::format("{:s} loses {:d} mana.", target->getNameDescription(), manaLoss);
					} else if (attacker == target) {
						spectatorMessage =
						    fmt::format("{:s} loses {:d} mana due to {:s} own attack.", target->getNameDescription(),
						                manaLoss, targetPlayer->getSex() == PLAYERSEX_FEMALE ? "her" : "his");
					} else {
						spectatorMessage =
						    fmt::format("{:s} loses {:d} mana due to an attack by {:s}.", target->getNameDescription(),
						                manaLoss, attacker->getNameDescription());
					}
					spectatorMessage[0] = std::toupper(spectatorMessage[0]);
				}
				message.text = spectatorMessage;
			}
			spectatorPlayer->sendTextMessage(message);
		}
	}

	return true;
}

void Game::addCreatureHealth(const std::shared_ptr<const Creature>& target)
{
	SpectatorVec spectators;
	map.getSpectators(spectators, target->getPosition(), true, true);
	addCreatureHealth(spectators, target);
}

void Game::addCreatureHealth(const SpectatorVec& spectators, const std::shared_ptr<const Creature>& target)
{
	for (const auto& spectator : spectators) {
		if (const auto& tmpPlayer = spectator->getPlayer()) {
			tmpPlayer->sendCreatureHealth(target);
		}
	}
}

void Game::addMagicEffect(const Position& pos, uint8_t effect)
{
	SpectatorVec spectators;
	map.getSpectators(spectators, pos, true, true);
	addMagicEffect(spectators, pos, effect);
}

void Game::addMagicEffect(const SpectatorVec& spectators, const Position& pos, uint8_t effect)
{
	for (const auto& spectator : spectators) {
		if (const auto& tmpPlayer = spectator->getPlayer()) {
			tmpPlayer->sendMagicEffect(pos, effect);
		}
	}
}

void Game::addDistanceEffect(const Position& fromPos, const Position& toPos, uint8_t effect)
{
	SpectatorVec spectators, toPosSpectators;
	map.getSpectators(spectators, fromPos, true, true);
	map.getSpectators(toPosSpectators, toPos, true, true);
	spectators.addSpectators(toPosSpectators);

	addDistanceEffect(spectators, fromPos, toPos, effect);
}

void Game::addDistanceEffect(const SpectatorVec& spectators, const Position& fromPos, const Position& toPos,
                             uint8_t effect)
{
	for (const auto& spectator : spectators) {
		if (const auto& tmpPlayer = spectator->getPlayer()) {
			tmpPlayer->sendDistanceShoot(fromPos, toPos, effect);
		}
	}
}

void Game::startDecay(const std::shared_ptr<Item>& item)
{
	if (!item || !item->canDecay()) {
		return;
	}

	ItemDecayState_t decayState = item->getDecaying();
	if (decayState == DECAYING_TRUE) {
		return;
	}

	if (item->getDuration() > 0) {
		item->setDecaying(DECAYING_TRUE);
		toDecayItems.push_back(item);
	} else {
		internalDecayItem(item);
	}
}

void Game::internalDecayItem(const std::shared_ptr<Item>& item)
{
	const int32_t decayTo = item->getDecayTo();
	if (decayTo > 0) {
		startDecay(transformItem(item, decayTo));
	} else {
		if (const auto& player = item->getHoldingPlayer()) {
			const ItemType& it = Item::items[item->getID()];
			player->sendSupplyUsed(it.transformDeEquipTo != 0 ? Item::items[it.transformDeEquipTo].clientId
			                                                  : item->getClientID());
		}
		ReturnValue ret = internalRemoveItem(item);
		if (ret != RETURNVALUE_NOERROR) {
			std::cout << "[Debug - Game::internalDecayItem] internalDecayItem failed, error code: "
			          << static_cast<uint32_t>(ret) << ", item id: " << item->getID() << std::endl;
		}
	}
}

void Game::checkDecay()
{
	g_scheduler.addEvent(createSchedulerTask(EVENT_DECAYINTERVAL, [this]() { checkDecay(); }));
	size_t bucket = (lastBucket + 1) % EVENT_DECAY_BUCKETS;

	auto it = decayItems[bucket].begin(), end = decayItems[bucket].end();
	while (it != end) {
		const auto item = *it;
		if (!item->canDecay()) {
			item->setDecaying(DECAYING_FALSE);
			it = decayItems[bucket].erase(it);
			continue;
		}

		int32_t duration = item->getDuration();
		int32_t decreaseTime = std::min<int32_t>(EVENT_DECAYINTERVAL * EVENT_DECAY_BUCKETS, duration);

		duration -= decreaseTime;
		item->decreaseDuration(decreaseTime);

		if (duration <= 0) {
			it = decayItems[bucket].erase(it);
			internalDecayItem(item);
		} else if (duration < EVENT_DECAYINTERVAL * EVENT_DECAY_BUCKETS) {
			it = decayItems[bucket].erase(it);
			size_t newBucket = (bucket + ((duration + EVENT_DECAYINTERVAL / 2) / 1000)) % EVENT_DECAY_BUCKETS;
			if (newBucket == bucket) {
				internalDecayItem(item);
			} else {
				decayItems[newBucket].push_back(item);
			}
		} else {
			++it;
		}
	}

	lastBucket = bucket;
	cleanup();
}

void Game::shutdown()
{
	std::cout << "Shutting down..." << std::flush;

	g_scheduler.shutdown();
	g_databaseTasks.shutdown();
	g_dispatcher.shutdown();
	map.spawns.clear();

	cleanup();

	if (serviceManager) {
		serviceManager->stop();
	}

	ConnectionManager::getInstance().closeAll();

	std::cout << " done!" << std::endl;
}

void Game::cleanup()
{
	for (const auto& item : toDecayItems) {
		const uint32_t dur = item->getDuration();
		if (dur >= EVENT_DECAYINTERVAL * EVENT_DECAY_BUCKETS) {
			decayItems[lastBucket].push_back(item);
		} else {
			decayItems[(lastBucket + 1 + dur / 1000) % EVENT_DECAY_BUCKETS].push_back(item);
		}
	}
	toDecayItems.clear();
}

void Game::broadcastMessage(const std::string& text, MessageClasses type) const
{
	std::cout << "> Broadcasted message: \"" << text << "\"." << std::endl;
	for (const auto& it : players) {
		it.second->sendTextMessage(type, text);
	}
}

void Game::updateCreatureWalkthrough(const std::shared_ptr<const Creature>& creature)
{
	// send to clients
	SpectatorVec spectators;
	map.getSpectators(spectators, creature->getPosition(), true, true);
	for (const auto& spectator : spectators) {
		assert(spectator->getPlayer() != nullptr);

		const auto& spectatorPlayer = std::static_pointer_cast<Player>(spectator);
		spectatorPlayer->sendCreatureWalkthrough(creature, spectatorPlayer->canWalkthroughEx(creature));
	}
}

void Game::updateKnownCreature(const std::shared_ptr<const Creature>& creature)
{
	// send to clients
	SpectatorVec spectators;
	map.getSpectators(spectators, creature->getPosition(), true, true);
	for (const auto& spectator : spectators) {
		assert(spectator->getPlayer() != nullptr);
		std::static_pointer_cast<Player>(spectator)->sendUpdateTileCreature(creature);
	}
}

void Game::updateCreatureSkull(const std::shared_ptr<const Creature>& creature)
{
	if (getWorldType() != WORLD_TYPE_PVP) {
		return;
	}

	SpectatorVec spectators;
	map.getSpectators(spectators, creature->getPosition(), true, true);
	for (const auto& spectator : spectators) {
		assert(spectator->getPlayer() != nullptr);
		std::static_pointer_cast<Player>(spectator)->sendCreatureSkull(creature);
	}
}

void Game::updatePlayerShield(const std::shared_ptr<Player>& player)
{
	SpectatorVec spectators;
	map.getSpectators(spectators, player->getPosition(), true, true);
	for (const auto& spectator : spectators) {
		assert(spectator->getPlayer() != nullptr);
		std::static_pointer_cast<Player>(spectator)->sendCreatureShield(player);
	}
}

void Game::checkPlayersRecord()
{
	const size_t playersOnline = getPlayersOnline();
	if (playersOnline > playersRecord) {
		uint32_t previousRecord = playersRecord;
		playersRecord = playersOnline;

		for (auto& it : g_globalEvents->getEventMap(GLOBALEVENT_RECORD)) {
			it.second.executeRecord(playersRecord, previousRecord);
		}
		updatePlayersRecord();
	}
}

void Game::updatePlayersRecord() const
{
	Database& db = Database::getInstance();
	db.executeQuery(
	    fmt::format("UPDATE `server_config` SET `value` = '{:d}' WHERE `config` = 'players_record'", playersRecord));
}

void Game::loadPlayersRecord()
{
	Database& db = Database::getInstance();

	DBResult_ptr result = db.storeQuery("SELECT `value` FROM `server_config` WHERE `config` = 'players_record'");
	if (result) {
		playersRecord = result->getNumber<uint32_t>("value");
	} else {
		db.executeQuery("INSERT INTO `server_config` (`config`, `value`) VALUES ('players_record', '0')");
	}
}

void Game::playerInviteToParty(uint32_t playerId, uint32_t invitedId)
{
	if (playerId == invitedId) {
		return;
	}

	const auto& player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	const auto& invitedPlayer = getPlayerByID(invitedId);
	if (!invitedPlayer || invitedPlayer->isInviting(player)) {
		return;
	}

	if (invitedPlayer->getParty()) {
		player->sendTextMessage(MESSAGE_INFO_DESCR,
		                        fmt::format("{:s} is already in a party.", invitedPlayer->getName()));
		return;
	}

	Party* party = player->getParty();
	if (!party) {
		party = new Party(player);
	} else if (party->getLeader() != player) {
		return;
	}

	if (!tfs::events::party::onInvite(party, invitedPlayer)) {
		if (party->empty()) {
			player->setParty(nullptr);
			delete party;
		}
		return;
	}

	party->invitePlayer(invitedPlayer);
}

void Game::playerJoinParty(uint32_t playerId, uint32_t leaderId)
{
	const auto& player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	const auto& leader = getPlayerByID(leaderId);
	if (!leader || !leader->isInviting(player)) {
		return;
	}

	Party* party = leader->getParty();
	if (!party || party->getLeader() != leader) {
		return;
	}

	if (player->getParty()) {
		player->sendTextMessage(MESSAGE_INFO_DESCR, "You are already in a party.");
		return;
	}

	party->joinParty(player);
}

void Game::playerRevokePartyInvitation(uint32_t playerId, uint32_t invitedId)
{
	const auto& player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	Party* party = player->getParty();
	if (!party || party->getLeader() != player) {
		return;
	}

	if (const auto& invitedPlayer = getPlayerByID(invitedId)) {
		if (player->isInviting(invitedPlayer)) {
			party->revokeInvitation(invitedPlayer);
		}
	}
}

void Game::playerPassPartyLeadership(uint32_t playerId, uint32_t newLeaderId)
{
	const auto& player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	Party* party = player->getParty();
	if (!party || party->getLeader() != player) {
		return;
	}

	if (const auto& newLeader = getPlayerByID(newLeaderId)) {
		if (player->isPartner(newLeader)) {
			party->passPartyLeadership(newLeader);
		}
	}
}

void Game::playerLeaveParty(uint32_t playerId)
{
	const auto& player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	Party* party = player->getParty();
	if (!party || player->hasCondition(CONDITION_INFIGHT)) {
		return;
	}

	party->leaveParty(player);
}

void Game::playerEnableSharedPartyExperience(uint32_t playerId, bool sharedExpActive)
{
	const auto& player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	Party* party = player->getParty();
	if (!party || (player->hasCondition(CONDITION_INFIGHT) && player->getZone() != ZONE_PROTECTION)) {
		return;
	}

	party->setSharedExperience(player, sharedExpActive);
}

void Game::sendGuildMotd(uint32_t playerId)
{
	if (const auto& player = getPlayerByID(playerId)) {
		if (const auto& guild = player->getGuild()) {
			player->sendChannelMessage("Message of the Day", guild->getMotd(), TALKTYPE_CHANNEL_R1, CHANNEL_GUILD);
		}
	}
}

void Game::kickPlayer(uint32_t playerId, bool displayEffect)
{
	if (const auto& player = getPlayerByID(playerId)) {
		player->kickPlayer(displayEffect);
	}
}

void Game::playerReportRuleViolation(uint32_t playerId, const std::string& targetName, uint8_t reportType,
                                     uint8_t reportReason, const std::string& comment, const std::string& translation)
{
	if (const auto& player = getPlayerByID(playerId)) {
		tfs::events::player::onReportRuleViolation(player, targetName, reportType, reportReason, comment, translation);
	}
}

void Game::playerDebugAssert(uint32_t playerId, const std::string& assertLine, const std::string& date,
                             const std::string& description, const std::string& comment)
{
	if (const auto& player = getPlayerByID(playerId)) {
		// TODO: move debug assertions to database
		auto fs = std::ofstream{"client_assertions.txt", std::ios::app};
		fs << "----- " << std::format("{:%d/%m/%Y %T}", std::chrono::system_clock::now()) << " - " << player->getName()
		   << " (" << player->getIP().to_string() << ") -----\n"
		   << assertLine << '\n'
		   << date << '\n'
		   << description << '\n'
		   << comment << "\n\n";
	}
}

void Game::playerLeaveMarket(uint32_t playerId)
{
	if (const auto& player = getPlayerByID(playerId)) {
		player->setInMarket(false);
	}
}

void Game::playerBrowseMarket(uint32_t playerId, uint16_t spriteId)
{
	const auto& player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	if (!player->isInMarket()) {
		player->sendMarketEnter();
	}

	const ItemType& it = Item::items.getItemIdByClientId(spriteId);
	if (it.id == 0) {
		return;
	}

	if (it.wareId == 0) {
		return;
	}

	const MarketOfferList& buyOffers = tfs::iomarket::getActiveOffers(MARKETACTION_BUY, it.id);
	const MarketOfferList& sellOffers = tfs::iomarket::getActiveOffers(MARKETACTION_SELL, it.id);
	player->sendMarketBrowseItem(it.id, buyOffers, sellOffers);
	tfs::events::player::onLookInMarket(player, &it);
}

void Game::playerBrowseMarketOwnOffers(uint32_t playerId)
{
	if (const auto& player = getPlayerByID(playerId)) {
		if (player->isInMarket()) {
			const MarketOfferList& buyOffers = tfs::iomarket::getOwnOffers(MARKETACTION_BUY, player->getGUID());
			const MarketOfferList& sellOffers = tfs::iomarket::getOwnOffers(MARKETACTION_SELL, player->getGUID());
			player->sendMarketBrowseOwnOffers(buyOffers, sellOffers);
		}
	}
}

void Game::playerBrowseMarketOwnHistory(uint32_t playerId)
{
	if (const auto& player = getPlayerByID(playerId)) {
		if (player->isInMarket()) {
			const HistoryMarketOfferList& buyOffers = tfs::iomarket::getOwnHistory(MARKETACTION_BUY, player->getGUID());
			const HistoryMarketOfferList& sellOffers =
			    tfs::iomarket::getOwnHistory(MARKETACTION_SELL, player->getGUID());
			player->sendMarketBrowseOwnHistory(buyOffers, sellOffers);
		}
	}
}

void Game::playerCreateMarketOffer(uint32_t playerId, uint8_t type, uint16_t spriteId, uint16_t amount, uint64_t price,
                                   bool anonymous)
{
	if (amount == 0 || amount > 64000) {
		return;
	}

	if (price == 0 || price > 999999999999) {
		return;
	}

	if (type != MARKETACTION_BUY && type != MARKETACTION_SELL) {
		return;
	}

	const auto& player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	if (!player->isInMarket()) {
		return;
	}

	if (getBoolean(ConfigManager::MARKET_PREMIUM) && !player->isPremium()) {
		player->sendTextMessage(MESSAGE_MARKET, "Only premium accounts may create offers for that object.");
		return;
	}

	const ItemType& itt = Item::items.getItemIdByClientId(spriteId);
	if (itt.id == 0 || itt.wareId == 0) {
		return;
	}

	const ItemType& it = Item::items.getItemIdByClientId(itt.wareId);
	if (it.id == 0 || it.wareId == 0) {
		return;
	}

	if (!it.stackable && amount > 2000) {
		return;
	}

	const uint32_t maxOfferCount = getNumber(ConfigManager::MAX_MARKET_OFFERS_AT_A_TIME_PER_PLAYER);
	if (maxOfferCount != 0 && tfs::iomarket::getPlayerOfferCount(player->getGUID()) >= maxOfferCount) {
		return;
	}

	uint64_t fee = (price / 100.) * amount;
	if (fee < MIN_MARKET_FEE) {
		fee = MIN_MARKET_FEE;
	} else if (fee > MAX_MARKET_FEE) {
		fee = MAX_MARKET_FEE;
	}

	uint64_t playerMoney = player->getMoney();
	if (type == MARKETACTION_SELL) {
		if (fee > (playerMoney + player->getBankBalance())) {
			return;
		}

		const auto& itemList = getMarketItemList(it.wareId, amount, *player);
		if (itemList.empty()) {
			return;
		}

		if (it.stackable) {
			uint16_t tmpAmount = amount;
			for (const auto& item : itemList) {
				uint16_t removeCount = std::min<uint16_t>(tmpAmount, item->getItemCount());
				tmpAmount -= removeCount;
				internalRemoveItem(item, removeCount);

				if (tmpAmount == 0) {
					break;
				}
			}
		} else {
			for (const auto& item : itemList) {
				internalRemoveItem(item);
			}
		}

		const auto debitCash = std::min(playerMoney, fee);
		const auto debitBank = fee - debitCash;
		removeMoney(player, debitCash);
		player->setBankBalance(player->getBankBalance() - debitBank);
	} else {
		uint64_t totalPrice = static_cast<uint64_t>(price) * amount;
		totalPrice += fee;
		if (totalPrice > (playerMoney + player->getBankBalance())) {
			return;
		}

		const auto debitCash = std::min(playerMoney, totalPrice);
		const auto debitBank = totalPrice - debitCash;
		removeMoney(player, debitCash);
		player->setBankBalance(player->getBankBalance() - debitBank);
	}

	tfs::iomarket::createOffer(player->getGUID(), static_cast<MarketAction_t>(type), it.id, amount, price, anonymous);

	player->sendMarketEnter();
	const MarketOfferList& buyOffers = tfs::iomarket::getActiveOffers(MARKETACTION_BUY, it.id);
	const MarketOfferList& sellOffers = tfs::iomarket::getActiveOffers(MARKETACTION_SELL, it.id);
	player->sendMarketBrowseItem(it.id, buyOffers, sellOffers);
}

void Game::playerCancelMarketOffer(uint32_t playerId, uint32_t timestamp, uint16_t counter)
{
	const auto& player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	if (!player->isInMarket()) {
		return;
	}

	MarketOfferEx offer = tfs::iomarket::getOfferByCounter(timestamp, counter);
	if (offer.id == 0 || offer.playerId != player->getGUID()) {
		return;
	}

	if (offer.type == MARKETACTION_BUY) {
		player->setBankBalance(player->getBankBalance() + offer.price * offer.amount);
		player->sendMarketEnter();
	} else {
		const ItemType& it = Item::items[offer.itemId];
		if (it.id == 0) {
			return;
		}

		if (it.stackable) {
			uint16_t tmpAmount = offer.amount;
			while (tmpAmount > 0) {
				int32_t stackCount = std::min<int32_t>(ITEM_STACK_SIZE, tmpAmount);
				const auto& item = Item::CreateItem(it.id, stackCount);
				if (internalAddItem(player->getInbox(), item, INDEX_WHEREEVER, FLAG_NOLIMIT) != RETURNVALUE_NOERROR) {
					break;
				}

				tmpAmount -= stackCount;
			}
		} else {
			int32_t subType;
			if (it.charges != 0) {
				subType = it.charges;
			} else {
				subType = -1;
			}

			for (uint16_t i = 0; i < offer.amount; ++i) {
				const auto& item = Item::CreateItem(it.id, subType);
				if (internalAddItem(player->getInbox(), item, INDEX_WHEREEVER, FLAG_NOLIMIT) != RETURNVALUE_NOERROR) {
					break;
				}
			}
		}
	}

	tfs::iomarket::moveOfferToHistory(offer.id, OFFERSTATE_CANCELLED);
	offer.amount = 0;
	offer.timestamp += getNumber(ConfigManager::MARKET_OFFER_DURATION);
	player->sendMarketCancelOffer(offer);
	player->sendMarketEnter();
}

void Game::playerAcceptMarketOffer(uint32_t playerId, uint32_t timestamp, uint16_t counter, uint16_t amount)
{
	if (amount == 0 || amount > 64000) {
		return;
	}

	const auto& player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	if (!player->isInMarket()) {
		return;
	}

	MarketOfferEx offer = tfs::iomarket::getOfferByCounter(timestamp, counter);
	if (offer.id == 0) {
		return;
	}

	uint32_t offerAccountId = IOLoginData::getAccountIdByPlayerId(offer.playerId);
	if (offerAccountId == player->getAccount()) {
		player->sendTextMessage(MESSAGE_MARKET, "You cannot accept your own offer.");
		return;
	}

	if (amount > offer.amount) {
		return;
	}

	const ItemType& it = Item::items[offer.itemId];
	if (it.id == 0) {
		return;
	}

	uint64_t totalPrice = offer.price * amount;

	if (offer.type == MARKETACTION_BUY) {
		const auto& itemList = getMarketItemList(it.wareId, amount, *player);
		if (itemList.empty()) {
			return;
		}

		auto buyerPlayer = getPlayerByGUID(offer.playerId);
		if (!buyerPlayer) {
			buyerPlayer = std::make_shared<Player>(nullptr);
			if (!IOLoginData::loadPlayerById(buyerPlayer, offer.playerId)) {
				return;
			}
		}

		if (it.stackable) {
			uint16_t tmpAmount = amount;
			for (const auto& item : itemList) {
				uint16_t removeCount = std::min<uint16_t>(tmpAmount, item->getItemCount());
				tmpAmount -= removeCount;
				internalRemoveItem(item, removeCount);

				if (tmpAmount == 0) {
					break;
				}
			}
		} else {
			for (const auto& item : itemList) {
				internalRemoveItem(item);
			}
		}

		player->setBankBalance(player->getBankBalance() + totalPrice);

		if (it.stackable) {
			uint16_t tmpAmount = amount;
			while (tmpAmount > 0) {
				uint16_t stackCount = std::min<uint16_t>(ITEM_STACK_SIZE, tmpAmount);
				const auto& item = Item::CreateItem(it.id, stackCount);
				if (internalAddItem(buyerPlayer->getInbox(), item, INDEX_WHEREEVER, FLAG_NOLIMIT) !=
				    RETURNVALUE_NOERROR) {
					break;
				}

				tmpAmount -= stackCount;
			}
		} else {
			int32_t subType;
			if (it.charges != 0) {
				subType = it.charges;
			} else {
				subType = -1;
			}

			for (uint16_t i = 0; i < amount; ++i) {
				const auto& item = Item::CreateItem(it.id, subType);
				if (internalAddItem(buyerPlayer->getInbox(), item, INDEX_WHEREEVER, FLAG_NOLIMIT) !=
				    RETURNVALUE_NOERROR) {
					break;
				}
			}
		}

		if (buyerPlayer->isOffline()) {
			IOLoginData::savePlayer(buyerPlayer);
		} else {
			buyerPlayer->onReceiveMail();
		}
	} else {
		uint64_t playerMoney = player->getMoney();
		if (totalPrice > (playerMoney + player->getBankBalance())) {
			return;
		}

		const auto debitCash = std::min(playerMoney, totalPrice);
		const auto debitBank = totalPrice - debitCash;
		removeMoney(player, debitCash);
		player->setBankBalance(player->getBankBalance() - debitBank);

		if (it.stackable) {
			uint16_t tmpAmount = amount;
			while (tmpAmount > 0) {
				uint16_t stackCount = std::min<uint16_t>(ITEM_STACK_SIZE, tmpAmount);
				const auto& item = Item::CreateItem(it.id, stackCount);
				if (internalAddItem(player->getInbox(), item, INDEX_WHEREEVER, FLAG_NOLIMIT) != RETURNVALUE_NOERROR) {
					break;
				}

				tmpAmount -= stackCount;
			}
		} else {
			int32_t subType;
			if (it.charges != 0) {
				subType = it.charges;
			} else {
				subType = -1;
			}

			for (uint16_t i = 0; i < amount; ++i) {
				const auto& item = Item::CreateItem(it.id, subType);
				if (internalAddItem(player->getInbox(), item, INDEX_WHEREEVER, FLAG_NOLIMIT) != RETURNVALUE_NOERROR) {
					break;
				}
			}
		}

		const auto& sellerPlayer = getPlayerByGUID(offer.playerId);
		if (sellerPlayer) {
			sellerPlayer->setBankBalance(sellerPlayer->getBankBalance() + totalPrice);
		} else {
			IOLoginData::increaseBankBalance(offer.playerId, totalPrice);
		}

		player->onReceiveMail();
	}

	const int32_t marketOfferDuration = getNumber(ConfigManager::MARKET_OFFER_DURATION);

	tfs::iomarket::appendHistory(player->getGUID(),
	                             (offer.type == MARKETACTION_BUY ? MARKETACTION_SELL : MARKETACTION_BUY), offer.itemId,
	                             amount, offer.price, offer.timestamp + marketOfferDuration, OFFERSTATE_ACCEPTEDEX);

	tfs::iomarket::appendHistory(offer.playerId, offer.type, offer.itemId, amount, offer.price,
	                             offer.timestamp + marketOfferDuration, OFFERSTATE_ACCEPTED);

	offer.amount -= amount;

	if (offer.amount == 0) {
		tfs::iomarket::deleteOffer(offer.id);
	} else {
		tfs::iomarket::acceptOffer(offer.id, amount);
	}

	player->sendMarketEnter();
	offer.timestamp += marketOfferDuration;
	player->sendMarketAcceptOffer(offer);
}

void Game::parsePlayerExtendedOpcode(uint32_t playerId, uint8_t opcode, const std::string& buffer)
{
	if (const auto& player = getPlayerByID(playerId)) {
		for (CreatureEvent* creatureEvent : player->getCreatureEvents(CREATURE_EVENT_EXTENDED_OPCODE)) {
			creatureEvent->executeExtendedOpcode(player, opcode, buffer);
		}
	}
}

void Game::parsePlayerNetworkMessage(uint32_t playerId, uint8_t recvByte, NetworkMessage_ptr msg)
{
	if (const auto& player = getPlayerByID(playerId)) {
		tfs::events::player::onNetworkMessage(player, recvByte, msg);
	}
}

std::vector<std::shared_ptr<Item>> Game::getMarketItemList(uint16_t wareId, uint16_t sufficientCount, Player& player)
{
	uint16_t count = 0;
	auto containers = std::deque<std::shared_ptr<Container>>{player.getInbox()};

	for (const auto& chest : player.getDepotChests()) {
		if (!chest.second->empty()) {
			containers.push_front(chest.second);
		}
	}

	std::vector<std::shared_ptr<Item>> itemList;

	do {
		const auto container = containers.front();
		containers.pop_front();

		for (const auto& item : container->getItemList()) {
			const auto& containerItem = item->getContainer();
			if (containerItem && !containerItem->empty()) {
				containers.push_back(containerItem);
				continue;
			}

			const ItemType& itemType = Item::items[item->getID()];
			if (itemType.wareId != wareId) {
				continue;
			}

			if (containerItem && (!itemType.isContainer() || containerItem->capacity() != itemType.maxItems)) {
				continue;
			}

			if (!item->hasMarketAttributes()) {
				continue;
			}

			itemList.push_back(item);

			count += Item::countByType(item, -1);
			if (count >= sufficientCount) {
				return itemList;
			}
		}
	} while (!containers.empty());

	return {};
}

void Game::forceAddCondition(uint32_t creatureId, Condition* condition)
{
	const auto& creature = getCreatureByID(creatureId);
	if (!creature) {
		delete condition;
		return;
	}

	creature->addCondition(condition, true);
}

void Game::forceRemoveCondition(uint32_t creatureId, ConditionType_t type)
{
	if (const auto& creature = getCreatureByID(creatureId)) {
		creature->removeCondition(type, true);
	}
}

void Game::sendOfflineTrainingDialog(const std::shared_ptr<Player>& player)
{
	if (!player) {
		return;
	}

	if (!player->hasModalWindowOpen(offlineTrainingWindow.id)) {
		player->sendModalWindow(offlineTrainingWindow);
	}
}

void Game::playerAnswerModalWindow(uint32_t playerId, uint32_t modalWindowId, uint8_t button, uint8_t choice)
{
	const auto& player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	if (!player->hasModalWindowOpen(modalWindowId)) {
		return;
	}

	player->onModalWindowHandled(modalWindowId);

	// offline training, hard-coded
	if (modalWindowId == std::numeric_limits<uint32_t>::max()) {
		if (button == offlineTrainingWindow.defaultEnterButton) {
			if (choice == SKILL_SWORD || choice == SKILL_AXE || choice == SKILL_CLUB || choice == SKILL_DISTANCE ||
			    choice == SKILL_MAGLEVEL) {
				const auto& bedItem = player->getBedItem();
				if (bedItem && bedItem->sleep(player)) {
					player->setOfflineTrainingSkill(choice);
					return;
				}
			}
		} else {
			player->sendTextMessage(MESSAGE_EVENT_ADVANCE, "Offline training aborted.");
		}

		player->setBedItem(nullptr);
	} else {
		for (auto creatureEvent : player->getCreatureEvents(CREATURE_EVENT_MODALWINDOW)) {
			creatureEvent->executeModalWindow(player, modalWindowId, button, choice);
		}
	}
}

void Game::addPlayer(std::shared_ptr<Player> player)
{
	const std::string& lowercase_name = boost::algorithm::to_lower_copy(player->getName());
	mappedPlayerNames[lowercase_name] = player;
	mappedPlayerGuids[player->getGUID()] = player;
	wildcardTree.insert(lowercase_name);
	players[player->getID()] = std::move(player);
}

void Game::removePlayer(const std::shared_ptr<Player>& player)
{
	const std::string& lowercase_name = boost::algorithm::to_lower_copy(player->getName());
	mappedPlayerNames.erase(lowercase_name);
	mappedPlayerGuids.erase(player->getGUID());
	wildcardTree.remove(lowercase_name);
	players.erase(player->getID());
}

void Game::addNpc(std::shared_ptr<Npc> npc) { npcs[npc->getID()] = std::move(npc); }

void Game::removeNpc(const std::shared_ptr<Npc>& npc) { npcs.erase(npc->getID()); }

void Game::addMonster(std::shared_ptr<Monster> monster) { monsters[monster->getID()] = std::move(monster); }

void Game::removeMonster(const std::shared_ptr<Monster>& monster) { monsters.erase(monster->getID()); }

Guild_ptr Game::getGuild(uint32_t id) const
{
	auto it = guilds.find(id);
	if (it == guilds.end()) {
		return nullptr;
	}
	return it->second;
}

void Game::addGuild(Guild_ptr guild) { guilds[guild->getId()] = std::move(guild); }

void Game::removeGuild(uint32_t guildId) { guilds.erase(guildId); }

void Game::internalRemoveItems(const std::vector<std::shared_ptr<Item>>& itemList, uint32_t amount, bool stackable)
{
	if (stackable) {
		for (const auto& item : itemList) {
			if (item->getItemCount() > amount) {
				internalRemoveItem(item, amount);
				break;
			} else {
				amount -= item->getItemCount();
				internalRemoveItem(item);
			}
		}
	} else {
		for (const auto& item : itemList) {
			internalRemoveItem(item);
		}
	}
}

std::shared_ptr<BedItem> Game::getBedBySleeper(uint32_t guid) const
{
	auto it = bedSleepersMap.find(guid);
	if (it == bedSleepersMap.end()) {
		return nullptr;
	}
	return it->second;
}

void Game::setBedSleeper(std::shared_ptr<BedItem> bed, uint32_t guid) { bedSleepersMap[guid] = std::move(bed); }

void Game::removeBedSleeper(uint32_t guid) { bedSleepersMap.erase(guid); }

void Game::updatePodium(const std::shared_ptr<Item>& item)
{
	if (!item->getPodium()) {
		return;
	}

	const auto& tile = item->getTile();
	if (!tile) {
		return;
	}

	// send to clients
	SpectatorVec spectators;
	map.getSpectators(spectators, item->getPosition(), true, true);
	for (const auto& spectator : spectators) {
		assert(spectator->getPlayer() != nullptr);
		std::static_pointer_cast<Player>(spectator)->sendUpdateTileItem(tile, item->getPosition(), item);
	}
}

std::shared_ptr<Item> Game::getUniqueItem(uint16_t uniqueId)
{
	auto it = uniqueItems.find(uniqueId);
	if (it == uniqueItems.end()) {
		return nullptr;
	}
	return it->second;
}

bool Game::addUniqueItem(uint16_t uniqueId, const std::shared_ptr<Item>& item)
{
	auto result = uniqueItems.emplace(uniqueId, item);
	if (!result.second) {
		std::cout << "Duplicate unique id: " << uniqueId << std::endl;
	}
	return result.second;
}

void Game::removeUniqueItem(uint16_t uniqueId) { uniqueItems.erase(uniqueId); }

bool Game::reload(ReloadTypes_t reloadType)
{
	switch (reloadType) {
		case RELOAD_TYPE_ACTIONS:
			return g_actions->reload();
		case RELOAD_TYPE_CHAT:
			return g_chat->load();
		case RELOAD_TYPE_CONFIG:
			return ConfigManager::load();
		case RELOAD_TYPE_CREATURESCRIPTS: {
			g_creatureEvents->reload();
			g_creatureEvents->removeInvalidEvents();
			return true;
		}
		case RELOAD_TYPE_EVENTS:
			return tfs::events::reload();
		case RELOAD_TYPE_GLOBALEVENTS:
			return g_globalEvents->reload();
		case RELOAD_TYPE_ITEMS:
			return Item::items.reload();
		case RELOAD_TYPE_MONSTERS:
			return g_monsters.reload();
		case RELOAD_TYPE_MOUNTS:
			return mounts.reload();
		case RELOAD_TYPE_MOVEMENTS:
			return g_moveEvents->reload();
		case RELOAD_TYPE_NPCS: {
			Npcs::reload();
			return true;
		}

		case RELOAD_TYPE_SPELLS: {
			if (!g_spells->reload()) {
				std::cout << "[Error - Game::reload] Failed to reload spells." << std::endl;
				std::terminate();
			} else if (!g_monsters.reload()) {
				std::cout << "[Error - Game::reload] Failed to reload monsters." << std::endl;
				std::terminate();
			}
			return true;
		}

		case RELOAD_TYPE_TALKACTIONS:
			return g_talkActions->reload();

		case RELOAD_TYPE_WEAPONS: {
			bool results = g_weapons->reload();
			g_weapons->loadDefaults();
			return results;
		}

		case RELOAD_TYPE_SCRIPTS: {
			// commented out stuff is TODO, once we approach further in revscriptsys
			g_actions->clear(true);
			g_creatureEvents->clear(true);
			g_moveEvents->clear(true);
			g_talkActions->clear(true);
			g_globalEvents->clear(true);
			g_weapons->clear(true);
			g_weapons->loadDefaults();
			g_spells->clear(true);
			g_scripts->loadScripts("scripts", false, true);
			g_creatureEvents->removeInvalidEvents();
			/*
			Npcs::reload();
			Item::items.reload();
			mounts.reload();
			ConfigManager::reload();
			tfs::events::load();
			g_chat->load();
			*/
			return true;
		}

		default: {
			if (!g_spells->reload()) {
				std::cout << "[Error - Game::reload] Failed to reload spells." << std::endl;
				std::terminate();
			} else if (!g_monsters.reload()) {
				std::cout << "[Error - Game::reload] Failed to reload monsters." << std::endl;
				std::terminate();
			}

			g_actions->reload();
			ConfigManager::load();
			g_creatureEvents->reload();
			g_monsters.reload();
			g_moveEvents->reload();
			Npcs::reload();
			g_talkActions->reload();
			Item::items.reload();
			g_weapons->reload();
			g_weapons->clear(true);
			g_weapons->loadDefaults();
			mounts.reload();
			g_globalEvents->reload();
			tfs::events::reload();
			g_chat->load();
			g_actions->clear(true);
			g_creatureEvents->clear(true);
			g_moveEvents->clear(true);
			g_talkActions->clear(true);
			g_globalEvents->clear(true);
			g_spells->clear(true);
			g_scripts->loadScripts("scripts", false, true);
			g_creatureEvents->removeInvalidEvents();
			return true;
		}
	}
	return true;
}
