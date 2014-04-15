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

#include "pugicast.h"

#include "items.h"
#include "commands.h"
#include "creature.h"
#include "monster.h"
#include "game.h"
#include "tile.h"
#include "house.h"
#include "actions.h"
#include "combat.h"
#include "iologindata.h"
#include "iomarket.h"
#include "chat.h"
#include "talkaction.h"
#include "spells.h"
#include "configmanager.h"
#include "ban.h"
#include "raids.h"
#include "database.h"
#include "server.h"
#include "ioguild.h"
#include "quests.h"
#include "globalevent.h"
#include "mounts.h"
#include "beds.h"
#include "scheduler.h"
#include "monster.h"
#include "spawn.h"
#include "connection.h"
#include "events.h"

extern ConfigManager g_config;
extern Actions* g_actions;
extern Chat g_chat;
extern TalkActions* g_talkActions;
extern Spells* g_spells;
extern Vocations g_vocations;
extern GlobalEvents* g_globalEvents;
extern Events* g_events;

Game::Game() :
	wildcardTree(false),
	offlineTrainingWindow(std::numeric_limits<uint32_t>::max(), "Choose a Skill", "Please choose a skill:")
{
	gameState = GAME_STATE_NORMAL;
	worldType = WORLD_TYPE_PVP;

	services = nullptr;
	lastStageLevel = 0;
	playersRecord = 0;
	motdNum = 0;
	useLastStageLevel = false;
	stagesEnabled = false;

	lastBucket = 0;

	//(1440 minutes/day)/(3600 seconds/day)*10 seconds event interval
	int32_t dayCycle = 3600;
	lightHourDelta = 1440 * 10 / dayCycle;
	lightHour = SUNRISE + (SUNSET - SUNRISE) / 2;
	lightLevel = LIGHT_LEVEL_DAY;
	lightState = LIGHT_STATE_DAY;

	offlineTrainingWindow.choices.emplace_back("Sword Fighting and Shielding", SKILL_SWORD);
	offlineTrainingWindow.choices.emplace_back("Axe Fighting and Shielding", SKILL_AXE);
	offlineTrainingWindow.choices.emplace_back("Club Fighting and Shielding", SKILL_CLUB);
	offlineTrainingWindow.choices.emplace_back("Distance Fighting and Shielding", SKILL_DISTANCE);
	offlineTrainingWindow.choices.emplace_back("Magic Level and Shielding", SKILL_MAGLEVEL);
	offlineTrainingWindow.buttons.emplace_back("Okay", 1);
	offlineTrainingWindow.buttons.emplace_back("Cancel", 0);
	offlineTrainingWindow.defaultEnterButton = 1;
	offlineTrainingWindow.defaultEscapeButton = 0;
	offlineTrainingWindow.priority = true;
}

Game::~Game()
{
	for (const auto& it : guilds) {
		delete it.second;
	}
}

void Game::start(ServiceManager* servicer)
{
	services = servicer;

	g_scheduler.addEvent(createSchedulerTask(EVENT_LIGHTINTERVAL, std::bind(&Game::checkLight, this)));
	g_scheduler.addEvent(createSchedulerTask(EVENT_CREATURE_THINK_INTERVAL, std::bind(&Game::checkCreatures, this, 0)));
	g_scheduler.addEvent(createSchedulerTask(EVENT_DECAYINTERVAL, std::bind(&Game::checkDecay, this)));
}

GameState_t Game::getGameState() const
{
	return gameState;
}

void Game::setWorldType(WorldType_t type)
{
	worldType = type;
}

void Game::setGameState(GameState_t newState)
{
	if (gameState == GAME_STATE_SHUTDOWN) {
		return;    //this cannot be stopped
	}

	if (gameState == newState) {
		return;
	}

	gameState = newState;
	switch (newState) {
		case GAME_STATE_INIT: {
			commands.loadFromXml();

			loadExperienceStages();

			groups.load();
			g_chat.load();

			Spawns::getInstance()->startup();

			Raids::getInstance()->loadFromXml();
			Raids::getInstance()->startup();

			Quests::getInstance()->loadFromXml();

			Mounts::getInstance()->loadFromXml();

			loadMotdNum();
			loadPlayersRecord();

			g_globalEvents->startup();
			break;
		}

		case GAME_STATE_SHUTDOWN: {
			g_globalEvents->execute(GLOBALEVENT_SHUTDOWN);

			//kick all players that are still online
			auto it = players.begin();
			while (it != players.end()) {
				it->second->kickPlayer(true);
				it = players.begin();
			}

			saveMotdNum();
			saveGameState();

			g_dispatcher.addTask(
				createTask(std::bind(&Game::shutdown, this)));

			g_scheduler.stop();
			g_dispatcher.stop();
			break;
		}

		case GAME_STATE_CLOSED: {
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

	for (const auto& it : players) {
		it.second->loginPosition = it.second->getPosition();
		IOLoginData::savePlayer(it.second);
	}

	map.saveMap();

	if (gameState == GAME_STATE_MAINTAIN) {
		setGameState(GAME_STATE_NORMAL);
	}
}

int32_t Game::loadMainMap(const std::string& filename)
{
	Monster::despawnRange = g_config.getNumber(ConfigManager::DEFAULT_DESPAWNRANGE);
	Monster::despawnRadius = g_config.getNumber(ConfigManager::DEFAULT_DESPAWNRADIUS);
	return map.loadMap("data/world/" + filename + ".otbm");
}

void Game::loadMap(const std::string& path)
{
	map.loadMap(path);
}

Cylinder* Game::internalGetCylinder(Player* player, const Position& pos)
{
	if (pos.x != 0xFFFF) {
		return getTile(pos.x, pos.y, pos.z);
	}

	//container
	if (pos.y & 0x40) {
		uint8_t from_cid = pos.y & 0x0F;
		return player->getContainerByID(from_cid);
	}

	//inventory
	return player;
}

Thing* Game::internalGetThing(Player* player, const Position& pos, int32_t index, uint32_t spriteId /*= 0*/, stackPosType_t type /*= STACKPOS_NORMAL*/)
{
	if (pos.x != 0xFFFF) {
		Tile* tile = getTile(pos.x, pos.y, pos.z);
		if (tile) {
			/*look at*/
			if (type == STACKPOS_LOOK) {
				return tile->getTopVisibleThing(player);
			}

			Thing* thing;

			/*for move operations*/
			if (type == STACKPOS_MOVE) {
				Item* item = tile->getTopDownItem();
				if (item && item->isMoveable()) {
					thing = item;
				} else {
					thing = tile->getTopVisibleCreature(player);
				}
			} else if (type == STACKPOS_USEITEM) {
				//First check items with topOrder 2 (ladders, signs, splashes)
				Item* item = tile->getItemByTopOrder(2);
				if (item && g_actions->hasAction(item)) {
					thing = item;
				} else {
					//then down items
					thing = tile->getTopDownItem();
					if (!thing) {
						thing = tile->getTopTopItem(); //then last we check items with topOrder 3 (doors etc)
						if (!thing) {
							thing = tile->ground;
						}
					}
				}
			} else if (type == STACKPOS_USE) {
				thing = tile->getTopDownItem();
			} else {
				thing = tile->__getThing(index);
			}

			if (player && tile->hasFlag(TILESTATE_SUPPORTS_HANGABLE)) {
				//do extra checks here if the thing is accessable
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
			return thing;
		}
	} else {
		//container
		if (pos.y & 0x40) {
			uint8_t fromCid = pos.y & 0x0F;
			uint8_t slot = pos.z;

			Container* parentContainer = player->getContainerByID(fromCid);
			if (!parentContainer) {
				return nullptr;
			}

			if (parentContainer->getID() == ITEM_BROWSEFIELD) {
				Tile* tile = parentContainer->getTile();
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
			return parentContainer->getItemByIndex(player->getContainerIndex(fromCid) + slot);
		} else if (pos.y == 0 && pos.z == 0) {
			const ItemType& it = Item::items.getItemIdByClientId(spriteId);
			if (it.id == 0) {
				return nullptr;
			}

			int32_t subType;
			if (it.isFluidContainer() && index < int32_t(sizeof(reverseFluidMap) / sizeof(int8_t))) {
				subType = reverseFluidMap[index];
			} else {
				subType = -1;
			}

			return findItemOfType(player, it.id, true, subType);
		} else { //inventory
			slots_t slot = static_cast<slots_t>(pos.y);
			return player->getInventoryItem(slot);
		}
	}
	return nullptr;
}

void Game::internalGetPosition(Item* item, Position& pos, uint8_t& stackpos)
{
	pos.x = 0;
	pos.y = 0;
	pos.z = 0;
	stackpos = 0;

	Cylinder* topParent = item->getTopParent();
	if (topParent) {
		if (Player* player = dynamic_cast<Player*>(topParent)) {
			pos.x = 0xFFFF;

			Container* container = dynamic_cast<Container*>(item->getParent());
			if (container) {
				pos.y = (uint16_t)0x40 | (uint16_t)player->getContainerID(container);
				pos.z = container->__getIndexOfThing(item);
				stackpos = pos.z;
			} else {
				pos.y = player->__getIndexOfThing(item);
				stackpos = pos.y;
			}
		} else if (Tile* tile = topParent->getTile()) {
			pos = tile->getPosition();
			stackpos = tile->__getIndexOfThing(item);
		}
	}
}

void Game::setTile(Tile* newTile)
{
	return map.setTile(newTile->getPosition(), newTile);
}

Tile* Game::getTile(int32_t x, int32_t y, int32_t z)
{
	return map.getTile(x, y, z);
}

Tile* Game::getTile(const Position& pos)
{
	return map.getTile(pos.x, pos.y, pos.z);
}

QTreeLeafNode* Game::getLeaf(uint32_t x, uint32_t y)
{
	return map.getLeaf(x, y);
}

Creature* Game::getCreatureByID(uint32_t id)
{
	if (id <= Player::playerAutoID) {
		return getPlayerByID(id);
	} else if (id <= Monster::monsterAutoID) {
		return getMonsterByID(id);
	} else if (id <= Npc::npcAutoID) {
		return getNpcByID(id);
	}
	return nullptr;
}

Monster* Game::getMonsterByID(uint32_t id)
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

Npc* Game::getNpcByID(uint32_t id)
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

Player* Game::getPlayerByID(uint32_t id)
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

Creature* Game::getCreatureByName(const std::string& s)
{
	if (s.empty()) {
		return nullptr;
	}

	const std::string& lowerCaseName = asLowerCaseString(s);

	auto m_it = mappedPlayerNames.find(lowerCaseName);
	if (m_it != mappedPlayerNames.end()) {
		return m_it->second;
	}

	for (const auto& it : npcs) {
		if (lowerCaseName == asLowerCaseString(it.second->getName())) {
			return it.second;
		}
	}

	for (const auto& it : monsters) {
		if (lowerCaseName == asLowerCaseString(it.second->getName())) {
			return it.second;
		}
	}
	return nullptr;
}

Npc* Game::getNpcByName(const std::string& s)
{
	if (s.empty()) {
		return nullptr;
	}

	const char* npcName = s.c_str();
	for (const auto& it : npcs) {
		if (strcasecmp(npcName, it.second->getName().c_str()) == 0) {
			return it.second;
		}
	}
	return nullptr;
}

Player* Game::getPlayerByName(const std::string& s)
{
	if (s.empty()) {
		return nullptr;
	}

	auto it = mappedPlayerNames.find(asLowerCaseString(s));
	if (it == mappedPlayerNames.end()) {
		return nullptr;
	}
	return it->second;
}

Player* Game::getPlayerByGUID(const uint32_t& guid)
{
	if (guid == 0) {
		return nullptr;
	}

	for (const auto& it : players) {
		if (guid == it.second->getGUID()) {
			return it.second;
		}
	}
	return nullptr;
}

ReturnValue Game::getPlayerByNameWildcard(const std::string& s, Player*& player)
{
	size_t strlen = s.length();
	if (strlen == 0 || strlen > 20) {
		return RET_PLAYERWITHTHISNAMEISNOTONLINE;
	}

	if (s.back() == '~') {
		const std::string& query = asLowerCaseString(s.substr(0, strlen - 1));
		std::string result;
		ReturnValue ret = wildcardTree.findOne(query, result);
		if (ret != RET_NOERROR) {
			return ret;
		}

		player = getPlayerByName(result);
	} else {
		player = getPlayerByName(s);
	}

	if (!player) {
		return RET_PLAYERWITHTHISNAMEISNOTONLINE;
	}

	return RET_NOERROR;
}

Player* Game::getPlayerByAccount(uint32_t acc)
{
	for (const auto& it : players) {
		if (it.second->getAccount() == acc) {
			return it.second;
		}
	}
	return nullptr;
}

bool Game::internalPlaceCreature(Creature* creature, const Position& pos, bool extendedPos /*=false*/, bool forced /*= false*/)
{
	if (creature->getParent() != nullptr) {
		return false;
	}

	if (!map.placeCreature(pos, creature, extendedPos, forced)) {
		return false;
	}

	creature->useThing2();
	creature->setID();
	creature->addList();
	return true;
}

bool Game::placeCreature(Creature* creature, const Position& pos, bool extendedPos /*=false*/, bool forced /*= false*/)
{
	if (!internalPlaceCreature(creature, pos, extendedPos, forced)) {
		return false;
	}

	SpectatorVec list;
	getSpectators(list, creature->getPosition(), true);
	for (Creature* spectator : list) {
		if (Player* tmpPlayer = spectator->getPlayer()) {
			tmpPlayer->sendCreatureAppear(creature, creature->getPosition(), true);
		}
	}

	for (Creature* spectator : list) {
		spectator->onCreatureAppear(creature, true);
	}

	Cylinder* creatureParent = creature->getParent();
	int32_t newIndex = creatureParent->__getIndexOfThing(creature);
	creatureParent->postAddNotification(creature, nullptr, newIndex);

	// TODO: Move this code to Player::onCreatureAppear where creature == this.
	Player* player = creature->getPlayer();
	if (player) {
		int32_t offlineTime;
		if (player->getLastLogout() != 0) {
			// Not counting more than 21 days to prevent overflow when multiplying with 1000 (for milliseconds).
			offlineTime = std::min<int32_t>(time(nullptr) - player->getLastLogout(), 86400 * 21);
		} else {
			offlineTime = 0;
		}

		Condition* conditionMuted = player->getCondition(CONDITION_MUTED, CONDITIONID_DEFAULT);
		if (conditionMuted && conditionMuted->getTicks() > 0) {
			conditionMuted->setTicks(conditionMuted->getTicks() - (offlineTime * 1000));
			if (conditionMuted->getTicks() <= 0) {
				player->removeCondition(conditionMuted);
			} else {
				player->addCondition(conditionMuted->clone());
			}
		}

		Condition* conditionTrade = player->getCondition(CONDITION_CHANNELMUTEDTICKS, CONDITIONID_DEFAULT, CHANNEL_ADVERTISING);
		if (conditionTrade && conditionTrade->getTicks() > 0) {
			conditionTrade->setTicks(conditionTrade->getTicks() - (offlineTime * 1000));
			if (conditionTrade->getTicks() <= 0) {
				player->removeCondition(conditionTrade);
			} else {
				player->addCondition(conditionTrade->clone());
			}
		}

		Condition* conditionTradeRook = player->getCondition(CONDITION_CHANNELMUTEDTICKS, CONDITIONID_DEFAULT, CHANNEL_ADVERTISINGROOKGAARD);
		if (conditionTradeRook && conditionTradeRook->getTicks() > 0) {
			conditionTradeRook->setTicks(conditionTradeRook->getTicks() - (offlineTime * 1000));
			if (conditionTradeRook->getTicks() <= 0) {
				player->removeCondition(conditionTradeRook);
			} else {
				player->addCondition(conditionTradeRook->clone());
			}
		}

		Condition* conditionHelp = player->getCondition(CONDITION_CHANNELMUTEDTICKS, CONDITIONID_DEFAULT, CHANNEL_HELP);
		if (conditionHelp && conditionHelp->getTicks() > 0) {
			conditionHelp->setTicks(conditionHelp->getTicks() - (offlineTime * 1000));
			if (conditionHelp->getTicks() <= 0) {
				player->removeCondition(conditionHelp);
			} else {
				player->addCondition(conditionHelp->clone());
			}
		}

		Condition* conditionYell = player->getCondition(CONDITION_YELLTICKS, CONDITIONID_DEFAULT);
		if (conditionYell && conditionYell->getTicks() > 0) {
			conditionYell->setTicks(conditionYell->getTicks() - (offlineTime * 1000));
			if (conditionYell->getTicks() <= 0) {
				player->removeCondition(conditionYell);
			} else {
				player->addCondition(conditionYell->clone());
			}
		}

		if (player->isPremium()) {
			int32_t value;
			player->getStorageValue(STORAGEVALUE_PROMOTION, value);

			if (player->isPromoted() && value != 1) {
				player->addStorageValue(STORAGEVALUE_PROMOTION, 1);
			} else if (!player->isPromoted() && value == 1) {
				player->setVocation(g_vocations.getPromotedVocation(player->getVocationId()));
			}
		} else if (player->isPromoted()) {
			player->setVocation(player->vocation->getFromVocation());
		}

		bool sentStats = false;

		int16_t oldStaminaMinutes = player->getStaminaMinutes();
		player->regenerateStamina(offlineTime);

		int32_t offlineTrainingSkill = player->getOfflineTrainingSkill();
		if (offlineTrainingSkill != -1) {
			player->setOfflineTrainingSkill(-1);
			uint32_t offlineTrainingTime = std::max<int32_t>(0, std::min<int32_t>(offlineTime, std::min<int32_t>(43200, player->getOfflineTrainingTime() / 1000)));

			if (offlineTime >= 600) {
				player->removeOfflineTrainingTime(offlineTrainingTime * 1000);

				int32_t remainder = offlineTime - offlineTrainingTime;
				if (remainder > 0) {
					player->addOfflineTrainingTime(remainder * 1000);
				}

				if (offlineTrainingTime >= 60) {
					std::ostringstream ss;
					ss << "During your absence you trained for ";
					int32_t hours = offlineTrainingTime / 3600;
					if (hours > 1) {
						ss << hours << " hours";
					} else if (hours == 1) {
						ss << "1 hour";
					}

					int32_t minutes = (offlineTrainingTime % 3600) / 60;
					if (minutes != 0) {
						if (hours != 0) {
							ss << " and ";
						}

						if (minutes > 1) {
							ss << minutes << " minutes";
						} else {
							ss << "1 minute";
						}
					}

					ss << '.';
					player->sendTextMessage(MESSAGE_EVENT_ADVANCE, ss.str());

					Vocation* vocation;
					if (player->isPromoted()) {
						vocation = player->getVocation();
					} else {
						int32_t promotedVocationId = g_vocations.getPromotedVocation(player->getVocationId());
						vocation = g_vocations.getVocation(promotedVocationId);
						if (!vocation) {
							vocation = player->getVocation();
						}
					}

					bool sendUpdateSkills = false;
					if (offlineTrainingSkill == SKILL_CLUB || offlineTrainingSkill == SKILL_SWORD || offlineTrainingSkill == SKILL_AXE) {
						float modifier = vocation->getAttackSpeed() / 1000.f;
						sendUpdateSkills = player->addOfflineTrainingTries((skills_t)offlineTrainingSkill, (offlineTrainingTime / modifier) / 2);
					} else if (offlineTrainingSkill == SKILL_DISTANCE) {
						float modifier = vocation->getAttackSpeed() / 1000.f;
						sendUpdateSkills = player->addOfflineTrainingTries((skills_t)offlineTrainingSkill, (offlineTrainingTime / modifier) / 4);
					} else if (offlineTrainingSkill == SKILL_MAGLEVEL) {
						int32_t gainTicks = vocation->getManaGainTicks() * 2;
						if (gainTicks == 0) {
							gainTicks = 1;
						}

						player->addOfflineTrainingTries(SKILL_MAGLEVEL, offlineTrainingTime * (vocation->getManaGainAmount() / gainTicks));
					}

					if (player->addOfflineTrainingTries(SKILL_SHIELD, offlineTrainingTime / 4) || sendUpdateSkills) {
						player->sendSkills();
					}
				}

				player->sendStats();
				sentStats = true;
			} else {
				player->sendTextMessage(MESSAGE_EVENT_ADVANCE, "You must be logged out for more than 10 minutes to start offline training.");
			}
		} else {
			uint16_t oldMinutes = player->getOfflineTrainingTime() / 60 / 1000;
			player->addOfflineTrainingTime(offlineTime * 1000);
			uint16_t newMinutes = player->getOfflineTrainingTime() / 60 / 1000;
			if (oldMinutes != newMinutes) {
				player->sendStats();
				sentStats = true;
			}
		}

		if (!sentStats && player->getStaminaMinutes() != oldStaminaMinutes) {
			player->sendStats();
		}
	}

	addCreatureCheck(creature);
	creature->onPlacedCreature();
	return true;
}

bool Game::removeCreature(Creature* creature, bool isLogout /*= true*/)
{
	if (creature->isRemoved()) {
		return false;
	}

	Tile* tile = creature->getTile();

	std::vector<int32_t> oldStackPosVector;

	SpectatorVec list;
	getSpectators(list, tile->getPosition(), true);
	for (Creature* spectator : list) {
		if (Player* player = spectator->getPlayer()) {
			oldStackPosVector.push_back(player->canSeeCreature(creature) ? tile->getStackposOfCreature(player, creature) : -1);
		}
	}

	int32_t index = tile->__getIndexOfThing(creature);

	if (!Map::removeCreature(creature)) {
		return false;
	}

	const Position& tilePosition = tile->getPosition();

	//send to client
	size_t i = 0;
	for (Creature* spectator : list) {
		if (Player* player = spectator->getPlayer()) {
			player->sendRemoveTileThing(tilePosition, oldStackPosVector[i++]);
		}
	}

	//event method
	for (Creature* spectator : list) {
		spectator->onCreatureDisappear(creature, index, isLogout);
	}

	creature->getParent()->postRemoveNotification(creature, nullptr, index, true);

	creature->removeList();
	creature->setRemoved();
	ReleaseCreature(creature);

	removeCreatureCheck(creature);

	for (Creature* summon : creature->summons) {
		summon->setLossSkill(false);
		removeCreature(summon);
	}

	creature->onRemovedCreature();
	return true;
}

void Game::playerMoveThing(uint32_t playerId, const Position& fromPos,
                           uint16_t spriteId, uint8_t fromStackPos, const Position& toPos, uint8_t count)
{
	Player* player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	uint8_t fromIndex = 0;
	if (fromPos.x == 0xFFFF) {
		if (fromPos.y & 0x40) {
			fromIndex = fromPos.z;
		} else {
			fromIndex = fromPos.y;
		}
	} else {
		fromIndex = fromStackPos;
	}

	Thing* thing = internalGetThing(player, fromPos, fromIndex, spriteId, STACKPOS_MOVE);
	if (!thing) {
		player->sendCancelMessage(RET_NOTPOSSIBLE);
		return;
	}

	Cylinder* toCylinder = internalGetCylinder(player, toPos);
	if (!toCylinder) {
		player->sendCancelMessage(RET_NOTPOSSIBLE);
		return;
	}

	if (Creature* movingCreature = thing->getCreature()) {
		if (Position::areInRange<1, 1, 0>(movingCreature->getPosition(), player->getPosition())) {
			SchedulerTask* task = createSchedulerTask(1000,
			                      std::bind(&Game::playerMoveCreature, this, player->getID(),
			                                  movingCreature->getID(), movingCreature->getPosition(), toCylinder->getPosition()));
			player->setNextActionTask(task);
		} else {
			playerMoveCreature(playerId, movingCreature->getID(), movingCreature->getPosition(), toCylinder->getPosition());
		}
	} else if (thing->getItem()) {
		playerMoveItem(playerId, fromPos, spriteId, fromStackPos, toPos, count);
	}
}

void Game::playerMoveCreature(uint32_t playerId, uint32_t movingCreatureId,
                              const Position& movingCreatureOrigPos, const Position& toPos)
{
	Player* player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	Tile* toTile = getTile(toPos);
	if (!toTile) {
		player->sendCancelMessage(RET_NOTPOSSIBLE);
		return;
	}

	Creature* movingCreature = getCreatureByID(movingCreatureId);
	if (!movingCreature) {
		return;
	}

	if (!player->canDoAction()) {
		uint32_t delay = player->getNextActionTime();
		SchedulerTask* task = createSchedulerTask(delay, std::bind(&Game::playerMoveCreature,
		                      this, playerId, movingCreatureId, movingCreatureOrigPos, toPos));
		player->setNextActionTask(task);
		return;
	}

	player->setNextActionTask(nullptr);

	if (!Position::areInRange<1, 1, 0>(movingCreatureOrigPos, player->getPosition())) {
		//need to walk to the creature first before moving it
		std::list<Direction> listDir;
		if (player->getPathTo(movingCreatureOrigPos, listDir, 0, 1, true, true)) {
			g_dispatcher.addTask(createTask(std::bind(&Game::playerAutoWalk,
			                                this, player->getID(), listDir)));
			SchedulerTask* task = createSchedulerTask(1500, std::bind(&Game::playerMoveCreature, this,
			                      playerId, movingCreatureId, movingCreatureOrigPos, toPos));
			player->setNextWalkActionTask(task);
		} else {
			player->sendCancelMessage(RET_THEREISNOWAY);
		}
		return;
	}

	if ((!movingCreature->isPushable() && !player->hasFlag(PlayerFlag_CanPushAllCreatures)) ||
	        (movingCreature->isInGhostMode() && !player->isAccessPlayer())) {
		player->sendCancelMessage(RET_NOTMOVEABLE);
		return;
	}

	//check throw distance
	const Position& movingCreaturePos = movingCreature->getPosition();
	if ((Position::getDistanceX(movingCreaturePos, toPos) > movingCreature->getThrowRange()) || (Position::getDistanceY(movingCreaturePos, toPos) > movingCreature->getThrowRange()) || (Position::getDistanceZ(movingCreaturePos, toPos) * 4 > movingCreature->getThrowRange())) {
		player->sendCancelMessage(RET_DESTINATIONOUTOFREACH);
		return;
	}

	Tile* movingCreatureTile = movingCreature->getTile();
	if (!movingCreatureTile) {
		player->sendCancelMessage(RET_NOTMOVEABLE);
		return;
	}

	if (player != movingCreature) {
		if (toTile->hasProperty(CONST_PROP_BLOCKPATH)) {
			player->sendCancelMessage(RET_NOTENOUGHROOM);
			return;
		} else if ((movingCreature->getZone() == ZONE_PROTECTION && !toTile->hasFlag(TILESTATE_PROTECTIONZONE)) || (movingCreature->getZone() == ZONE_NOPVP && !toTile->hasFlag(TILESTATE_NOPVPZONE))) {
			player->sendCancelMessage(RET_NOTPOSSIBLE);
			return;
		} else {
			if (CreatureVector* tileCreatures = toTile->getCreatures()) {
				for (Creature* tileCreature : *tileCreatures) {
					if (!tileCreature->isInGhostMode()) {
						player->sendCancelMessage(RET_NOTENOUGHROOM);
						return;
					}
				}
			}

			Npc* movingNpc = movingCreature->getNpc();
			if (movingNpc && !Spawns::getInstance()->isInZone(movingNpc->getMasterPos(), movingNpc->getMasterRadius(), toPos)) {
				player->sendCancelMessage(RET_NOTENOUGHROOM);
				return;
			}
		}
	}

	if (!g_events->eventPlayerOnMoveCreature(player, movingCreature, movingCreaturePos, toPos)) {
		return;
	}

	ReturnValue ret = internalMoveCreature(movingCreature, movingCreatureTile, toTile);
	if (ret != RET_NOERROR) {
		player->sendCancelMessage(ret);
	}
}

ReturnValue Game::internalMoveCreature(Creature* creature, Direction direction, uint32_t flags /*= 0*/)
{
	Cylinder* fromTile = creature->getTile();
	Cylinder* toTile = nullptr;

	creature->setLastPosition(creature->getPosition());
	const Position& currentPos = creature->getPosition();
	Position destPos = currentPos;
	bool diagonalMovement;

	switch (direction) {
		case NORTHWEST:
		case NORTHEAST:
		case SOUTHWEST:
		case SOUTHEAST:
			diagonalMovement = true;
			break;

		default:
			diagonalMovement = false;
			break;
	}

	destPos = getNextPosition(direction, destPos);

	if (creature->getPlayer() && !diagonalMovement) {
		//try go up
		if (currentPos.z != 8 && creature->getTile()->hasHeight(3)) {
			Tile* tmpTile = getTile(currentPos.x, currentPos.y, currentPos.getZ() - 1);
			if (tmpTile == nullptr || (tmpTile->ground == nullptr && !tmpTile->hasProperty(CONST_PROP_BLOCKSOLID))) {
				tmpTile = getTile(destPos.x, destPos.y, destPos.getZ() - 1);
				if (tmpTile && tmpTile->ground && !tmpTile->hasProperty(CONST_PROP_BLOCKSOLID)) {
					flags = flags | FLAG_IGNOREBLOCKITEM | FLAG_IGNOREBLOCKCREATURE;

					if (!tmpTile->floorChange()) {
						destPos.z--;
					}
				}
			}
		} else {
			//try go down
			Tile* tmpTile = getTile(destPos);
			if (currentPos.z != 7 && (tmpTile == nullptr || (tmpTile->ground == nullptr && !tmpTile->hasProperty(CONST_PROP_BLOCKSOLID)))) {
				tmpTile = getTile(destPos.x, destPos.y, destPos.z + 1);
				if (tmpTile && tmpTile->hasHeight(3)) {
					flags |= FLAG_IGNOREBLOCKITEM | FLAG_IGNOREBLOCKCREATURE;
					destPos.z++;
				}
			}
		}
	}

	toTile = getTile(destPos);
	ReturnValue ret = RET_NOTPOSSIBLE;

	if (toTile != nullptr) {
		ret = internalMoveCreature(creature, fromTile, toTile, flags);
	}

	return ret;
}

ReturnValue Game::internalMoveCreature(Creature* creature, Cylinder* fromCylinder, Cylinder* toCylinder, uint32_t flags /*= 0*/)
{
	//check if we can move the creature to the destination
	ReturnValue ret = toCylinder->__queryAdd(0, creature, 1, flags);
	if (ret != RET_NOERROR) {
		return ret;
	}

	fromCylinder->getTile()->moveCreature(creature, toCylinder);

	int32_t index = 0;
	Item* toItem = nullptr;
	Cylinder* subCylinder = nullptr;

	uint32_t n = 0;

	while ((subCylinder = toCylinder->__queryDestination(index, creature, &toItem, flags)) != toCylinder) {
		toCylinder->getTile()->moveCreature(creature, subCylinder);

		if (creature->getParent() != subCylinder) {
			//could happen if a script move the creature
			break;
		}

		toCylinder = subCylinder;
		flags = 0;

		//to prevent infinite loop
		if (++n >= MAP_MAX_LAYERS) {
			break;
		}
	}

	return RET_NOERROR;
}

void Game::playerMoveItem(uint32_t playerId, const Position& fromPos,
                          uint16_t spriteId, uint8_t fromStackPos, const Position& toPos, uint8_t count)
{
	Player* player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	if (!player->canDoAction()) {
		uint32_t delay = player->getNextActionTime();
		SchedulerTask* task = createSchedulerTask(delay, std::bind(&Game::playerMoveItem, this,
		                      playerId, fromPos, spriteId, fromStackPos, toPos, count));
		player->setNextActionTask(task);
		return;
	}

	player->setNextActionTask(nullptr);

	Cylinder* fromCylinder = internalGetCylinder(player, fromPos);
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

	Thing* thing = internalGetThing(player, fromPos, fromIndex, spriteId, STACKPOS_MOVE);
	if (!thing || !thing->getItem()) {
		player->sendCancelMessage(RET_NOTPOSSIBLE);
		return;
	}

	Item* item = thing->getItem();

	Cylinder* toCylinder = internalGetCylinder(player, toPos);
	uint8_t toIndex = 0;

	if (toPos.x == 0xFFFF) {
		if (toPos.y & 0x40) {
			toIndex = toPos.z;
		} else {
			toIndex = toPos.y;
		}
	}

	if (fromCylinder == nullptr || toCylinder == nullptr || item == nullptr || item->getClientID() != spriteId) {
		player->sendCancelMessage(RET_NOTPOSSIBLE);
		return;
	}

	if (!item->isPushable() || item->hasAttribute(ITEM_ATTRIBUTE_UNIQUEID)) {
		player->sendCancelMessage(RET_NOTMOVEABLE);
		return;
	}

	const Position& playerPos = player->getPosition();
	const Position& mapFromPos = fromCylinder->getTile()->getPosition();
	if (playerPos.z != mapFromPos.z) {
		player->sendCancelMessage(playerPos.z > mapFromPos.z ? RET_FIRSTGOUPSTAIRS : RET_FIRSTGODOWNSTAIRS);
		return;
	}

	if (!Position::areInRange<1, 1>(playerPos, mapFromPos)) {
		//need to walk to the item first before using it
		std::list<Direction> listDir;
		if (player->getPathTo(item->getPosition(), listDir, 0, 1, true, true)) {
			g_dispatcher.addTask(createTask(std::bind(&Game::playerAutoWalk,
			                                this, player->getID(), listDir)));

			SchedulerTask* task = createSchedulerTask(400, std::bind(&Game::playerMoveItem, this,
			                      playerId, fromPos, spriteId, fromStackPos, toPos, count));
			player->setNextWalkActionTask(task);
		} else {
			player->sendCancelMessage(RET_THEREISNOWAY);
		}
		return;
	}

	const Tile* toCylinderTile = toCylinder->getTile();
	const Position& mapToPos = toCylinderTile->getPosition();

	//hangable item specific code
	if (item->isHangable() && toCylinderTile->hasFlag(TILESTATE_SUPPORTS_HANGABLE)) {
		//destination supports hangable objects so need to move there first
		bool vertical = toCylinderTile->hasProperty(CONST_PROP_ISVERTICAL);
		if (vertical) {
			if (playerPos.x + 1 == mapToPos.x) {
				player->sendCancelMessage(RET_NOTPOSSIBLE);
				return;
			}
		} else { // horizontal
			if (playerPos.y + 1 == mapToPos.y) {
				player->sendCancelMessage(RET_NOTPOSSIBLE);
				return;
			}
		}

		if (!Position::areInRange<1, 1, 0>(playerPos, mapToPos)) {
			Position walkPos = mapToPos;
			if (vertical) {
				walkPos.x++;
			} else {
				walkPos.y++;
			}

			Position itemPos = fromPos;
			uint8_t itemStackPos = fromStackPos;

			if (fromPos.x != 0xFFFF && Position::areInRange<1, 1>(mapFromPos, playerPos)
			        && !Position::areInRange<1, 1, 0>(mapFromPos, walkPos)) {
				//need to pickup the item first
				Item* moveItem = nullptr;

				ReturnValue ret = internalMoveItem(fromCylinder, player, INDEX_WHEREEVER, item, count, &moveItem);
				if (ret != RET_NOERROR) {
					player->sendCancelMessage(ret);
					return;
				}

				//changing the position since its now in the inventory of the player
				internalGetPosition(moveItem, itemPos, itemStackPos);
			}

			std::list<Direction> listDir;
			if (player->getPathTo(walkPos, listDir, 0, 0, true, true)) {
				g_dispatcher.addTask(createTask(std::bind(&Game::playerAutoWalk,
				                                this, player->getID(), listDir)));

				SchedulerTask* task = createSchedulerTask(400, std::bind(&Game::playerMoveItem, this,
				                      playerId, itemPos, spriteId, itemStackPos, toPos, count));
				player->setNextWalkActionTask(task);
			} else {
				player->sendCancelMessage(RET_THEREISNOWAY);
			}
			return;
		}
	}

	if ((Position::getDistanceX(playerPos, mapToPos) > item->getThrowRange()) ||
	        (Position::getDistanceY(playerPos, mapToPos) > item->getThrowRange()) ||
	        (Position::getDistanceZ(mapFromPos, mapToPos) * 4 > item->getThrowRange())) {
		player->sendCancelMessage(RET_DESTINATIONOUTOFREACH);
		return;
	}

	if (!canThrowObjectTo(mapFromPos, mapToPos)) {
		player->sendCancelMessage(RET_CANNOTTHROW);
		return;
	}

	if (!g_events->eventPlayerOnMoveItem(player, item, count, fromPos, toPos)) {
		return;
	}

	ReturnValue ret = internalMoveItem(fromCylinder, toCylinder, toIndex, item, count, nullptr, 0, player);
	if (ret != RET_NOERROR) {
		player->sendCancelMessage(ret);
	}
}

ReturnValue Game::internalMoveItem(Cylinder* fromCylinder, Cylinder* toCylinder, int32_t index,
                                   Item* item, uint32_t count, Item** _moveItem, uint32_t flags /*= 0*/, Creature* actor/* = nullptr*/, Item* tradeItem/* = nullptr*/)
{
	if (!toCylinder) {
		return RET_NOTPOSSIBLE;
	}

	Tile* fromTile = fromCylinder->getTile();
	if (fromTile) {
		auto it = browseFields.find(fromTile);
		if (it != browseFields.end() && it->second == fromCylinder) {
			fromCylinder = fromTile;
		}
	}

	Item* toItem = nullptr;

	Cylinder* subCylinder;
	int floorN = 0;

	while ((subCylinder = toCylinder->__queryDestination(index, item, &toItem, flags)) != toCylinder) {
		toCylinder = subCylinder;
		flags = 0;

		//to prevent infinite loop
		if (++floorN >= MAP_MAX_LAYERS) {
			break;
		}
	}

	//destination is the same as the source?
	if (item == toItem) {
		return RET_NOERROR;    //silently ignore move
	}

	//check if we can add this item
	ReturnValue ret = toCylinder->__queryAdd(index, item, count, flags, actor);
	if (ret == RET_NEEDEXCHANGE) {
		//check if we can add it to source cylinder
		ret = fromCylinder->__queryAdd(fromCylinder->__getIndexOfThing(item), toItem, toItem->getItemCount(), 0);
		if (ret == RET_NOERROR) {
			//check how much we can move
			uint32_t maxExchangeQueryCount = 0;
			ReturnValue retExchangeMaxCount = fromCylinder->__queryMaxCount(INDEX_WHEREEVER, toItem, toItem->getItemCount(), maxExchangeQueryCount, 0);

			if (retExchangeMaxCount != RET_NOERROR && maxExchangeQueryCount == 0) {
				return retExchangeMaxCount;
			}

			if (toCylinder->__queryRemove(toItem, toItem->getItemCount(), flags) == RET_NOERROR) {
				int32_t oldToItemIndex = toCylinder->__getIndexOfThing(toItem);
				toCylinder->__removeThing(toItem, toItem->getItemCount());
				fromCylinder->__addThing(toItem);

				if (oldToItemIndex != -1) {
					toCylinder->postRemoveNotification(toItem, fromCylinder, oldToItemIndex, true);
				}

				int32_t newToItemIndex = fromCylinder->__getIndexOfThing(toItem);
				if (newToItemIndex != -1) {
					fromCylinder->postAddNotification(toItem, toCylinder, newToItemIndex);
				}

				ret = toCylinder->__queryAdd(index, item, count, flags);
				toItem = nullptr;
			}
		}
	}

	if (ret != RET_NOERROR) {
		return ret;
	}

	//check how much we can move
	uint32_t maxQueryCount = 0;
	ReturnValue retMaxCount = toCylinder->__queryMaxCount(index, item, count, maxQueryCount, flags);
	if (retMaxCount != RET_NOERROR && maxQueryCount == 0) {
		return retMaxCount;
	}

	uint32_t m;
	if (item->isStackable()) {
		m = std::min<uint32_t>(count, maxQueryCount);
	} else {
		m = maxQueryCount;
	}

	Item* moveItem = item;

	//check if we can remove this item
	ret = fromCylinder->__queryRemove(item, m, flags);
	if (ret != RET_NOERROR) {
		return ret;
	}

	if (tradeItem) {
		if (toCylinder->getItem() == tradeItem) {
			return RET_NOTENOUGHROOM;
		}

		Cylinder* tmpCylinder = toCylinder->getParent();
		while (tmpCylinder) {
			if (tmpCylinder->getItem() == tradeItem) {
				return RET_NOTENOUGHROOM;
			}

			tmpCylinder = tmpCylinder->getParent();
		}
	}

	//remove the item
	int32_t itemIndex = fromCylinder->__getIndexOfThing(item);
	Item* updateItem = nullptr;
	fromCylinder->__removeThing(item, m);
	bool isCompleteRemoval = item->isRemoved();

	//update item(s)
	if (item->isStackable()) {
		uint32_t n;

		if (toItem && toItem->getID() == item->getID()) {
			n = std::min<uint32_t>(100 - toItem->getItemCount(), m);
			toCylinder->__updateThing(toItem, toItem->getID(), toItem->getItemCount() + n);
			updateItem = toItem;
		} else {
			n = 0;
		}

		int32_t newCount = m - n;
		if (newCount > 0) {
			moveItem = Item::CreateItem(item->getID(), newCount);
		} else {
			moveItem = nullptr;
		}

		if (item->isRemoved()) {
			ReleaseItem(item);
		}
	}

	//add item
	if (moveItem /*m - n > 0*/) {
		toCylinder->__addThing(index, moveItem);
	}

	if (itemIndex != -1) {
		fromCylinder->postRemoveNotification(item, toCylinder, itemIndex, isCompleteRemoval);
	}

	if (moveItem) {
		int32_t moveItemIndex = toCylinder->__getIndexOfThing(moveItem);
		if (moveItemIndex != -1) {
			toCylinder->postAddNotification(moveItem, fromCylinder, moveItemIndex);
		}
	}

	if (updateItem) {
		int32_t updateItemIndex = toCylinder->__getIndexOfThing(updateItem);
		if (updateItemIndex != -1) {
			toCylinder->postAddNotification(updateItem, fromCylinder, updateItemIndex);
		}
	}

	if (_moveItem) {
		if (moveItem) {
			*_moveItem = moveItem;
		} else {
			*_moveItem = item;
		}
	}

	//we could not move all, inform the player
	if (item->isStackable() && maxQueryCount < count) {
		return retMaxCount;
	}

	return ret;
}

ReturnValue Game::internalAddItem(Cylinder* toCylinder, Item* item, int32_t index /*= INDEX_WHEREEVER*/,
                                  uint32_t flags/* = 0*/, bool test/* = false*/)
{
	uint32_t remainderCount = 0;
	return internalAddItem(toCylinder, item, index, flags, test, remainderCount);
}

ReturnValue Game::internalAddItem(Cylinder* toCylinder, Item* item, int32_t index,
                                  uint32_t flags, bool test, uint32_t& remainderCount)
{
	remainderCount = 0;

	if (toCylinder == nullptr || item == nullptr) {
		return RET_NOTPOSSIBLE;
	}

	Cylinder* destCylinder = toCylinder;
	Item* toItem = nullptr;
	toCylinder = toCylinder->__queryDestination(index, item, &toItem, flags);

	//check if we can add this item
	ReturnValue ret = toCylinder->__queryAdd(index, item, item->getItemCount(), flags);
	if (ret != RET_NOERROR) {
		return ret;
	}

	/*
	Check if we can move add the whole amount, we do this by checking against the original cylinder,
	since the queryDestination can return a cylinder that might only hold a part of the full amount.
	*/
	uint32_t maxQueryCount = 0;
	ret = destCylinder->__queryMaxCount(INDEX_WHEREEVER, item, item->getItemCount(), maxQueryCount, flags);

	if (ret != RET_NOERROR) {
		return ret;
	}

	if (test) {
		return RET_NOERROR;
	}

	if (item->isStackable() && toItem && toItem->getID() == item->getID()) {
		uint32_t m = std::min<uint32_t>(item->getItemCount(), maxQueryCount);
		uint32_t n = 0;

		if (toItem->getID() == item->getID()) {
			n = std::min<uint32_t>(100 - toItem->getItemCount(), m);
			toCylinder->__updateThing(toItem, toItem->getID(), toItem->getItemCount() + n);
		}

		int32_t count = m - n;
		if (count > 0) {
			if (item->getItemCount() != count) {
				Item* remainderItem = Item::CreateItem(item->getID(), count);
				if (internalAddItem(destCylinder, remainderItem, INDEX_WHEREEVER, flags, false) != RET_NOERROR) {
					ReleaseItem(remainderItem);
					remainderCount = count;
				}
			} else {
				toCylinder->__addThing(index, item);

				int32_t itemIndex = toCylinder->__getIndexOfThing(item);
				if (itemIndex != -1) {
					toCylinder->postAddNotification(item, nullptr, itemIndex);
				}
			}
		} else {
			//fully merged with toItem, item will be destroyed
			item->onRemoved();
			ReleaseItem(item);

			int32_t itemIndex = toCylinder->__getIndexOfThing(toItem);
			if (itemIndex != -1) {
				toCylinder->postAddNotification(toItem, nullptr, itemIndex);
			}
		}
	} else {
		toCylinder->__addThing(index, item);

		int32_t itemIndex = toCylinder->__getIndexOfThing(item);
		if (itemIndex != -1) {
			toCylinder->postAddNotification(item, nullptr, itemIndex);
		}
	}

	return RET_NOERROR;
}

ReturnValue Game::internalRemoveItem(Item* item, int32_t count /*= -1*/, bool test /*= false*/, uint32_t flags /*= 0*/)
{
	Cylinder* cylinder = item->getParent();
	if (cylinder == nullptr) {
		return RET_NOTPOSSIBLE;
	}

	Tile* fromTile = cylinder->getTile();
	if (fromTile) {
		auto it = browseFields.find(fromTile);
		if (it != browseFields.end() && it->second == cylinder) {
			cylinder = fromTile;
		}
	}

	if (count == -1) {
		count = item->getItemCount();
	}

	//check if we can remove this item
	ReturnValue ret = cylinder->__queryRemove(item, count, flags | FLAG_IGNORENOTMOVEABLE);
	if (ret != RET_NOERROR) {
		return ret;
	}

	if (!item->canRemove()) {
		return RET_NOTPOSSIBLE;
	}

	if (!test) {
		int32_t index = cylinder->__getIndexOfThing(item);

		//remove the item
		cylinder->__removeThing(item, count);
		bool isCompleteRemoval = false;

		if (item->isRemoved()) {
			isCompleteRemoval = true;
			ReleaseItem(item);
		}

		cylinder->postRemoveNotification(item, nullptr, index, isCompleteRemoval);
	}

	item->onRemoved();
	return RET_NOERROR;
}

ReturnValue Game::internalPlayerAddItem(Player* player, Item* item, bool dropOnMap /*= true*/, slots_t slot /*= CONST_SLOT_WHEREEVER*/)
{
	uint32_t remainderCount = 0;
	ReturnValue ret = internalAddItem(player, item, (int32_t)slot, 0, false, remainderCount);
	if (remainderCount > 0) {
		Item* remainderItem = Item::CreateItem(item->getID(), remainderCount);
		ReturnValue remaindRet = internalAddItem(player->getTile(), remainderItem, INDEX_WHEREEVER, FLAG_NOLIMIT);
		if (remaindRet != RET_NOERROR) {
			ReleaseItem(remainderItem);
		}
	}

	if (ret != RET_NOERROR && dropOnMap) {
		ret = internalAddItem(player->getTile(), item, INDEX_WHEREEVER, FLAG_NOLIMIT);
	}

	return ret;
}

Item* Game::findItemOfType(Cylinder* cylinder, uint16_t itemId,
                           bool depthSearch /*= true*/, int32_t subType /*= -1*/)
{
	if (cylinder == nullptr) {
		return nullptr;
	}

	std::vector<Container*> containers;
	for (int32_t i = cylinder->__getFirstIndex(), j = cylinder->__getLastIndex(); i < j; ++i) {
		Thing* thing = cylinder->__getThing(i);
		if (!thing) {
			continue;
		}

		Item* item = thing->getItem();
		if (!item) {
			continue;
		}

		if (item->getID() == itemId && (subType == -1 || subType == item->getSubType())) {
			return item;
		}

		if (depthSearch) {
			Container* container = item->getContainer();
			if (container) {
				containers.push_back(container);
			}
		}
	}

	size_t i = 0;
	while (i < containers.size()) {
		Container* container = containers[i++];
		for (Item* item : container->getItemList()) {
			if (item->getID() == itemId && (subType == -1 || subType == item->getSubType())) {
				return item;
			}

			Container* tmpContainer = item->getContainer();
			if (tmpContainer) {
				containers.push_back(tmpContainer);
			}
		}
	}
	return nullptr;
}

bool Game::removeMoney(Cylinder* cylinder, uint64_t money, uint32_t flags /*= 0*/)
{
	if (cylinder == nullptr) {
		return false;
	}

	if (money <= 0) {
		return true;
	}

	std::vector<Container*> containers;

	typedef std::multimap<uint64_t, Item*, std::less<uint64_t>> MoneyMap;
	typedef MoneyMap::value_type moneymap_pair;
	MoneyMap moneyMap;
	uint64_t moneyCount = 0;

	for (int32_t i = cylinder->__getFirstIndex(), j = cylinder->__getLastIndex(); i < j; ++i) {
		Thing* thing = cylinder->__getThing(i);
		if (!thing) {
			continue;
		}

		Item* item = thing->getItem();
		if (!item) {
			continue;
		}

		Container* container = item->getContainer();
		if (container) {
			containers.push_back(container);
		} else if (item->getWorth() != 0) {
			moneyCount += item->getWorth();
			moneyMap.insert(moneymap_pair(item->getWorth(), item));
		}
	}

	size_t i = 0;
	while (i < containers.size()) {
		Container* container = containers[i++];
		for (Item* item : container->getItemList()) {
			Container* tmpContainer = item->getContainer();
			if (tmpContainer) {
				containers.push_back(tmpContainer);
			} else if (item->getWorth() != 0) {
				moneyCount += item->getWorth();
				moneyMap.insert(moneymap_pair(item->getWorth(), item));
			}
		}
	}

	/*not enough money*/
	if (moneyCount < money) {
		return false;
	}

	for (MoneyMap::const_iterator mit = moneyMap.begin(), mend = moneyMap.end(); mit != mend && money > 0; ++mit) {
		Item* item = mit->second;
		internalRemoveItem(item);

		if (mit->first > money) {
			/* Remove a monetary value from an item*/
			uint64_t remaind = item->getWorth() - money;
			addMoney(cylinder, remaind, flags);
			money = 0;
		} else {
			money -= mit->first;
		}
	}

	return money == 0;
}

void Game::addMoney(Cylinder* cylinder, uint64_t money, uint32_t flags /*= 0*/)
{
	uint32_t crys = money / 10000;
	money -= crys * 10000;
	while (crys > 0) {
		Item* remaindItem = Item::CreateItem(ITEM_CRYSTAL_COIN, std::min<int32_t>(100, crys));

		ReturnValue ret = internalAddItem(cylinder, remaindItem, INDEX_WHEREEVER, flags);
		if (ret != RET_NOERROR) {
			internalAddItem(cylinder->getTile(), remaindItem, INDEX_WHEREEVER, FLAG_NOLIMIT);
		}

		crys -= std::min<int32_t>(100, crys);
	}

	uint16_t plat = money / 100;
	if (plat != 0) {
		Item* remaindItem = Item::CreateItem(ITEM_PLATINUM_COIN, plat);

		ReturnValue ret = internalAddItem(cylinder, remaindItem, INDEX_WHEREEVER, flags);
		if (ret != RET_NOERROR) {
			internalAddItem(cylinder->getTile(), remaindItem, INDEX_WHEREEVER, FLAG_NOLIMIT);
		}

		money -= plat * 100;
	}

	if (money != 0) {
		Item* remaindItem = Item::CreateItem(ITEM_GOLD_COIN, money);

		ReturnValue ret = internalAddItem(cylinder, remaindItem, INDEX_WHEREEVER, flags);
		if (ret != RET_NOERROR) {
			internalAddItem(cylinder->getTile(), remaindItem, INDEX_WHEREEVER, FLAG_NOLIMIT);
		}
	}
}

Item* Game::transformItem(Item* item, uint16_t newId, int32_t newCount /*= -1*/)
{
	if (item->getID() == newId && (newCount == -1 || (newCount == item->getSubType() && newCount != 0))) { //chargeless item placed on map = infinite
		return item;
	}

	Cylinder* cylinder = item->getParent();
	if (cylinder == nullptr) {
		return nullptr;
	}

	Tile* fromTile = cylinder->getTile();
	if (fromTile) {
		auto it = browseFields.find(fromTile);
		if (it != browseFields.end() && it->second == cylinder) {
			cylinder = fromTile;
		}
	}

	int32_t itemIndex = cylinder->__getIndexOfThing(item);
	if (itemIndex == -1) {
		return item;
	}

	if (!item->canTransform()) {
		return item;
	}

	const ItemType& curType = Item::items[item->getID()];
	const ItemType& newType = Item::items[newId];
	if (curType.alwaysOnTop != newType.alwaysOnTop) {
		//This only occurs when you transform items on tiles from a downItem to a topItem (or vice versa)
		//Remove the old, and add the new
		ReturnValue ret = internalRemoveItem(item);
		if (ret != RET_NOERROR) {
			return item;
		}

		Item* newItem;
		if (newCount == -1) {
			newItem = Item::CreateItem(newId);
		} else {
			newItem = Item::CreateItem(newId, newCount);
		}

		if (!newItem) {
			return nullptr;
		}

		newItem->stealAttributes(item);

		ret = internalAddItem(cylinder, newItem, INDEX_WHEREEVER, FLAG_NOLIMIT);
		if (ret != RET_NOERROR) {
			delete newItem;
			return nullptr;
		}

		return newItem;
	}

	if (curType.type == newType.type) {
		//Both items has the same type so we can safely change id/subtype
		if (newCount == 0 && (item->isStackable() || item->hasAttribute(ITEM_ATTRIBUTE_CHARGES))) {
			if (item->isStackable()) {
				internalRemoveItem(item);
				return nullptr;
			} else {
				int32_t newItemId = newId;

				if (curType.id == newType.id) {
					newItemId = curType.decayTo;
				}

				if (newItemId == -1) {
					internalRemoveItem(item);
					return nullptr;
				} else if (newItemId != newId) {
					//Replacing the the old item with the new while maintaining the old position
					Item* newItem = Item::CreateItem(newItemId, 1);
					if (newItem == nullptr) {
						return nullptr;
					}

					cylinder->__replaceThing(itemIndex, newItem);
					cylinder->postAddNotification(newItem, cylinder, itemIndex);

					item->setParent(nullptr);
					cylinder->postRemoveNotification(item, cylinder, itemIndex, true);
					ReleaseItem(item);
					return newItem;
				} else {
					return transformItem(item, newItemId);
				}
			}
		} else {
			cylinder->postRemoveNotification(item, cylinder, itemIndex, false);
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

			cylinder->__updateThing(item, itemId, count);
			cylinder->postAddNotification(item, cylinder, itemIndex);
			return item;
		}
	}

	//Replacing the the old item with the new while maintaining the old position
	Item* newItem;
	if (newCount == -1) {
		newItem = Item::CreateItem(newId);
	} else {
		newItem = Item::CreateItem(newId, newCount);
	}

	if (newItem == nullptr) {
		return nullptr;
	}

	cylinder->__replaceThing(itemIndex, newItem);
	cylinder->postAddNotification(newItem, cylinder, itemIndex);

	item->setParent(nullptr);
	cylinder->postRemoveNotification(item, cylinder, itemIndex, true);
	ReleaseItem(item);

	return newItem;
}

ReturnValue Game::internalTeleport(Thing* thing, const Position& newPos, bool pushMove/* = true*/, uint32_t flags /*= 0*/)
{
	if (newPos == thing->getPosition()) {
		return RET_NOERROR;
	} else if (thing->isRemoved()) {
		return RET_NOTPOSSIBLE;
	}

	Tile* toTile = getTile(newPos.x, newPos.y, newPos.z);
	if (toTile) {
		if (Creature* creature = thing->getCreature()) {
			ReturnValue ret = toTile->__queryAdd(0, creature, 1, FLAG_NOLIMIT);
			if (ret != RET_NOERROR) {
				return ret;
			}

			creature->getTile()->moveCreature(creature, toTile, !pushMove);
			return RET_NOERROR;
		} else if (Item* item = thing->getItem()) {
			return internalMoveItem(item->getParent(), toTile, INDEX_WHEREEVER, item, item->getItemCount(), nullptr, flags);
		}
	}
	return RET_NOTPOSSIBLE;
}

//Implementation of player invoked events
void Game::playerMove(uint32_t playerId, Direction direction)
{
	Player* player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	player->resetIdleTime();
	player->setNextWalkActionTask(nullptr);

	player->startAutoWalk(std::list<Direction> { direction });
}

bool Game::playerBroadcastMessage(Player* player, const std::string& text) const
{
	if (!player->hasFlag(PlayerFlag_CanBroadcast)) {
		return false;
	}

	std::cout << "> " << player->getName() << " broadcasted: \"" << text << "\"." << std::endl;

	for (const auto& it : players) {
		it.second->sendCreatureSay(player, TALKTYPE_BROADCAST, text);
	}

	return true;
}

void Game::playerCreatePrivateChannel(uint32_t playerId)
{
	Player* player = getPlayerByID(playerId);
	if (!player || !player->isPremium()) {
		return;
	}

	ChatChannel* channel = g_chat.createChannel(*player, CHANNEL_PRIVATE);
	if (!channel || !channel->addUser(*player)) {
		return;
	}

	player->sendCreatePrivateChannel(channel->getId(), channel->getName());
}

void Game::playerChannelInvite(uint32_t playerId, const std::string& name)
{
	Player* player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	PrivateChatChannel* channel = g_chat.getPrivateChannel(*player);
	if (!channel) {
		return;
	}

	Player* invitePlayer = getPlayerByName(name);
	if (!invitePlayer) {
		return;
	}

	if (player == invitePlayer) {
		return;
	}

	channel->invitePlayer(*player, *invitePlayer);
}

void Game::playerChannelExclude(uint32_t playerId, const std::string& name)
{
	Player* player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	PrivateChatChannel* channel = g_chat.getPrivateChannel(*player);
	if (!channel) {
		return;
	}

	Player* excludePlayer = getPlayerByName(name);
	if (!excludePlayer) {
		return;
	}

	if (player == excludePlayer) {
		return;
	}

	channel->excludePlayer(*player, *excludePlayer);
}

void Game::playerRequestChannels(uint32_t playerId)
{
	Player* player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	player->sendChannelsDialog();
}

void Game::playerOpenChannel(uint32_t playerId, uint16_t channelId)
{
	Player* player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	ChatChannel* channel = g_chat.addUserToChannel(*player, channelId);
	if (!channel) {
		return;
	}

	const InvitedMap* invitedUsers = channel->getInvitedUsersPtr();
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
	Player* player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	g_chat.removeUserFromChannel(*player, channelId);
}

void Game::playerOpenPrivateChannel(uint32_t playerId, std::string& receiver)
{
	Player* player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	if (!IOLoginData::formatPlayerName(receiver)) {
		player->sendCancel("A player with this name does not exist.");
		return;
	}

	player->sendOpenPrivateChannel(receiver);
}

void Game::playerCloseNpcChannel(uint32_t playerId)
{
	Player* player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	SpectatorVec list;
	getSpectators(list, player->getPosition());
	for (Creature* spectator : list) {
		if (Npc* npc = spectator->getNpc()) {
			npc->onPlayerCloseChannel(player);
		}
	}
}

void Game::playerReceivePing(uint32_t playerId)
{
	Player* player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	player->receivePing();
}

void Game::playerReceivePingBack(uint32_t playerId)
{
	Player* player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	player->sendPingBack();
}

void Game::playerAutoWalk(uint32_t playerId, const std::list<Direction>& listDir)
{
	Player* player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	player->resetIdleTime();
	player->setNextWalkTask(nullptr);
	player->startAutoWalk(listDir);
}

void Game::playerStopAutoWalk(uint32_t playerId)
{
	Player* player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	player->stopWalk();
}

void Game::playerUseItemEx(uint32_t playerId, const Position& fromPos, uint8_t fromStackPos, uint16_t fromSpriteId,
                           const Position& toPos, uint8_t toStackPos, uint16_t toSpriteId, bool isHotkey)
{
	Player* player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	if (isHotkey && !g_config.getBoolean(ConfigManager::AIMBOT_HOTKEY_ENABLED)) {
		return;
	}

	Thing* thing = internalGetThing(player, fromPos, fromStackPos, fromSpriteId, STACKPOS_USEITEM);
	if (!thing) {
		player->sendCancelMessage(RET_NOTPOSSIBLE);
		return;
	}

	Item* item = thing->getItem();
	if (!item || !item->isUseable() || item->getClientID() != fromSpriteId) {
		player->sendCancelMessage(RET_CANNOTUSETHISOBJECT);
		return;
	}

	Position walkToPos = fromPos;
	ReturnValue ret = g_actions->canUse(player, fromPos);
	if (ret == RET_NOERROR) {
		ret = g_actions->canUse(player, toPos, item);
		if (ret == RET_TOOFARAWAY) {
			walkToPos = toPos;
		}
	}

	if (ret != RET_NOERROR) {
		if (ret == RET_TOOFARAWAY) {
			Position itemPos = fromPos;
			uint8_t itemStackPos = fromStackPos;

			if (fromPos.x != 0xFFFF && toPos.x != 0xFFFF && Position::areInRange<1, 1, 0>(fromPos, player->getPosition()) &&
			        !Position::areInRange<1, 1, 0>(fromPos, toPos)) {
				Item* moveItem = nullptr;

				ret = internalMoveItem(item->getParent(), player, INDEX_WHEREEVER, item, item->getItemCount(), &moveItem);
				if (ret != RET_NOERROR) {
					player->sendCancelMessage(ret);
					return;
				}

				//changing the position since its now in the inventory of the player
				internalGetPosition(moveItem, itemPos, itemStackPos);
			}

			std::list<Direction> listDir;
			if (player->getPathTo(walkToPos, listDir, 0, 1, true, true)) {
				g_dispatcher.addTask(createTask(std::bind(&Game::playerAutoWalk, this, player->getID(), listDir)));

				SchedulerTask* task = createSchedulerTask(400, std::bind(&Game::playerUseItemEx, this,
				                      playerId, itemPos, itemStackPos, fromSpriteId, toPos, toStackPos, toSpriteId, isHotkey));
				player->setNextWalkActionTask(task);
			} else {
				player->sendCancelMessage(RET_THEREISNOWAY);
			}
			return;
		}

		player->sendCancelMessage(ret);
		return;
	}

	if (!player->canDoAction()) {
		uint32_t delay = player->getNextActionTime();
		SchedulerTask* task = createSchedulerTask(delay, std::bind(&Game::playerUseItemEx, this,
		                      playerId, fromPos, fromStackPos, fromSpriteId, toPos, toStackPos, toSpriteId, isHotkey));
		player->setNextActionTask(task);
		return;
	}

	player->resetIdleTime();
	player->setNextActionTask(nullptr);

	g_actions->useItemEx(player, fromPos, toPos, toStackPos, item, isHotkey);
}

void Game::playerUseItem(uint32_t playerId, const Position& pos, uint8_t stackPos,
                         uint8_t index, uint16_t spriteId, bool isHotkey)
{
	Player* player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	if (isHotkey && !g_config.getBoolean(ConfigManager::AIMBOT_HOTKEY_ENABLED)) {
		return;
	}

	Thing* thing = internalGetThing(player, pos, stackPos, spriteId, STACKPOS_USEITEM);
	if (!thing) {
		player->sendCancelMessage(RET_NOTPOSSIBLE);
		return;
	}

	Item* item = thing->getItem();
	if (!item || item->isUseable() || item->getClientID() != spriteId) {
		player->sendCancelMessage(RET_CANNOTUSETHISOBJECT);
		return;
	}

	ReturnValue ret = g_actions->canUse(player, pos);
	if (ret != RET_NOERROR) {
		if (ret == RET_TOOFARAWAY) {
			std::list<Direction> listDir;
			if (player->getPathTo(pos, listDir, 0, 1, true, true)) {
				g_dispatcher.addTask(createTask(std::bind(&Game::playerAutoWalk,
				                                this, player->getID(), listDir)));

				SchedulerTask* task = createSchedulerTask(400, std::bind(&Game::playerUseItem, this,
				                      playerId, pos, stackPos, index, spriteId, isHotkey));
				player->setNextWalkActionTask(task);
				return;
			}

			ret = RET_THEREISNOWAY;
		}

		player->sendCancelMessage(ret);
		return;
	}

	if (!player->canDoAction()) {
		uint32_t delay = player->getNextActionTime();
		SchedulerTask* task = createSchedulerTask(delay, std::bind(&Game::playerUseItem, this,
		                      playerId, pos, stackPos, index, spriteId, isHotkey));
		player->setNextActionTask(task);
		return;
	}

	player->resetIdleTime();
	player->setNextActionTask(nullptr);

	g_actions->useItem(player, pos, index, item, isHotkey);
}

void Game::playerUseWithCreature(uint32_t playerId, const Position& fromPos, uint8_t fromStackPos, uint32_t creatureId, uint16_t spriteId, bool isHotkey)
{
	Player* player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	Creature* creature = getCreatureByID(creatureId);
	if (!creature) {
		return;
	}

	if (!Position::areInRange<7, 5, 0>(creature->getPosition(), player->getPosition())) {
		return;
	}

	if (!g_config.getBoolean(ConfigManager::AIMBOT_HOTKEY_ENABLED)) {
		if (creature->getPlayer() || isHotkey) {
			player->sendCancelMessage(RET_DIRECTPLAYERSHOOT);
			return;
		}
	}

	Thing* thing = internalGetThing(player, fromPos, fromStackPos, spriteId, STACKPOS_USEITEM);
	if (!thing) {
		player->sendCancelMessage(RET_NOTPOSSIBLE);
		return;
	}

	Item* item = thing->getItem();
	if (!item || !item->isUseable() || item->getClientID() != spriteId) {
		player->sendCancelMessage(RET_CANNOTUSETHISOBJECT);
		return;
	}

	Position toPos = creature->getPosition();
	Position walkToPos = fromPos;
	ReturnValue ret = g_actions->canUse(player, fromPos);
	if (ret == RET_NOERROR) {
		ret = g_actions->canUse(player, toPos, item);
		if (ret == RET_TOOFARAWAY) {
			walkToPos = toPos;
		}
	}

	if (ret != RET_NOERROR) {
		if (ret == RET_TOOFARAWAY) {
			Position itemPos = fromPos;
			uint8_t itemStackPos = fromStackPos;

			if (fromPos.x != 0xFFFF && Position::areInRange<1, 1, 0>(fromPos, player->getPosition()) && !Position::areInRange<1, 1, 0>(fromPos, toPos)) {
				Item* moveItem = nullptr;
				ret = internalMoveItem(item->getParent(), player, INDEX_WHEREEVER, item, item->getItemCount(), &moveItem);
				if (ret != RET_NOERROR) {
					player->sendCancelMessage(ret);
					return;
				}

				//changing the position since its now in the inventory of the player
				internalGetPosition(moveItem, itemPos, itemStackPos);
			}

			std::list<Direction> listDir;
			if (player->getPathTo(walkToPos, listDir, 0, 1, true, true)) {
				g_dispatcher.addTask(createTask(std::bind(&Game::playerAutoWalk,
				                                this, player->getID(), listDir)));

				SchedulerTask* task = createSchedulerTask(400, std::bind(&Game::playerUseWithCreature, this,
				                      playerId, itemPos, itemStackPos, creatureId, spriteId, isHotkey));
				player->setNextWalkActionTask(task);
			} else {
				player->sendCancelMessage(RET_THEREISNOWAY);
			}
			return;
		}

		player->sendCancelMessage(ret);
		return;
	}

	if (!player->canDoAction()) {
		uint32_t delay = player->getNextActionTime();
		SchedulerTask* task = createSchedulerTask(delay, std::bind(&Game::playerUseWithCreature, this,
		                      playerId, fromPos, fromStackPos, creatureId, spriteId, isHotkey));
		player->setNextActionTask(task);
		return;
	}

	player->resetIdleTime();
	player->setNextActionTask(nullptr);

	g_actions->useItemEx(player, fromPos, creature->getPosition(), creature->getParent()->__getIndexOfThing(creature), item, isHotkey, creatureId);
}

void Game::playerCloseContainer(uint32_t playerId, uint8_t cid)
{
	Player* player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	player->closeContainer(cid);
	player->sendCloseContainer(cid);
}

void Game::playerMoveUpContainer(uint32_t playerId, uint8_t cid)
{
	Player* player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	Container* container = player->getContainerByID(cid);
	if (!container) {
		return;
	}

	Container* parentContainer = dynamic_cast<Container*>(container->getParent());
	if (!parentContainer) {
		Tile* tile = container->getTile();
		if (!tile) {
			return;
		}

		auto it = browseFields.find(tile);
		if (it == browseFields.end()) {
			parentContainer = new Container(tile);
			parentContainer->useThing2();
			browseFields[tile] = parentContainer;
			g_scheduler.addEvent(createSchedulerTask(30000, std::bind(&Game::decreaseBrowseFieldRef, this, tile->getPosition())));
		} else {
			parentContainer = it->second;
		}
	}

	player->addContainer(cid, parentContainer);
	player->sendContainer(cid, parentContainer, parentContainer->hasParent(), player->getContainerIndex(cid));
}

void Game::playerUpdateContainer(uint32_t playerId, uint8_t cid)
{
	Player* player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	Container* container = player->getContainerByID(cid);
	if (!container) {
		return;
	}

	player->sendContainer(cid, container, container->hasParent(), player->getContainerIndex(cid));
}

void Game::playerRotateItem(uint32_t playerId, const Position& pos, uint8_t stackPos, const uint16_t spriteId)
{
	Player* player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	Thing* thing = internalGetThing(player, pos, stackPos);
	if (!thing) {
		return;
	}

	Item* item = thing->getItem();
	if (!item || item->getClientID() != spriteId || !item->isRoteable() || item->hasAttribute(ITEM_ATTRIBUTE_UNIQUEID)) {
		player->sendCancelMessage(RET_NOTPOSSIBLE);
		return;
	}

	if (pos.x != 0xFFFF && !Position::areInRange<1, 1, 0>(pos, player->getPosition())) {
		std::list<Direction> listDir;
		if (player->getPathTo(pos, listDir, 0, 1, true, true)) {
			g_dispatcher.addTask(createTask(std::bind(&Game::playerAutoWalk,
			                                this, player->getID(), listDir)));

			SchedulerTask* task = createSchedulerTask(400, std::bind(&Game::playerRotateItem, this,
			                      playerId, pos, stackPos, spriteId));
			player->setNextWalkActionTask(task);
		} else {
			player->sendCancelMessage(RET_THEREISNOWAY);
		}
		return;
	}

	uint16_t newId = Item::items[item->getID()].rotateTo;
	if (newId != 0) {
		transformItem(item, newId);
	}
}

void Game::playerWriteItem(uint32_t playerId, uint32_t windowTextId, const std::string& text)
{
	Player* player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	uint16_t maxTextLength = 0;
	uint32_t internalWindowTextId = 0;

	Item* writeItem = player->getWriteItem(internalWindowTextId, maxTextLength);
	if (text.length() > maxTextLength || windowTextId != internalWindowTextId) {
		return;
	}

	if (!writeItem || writeItem->isRemoved()) {
		player->sendCancelMessage(RET_NOTPOSSIBLE);
		return;
	}

	Cylinder* topParent = writeItem->getTopParent();

	Player* owner = dynamic_cast<Player*>(topParent);
	if (owner && owner != player) {
		player->sendCancelMessage(RET_NOTPOSSIBLE);
		return;
	}

	if (!Position::areInRange<1, 1, 0>(writeItem->getPosition(), player->getPosition())) {
		player->sendCancelMessage(RET_NOTPOSSIBLE);
		return;
	}

	for (auto creatureEvent : player->getCreatureEvents(CREATURE_EVENT_TEXTEDIT)) {
		if (!creatureEvent->executeTextEdit(player, writeItem, text)) {
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
	Player* player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	const Position& playerPos = player->getPosition();
	if (playerPos.z != pos.z) {
		player->sendCancelMessage(playerPos.z > pos.z ? RET_FIRSTGOUPSTAIRS : RET_FIRSTGODOWNSTAIRS);
		return;
	}

	if (!Position::areInRange<1, 1>(playerPos, pos)) {
		std::list<Direction> listDir;
		if (player->getPathTo(pos, listDir, 0, 1, true, true)) {
			g_dispatcher.addTask(createTask(std::bind(&Game::playerAutoWalk,
			                                this, player->getID(), listDir)));
			SchedulerTask* task = createSchedulerTask(400, std::bind(
			                          &Game::playerBrowseField, this, playerId, pos
			                      ));
			player->setNextWalkActionTask(task);
		} else {
			player->sendCancelMessage(RET_THEREISNOWAY);
		}
		return;
	}

	Tile* tile = getTile(pos);
	if (!tile) {
		return;
	}

	if (!g_events->eventPlayerOnBrowseField(player, pos)) {
		return;
	}

	Container* container;

	auto it = browseFields.find(tile);
	if (it == browseFields.end()) {
		container = new Container(tile);
		container->useThing2();
		browseFields[tile] = container;
		g_scheduler.addEvent(createSchedulerTask(30000, std::bind(&Game::decreaseBrowseFieldRef, this, tile->getPosition())));
	} else {
		container = it->second;
	}

	uint8_t dummyContainerId = 0xF - ((pos.x % 3) * 3 + (pos.y % 3));
	Container* openContainer = player->getContainerByID(dummyContainerId);
	if (openContainer) {
		player->onCloseContainer(openContainer);
		player->closeContainer(dummyContainerId);
	} else {
		player->addContainer(dummyContainerId, container);
		player->sendContainer(dummyContainerId, container, false, 0);
	}
}

void Game::playerSeekInContainer(uint32_t playerId, uint8_t containerId, uint16_t index)
{
	Player* player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	Container* container = player->getContainerByID(containerId);
	if (!container || !container->hasPagination()) {
		return;
	}

	if ((index % container->capacity()) != 0 || index >= container->size()) {
		return;
	}

	player->setContainerIndex(containerId, index);
	player->sendContainer(containerId, container, false, index);
}

void Game::playerUpdateHouseWindow(uint32_t playerId, uint8_t listId, uint32_t windowTextId, const std::string& text)
{
	Player* player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	uint32_t internalWindowTextId;
	uint32_t internalListId;

	House* house = player->getEditHouse(internalWindowTextId, internalListId);
	if (house && internalWindowTextId == windowTextId && listId == 0) {
		house->setAccessList(internalListId, text);
		player->setEditHouse(nullptr);
	}
}

void Game::playerRequestTrade(uint32_t playerId, const Position& pos, uint8_t stackPos,
                              uint32_t tradePlayerId, uint16_t spriteId)
{
	Player* player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	Player* tradePartner = getPlayerByID(tradePlayerId);
	if (!tradePartner || tradePartner == player) {
		player->sendTextMessage(MESSAGE_INFO_DESCR, "Sorry, not possible.");
		return;
	}

	if (!Position::areInRange<2, 2, 0>(tradePartner->getPosition(), player->getPosition())) {
		std::ostringstream ss;
		ss << tradePartner->getName() << " tells you to move closer.";
		player->sendTextMessage(MESSAGE_INFO_DESCR, ss.str());
		return;
	}

	if (!canThrowObjectTo(tradePartner->getPosition(), player->getPosition())) {
		player->sendCancelMessage(RET_CREATUREISNOTREACHABLE);
		return;
	}

	Item* tradeItem = dynamic_cast<Item*>(internalGetThing(player, pos, stackPos, spriteId, STACKPOS_USE));
	if (!tradeItem || tradeItem->getClientID() != spriteId || !tradeItem->isPickupable() || tradeItem->hasAttribute(ITEM_ATTRIBUTE_UNIQUEID)) {
		player->sendCancelMessage(RET_NOTPOSSIBLE);
		return;
	}

	const Position& playerPosition = player->getPosition();
	const Position& tradeItemPosition = tradeItem->getPosition();
	if (playerPosition.z != tradeItemPosition.z) {
		player->sendCancelMessage(playerPosition.z > tradeItemPosition.z ? RET_FIRSTGOUPSTAIRS : RET_FIRSTGODOWNSTAIRS);
		return;
	}

	if (!Position::areInRange<1, 1>(tradeItemPosition, playerPosition)) {
		std::list<Direction> listDir;
		if (player->getPathTo(pos, listDir, 0, 1, true, true)) {
			g_dispatcher.addTask(createTask(std::bind(&Game::playerAutoWalk,
			                                this, player->getID(), listDir)));

			SchedulerTask* task = createSchedulerTask(400, std::bind(&Game::playerRequestTrade, this,
			                      playerId, pos, stackPos, tradePlayerId, spriteId));
			player->setNextWalkActionTask(task);
		} else {
			player->sendCancelMessage(RET_THEREISNOWAY);
		}
		return;
	}

	Container* tradeItemContainer = tradeItem->getContainer();
	if (tradeItemContainer) {
		for (const auto& it : tradeItems) {
			Item* item = it.first;
			if (tradeItem == item) {
				player->sendTextMessage(MESSAGE_INFO_DESCR, "This item is already being traded.");
				return;
			}

			if (tradeItemContainer->isHoldingItem(item)) {
				player->sendTextMessage(MESSAGE_INFO_DESCR, "This item is already being traded.");
				return;
			}

			Container* container = item->getContainer();
			if (container && container->isHoldingItem(tradeItem)) {
				player->sendTextMessage(MESSAGE_INFO_DESCR, "This item is already being traded.");
				return;
			}
		}
	} else {
		for (const auto& it : tradeItems) {
			Item* item = it.first;
			if (tradeItem == item) {
				player->sendTextMessage(MESSAGE_INFO_DESCR, "This item is already being traded.");
				return;
			}

			Container* container = item->getContainer();
			if (container && container->isHoldingItem(tradeItem)) {
				player->sendTextMessage(MESSAGE_INFO_DESCR, "This item is already being traded.");
				return;
			}
		}
	}

	Container* tradeContainer = tradeItem->getContainer();
	if (tradeContainer && tradeContainer->getItemHoldingCount() + 1 > 100) {
		player->sendTextMessage(MESSAGE_INFO_DESCR, "You can not trade more than 100 items.");
		return;
	}
	
	if (!g_events->eventPlayerOnTradeRequest(player, tradePartner, tradeItem)) {
		return;
	}

	internalStartTrade(player, tradePartner, tradeItem);
}

bool Game::internalStartTrade(Player* player, Player* tradePartner, Item* tradeItem)
{
	if (player->tradeState != TRADE_NONE && !(player->tradeState == TRADE_ACKNOWLEDGE && player->tradePartner == tradePartner)) {
		player->sendCancelMessage(RET_YOUAREALREADYTRADING);
		return false;
	} else if (tradePartner->tradeState != TRADE_NONE && tradePartner->tradePartner != player) {
		player->sendCancelMessage(RET_THISPLAYERISALREADYTRADING);
		return false;
	}

	player->tradePartner = tradePartner;
	player->tradeItem = tradeItem;
	player->tradeState = TRADE_INITIATED;
	tradeItem->useThing2();
	tradeItems[tradeItem] = player->getID();

	player->sendTradeItemRequest(player, tradeItem, true);

	if (tradePartner->tradeState == TRADE_NONE) {
		std::ostringstream ss;
		ss << player->getName() << " wants to trade with you.";
		tradePartner->sendTextMessage(MESSAGE_EVENT_ADVANCE, ss.str());
		tradePartner->tradeState = TRADE_ACKNOWLEDGE;
		tradePartner->tradePartner = player;
	} else {
		Item* counterOfferItem = tradePartner->tradeItem;
		player->sendTradeItemRequest(tradePartner, counterOfferItem, false);
		tradePartner->sendTradeItemRequest(player, tradeItem, false);
	}

	return true;
}

void Game::playerAcceptTrade(uint32_t playerId)
{
	Player* player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	if (!(player->getTradeState() == TRADE_ACKNOWLEDGE || player->getTradeState() == TRADE_INITIATED)) {
		return;
	}

	Player* tradePartner = player->tradePartner;
	if (!tradePartner) {
		return;
	}

	if (!canThrowObjectTo(tradePartner->getPosition(), player->getPosition())) {
		player->sendCancelMessage(RET_CREATUREISNOTREACHABLE);
		return;
	}

	player->setTradeState(TRADE_ACCEPT);

	if (tradePartner->getTradeState() == TRADE_ACCEPT) {
		Item* tradeItem1 = player->tradeItem;
		Item* tradeItem2 = tradePartner->tradeItem;

		player->setTradeState(TRADE_TRANSFER);
		tradePartner->setTradeState(TRADE_TRANSFER);

		std::map<Item*, uint32_t>::iterator it = tradeItems.find(tradeItem1);
		if (it != tradeItems.end()) {
			ReleaseItem(it->first);
			tradeItems.erase(it);
		}

		it = tradeItems.find(tradeItem2);
		if (it != tradeItems.end()) {
			ReleaseItem(it->first);
			tradeItems.erase(it);
		}

		bool isSuccess = false;

		ReturnValue ret1 = internalAddItem(tradePartner, tradeItem1, INDEX_WHEREEVER, 0, true);
		ReturnValue ret2 = internalAddItem(player, tradeItem2, INDEX_WHEREEVER, 0, true);
		if (ret1 == RET_NOERROR && ret2 == RET_NOERROR) {
			ret1 = internalRemoveItem(tradeItem1, tradeItem1->getItemCount(), true);
			ret2 = internalRemoveItem(tradeItem2, tradeItem2->getItemCount(), true);
			if (ret1 == RET_NOERROR && ret2 == RET_NOERROR) {
				Cylinder* cylinder1 = tradeItem1->getParent();
				Cylinder* cylinder2 = tradeItem2->getParent();

				uint32_t count1 = tradeItem1->getItemCount();
				uint32_t count2 = tradeItem2->getItemCount();

				ret1 = internalMoveItem(cylinder1, tradePartner, INDEX_WHEREEVER, tradeItem1, count1, nullptr, FLAG_IGNOREAUTOSTACK, nullptr, tradeItem2);
				if (ret1 == RET_NOERROR) {
					internalMoveItem(cylinder2, player, INDEX_WHEREEVER, tradeItem2, count2, nullptr, FLAG_IGNOREAUTOSTACK);

					tradeItem1->onTradeEvent(ON_TRADE_TRANSFER, tradePartner);
					tradeItem2->onTradeEvent(ON_TRADE_TRANSFER, player);

					isSuccess = true;
				}
			}
		}

		if (!isSuccess) {
			std::string errorDescription;

			if (tradePartner->tradeItem) {
				errorDescription = getTradeErrorDescription(ret1, tradeItem1);
				tradePartner->sendTextMessage(MESSAGE_EVENT_ADVANCE, errorDescription);
				tradePartner->tradeItem->onTradeEvent(ON_TRADE_CANCEL, tradePartner);
			}

			if (player->tradeItem) {
				errorDescription = getTradeErrorDescription(ret2, tradeItem2);
				player->sendTextMessage(MESSAGE_EVENT_ADVANCE, errorDescription);
				player->tradeItem->onTradeEvent(ON_TRADE_CANCEL, player);
			}
		}

		player->setTradeState(TRADE_NONE);
		player->tradeItem = nullptr;
		player->tradePartner = nullptr;
		player->sendTradeClose();

		tradePartner->setTradeState(TRADE_NONE);
		tradePartner->tradeItem = nullptr;
		tradePartner->tradePartner = nullptr;
		tradePartner->sendTradeClose();
	}
}

std::string Game::getTradeErrorDescription(ReturnValue ret, Item* item)
{
	if (item) {
		if (ret == RET_NOTENOUGHCAPACITY) {
			std::ostringstream ss;
			ss << "You do not have enough capacity to carry";

			if (item->isStackable() && item->getItemCount() > 1) {
				ss << " these objects.";
			} else {
				ss << " this object.";
			}

			ss << std::endl << ' ' << item->getWeightDescription();
			return ss.str();
		} else if (ret == RET_NOTENOUGHROOM || ret == RET_CONTAINERNOTENOUGHROOM) {
			std::ostringstream ss;
			ss << "You do not have enough room to carry";

			if (item->isStackable() && item->getItemCount() > 1) {
				ss << " these objects.";
			} else {
				ss << " this object.";
			}

			return ss.str();
		}
	}
	return "Trade could not be completed.";
}

void Game::playerLookInTrade(uint32_t playerId, bool lookAtCounterOffer, uint8_t index)
{
	Player* player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	Player* tradePartner = player->tradePartner;
	if (!tradePartner) {
		return;
	}

	Item* tradeItem;
	if (lookAtCounterOffer) {
		tradeItem = tradePartner->getTradeItem();
	} else {
		tradeItem = player->getTradeItem();
	}

	if (!tradeItem) {
		return;
	}

	const Position& playerPosition = player->getPosition();
	const Position& tradeItemPosition = tradeItem->getPosition();

	int32_t lookDistance = std::max<int32_t>(Position::getDistanceX(playerPosition, tradeItemPosition),
	                                         Position::getDistanceY(playerPosition, tradeItemPosition));
	if (index == 0) {
		g_events->eventPlayerOnLookInTrade(player, tradePartner, tradeItem, lookDistance);
		return;
	}

	Container* tradeContainer = tradeItem->getContainer();
	if (!tradeContainer) {
		return;
	}

	std::vector<const Container*> containers {tradeContainer};
	size_t i = 0;
	while (i < containers.size()) {
		const Container* container = containers[i++];
		for (Item* item : container->getItemList()) {
			Container* tmpContainer = item->getContainer();
			if (tmpContainer) {
				containers.push_back(tmpContainer);
			}

			if (--index == 0) {
				g_events->eventPlayerOnLookInTrade(player, tradePartner, item, lookDistance);
				return;
			}
		}
	}
}

void Game::playerCloseTrade(uint32_t playerId)
{
	Player* player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	internalCloseTrade(player);
}

bool Game::internalCloseTrade(Player* player)
{
	Player* tradePartner = player->tradePartner;
	if ((tradePartner && tradePartner->getTradeState() == TRADE_TRANSFER) || player->getTradeState() == TRADE_TRANSFER) {
		return true;
	}

	if (player->getTradeItem()) {
		std::map<Item*, uint32_t>::iterator it = tradeItems.find(player->getTradeItem());
		if (it != tradeItems.end()) {
			ReleaseItem(it->first);
			tradeItems.erase(it);
		}

		player->tradeItem->onTradeEvent(ON_TRADE_CANCEL, player);
		player->tradeItem = nullptr;
	}

	player->setTradeState(TRADE_NONE);
	player->tradePartner = nullptr;

	player->sendTextMessage(MESSAGE_STATUS_SMALL, "Trade cancelled.");
	player->sendTradeClose();

	if (tradePartner) {
		if (tradePartner->getTradeItem()) {
			std::map<Item*, uint32_t>::iterator it = tradeItems.find(tradePartner->getTradeItem());
			if (it != tradeItems.end()) {
				ReleaseItem(it->first);
				tradeItems.erase(it);
			}

			tradePartner->tradeItem->onTradeEvent(ON_TRADE_CANCEL, tradePartner);
			tradePartner->tradeItem = nullptr;
		}

		tradePartner->setTradeState(TRADE_NONE);
		tradePartner->tradePartner = nullptr;

		tradePartner->sendTextMessage(MESSAGE_STATUS_SMALL, "Trade cancelled.");
		tradePartner->sendTradeClose();
	}
	return true;
}

void Game::playerPurchaseItem(uint32_t playerId, uint16_t spriteId, uint8_t count, uint8_t amount,
                              bool ignoreCap/* = false*/, bool inBackpacks/* = false*/)
{
	if (amount == 0 || amount > 100) {
		return;
	}

	Player* player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	int32_t onBuy, onSell;

	Npc* merchant = player->getShopOwner(onBuy, onSell);
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

void Game::playerSellItem(uint32_t playerId, uint16_t spriteId, uint8_t count, uint8_t amount, bool ignoreEquipped)
{
	if (amount == 0 || amount > 100) {
		return;
	}

	Player* player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	int32_t onBuy, onSell;

	Npc* merchant = player->getShopOwner(onBuy, onSell);
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
	Player* player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	player->closeShopWindow();
}

void Game::playerLookInShop(uint32_t playerId, uint16_t spriteId, uint8_t count)
{
	Player* player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	int32_t onBuy, onSell;

	Npc* merchant = player->getShopOwner(onBuy, onSell);
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

	if (!g_events->eventPlayerOnLookInShop(player, &it, subType)) {
		return;
	}

	std::ostringstream ss;
	ss << "You see " << Item::getDescription(it, 1, nullptr, subType);
	player->sendTextMessage(MESSAGE_INFO_DESCR, ss.str());
}

void Game::playerLookAt(uint32_t playerId, const Position& pos, uint16_t spriteId, uint8_t stackPos)
{
	Player* player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	Thing* thing = internalGetThing(player, pos, stackPos, spriteId, STACKPOS_LOOK);
	if (!thing) {
		player->sendCancelMessage(RET_NOTPOSSIBLE);
		return;
	}

	Position thingPos = thing->getPosition();
	if (!player->canSee(thingPos)) {
		player->sendCancelMessage(RET_NOTPOSSIBLE);
		return;
	}

	Position playerPos = player->getPosition();

	int32_t lookDistance;
	if (thing != player) {
		lookDistance = std::max<int32_t>(Position::getDistanceX(playerPos, thingPos), Position::getDistanceY(playerPos, thingPos));
		if (playerPos.z != thingPos.z) {
			lookDistance += 15;
		}
	} else {
		lookDistance = -1;
	}

	g_events->eventPlayerOnLook(player, pos, thing, stackPos, lookDistance);
}

void Game::playerLookInBattleList(uint32_t playerId, uint32_t creatureId)
{
	Player* player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	Creature* creature = getCreatureByID(creatureId);
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

	int32_t lookDistance;
	if (creature != player) {
		const Position& playerPos = player->getPosition();
		lookDistance = std::max<int32_t>(Position::getDistanceX(playerPos, creaturePos), Position::getDistanceY(playerPos, creaturePos));
		if (playerPos.z != creaturePos.z) {
			lookDistance += 15;
		}
	} else {
		lookDistance = -1;
	}

	g_events->eventPlayerOnLookInBattleList(player, creature, lookDistance);
}

void Game::playerCancelAttackAndFollow(uint32_t playerId)
{
	Player* player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	playerSetAttackedCreature(playerId, 0);
	playerFollowCreature(playerId, 0);
	player->stopWalk();
}

void Game::playerSetAttackedCreature(uint32_t playerId, uint32_t creatureId)
{
	Player* player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	if (player->getAttackedCreature() && creatureId == 0) {
		player->setAttackedCreature(nullptr);
		player->sendCancelTarget();
		return;
	}

	Creature* attackCreature = getCreatureByID(creatureId);
	if (!attackCreature) {
		player->setAttackedCreature(nullptr);
		player->sendCancelTarget();
		return;
	}

	ReturnValue ret = Combat::canTargetCreature(player, attackCreature);
	if (ret != RET_NOERROR) {
		player->sendCancelMessage(ret);
		player->sendCancelTarget();
		player->setAttackedCreature(nullptr);
		return;
	}

	player->setAttackedCreature(attackCreature);
	g_dispatcher.addTask(createTask(std::bind(&Game::updateCreatureWalk, this, player->getID())));
}

void Game::playerFollowCreature(uint32_t playerId, uint32_t creatureId)
{
	Player* player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	player->setAttackedCreature(nullptr);
	g_dispatcher.addTask(createTask(std::bind(&Game::updateCreatureWalk, this, player->getID())));
	player->setFollowCreature(getCreatureByID(creatureId));
}

void Game::playerSetFightModes(uint32_t playerId, fightMode_t fightMode, chaseMode_t chaseMode, secureMode_t secureMode)
{
	Player* player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	player->setFightMode(fightMode);
	player->setChaseMode(chaseMode);
	player->setSecureMode(secureMode);
}

void Game::playerRequestAddVip(uint32_t playerId, const std::string& name)
{
	if (name.length() > 20) {
		return;
	}

	Player* player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	Player* vipPlayer = getPlayerByName(name);
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

		if (!vipPlayer->isInGhostMode() || player->isAccessPlayer()) {
			player->addVIP(vipPlayer->getGUID(), vipPlayer->getName(), VIPSTATUS_ONLINE);
		} else {
			player->addVIP(vipPlayer->getGUID(), vipPlayer->getName(), VIPSTATUS_OFFLINE);
		}
	}
}

void Game::playerRequestRemoveVip(uint32_t playerId, uint32_t guid)
{
	Player* player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	player->removeVIP(guid);
}

void Game::playerRequestEditVip(uint32_t playerId, uint32_t guid, const std::string& description, uint32_t icon, bool notify)
{
	Player* player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	player->editVIP(guid, description, icon, notify);
}

void Game::playerTurn(uint32_t playerId, Direction dir)
{
	Player* player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	if (!g_events->eventPlayerOnTurn(player, dir)) {
		return;
	}

	player->resetIdleTime();
	internalCreatureTurn(player, dir);
}

void Game::playerRequestOutfit(uint32_t playerId)
{
	if (!g_config.getBoolean(ConfigManager::ALLOW_CHANGEOUTFIT)) {
		return;
	}

	Player* player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	player->sendOutfitWindow();
}

void Game::playerToggleMount(uint32_t playerId, bool mount)
{
	Player* player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	player->toggleMount(mount);
}

void Game::playerChangeOutfit(uint32_t playerId, Outfit_t outfit)
{
	if (!g_config.getBoolean(ConfigManager::ALLOW_CHANGEOUTFIT)) {
		return;
	}

	Player* player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	if (!player->hasRequestedOutfit()) {
		return;
	}
	player->hasRequestedOutfit(false);

	if (outfit.lookMount != 0) {
		Mount* mount = Mounts::getInstance()->getMountByClientID(outfit.lookMount);
		if (!mount) {
			return;
		}

		if (!player->hasMount(mount)) {
			return;
		}

		if (player->isMounted()) {
			Mount* prevMount = Mounts::getInstance()->getMountByID(player->getCurrentMount());
			if (prevMount) {
				changeSpeed(player, mount->speed - prevMount->speed);
			}

			player->setCurrentMount(mount->id);
		} else {
			player->setCurrentMount(mount->id);
			outfit.lookMount = 0;
		}
	} else if (player->isMounted()) {
		player->dismount();
	}

	if (player->canWear(outfit.lookType, outfit.lookAddons)) {
		player->defaultOutfit = outfit;

		if (player->hasCondition(CONDITION_OUTFIT)) {
			return;
		}

		internalCreatureChangeOutfit(player, outfit);
	}
}

void Game::playerShowQuestLog(uint32_t playerId)
{
	Player* player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	player->sendQuestLog();
}

void Game::playerShowQuestLine(uint32_t playerId, uint16_t questId)
{
	Player* player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	Quest* quest = Quests::getInstance()->getQuestByID(questId);
	if (!quest) {
		return;
	}

	player->sendQuestLine(quest);
}

void Game::playerSay(uint32_t playerId, uint16_t channelId, SpeakClasses type,
                     const std::string& receiver, const std::string& text)
{
	Player* player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	player->resetIdleTime();

	uint32_t muteTime = player->isMuted();
	if (muteTime > 0) {
		std::ostringstream ss;
		ss << "You are still muted for " << muteTime << " seconds.";
		player->sendTextMessage(MESSAGE_STATUS_SMALL, ss.str());
		return;
	}

	if (playerSayCommand(player, text)) {
		return;
	}

	if (playerSaySpell(player, type, text)) {
		return;
	}

	if (!text.empty() && text.front() == '/' && player->isAccessPlayer()) {
		return;
	}

	if (type != TALKTYPE_PRIVATE_PN) {
		player->removeMessageBuffer();
	}

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
			g_chat.talkToChannel(*player, type, text, channelId);
			break;

		case TALKTYPE_PRIVATE_PN:
			playerSpeakToNpc(player, text);
			break;

		case TALKTYPE_BROADCAST:
			playerBroadcastMessage(player, text);
			break;

		default:
			break;
	}
}

bool Game::playerSayCommand(Player* player, const std::string& text)
{
	if (text.empty()) {
		return false;
	}

	char firstCharacter = text.front();
	for (char commandTag : commandTags) {
		if (commandTag == firstCharacter) {
			if (commands.exeCommand(*player, text)) {
				return true;
			}
		}
	}
	return false;
}

bool Game::playerSaySpell(Player* player, SpeakClasses type, const std::string& text)
{
	std::string words = text;

	TalkActionResult_t result = g_talkActions->playerSaySpell(player, type, words);
	if (result == TALKACTION_BREAK) {
		return true;
	}

	result = g_spells->playerSaySpell(player, words);
	if (result == TALKACTION_BREAK) {
		if (!g_config.getBoolean(ConfigManager::EMOTE_SPELLS)) {
			return internalCreatureSay(player, TALKTYPE_SAY, words, false);
		} else {
			return internalCreatureSay(player, TALKTYPE_MONSTER_SAY, words, false);
		}

	} else if (result == TALKACTION_FAILED) {
		return true;
	}

	return false;
}

bool Game::playerWhisper(Player* player, const std::string& text)
{
	SpectatorVec list;
	getSpectators(list, player->getPosition(), false, false,
	              Map::maxClientViewportX, Map::maxClientViewportX,
	              Map::maxClientViewportY, Map::maxClientViewportY);

	//send to client
	for (Creature* spectator : list) {
		if (Player* spectatorPlayer = spectator->getPlayer()) {
			if (!Position::areInRange<1, 1>(player->getPosition(), spectatorPlayer->getPosition())) {
				spectatorPlayer->sendCreatureSay(player, TALKTYPE_WHISPER, "pspsps");
			} else {
				spectatorPlayer->sendCreatureSay(player, TALKTYPE_WHISPER, text);
			}
		}
	}

	//event method
	for (Creature* spectator : list) {
		spectator->onCreatureSay(player, TALKTYPE_WHISPER, text);
	}

	return true;
}

bool Game::playerYell(Player* player, const std::string& text)
{
	if (player->getLevel() == 1) {
		player->sendTextMessage(MESSAGE_STATUS_SMALL, "You may not yell as long as you are on level 1.");
		return false;
	}

	if (player->hasCondition(CONDITION_YELLTICKS)) {
		player->sendCancelMessage(RET_YOUAREEXHAUSTED);
		return false;
	}

	if (player->getAccountType() < ACCOUNT_TYPE_GAMEMASTER) {
		Condition* condition = Condition::createCondition(CONDITIONID_DEFAULT, CONDITION_YELLTICKS, 30000, 0);
		player->addCondition(condition);
	}

	internalCreatureSay(player, TALKTYPE_YELL, asUpperCaseString(text), false);
	return true;
}

bool Game::playerSpeakTo(Player* player, SpeakClasses type, const std::string& receiver,
                         const std::string& text)
{
	Player* toPlayer = getPlayerByName(receiver);
	if (!toPlayer) {
		player->sendTextMessage(MESSAGE_STATUS_SMALL, "A player with this name is not online.");
		return false;
	}

	if (type == TALKTYPE_PRIVATE_RED_TO && (player->hasFlag(PlayerFlag_CanTalkRedPrivate) || player->getAccountType() >= ACCOUNT_TYPE_GAMEMASTER)) {
		type = TALKTYPE_PRIVATE_RED_FROM;
	} else {
		type = TALKTYPE_PRIVATE_FROM;
	}

	toPlayer->sendCreatureSay(player, type, text);
	toPlayer->onCreatureSay(player, type, text);

	if (toPlayer->isInGhostMode() && !player->isAccessPlayer()) {
		player->sendTextMessage(MESSAGE_STATUS_SMALL, "A player with this name is not online.");
	} else {
		std::ostringstream ss;
		ss << "Message sent to " << toPlayer->getName() << '.';
		player->sendTextMessage(MESSAGE_STATUS_SMALL, ss.str());
	}
	return true;
}

bool Game::playerSpeakToNpc(Player* player, const std::string& text)
{
	SpectatorVec list;
	getSpectators(list, player->getPosition());
	for (Creature* spectator : list) {
		if (spectator->getNpc()) {
			spectator->onCreatureSay(player, TALKTYPE_PRIVATE_PN, text);
		}
	}
	return true;
}

//--
bool Game::canThrowObjectTo(const Position& fromPos, const Position& toPos, bool checkLineOfSight /*= true*/,
                            int32_t rangex /*= Map::maxClientViewportX*/, int32_t rangey /*= Map::maxClientViewportY*/) const
{
	return map.canThrowObjectTo(fromPos, toPos, checkLineOfSight, rangex, rangey);
}

bool Game::isSightClear(const Position& fromPos, const Position& toPos, bool floorCheck) const
{
	return map.isSightClear(fromPos, toPos, floorCheck);
}

bool Game::internalCreatureTurn(Creature* creature, Direction dir)
{
	if (creature->getDirection() == dir) {
		return false;
	}

	creature->setDirection(dir);

	//send to client
	SpectatorVec list;
	getSpectators(list, creature->getPosition(), true, true);
	for (Creature* spectator : list) {
		spectator->getPlayer()->sendCreatureTurn(creature);
	}
	return true;
}

bool Game::internalCreatureSay(Creature* creature, SpeakClasses type, const std::string& text,
                               bool ghostMode, SpectatorVec* listPtr/* = nullptr*/, const Position* pos/* = nullptr*/)
{
	if (text.empty()) {
		return false;
	}

	if (!pos) {
		pos = &creature->getPosition();
	}

	SpectatorVec list;

	if (!listPtr || listPtr->empty()) {
		// This somewhat complex construct ensures that the cached SpectatorVec
		// is used if available and if it can be used, else a local vector is
		// used (hopefully the compiler will optimize away the construction of
		// the temporary when it's not used).
		if (type != TALKTYPE_YELL && type != TALKTYPE_MONSTER_YELL) {
			getSpectators(list, *pos, false, false,
			              Map::maxClientViewportX, Map::maxClientViewportX,
			              Map::maxClientViewportY, Map::maxClientViewportY);
		} else {
			getSpectators(list, *pos, true, false, 18, 18, 14, 14);
		}
	} else {
		list = (*listPtr);
	}

	//send to client
	for (Creature* spectator : list) {
		if (Player* tmpPlayer = spectator->getPlayer()) {
			if (!ghostMode || tmpPlayer->canSeeCreature(creature)) {
				tmpPlayer->sendCreatureSay(creature, type, text, pos);
			}
		}
	}

	//event method
	for (Creature* spectator : list) {
		spectator->onCreatureSay(creature, type, text);
	}
	return true;
}

void Game::checkCreatureWalk(uint32_t creatureId)
{
	Creature* creature = getCreatureByID(creatureId);
	if (creature && creature->getHealth() > 0) {
		creature->onWalk();
		cleanup();
	}
}

void Game::updateCreatureWalk(uint32_t creatureId)
{
	Creature* creature = getCreatureByID(creatureId);
	if (creature && creature->getHealth() > 0) {
		creature->goToFollowCreature();
	}
}

void Game::checkCreatureAttack(uint32_t creatureId)
{
	Creature* creature = getCreatureByID(creatureId);
	if (creature && creature->getHealth() > 0) {
		creature->onAttacking(0);
	}
}

void Game::addCreatureCheck(Creature* creature)
{
	creature->creatureCheck = true;

	if (creature->inCheckCreaturesVector) {
		// already in a vector
		return;
	}

	creature->inCheckCreaturesVector = true;
	checkCreatureLists[uniform_random(0, EVENT_CREATURECOUNT - 1)].push_back(creature);
	creature->useThing2();
}

void Game::removeCreatureCheck(Creature* creature)
{
	if (creature->inCheckCreaturesVector) {
		creature->creatureCheck = false;
	}
}

void Game::checkCreatures(size_t index)
{
	g_scheduler.addEvent(createSchedulerTask(EVENT_CHECK_CREATURE_INTERVAL, std::bind(&Game::checkCreatures, this, (index + 1) % EVENT_CREATURECOUNT)));
	auto& checkCreatureList = checkCreatureLists[index];
	for (auto it = checkCreatureList.begin(), end = checkCreatureList.end(); it != end;) {
		Creature* creature = *it;
		if (creature->creatureCheck) {
			if (creature->getHealth() > 0) {
				creature->onThink(EVENT_CREATURE_THINK_INTERVAL);
				creature->onAttacking(EVENT_CREATURE_THINK_INTERVAL);
				creature->executeConditions(EVENT_CREATURE_THINK_INTERVAL);
			} else {
				creature->onDeath();
			}
			++it;
		} else {
			creature->inCheckCreaturesVector = false;
			it = checkCreatureList.erase(it);
			ReleaseCreature(creature);
		}
	}

	cleanup();
}

void Game::changeSpeed(Creature* creature, int32_t varSpeedDelta)
{
	int32_t varSpeed = creature->getSpeed() - creature->getBaseSpeed();
	varSpeed += varSpeedDelta;

	creature->setSpeed(varSpeed);

	//send to clients
	SpectatorVec list;
	getSpectators(list, creature->getPosition(), false, true);
	for (Creature* spectator : list) {
		spectator->getPlayer()->sendChangeSpeed(creature, creature->getStepSpeed());
	}
}

void Game::internalCreatureChangeOutfit(Creature* creature, const Outfit_t& outfit)
{
	creature->setCurrentOutfit(outfit);

	if (creature->isInvisible()) {
		return;
	}

	//send to clients
	SpectatorVec list;
	getSpectators(list, creature->getPosition(), true, true);
	for (Creature* spectator : list) {
		spectator->getPlayer()->sendCreatureChangeOutfit(creature, outfit);
	}
}

void Game::internalCreatureChangeVisible(Creature* creature, bool visible)
{
	//send to clients
	SpectatorVec list;
	getSpectators(list, creature->getPosition(), true, true);
	for (Creature* spectator : list) {
		spectator->getPlayer()->sendCreatureChangeVisible(creature, visible);
	}
}

void Game::changeLight(const Creature* creature)
{
	//send to clients
	SpectatorVec list;
	getSpectators(list, creature->getPosition(), true, true);
	for (Creature* spectator : list) {
		spectator->getPlayer()->sendCreatureLight(creature);
	}
}

bool Game::combatBlockHit(CombatType_t combatType, Creature* attacker, Creature* target,
                          int32_t& healthChange, bool checkDefense, bool checkArmor, bool field)
{
	if (combatType == COMBAT_NONE) {
		return true;
	}

	if (target->getPlayer() && target->getPlayer()->isInGhostMode()) {
		return true;
	}

	if (healthChange > 0) {
		return false;
	}

	const Position& targetPos = target->getPosition();

	SpectatorVec list;
	getSpectators(list, targetPos, false, true);

	if (!target->isAttackable() || Combat::canDoCombat(attacker, target) != RET_NOERROR) {
		if (!target->isInGhostMode()) {
			addMagicEffect(list, targetPos, CONST_ME_POFF);
		}
		return true;
	}

	int32_t damage = -healthChange;
	BlockType_t blockType = target->blockHit(attacker, combatType, damage, checkDefense, checkArmor, field);
	healthChange = -damage;

	if (blockType == BLOCK_DEFENSE) {
		addMagicEffect(list, targetPos, CONST_ME_POFF);
		return true;
	} else if (blockType == BLOCK_ARMOR) {
		addMagicEffect(list, targetPos, CONST_ME_BLOCKHIT);
		return true;
	} else if (blockType == BLOCK_IMMUNITY) {
		uint8_t hitEffect = 0;

		switch (combatType) {
			case COMBAT_UNDEFINEDDAMAGE:
				break;

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

		addMagicEffect(list, targetPos, hitEffect);
		return true;
	}

	return false;
}

void Game::combatGetTypeInfo(CombatType_t combatType, Creature* target, TextColor_t& color, uint8_t& effect)
{
	switch (combatType) {
		case COMBAT_PHYSICALDAMAGE: {
			Item* splash = nullptr;
			switch (target->getRace()) {
				case RACE_VENOM:
					color = TEXTCOLOR_LIGHTGREEN;
					effect = CONST_ME_HITBYPOISON;
					splash = Item::CreateItem(ITEM_SMALLSPLASH, FLUID_GREEN);
					break;
				case RACE_BLOOD:
					color = TEXTCOLOR_RED;
					effect = CONST_ME_DRAWBLOOD;
					splash = Item::CreateItem(ITEM_SMALLSPLASH, FLUID_BLOOD);
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
					color = TEXTCOLOR_PURPLE;
					effect = CONST_ME_ENERGYHIT;
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
			color = TEXTCOLOR_PURPLE;
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

bool Game::combatChangeHealth(CombatType_t combatType, Creature* attacker, Creature* target, int32_t healthChange)
{
	const Position& targetPos = target->getPosition();

	if (healthChange > 0) {
		if (target->getHealth() <= 0) {
			return false;
		}

		Player* attackerPlayer;
		if (attacker) {
			attackerPlayer = attacker->getPlayer();
		} else {
			attackerPlayer = nullptr;
		}

		Player* targetPlayer = target->getPlayer();
		if (attackerPlayer && targetPlayer) {
			if (g_config.getBoolean(ConfigManager::CANNOT_ATTACK_SAME_LOOKFEET) && attackerPlayer->defaultOutfit.lookFeet == target->defaultOutfit.lookFeet && combatType != COMBAT_HEALING) {
				return false;
			}

			if (attackerPlayer->getSkull() == SKULL_BLACK && attackerPlayer->getSkullClient(targetPlayer) == SKULL_NONE) {
				return false;
			}
		}

		int32_t realHealthChange = target->getHealth();
		target->gainHealth(attacker, healthChange);
		realHealthChange = target->getHealth() - realHealthChange;

		if (realHealthChange > 0 && !target->isInGhostMode()) {
			std::string pluralString = (realHealthChange != 1 ? "s." : ".");
			std::ostringstream ss;
			if (!attacker) {
				ss << ucfirst(target->getNameDescription()) << " was healed for " << realHealthChange << " hitpoint" << pluralString;
			} else if (attacker == target) {
				ss << ucfirst(attacker->getNameDescription()) << " healed " << (targetPlayer ? (targetPlayer->getSex() == PLAYERSEX_FEMALE ? "herself for " : "himself for ") : "itself for ") << realHealthChange << " hitpoint" << pluralString;
			} else {
				ss << ucfirst(attacker->getNameDescription()) << " healed " << target->getNameDescription() << " for " << realHealthChange << " hitpoint" << pluralString;
			}

			std::string spectatorMessage = ss.str();

			TextMessage message;
			message.position = targetPos;
			message.primary.value = realHealthChange;
			message.primary.color = TEXTCOLOR_MAYABLUE;

			SpectatorVec list;
			getSpectators(list, targetPos, false, true);
			for (Creature* spectator : list) {
				Player* tmpPlayer = spectator->getPlayer();
				if (tmpPlayer == attackerPlayer && attackerPlayer != targetPlayer) {
					ss.str("");
					ss << "You heal " << target->getNameDescription() << " for " << realHealthChange << " hitpoint" << pluralString;
					message.type = MESSAGE_HEALED;
					message.text = ss.str();
				} else if (tmpPlayer == targetPlayer) {
					ss.str("");
					if (!attacker) {
						ss << "You were healed for " << realHealthChange << " hitpoint" << pluralString;
					} else if (targetPlayer == attackerPlayer) {
						ss << "You heal yourself for " << realHealthChange << " hitpoint" << pluralString;
					} else {
						ss << "You were healed by " << attacker->getNameDescription() << " for " << realHealthChange << " hitpoint" << pluralString;
					}
					message.type = MESSAGE_HEALED;
					message.text = ss.str();
				} else {
					message.type = MESSAGE_HEALED_OTHERS;
					message.text = spectatorMessage;
				}
				tmpPlayer->sendTextMessage(message);
			}
		}
	} else {
		SpectatorVec list;
		getSpectators(list, targetPos, true, true);

		if (!target->isAttackable() || Combat::canDoCombat(attacker, target) != RET_NOERROR) {
			addMagicEffect(list, targetPos, CONST_ME_POFF);
			return true;
		}

		Player* attackerPlayer;
		if (attacker) {
			attackerPlayer = attacker->getPlayer();
		} else {
			attackerPlayer = nullptr;
		}

		Player* targetPlayer = target->getPlayer();
		if (attackerPlayer && targetPlayer) {
			if (g_config.getBoolean(ConfigManager::CANNOT_ATTACK_SAME_LOOKFEET) && attacker->defaultOutfit.lookFeet == target->defaultOutfit.lookFeet && combatType != COMBAT_HEALING) {
				return false;
			}

			if (attackerPlayer->getSkull() == SKULL_BLACK && attackerPlayer->getSkullClient(targetPlayer) == SKULL_NONE) {
				return false;
			}
		}

		int32_t damage = -healthChange;
		if (damage == 0) {
			return true;
		}

		TextMessage message;
		message.position = targetPos;

		if (target->hasCondition(CONDITION_MANASHIELD) && combatType != COMBAT_UNDEFINEDDAMAGE) {
			int32_t manaDamage = std::min<int32_t>(target->getMana(), damage);
			if (manaDamage != 0) {
				for (CreatureEvent* creatureEvent : target->getCreatureEvents(CREATURE_EVENT_CHANGEMANA)) {
					creatureEvent->executeChangeMana(target, attacker, manaDamage);
				}

				target->drainMana(attacker, manaDamage);
				addMagicEffect(list, targetPos, CONST_ME_LOSEENERGY);

				std::ostringstream ss;
				if (!attacker) {
					ss << ucfirst(target->getNameDescription()) << " loses " << manaDamage << " mana.";
				} else if (attacker == target) {
					ss << ucfirst(target->getNameDescription()) << " loses " << manaDamage << " mana blocking an attack by " << (targetPlayer ? (targetPlayer->getSex() == PLAYERSEX_FEMALE ? "herself." : "himself.") : "itself.");
				} else {
					ss << ucfirst(target->getNameDescription()) << " loses " << manaDamage << " mana blocking an attack by " << attacker->getNameDescription() << '.';
				}

				std::string spectatorMessage = ss.str();

				message.primary.value = manaDamage;
				message.primary.color = TEXTCOLOR_BLUE;

				for (Creature* spectator : list) {
					Player* tmpPlayer = spectator->getPlayer();
					if (tmpPlayer->getPosition().z == targetPos.z) {
						if (tmpPlayer == attackerPlayer && attackerPlayer != targetPlayer) {
							ss.str("");
							ss << ucfirst(target->getNameDescription()) << " loses " << manaDamage << " mana blocking your attack.";
							message.type = MESSAGE_DAMAGE_DEALT;
							message.text = ss.str();
						} else if (tmpPlayer == targetPlayer) {
							ss.str("");
							if (!attacker) {
								ss << "You lose " << manaDamage << " mana.";
							} else if (targetPlayer == attackerPlayer) {
								ss << "You lose " << manaDamage << " mana blocking an attack by yourself.";
							} else {
								ss << "You lose " << manaDamage << " mana blocking an attack by " << attacker->getNameDescription() << '.';
							}
							message.type = MESSAGE_DAMAGE_RECEIVED;
							message.text = ss.str();
						} else {
							message.type = MESSAGE_DAMAGE_OTHERS;
							message.text = spectatorMessage;
						}
						tmpPlayer->sendTextMessage(message);
					}
				}

				damage = std::max<int32_t>(0, damage - manaDamage);
			}
		}

		int32_t targetHealth = target->getHealth();
		damage = std::min<int32_t>(targetHealth, damage);
		if (damage > 0) {
			const auto& changeHealthEvents = target->getCreatureEvents(CREATURE_EVENT_CHANGEHEALTH);
			if (!changeHealthEvents.empty()) {
				CombatDamage tmpDamage;
				tmpDamage.primary.type = combatType;
				tmpDamage.primary.value = damage;

				for (CreatureEvent* creatureEvent : changeHealthEvents) {
					creatureEvent->executeChangeHealth(target, attacker, tmpDamage);
				}
			}

			if (damage >= targetHealth) {
				for (CreatureEvent* creatureEvent : target->getCreatureEvents(CREATURE_EVENT_PREPAREDEATH)) {
					if (!creatureEvent->executeOnPrepareDeath(target, attacker)) {
						return false;
					}
				}
			}

			target->drainHealth(attacker, damage);
			addCreatureHealth(list, target);

			uint8_t hitEffect;
			combatGetTypeInfo(combatType, target, message.primary.color, hitEffect);

			if (message.primary.color != TEXTCOLOR_NONE) {
				message.primary.value = damage;
				addMagicEffect(list, targetPos, hitEffect);

				std::string pluralString = (damage != 1 ? "s" : "");
				std::ostringstream ss;
				if (!attacker) {
					ss << ucfirst(target->getNameDescription()) << " loses " << damage << " hitpoint" << pluralString << ".";
				} else if (attacker == target) {
					ss << ucfirst(target->getNameDescription()) << " loses " << damage << " hitpoint" << pluralString << " due to " << (targetPlayer ? (targetPlayer->getSex() == PLAYERSEX_FEMALE ? "her" : "his") : "its") << " own attack.";
				} else {
					ss << ucfirst(target->getNameDescription()) << " loses " << damage << " hitpoint" << pluralString << " due to an attack by " << attacker->getNameDescription() << '.';
				}

				std::string spectatorMessage = ss.str();
				for (Creature* spectator : list) {
					Player* tmpPlayer = spectator->getPlayer();
					if (tmpPlayer->getPosition().z == targetPos.z) {
						if (tmpPlayer == attackerPlayer && attackerPlayer != targetPlayer) {
							ss.str("");
							ss << ucfirst(target->getNameDescription()) << " loses " << damage << " hitpoint" << pluralString << " due to your attack.";
							message.type = MESSAGE_DAMAGE_DEALT;
							message.text = ss.str();
						} else if (tmpPlayer == targetPlayer) {
							ss.str("");
							if (!attacker) {
								ss << "You lose " << damage << " hitpoint" << pluralString << ".";
							} else if (targetPlayer == attackerPlayer) {
								ss << "You lose " << damage << " hitpoint" << pluralString << " due to your own attack.";
							} else {
								ss << "You lose " << damage << " hitpoint" << pluralString << " due to an attack by " << attacker->getNameDescription() << '.';
							}
							message.type = MESSAGE_DAMAGE_RECEIVED;
							message.text = ss.str();
						} else {
							message.type = MESSAGE_DAMAGE_OTHERS;
							message.text = spectatorMessage;
						}
						tmpPlayer->sendTextMessage(message);
					}
				}
			}
		}
	}

	return true;
}

bool Game::combatChangeHealth(Creature* attacker, Creature* target, CombatDamage& damage)
{
	const Position& targetPos = target->getPosition();
	int32_t healthChange = damage.primary.value + damage.secondary.value;
	if (healthChange > 0) {
		if (target->getHealth() <= 0) {
			return false;
		}

		Player* attackerPlayer;
		if (attacker) {
			attackerPlayer = attacker->getPlayer();
		} else {
			attackerPlayer = nullptr;
		}

		Player* targetPlayer = target->getPlayer();
		if (attackerPlayer && targetPlayer) {
			if (g_config.getBoolean(ConfigManager::CANNOT_ATTACK_SAME_LOOKFEET) && attackerPlayer->defaultOutfit.lookFeet == target->defaultOutfit.lookFeet && damage.primary.type != COMBAT_HEALING) {
				return false;
			}

			if (attackerPlayer->getSkull() == SKULL_BLACK && attackerPlayer->getSkullClient(targetPlayer) == SKULL_NONE) {
				return false;
			}
		}

		int32_t realHealthChange = target->getHealth();
		target->gainHealth(attacker, healthChange);
		realHealthChange = target->getHealth() - realHealthChange;

		if (realHealthChange > 0 && !target->isInGhostMode()) {
			std::string pluralString = (realHealthChange != 1 ? "s." : ".");
			std::ostringstream ss;
			if (!attacker) {
				ss << ucfirst(target->getNameDescription()) << " was healed for " << realHealthChange << " hitpoint" << pluralString;
			} else if (attacker == target) {
				ss << ucfirst(attacker->getNameDescription()) << " healed " << (targetPlayer ? (targetPlayer->getSex() == PLAYERSEX_FEMALE ? "herself for " : "himself for ") : "itself for ") << realHealthChange << " hitpoint" << pluralString;
			} else {
				ss << ucfirst(attacker->getNameDescription()) << " healed " << target->getNameDescription() << " for " << realHealthChange << " hitpoint" << pluralString;
			}

			std::string spectatorMessage = ss.str();

			TextMessage message;
			message.position = targetPos;
			message.primary.value = realHealthChange;
			message.primary.color = TEXTCOLOR_MAYABLUE;

			SpectatorVec list;
			getSpectators(list, targetPos, false, true);
			for (Creature* spectator : list) {
				Player* tmpPlayer = spectator->getPlayer();
				if (tmpPlayer == attackerPlayer && attackerPlayer != targetPlayer) {
					ss.str("");
					ss << "You heal " << target->getNameDescription() << " for " << realHealthChange << " hitpoint" << pluralString;
					message.type = MESSAGE_HEALED;
					message.text = ss.str();
				} else if (tmpPlayer == targetPlayer) {
					ss.str("");
					if (!attacker) {
						ss << "You were healed for " << realHealthChange << " hitpoint" << pluralString;
					} else if (targetPlayer == attackerPlayer) {
						ss << "You heal yourself for " << realHealthChange << " hitpoint" << pluralString;
					} else {
						ss << "You were healed by " << attacker->getNameDescription() << " for " << realHealthChange << " hitpoint" << pluralString;
					}
					message.type = MESSAGE_HEALED;
					message.text = ss.str();
				} else {
					message.type = MESSAGE_HEALED_OTHERS;
					message.text = spectatorMessage;
				}
				tmpPlayer->sendTextMessage(message);
			}
		}
	} else {
		SpectatorVec list;
		getSpectators(list, targetPos, true, true);

		if (!target->isAttackable() || Combat::canDoCombat(attacker, target) != RET_NOERROR) {
			addMagicEffect(list, targetPos, CONST_ME_POFF);
			return true;
		}

		Player* attackerPlayer;
		if (attacker) {
			attackerPlayer = attacker->getPlayer();
		} else {
			attackerPlayer = nullptr;
		}

		Player* targetPlayer = target->getPlayer();
		if (attackerPlayer && targetPlayer) {
			if (g_config.getBoolean(ConfigManager::CANNOT_ATTACK_SAME_LOOKFEET) && attacker->defaultOutfit.lookFeet == target->defaultOutfit.lookFeet && damage.primary.type != COMBAT_HEALING) {
				return false;
			}

			if (attackerPlayer->getSkull() == SKULL_BLACK && attackerPlayer->getSkullClient(targetPlayer) == SKULL_NONE) {
				return false;
			}
		}

		damage.primary.value = std::abs(damage.primary.value);
		damage.secondary.value = std::abs(damage.secondary.value);

		TextMessage message;
		message.position = targetPos;

		if (target->hasCondition(CONDITION_MANASHIELD) && damage.primary.type != COMBAT_UNDEFINEDDAMAGE) {
			int32_t manaDamage = std::min<int32_t>(target->getMana(), -healthChange);
			if (manaDamage != 0) {
				for (CreatureEvent* creatureEvent : target->getCreatureEvents(CREATURE_EVENT_CHANGEMANA)) {
					creatureEvent->executeChangeMana(target, attacker, manaDamage);
				}

				target->drainMana(attacker, manaDamage);
				addMagicEffect(list, targetPos, CONST_ME_LOSEENERGY);

				std::ostringstream ss;
				if (!attacker) {
					ss << ucfirst(target->getNameDescription()) << " loses " << manaDamage << " mana.";
				} else if (attacker == target) {
					ss << ucfirst(target->getNameDescription()) << " loses " << manaDamage << " mana blocking an attack by " << (targetPlayer ? (targetPlayer->getSex() == PLAYERSEX_FEMALE ? "herself." : "himself.") : "itself.");
				} else {
					ss << ucfirst(target->getNameDescription()) << " loses " << manaDamage << " mana blocking an attack by " << attacker->getNameDescription() << '.';
				}

				std::string spectatorMessage = ss.str();

				message.primary.value = manaDamage;
				message.primary.color = TEXTCOLOR_BLUE;

				for (Creature* spectator : list) {
					Player* tmpPlayer = spectator->getPlayer();
					if (tmpPlayer->getPosition().z == targetPos.z) {
						if (tmpPlayer == attackerPlayer && attackerPlayer != targetPlayer) {
							ss.str("");
							ss << ucfirst(target->getNameDescription()) << " loses " << manaDamage << " mana blocking your attack.";
							message.type = MESSAGE_DAMAGE_DEALT;
							message.text = ss.str();
						} else if (tmpPlayer == targetPlayer) {
							ss.str("");
							if (!attacker) {
								ss << "You lose " << manaDamage << " mana.";
							} else if (targetPlayer == attackerPlayer) {
								ss << "You lose " << manaDamage << " mana blocking an attack by yourself.";
							} else {
								ss << "You lose " << manaDamage << " mana blocking an attack by " << attacker->getNameDescription() << '.';
							}
							message.type = MESSAGE_DAMAGE_RECEIVED;
							message.text = ss.str();
						} else {
							message.type = MESSAGE_DAMAGE_OTHERS;
							message.text = spectatorMessage;
						}
						tmpPlayer->sendTextMessage(message);
					}
				}

				damage.primary.value -= manaDamage;
				if (damage.primary.value < 0) {
					damage.secondary.value = std::max<int32_t>(0, damage.secondary.value + damage.primary.value);
					damage.primary.value = 0;
				}
			}
		}

		int32_t targetHealth = target->getHealth();
		if (damage.primary.value >= targetHealth) {
			damage.primary.value = targetHealth;
			damage.secondary.value = 0;
		} else if (damage.secondary.value) {
			damage.secondary.value = std::min<int32_t>(damage.secondary.value, targetHealth - damage.primary.value);
		}

		int32_t realDamage = damage.primary.value + damage.secondary.value;
		if (realDamage) {
			for (CreatureEvent* creatureEvent : target->getCreatureEvents(CREATURE_EVENT_CHANGEHEALTH)) {
				creatureEvent->executeChangeHealth(target, attacker, damage);
			}

			if (realDamage >= targetHealth) {
				for (CreatureEvent* creatureEvent : target->getCreatureEvents(CREATURE_EVENT_PREPAREDEATH)) {
					if (!creatureEvent->executeOnPrepareDeath(target, attacker)) {
						return false;
					}
				}
			}

			target->drainHealth(attacker, realDamage);
			addCreatureHealth(list, target);

			message.primary.value = damage.primary.value;
			message.secondary.value = damage.secondary.value;

			uint8_t hitEffect;
			if (message.primary.value) {
				combatGetTypeInfo(damage.primary.type, target, message.primary.color, hitEffect);
				if (hitEffect != CONST_ME_NONE) {
					addMagicEffect(list, targetPos, hitEffect);
				}
			}

			if (message.secondary.value) {
				combatGetTypeInfo(damage.secondary.type, target, message.secondary.color, hitEffect);
				if (hitEffect != CONST_ME_NONE) {
					addMagicEffect(list, targetPos, hitEffect);
				}
			}

			if (message.primary.color != TEXTCOLOR_NONE || message.secondary.color != TEXTCOLOR_NONE) {
				std::string pluralString = (realDamage != 1 ? "s" : "");
				std::ostringstream ss;
				if (!attacker) {
					ss << ucfirst(target->getNameDescription()) << " loses " << realDamage << " hitpoint" << pluralString << ".";
				} else if (attacker == target) {
					ss << ucfirst(target->getNameDescription()) << " loses " << realDamage << " hitpoint" << pluralString << " due to " << (targetPlayer ? (targetPlayer->getSex() == PLAYERSEX_FEMALE ? "her" : "his") : "its") << " own attack.";
				} else {
					ss << ucfirst(target->getNameDescription()) << " loses " << realDamage << " hitpoint" << pluralString << " due to an attack by " << attacker->getNameDescription() << '.';
				}

				std::string spectatorMessage = ss.str();
				for (Creature* spectator : list) {
					Player* tmpPlayer = spectator->getPlayer();
					if (tmpPlayer->getPosition().z == targetPos.z) {
						if (tmpPlayer == attackerPlayer && attackerPlayer != targetPlayer) {
							ss.str("");
							ss << ucfirst(target->getNameDescription()) << " loses " << realDamage << " hitpoint" << pluralString << " due to your attack.";
							message.type = MESSAGE_DAMAGE_DEALT;
							message.text = ss.str();
						} else if (tmpPlayer == targetPlayer) {
							ss.str("");
							if (!attacker) {
								ss << "You lose " << realDamage << " hitpoint" << pluralString << ".";
							} else if (targetPlayer == attackerPlayer) {
								ss << "You lose " << realDamage << " hitpoint" << pluralString << " due to your own attack.";
							} else {
								ss << "You lose " << realDamage << " hitpoint" << pluralString << " due to an attack by " << attacker->getNameDescription() << '.';
							}
							message.type = MESSAGE_DAMAGE_RECEIVED;
							message.text = ss.str();
						} else {
							message.type = MESSAGE_DAMAGE_OTHERS;
							message.text = spectatorMessage;
						}
						tmpPlayer->sendTextMessage(message);
					}
				}
			}
		}
	}

	return true;
}

bool Game::combatChangeMana(Creature* attacker, Creature* target, int32_t manaChange)
{
	if (manaChange > 0) {
		if (attacker) {
			Player* attackerPlayer = attacker->getPlayer();
			Player* targetPlayer = target->getPlayer();

			if (attackerPlayer && targetPlayer) {
				if (g_config.getBoolean(ConfigManager::CANNOT_ATTACK_SAME_LOOKFEET) && attacker->defaultOutfit.lookFeet == target->defaultOutfit.lookFeet) {
					return false;
				}

				if (attackerPlayer->getSkull() == SKULL_BLACK && attackerPlayer->getSkullClient(targetPlayer) == SKULL_NONE) {
					return false;
				}
			}
		}

		for (CreatureEvent* creatureEvent : target->getCreatureEvents(CREATURE_EVENT_CHANGEMANA)) {
			creatureEvent->executeChangeMana(target, attacker, manaChange);
		}

		target->changeMana(manaChange);
	} else {
		const Position& targetPos = target->getPosition();
		if (!target->isAttackable() || Combat::canDoCombat(attacker, target) != RET_NOERROR) {
			addMagicEffect(targetPos, CONST_ME_POFF);
			return false;
		}

		Player* attackerPlayer;
		if (attacker) {
			attackerPlayer = attacker->getPlayer();
		} else {
			attackerPlayer = nullptr;
		}

		Player* targetPlayer = target->getPlayer();
		if (attackerPlayer && targetPlayer) {
			if (g_config.getBoolean(ConfigManager::CANNOT_ATTACK_SAME_LOOKFEET) && attacker->defaultOutfit.lookFeet == target->defaultOutfit.lookFeet) {
				return false;
			}

			if (attackerPlayer->getSkull() == SKULL_BLACK && attackerPlayer->getSkullClient(targetPlayer) == SKULL_NONE) {
				return false;
			}
		}

		int32_t manaLoss = std::min<int32_t>(target->getMana(), -manaChange);
		BlockType_t blockType = target->blockHit(attacker, COMBAT_MANADRAIN, manaLoss);
		if (blockType != BLOCK_NONE) {
			addMagicEffect(targetPos, CONST_ME_POFF);
			return false;
		}

		if (manaLoss <= 0) {
			return true;
		}

		for (CreatureEvent* creatureEvent : target->getCreatureEvents(CREATURE_EVENT_CHANGEMANA)) {
			creatureEvent->executeChangeMana(target, attacker, manaLoss);
		}

		target->drainMana(attacker, manaLoss);

		std::ostringstream ss;
		if (!attacker) {
			ss << ucfirst(target->getNameDescription()) << " loses " << manaLoss << " mana.";
		} else if (attacker == target) {
			ss << ucfirst(target->getNameDescription()) << " loses " << manaLoss << " mana blocking an attack by " << (targetPlayer ? (targetPlayer->getSex() == PLAYERSEX_FEMALE ? "herself." : "himself.") : "itself.");
		} else {
			ss << ucfirst(target->getNameDescription()) << " loses " << manaLoss << " mana blocking an attack by " << attacker->getNameDescription() << '.';
		}

		std::string spectatorMessage = ss.str();

		TextMessage message;
		message.position = targetPos;
		message.primary.value = manaLoss;
		message.primary.color = TEXTCOLOR_BLUE;

		SpectatorVec list;
		getSpectators(list, targetPos, false, true);
		for (Creature* spectator : list) {
			Player* tmpPlayer = spectator->getPlayer();
			if (tmpPlayer == attackerPlayer && attackerPlayer != targetPlayer) {
				ss.str("");
				ss << ucfirst(target->getNameDescription()) << " loses " << manaLoss << " mana blocking your attack.";
				message.type = MESSAGE_DAMAGE_DEALT;
				message.text = ss.str();
			} else if (tmpPlayer == targetPlayer) {
				ss.str("");
				if (!attacker) {
					ss << "You lose " << manaLoss << " mana.";
				} else if (targetPlayer == attackerPlayer) {
					ss << "You lose " << manaLoss << " mana blocking an attack by yourself.";
				} else {
					ss << "You lose " << manaLoss << " mana blocking an attack by " << attacker->getNameDescription() << '.';
				}
				message.type = MESSAGE_DAMAGE_RECEIVED;
				message.text = ss.str();
			} else {
				message.type = MESSAGE_DAMAGE_OTHERS;
				message.text = spectatorMessage;
			}
			tmpPlayer->sendTextMessage(message);
		}
	}

	return true;
}

void Game::addCreatureHealth(const Creature* target)
{
	SpectatorVec list;
	getSpectators(list, target->getPosition(), true, true);
	addCreatureHealth(list, target);
}

void Game::addCreatureHealth(const SpectatorVec& list, const Creature* target)
{
	for (Creature* spectator : list) {
		if (Player* tmpPlayer = spectator->getPlayer()) {
			tmpPlayer->sendCreatureHealth(target);
		}
	}
}

void Game::addMagicEffect(const Position& pos, uint8_t effect)
{
	SpectatorVec list;
	getSpectators(list, pos, true, true);
	addMagicEffect(list, pos, effect);
}

void Game::addMagicEffect(const SpectatorVec& list, const Position& pos, uint8_t effect)
{
	for (Creature* spectator : list) {
		if (Player* tmpPlayer = spectator->getPlayer()) {
			tmpPlayer->sendMagicEffect(pos, effect);
		}
	}
}

void Game::addDistanceEffect(const Position& fromPos, const Position& toPos, uint8_t effect)
{
	SpectatorVec list;
	getSpectators(list, fromPos, false, true);
	getSpectators(list, toPos, false, true);
	addDistanceEffect(list, fromPos, toPos, effect);
}

void Game::addDistanceEffect(const SpectatorVec& list, const Position& fromPos, const Position& toPos, uint8_t effect)
{
	for (Creature* spectator : list) {
		if (Player* tmpPlayer = spectator->getPlayer()) {
			tmpPlayer->sendDistanceShoot(fromPos, toPos, effect);
		}
	}
}

void Game::startDecay(Item* item)
{
	if (!item || !item->canDecay()) {
		return;
	}

	ItemDecayState_t decayState = item->getDecaying();
	if (decayState == DECAYING_TRUE) {
		return;
	}

	if (item->getDuration() > 0) {
		item->useThing2();
		item->setDecaying(DECAYING_TRUE);
		toDecayItems.push_front(item);
	} else {
		internalDecayItem(item);
	}
}

void Game::internalDecayItem(Item* item)
{
	const ItemType& it = Item::items[item->getID()];

	if (it.decayTo != 0) {
		Item* newItem = transformItem(item, it.decayTo);
		startDecay(newItem);
	} else {
		ReturnValue ret = internalRemoveItem(item);
		if (ret != RET_NOERROR) {
			std::cout << "DEBUG, internalDecayItem failed, error code: " << (int32_t) ret << "item id: " << item->getID() << std::endl;
		}
	}
}

void Game::checkDecay()
{
	g_scheduler.addEvent(createSchedulerTask(EVENT_DECAYINTERVAL, std::bind(&Game::checkDecay, this)));

	size_t bucket = (lastBucket + 1) % EVENT_DECAY_BUCKETS;

	for (auto it = decayItems[bucket].begin(); it != decayItems[bucket].end();) {
		Item* item = *it;
		if (!item->canDecay()) {
			item->setDecaying(DECAYING_FALSE);
			ReleaseItem(item);
			it = decayItems[bucket].erase(it);
			continue;
		}

		int32_t decreaseTime = EVENT_DECAYINTERVAL * EVENT_DECAY_BUCKETS;
		int32_t duration = item->getDuration();
		if (duration - decreaseTime < 0) {
			decreaseTime = duration;
		}

		duration -= decreaseTime;
		item->decreaseDuration(decreaseTime);

		if (duration <= 0) {
			it = decayItems[bucket].erase(it);
			internalDecayItem(item);
			ReleaseItem(item);
		} else if (duration < EVENT_DECAYINTERVAL * EVENT_DECAY_BUCKETS) {
			it = decayItems[bucket].erase(it);
			size_t newBucket = (bucket + ((duration + EVENT_DECAYINTERVAL / 2) / 1000)) % EVENT_DECAY_BUCKETS;
			if (newBucket == bucket) {
				internalDecayItem(item);
				ReleaseItem(item);
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

void Game::checkLight()
{
	g_scheduler.addEvent(createSchedulerTask(EVENT_LIGHTINTERVAL, std::bind(&Game::checkLight, this)));

	lightHour += lightHourDelta;

	if (lightHour > 1440) {
		lightHour -= 1440;
	}

	if (std::abs(lightHour - SUNRISE) < 2 * lightHourDelta) {
		lightState = LIGHT_STATE_SUNRISE;
	} else if (std::abs(lightHour - SUNSET) < 2 * lightHourDelta) {
		lightState = LIGHT_STATE_SUNSET;
	}

	int32_t newLightLevel = lightLevel;
	bool lightChange = false;

	switch (lightState) {
		case LIGHT_STATE_SUNRISE: {
			newLightLevel += (LIGHT_LEVEL_DAY - LIGHT_LEVEL_NIGHT) / 30;
			lightChange = true;
			break;
		}
		case LIGHT_STATE_SUNSET: {
			newLightLevel -= (LIGHT_LEVEL_DAY - LIGHT_LEVEL_NIGHT) / 30;
			lightChange = true;
			break;
		}
		default:
			break;
	}

	if (newLightLevel <= LIGHT_LEVEL_NIGHT) {
		lightLevel = LIGHT_LEVEL_NIGHT;
		lightState = LIGHT_STATE_NIGHT;
	} else if (newLightLevel >= LIGHT_LEVEL_DAY) {
		lightLevel = LIGHT_LEVEL_DAY;
		lightState = LIGHT_STATE_DAY;
	} else {
		lightLevel = newLightLevel;
	}

	if (lightChange) {
		LightInfo lightInfo;
		getWorldLightInfo(lightInfo);

		for (const auto& it : players) {
			it.second->sendWorldLight(lightInfo);
		}
	}
}

void Game::getWorldLightInfo(LightInfo& lightInfo) const
{
	lightInfo.level = lightLevel;
	lightInfo.color = 0xD7;
}

void Game::addCommandTag(char tag)
{
	for (char commandTag : commandTags) {
		if (commandTag == tag) {
			return;
		}
	}
	commandTags.push_back(tag);
}

void Game::resetCommandTag()
{
	commandTags.clear();
}

void Game::shutdown()
{
	std::cout << "Shutting down server..." << std::flush;

	g_scheduler.shutdown();
	g_dispatcher.shutdown();
	Spawns::getInstance()->clear();
	Raids::getInstance()->clear();

	cleanup();

	if (services) {
		services->stop();
	}

	ConnectionManager::getInstance()->closeAll();

	std::cout << " done!" << std::endl;
}

void Game::cleanup()
{
	//free memory
	for (auto creature : ToReleaseCreatures) {
		creature->releaseThing2();
	}
	ToReleaseCreatures.clear();

	for (auto item : ToReleaseItems) {
		item->releaseThing2();
	}
	ToReleaseItems.clear();

	for (Item* item : toDecayItems) {
		const uint32_t dur = item->getDuration();
		if (dur >= EVENT_DECAYINTERVAL * EVENT_DECAY_BUCKETS) {
			decayItems[lastBucket].push_back(item);
		} else {
			decayItems[(lastBucket + 1 + dur / 1000) % EVENT_DECAY_BUCKETS].push_back(item);
		}
	}
	toDecayItems.clear();
}

void Game::ReleaseCreature(Creature* creature)
{
	ToReleaseCreatures.push_back(creature);
}

void Game::ReleaseItem(Item* item)
{
	ToReleaseItems.push_back(item);
}

void Game::broadcastMessage(const std::string& text, MessageClasses type) const
{
	std::cout << "> Broadcasted message: \"" << text << "\"." << std::endl;
	for (const auto& it : players) {
		it.second->sendTextMessage(type, text);
	}
}

void Game::updateCreatureWalkthrough(const Creature* creature)
{
	//send to clients
	SpectatorVec list;
	getSpectators(list, creature->getPosition(), true, true);
	for (Creature* spectator : list) {
		Player* tmpPlayer = spectator->getPlayer();
		tmpPlayer->sendCreatureWalkthrough(creature, tmpPlayer->canWalkthroughEx(creature));
	}
}

void Game::updatePlayerSkull(Player* player)
{
	if (getWorldType() != WORLD_TYPE_PVP) {
		return;
	}

	SpectatorVec list;
	getSpectators(list, player->getPosition(), true, true);
	for (Creature* spectator : list) {
		spectator->getPlayer()->sendCreatureSkull(player);
	}
}

void Game::updatePlayerShield(Player* player)
{
	SpectatorVec list;
	getSpectators(list, player->getPosition(), true, true);
	for (Creature* spectator : list) {
		spectator->getPlayer()->sendCreatureShield(player);
	}
}

void Game::updatePlayerHelpers(const Player& player)
{
	uint32_t creatureId = player.getID();
	uint16_t helpers = player.getHelpers();

	SpectatorVec list;
	getSpectators(list, player.getPosition(), true, true);
	for (Creature* spectator : list) {
		spectator->getPlayer()->sendCreatureHelpers(creatureId, helpers);
	}
}

void Game::updateCreatureType(Creature* creature)
{
	const Player* masterPlayer = nullptr;

	uint32_t creatureId = creature->getID();
	CreatureType_t creatureType = creature->getType();
	if (creatureType == CREATURETYPE_MONSTER) {
		const Creature* master = creature->getMaster();
		if (master) {
			masterPlayer = master->getPlayer();
			if (masterPlayer) {
				creatureType = CREATURETYPE_SUMMON_OTHERS;
			}
		}
	}

	//send to clients
	SpectatorVec list;
	getSpectators(list, creature->getPosition(), true, true);

	if (creatureType == CREATURETYPE_SUMMON_OTHERS) {
		for (Creature* spectator : list) {
			Player* player = spectator->getPlayer();
			if (masterPlayer == player) {
				player->sendCreatureType(creatureId, CREATURETYPE_SUMMON_OWN);
			} else {
				player->sendCreatureType(creatureId, creatureType);
			}
		}
	} else {
		for (Creature* spectator : list) {
			spectator->getPlayer()->sendCreatureType(creatureId, creatureType);
		}
	}
}

void Game::updatePremium(Account& account)
{
	bool save = false;
	time_t timeNow = time(nullptr);

	if (account.premiumDays != 0 && account.premiumDays != std::numeric_limits<uint16_t>::max()) {
		if (account.lastDay == 0) {
			account.lastDay = timeNow;
			save = true;
		} else {
			uint32_t days = (timeNow - account.lastDay) / 86400;
			if (days > 0) {
				if (days >= account.premiumDays) {
					account.premiumDays = 0;
					account.lastDay = 0;
				} else {
					account.premiumDays -= days;
					uint32_t remainder = (timeNow - account.lastDay) % 86400;
					account.lastDay = timeNow - remainder;
				}

				save = true;
			}
		}
	} else if (account.lastDay != 0) {
		account.lastDay = 0;
		save = true;
	}

	if (save && !IOLoginData::saveAccount(account)) {
		std::cout << "> ERROR: Failed to save account: " << account.name << "!" << std::endl;
	}
}

void Game::loadMotdNum()
{
	Database* db = Database::getInstance();

	DBResult_ptr result = db->storeQuery("SELECT `value` FROM `server_config` WHERE `config` = 'motd_num'");
	if (result) {
		motdNum = result->getDataInt("value");
	} else {
		db->executeQuery("INSERT INTO `server_config` (`config`, `value`) VALUES ('motd_num', '0')");
	}

	result = db->storeQuery("SELECT `value` FROM `server_config` WHERE `config` = 'motd_hash'");
	if (result) {
		motdHash = result->getDataString("value");
		if (motdHash != transformToSHA1(g_config.getString(ConfigManager::MOTD))) {
			++motdNum;
		}
	} else {
		db->executeQuery("INSERT INTO `server_config` (`config`, `value`) VALUES ('motd_hash', '')");
	}
}

void Game::saveMotdNum() const
{
	Database* db = Database::getInstance();

	std::ostringstream query;
	query << "UPDATE `server_config` SET `value` = '" << motdNum << "' WHERE `config` = 'motd_num'";
	db->executeQuery(query.str());

	query.str("");
	query << "UPDATE `server_config` SET `value` = '" << transformToSHA1(g_config.getString(ConfigManager::MOTD)) << "' WHERE `config` = 'motd_hash'";
	db->executeQuery(query.str());
}

void Game::checkPlayersRecord()
{
	const size_t playersOnline = getPlayersOnline();
	if (playersOnline > playersRecord) {
		uint32_t previousRecord = playersRecord;
		playersRecord = playersOnline;

		for (const auto& it : g_globalEvents->getEventMap(GLOBALEVENT_RECORD)) {
			it.second->executeRecord(playersRecord, previousRecord);
		}
		updatePlayersRecord();
	}
}

void Game::updatePlayersRecord() const
{
	Database* db = Database::getInstance();

	std::ostringstream query;
	query << "UPDATE `server_config` SET `value` = '" << playersRecord << "' WHERE `config` = 'players_record'";
	db->executeQuery(query.str());
}

void Game::loadPlayersRecord()
{
	Database* db = Database::getInstance();

	DBResult_ptr result = db->storeQuery("SELECT `value` FROM `server_config` WHERE `config` = 'players_record'");
	if (result) {
		playersRecord = result->getDataInt("value");
	} else {
		db->executeQuery("INSERT INTO `server_config` (`config`, `value`) VALUES ('players_record', '0')");
	}
}

uint64_t Game::getExperienceStage(uint32_t level)
{
	if (!stagesEnabled) {
		return g_config.getNumber(ConfigManager::RATE_EXPERIENCE);
	}

	if (useLastStageLevel && level >= lastStageLevel) {
		return stages[lastStageLevel];
	}

	return stages[level];
}

bool Game::loadExperienceStages()
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file("data/XML/stages.xml");
	if (!result) {
		std::cout << "[Error - Game::loadExperienceStages] Failed to load data/XML/stages.xml: " << result.description() << std::endl;
		return false;
	}

	for (pugi::xml_node stageNode = doc.child("stages").first_child(); stageNode; stageNode = stageNode.next_sibling()) {
		if (strcasecmp(stageNode.name(), "config") == 0) {
			stagesEnabled = stageNode.attribute("enabled").as_bool();
		} else {
			uint32_t minLevel, maxLevel, multiplier;

			pugi::xml_attribute minLevelAttribute = stageNode.attribute("minlevel");
			if (minLevelAttribute) {
				minLevel = pugi::cast<uint32_t>(minLevelAttribute.value());
			} else {
				minLevel = 1;
			}

			pugi::xml_attribute maxLevelAttribute = stageNode.attribute("maxlevel");
			if (maxLevelAttribute) {
				maxLevel = pugi::cast<uint32_t>(maxLevelAttribute.value());
			} else {
				maxLevel = 0;
				lastStageLevel = minLevel;
				useLastStageLevel = true;
			}

			pugi::xml_attribute multiplierAttribute = stageNode.attribute("multiplier");
			if (multiplierAttribute) {
				multiplier = pugi::cast<uint32_t>(multiplierAttribute.value());
			} else {
				multiplier = 1;
			}

			if (useLastStageLevel) {
				stages[lastStageLevel] = multiplier;
			} else {
				for (uint32_t i = minLevel; i <= maxLevel; ++i) {
					stages[i] = multiplier;
				}
			}
		}
	}
	return true;
}

void Game::playerInviteToParty(uint32_t playerId, uint32_t invitedId)
{
	Player* player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	Player* invitedPlayer = getPlayerByID(invitedId);
	if (!invitedPlayer || invitedPlayer->isInviting(player)) {
		return;
	}

	if (invitedPlayer->getParty()) {
		std::ostringstream ss;
		ss << invitedPlayer->getName() << " is already in a party.";
		player->sendTextMessage(MESSAGE_INFO_DESCR, ss.str());
		return;
	}

	Party* party = player->getParty();
	if (!party) {
		party = new Party(player);
	} else if (party->getLeader() != player) {
		return;
	}

	party->invitePlayer(*invitedPlayer);
}

void Game::playerJoinParty(uint32_t playerId, uint32_t leaderId)
{
	Player* player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	Player* leader = getPlayerByID(leaderId);
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

	party->joinParty(*player);
}

void Game::playerRevokePartyInvitation(uint32_t playerId, uint32_t invitedId)
{
	Player* player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	Party* party = player->getParty();
	if (!party || party->getLeader() != player) {
		return;
	}

	Player* invitedPlayer = getPlayerByID(invitedId);
	if (!invitedPlayer || !player->isInviting(invitedPlayer)) {
		return;
	}

	party->revokeInvitation(*invitedPlayer);
}

void Game::playerPassPartyLeadership(uint32_t playerId, uint32_t newLeaderId)
{
	Player* player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	Party* party = player->getParty();
	if (!party || party->getLeader() != player) {
		return;
	}

	Player* newLeader = getPlayerByID(newLeaderId);
	if (!newLeader || !player->isPartner(newLeader)) {
		return;
	}

	party->passPartyLeadership(newLeader);
}

void Game::playerLeaveParty(uint32_t playerId)
{
	Player* player = getPlayerByID(playerId);
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
	Player* player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	Party* party = player->getParty();
	if (!party || player->hasCondition(CONDITION_INFIGHT)) {
		return;
	}

	party->setSharedExperience(player, sharedExpActive);
}

void Game::sendGuildMotd(uint32_t playerId)
{
	Player* player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	Guild* guild = player->getGuild();
	if (guild) {
		player->sendChannelMessage("Message of the Day", guild->getMotd(), TALKTYPE_CHANNEL_R1, CHANNEL_GUILD);
	}
}

void Game::kickPlayer(uint32_t playerId, bool displayEffect)
{
	Player* player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	player->kickPlayer(displayEffect);
}

void Game::playerReportBug(uint32_t playerId, const std::string& bug)
{
	Player* player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	if (player->getAccountType() == ACCOUNT_TYPE_NORMAL) {
		return;
	}

	std::string fileName = "data/reports/" + player->getName() + " report.txt";
	FILE* file = fopen(fileName.c_str(), "a");
	if (file) {
		const Position& position = player->getPosition();
		fprintf(file, "------------------------------\nName: %s [Position X: %u Y: %u Z: %u]\nBug Report: %s\n", player->getName().c_str(), position.x, position.y, position.z, bug.c_str());
		fclose(file);
	}

	player->sendTextMessage(MESSAGE_EVENT_DEFAULT, "Your report has been sent to " + g_config.getString(ConfigManager::SERVER_NAME) + ".");
}

void Game::playerDebugAssert(uint32_t playerId, const std::string& assertLine, const std::string& date, const std::string& description, const std::string& comment)
{
	Player* player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	// TODO: move debug assertions to database
	FILE* file = fopen("client_assertions.txt", "a");
	if (file) {
		fprintf(file, "----- %s - %s (%s) -----\n", formatDate(time(nullptr)).c_str(), player->getName().c_str(), convertIPToString(player->getIP()).c_str());
		fprintf(file, "%s\n%s\n%s\n%s\n", assertLine.c_str(), date.c_str(), description.c_str(), comment.c_str());
		fclose(file);
	}
}

void Game::playerLeaveMarket(uint32_t playerId)
{
	Player* player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	player->setInMarket(false);
}

void Game::playerBrowseMarket(uint32_t playerId, uint16_t spriteId)
{
	Player* player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	if (!player->isInMarket()) {
		return;
	}

	const ItemType& it = Item::items.getItemIdByClientId(spriteId);
	if (it.id == 0) {
		return;
	}

	if (it.wareId == 0) {
		return;
	}

	const MarketOfferList& buyOffers = IOMarket::getActiveOffers(MARKETACTION_BUY, it.id);
	const MarketOfferList& sellOffers = IOMarket::getActiveOffers(MARKETACTION_SELL, it.id);
	player->sendMarketBrowseItem(it.id, buyOffers, sellOffers);
	player->sendMarketDetail(it.id);
}

void Game::playerBrowseMarketOwnOffers(uint32_t playerId)
{
	Player* player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	if (!player->isInMarket()) {
		return;
	}

	const MarketOfferList& buyOffers = IOMarket::getOwnOffers(MARKETACTION_BUY, player->getGUID());
	const MarketOfferList& sellOffers = IOMarket::getOwnOffers(MARKETACTION_SELL, player->getGUID());
	player->sendMarketBrowseOwnOffers(buyOffers, sellOffers);
}

void Game::playerBrowseMarketOwnHistory(uint32_t playerId)
{
	Player* player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	if (!player->isInMarket()) {
		return;
	}

	const HistoryMarketOfferList& buyOffers = IOMarket::getOwnHistory(MARKETACTION_BUY, player->getGUID());
	const HistoryMarketOfferList& sellOffers = IOMarket::getOwnHistory(MARKETACTION_SELL, player->getGUID());
	player->sendMarketBrowseOwnHistory(buyOffers, sellOffers);
}

void Game::playerCreateMarketOffer(uint32_t playerId, uint8_t type, uint16_t spriteId, uint16_t amount, uint32_t price, bool anonymous)
{
	if (amount == 0 || amount > 64000) {
		return;
	}

	if (price == 0 || price > 999999999) {
		return;
	}

	if (type != MARKETACTION_BUY && type != MARKETACTION_SELL) {
		return;
	}

	Player* player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	if (!player->isInMarket()) {
		return;
	}

	if (g_config.getBoolean(ConfigManager::MARKET_PREMIUM) && !player->isPremium()) {
		player->sendMarketLeave();
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

	const int32_t maxOfferCount = g_config.getNumber(ConfigManager::MAX_MARKET_OFFERS_AT_A_TIME_PER_PLAYER);
	if (maxOfferCount > 0) {
		const int32_t offerCount = IOMarket::getPlayerOfferCount(player->getGUID());
		if (offerCount == -1 || offerCount >= maxOfferCount) {
			return;
		}
	}

	uint64_t fee = (price / 100.) * amount;
	if (fee < 20) {
		fee = 20;
	} else if (fee > 1000) {
		fee = 1000;
	}

	if (type == MARKETACTION_SELL) {
		if (fee > player->bankBalance) {
			return;
		}

		DepotChest* depotChest = player->getDepotChest(player->getLastDepotId(), false);
		if (!depotChest) {
			return;
		}

		ItemList itemList;
		uint32_t count = 0;
		std::vector<Container*> containers {depotChest, player->getInbox()};
		bool enough = false;

		size_t i = 0;
		do {
			Container* container = containers[i++];
			for (Item* item : container->getItemList()) {
				Container* c = item->getContainer();
				if (c && !c->empty()) {
					containers.push_back(c);
					continue;
				}

				const ItemType& itemType = Item::items[item->getID()];
				if (itemType.wareId != it.wareId) {
					continue;
				}

				if (item->hasAttributes()) {
					bool badAttribute = false;

					ItemAttributes* attributes = item->getAttributes();
					for (const auto& attr : attributes->getList()) {
						if (attr.type == ITEM_ATTRIBUTE_CHARGES) {
							uint16_t charges = static_cast<uint16_t>(0xFFFF & reinterpret_cast<ptrdiff_t>(attr.value));
							if (charges != itemType.charges) {
								badAttribute = true;
								continue;
							}
						} else if (attr.type == ITEM_ATTRIBUTE_DURATION) {
							uint32_t duration = static_cast<uint32_t>(0xFFFFFFFF & reinterpret_cast<ptrdiff_t>(attr.value));
							if (duration != itemType.decayTime) {
								badAttribute = true;
								continue;
							}
						} else {
							badAttribute = true;
							break;
						}
					}

					if (badAttribute) {
						continue;
					}
				}

				itemList.push_back(item);

				count += Item::countByType(item, -1);
				if (count >= amount) {
					enough = true;
					break;
				}
			}

			if (enough) {
				break;
			}
		} while (i < containers.size());

		if (!enough) {
			return;
		}

		if (it.stackable) {
			uint16_t tmpAmount = amount;
			for (Item* item : itemList) {
				uint16_t removeCount = std::min<uint16_t>(tmpAmount, item->getItemCount());
				tmpAmount -= removeCount;
				internalRemoveItem(item, removeCount);

				if (tmpAmount == 0) {
					break;
				}
			}
		} else {
			for (Item* item : itemList) {
				internalRemoveItem(item);
			}
		}

		player->bankBalance -= fee;
	} else {
		uint64_t totalPrice = (uint64_t)price * amount;
		totalPrice += fee;
		if (totalPrice > player->bankBalance) {
			return;
		}

		player->bankBalance -= totalPrice;
	}

	IOMarket::createOffer(player->getGUID(), (MarketAction_t)type, it.id, amount, price, anonymous);

	player->sendMarketEnter(player->getLastDepotId());
	const MarketOfferList& buyOffers = IOMarket::getActiveOffers(MARKETACTION_BUY, it.id);
	const MarketOfferList& sellOffers = IOMarket::getActiveOffers(MARKETACTION_SELL, it.id);
	player->sendMarketBrowseItem(it.id, buyOffers, sellOffers);
}

void Game::playerCancelMarketOffer(uint32_t playerId, uint32_t timestamp, uint16_t counter)
{
	Player* player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	if (!player->isInMarket()) {
		return;
	}

	MarketOfferEx offer = IOMarket::getOfferByCounter(timestamp, counter);
	if (offer.id == 0 || offer.playerId != player->getGUID()) {
		return;
	}

	if (offer.type == MARKETACTION_BUY) {
		player->bankBalance += (uint64_t)offer.price * offer.amount;
		player->sendMarketEnter(player->getLastDepotId());
	} else {
		const ItemType& it = Item::items[offer.itemId];
		if (it.id == 0) {
			return;
		}

		if (it.stackable) {
			uint16_t tmpAmount = offer.amount;
			while (tmpAmount > 0) {
				int32_t stackCount = std::min<int32_t>(100, tmpAmount);
				Item* item = Item::CreateItem(it.id, stackCount);
				if (internalAddItem(player->getInbox(), item, INDEX_WHEREEVER, FLAG_NOLIMIT) != RET_NOERROR) {
					delete item;
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
				Item* item = Item::CreateItem(it.id, subType);
				if (internalAddItem(player->getInbox(), item, INDEX_WHEREEVER, FLAG_NOLIMIT) != RET_NOERROR) {
					delete item;
					break;
				}
			}
		}
	}

	IOMarket::moveOfferToHistory(offer.id, OFFERSTATE_CANCELLED);
	offer.amount = 0;
	offer.timestamp += g_config.getNumber(ConfigManager::MARKET_OFFER_DURATION);
	player->sendMarketCancelOffer(offer);
}

void Game::playerAcceptMarketOffer(uint32_t playerId, uint32_t timestamp, uint16_t counter, uint16_t amount)
{
	if (amount == 0 || amount > 64000) {
		return;
	}

	Player* player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	if (!player->isInMarket()) {
		return;
	}

	MarketOfferEx offer = IOMarket::getOfferByCounter(timestamp, counter);
	if (offer.id == 0) {
		return;
	}

	if (amount > offer.amount) {
		return;
	}

	const ItemType& it = Item::items[offer.itemId];
	if (it.id == 0) {
		return;
	}

	uint64_t totalPrice = (uint64_t)offer.price * amount;

	if (offer.type == MARKETACTION_BUY) {
		DepotChest* depotChest = player->getDepotChest(player->getLastDepotId(), false);
		if (!depotChest) {
			return;
		}

		ItemList itemList;
		uint32_t count = 0;
		std::vector<Container*> containers {depotChest, player->getInbox()};
		bool enough = false;

		size_t i = 0;
		do {
			Container* container = containers[i++];
			for (Item* item : container->getItemList()) {
				Container* c = item->getContainer();
				if (c && !c->empty()) {
					containers.push_back(c);
					continue;
				}

				const ItemType& itemType = Item::items[item->getID()];
				if (itemType.wareId != it.id) {
					continue;
				}

				// TODO: Disallow other items with other attributes than charges and duration
				if (item->hasAttributes()) {
					bool badAttribute = false;

					ItemAttributes* attributes = item->getAttributes();
					for (const auto& attr : attributes->getList()) {
						if (attr.type == ITEM_ATTRIBUTE_CHARGES) {
							uint16_t charges = static_cast<uint16_t>(0xFFFF & reinterpret_cast<ptrdiff_t>(attr.value));
							if (charges != itemType.charges) {
								badAttribute = true;
								continue;
							}
						} else if (attr.type == ITEM_ATTRIBUTE_DURATION) {
							uint32_t duration = static_cast<uint32_t>(0xFFFFFFFF & reinterpret_cast<ptrdiff_t>(attr.value));
							if (duration != itemType.decayTime) {
								badAttribute = true;
								continue;
							}
						} else {
							badAttribute = true;
							break;
						}
					}

					if (badAttribute) {
						continue;
					}
				}

				itemList.push_back(item);

				count += Item::countByType(item, -1);
				if (count >= amount) {
					enough = true;
					break;
				}
			}

			if (enough) {
				break;
			}
		} while (i < containers.size());

		if (!enough) {
			return;
		}

		Player* buyerPlayer = getPlayerByGUID(offer.playerId);
		if (!buyerPlayer) {
			buyerPlayer = new Player(nullptr);
			if (!IOLoginData::loadPlayerById(buyerPlayer, offer.playerId)) {
				delete buyerPlayer;
				return;
			}
		}

		if (it.stackable) {
			uint16_t tmpAmount = amount;
			for (Item* item : itemList) {
				uint16_t removeCount = std::min<uint16_t>(tmpAmount, item->getItemCount());
				tmpAmount -= removeCount;
				internalRemoveItem(item, removeCount);

				if (tmpAmount == 0) {
					break;
				}
			}
		} else {
			for (Item* item : itemList) {
				internalRemoveItem(item);
			}
		}

		player->bankBalance += totalPrice;

		if (it.stackable) {
			uint16_t tmpAmount = amount;
			while (tmpAmount > 0) {
				uint16_t stackCount = std::min<uint16_t>(100, tmpAmount);
				Item* item = Item::CreateItem(it.id, stackCount);
				if (internalAddItem(buyerPlayer->getInbox(), item, INDEX_WHEREEVER, FLAG_NOLIMIT) != RET_NOERROR) {
					delete item;
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
				Item* item = Item::CreateItem(it.id, subType);
				if (internalAddItem(buyerPlayer->getInbox(), item, INDEX_WHEREEVER, FLAG_NOLIMIT) != RET_NOERROR) {
					delete item;
					break;
				}
			}
		}

		if (buyerPlayer->isOffline()) {
			IOLoginData::savePlayer(buyerPlayer);
			delete buyerPlayer;
		} else {
			buyerPlayer->onReceiveMail();
		}
	} else {
		if (totalPrice > player->bankBalance) {
			return;
		}

		player->bankBalance -= totalPrice;

		if (it.stackable) {
			uint16_t tmpAmount = amount;
			while (tmpAmount > 0) {
				uint16_t stackCount = std::min<uint16_t>(100, tmpAmount);
				Item* item = Item::CreateItem(it.id, stackCount);
				if (internalAddItem(player->getInbox(), item, INDEX_WHEREEVER, FLAG_NOLIMIT) != RET_NOERROR) {
					delete item;
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
				Item* item = Item::CreateItem(it.id, subType);
				if (internalAddItem(player->getInbox(), item, INDEX_WHEREEVER, FLAG_NOLIMIT) != RET_NOERROR) {
					delete item;
					break;
				}
			}
		}

		Player* sellerPlayer = getPlayerByGUID(offer.playerId);
		if (sellerPlayer) {
			sellerPlayer->bankBalance += totalPrice;
		} else {
			IOLoginData::increaseBankBalance(offer.playerId, totalPrice);
		}

		player->onReceiveMail();
	}

	const int32_t marketOfferDuration = g_config.getNumber(ConfigManager::MARKET_OFFER_DURATION);

	IOMarket::appendHistory(player->getGUID(), (offer.type == MARKETACTION_BUY ? MARKETACTION_SELL : MARKETACTION_BUY), offer.itemId, amount, offer.price, offer.timestamp + marketOfferDuration, OFFERSTATE_ACCEPTEDEX);

	IOMarket::appendHistory(offer.playerId, offer.type, offer.itemId, amount, offer.price, offer.timestamp + marketOfferDuration, OFFERSTATE_ACCEPTED);

	offer.amount -= amount;

	if (offer.amount == 0) {
		IOMarket::deleteOffer(offer.id);
	} else {
		IOMarket::acceptOffer(offer.id, amount);
	}

	player->sendMarketEnter(player->getLastDepotId());
	offer.timestamp += marketOfferDuration;
	player->sendMarketAcceptOffer(offer);
}

void Game::checkExpiredMarketOffers()
{
	const ExpiredMarketOfferList& expiredBuyOffers = IOMarket::getExpiredOffers(MARKETACTION_BUY);
	for (const ExpiredMarketOffer& offer : expiredBuyOffers) {
		uint64_t totalPrice = (uint64_t)offer.price * offer.amount;

		Player* player = getPlayerByGUID(offer.playerId);
		if (player) {
			player->bankBalance += totalPrice;
		} else {
			IOLoginData::increaseBankBalance(offer.playerId, totalPrice);
		}

		IOMarket::moveOfferToHistory(offer.id, OFFERSTATE_EXPIRED);
	}

	const ExpiredMarketOfferList& expiredSellOffers = IOMarket::getExpiredOffers(MARKETACTION_SELL);
	for (const ExpiredMarketOffer& offer : expiredSellOffers) {
		Player* player = getPlayerByGUID(offer.playerId);
		if (!player) {
			player = new Player(nullptr);
			if (!IOLoginData::loadPlayerById(player, offer.playerId)) {
				delete player;
				continue;
			}
		}

		const ItemType& itemType = Item::items[offer.itemId];
		if (itemType.id == 0) {
			continue;
		}

		if (itemType.stackable) {
			uint16_t tmpAmount = offer.amount;
			while (tmpAmount > 0) {
				uint16_t stackCount = std::min<uint16_t>(100, tmpAmount);
				Item* item = Item::CreateItem(itemType.id, stackCount);
				if (internalAddItem(player->getInbox(), item, INDEX_WHEREEVER, FLAG_NOLIMIT) != RET_NOERROR) {
					delete item;
					break;
				}

				tmpAmount -= stackCount;
			}
		} else {
			int32_t subType;
			if (itemType.charges != 0) {
				subType = itemType.charges;
			} else {
				subType = -1;
			}

			for (uint16_t i = 0; i < offer.amount; ++i) {
				Item* item = Item::CreateItem(itemType.id, subType);
				if (internalAddItem(player->getInbox(), item, INDEX_WHEREEVER, FLAG_NOLIMIT) != RET_NOERROR) {
					delete item;
					break;
				}
			}
		}

		if (player->isOffline()) {
			IOLoginData::savePlayer(player);
			delete player;
		}

		IOMarket::moveOfferToHistory(offer.id, OFFERSTATE_EXPIRED);
	}

	int32_t checkExpiredMarketOffersEachMinutes = g_config.getNumber(ConfigManager::CHECK_EXPIRED_MARKET_OFFERS_EACH_MINUTES);
	if (checkExpiredMarketOffersEachMinutes <= 0) {
		return;
	}

	g_scheduler.addEvent(createSchedulerTask(checkExpiredMarketOffersEachMinutes * 60 * 1000, std::bind(&Game::checkExpiredMarketOffers, this)));
}

void Game::parsePlayerExtendedOpcode(uint32_t playerId, uint8_t opcode, const std::string& buffer)
{
	Player* player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	for (CreatureEvent* creatureEvent : player->getCreatureEvents(CREATURE_EVENT_EXTENDED_OPCODE)) {
		creatureEvent->executeExtendedOpcode(player, opcode, buffer);
	}
}

void Game::forceAddCondition(uint32_t creatureId, Condition* condition)
{
	Creature* creature = getCreatureByID(creatureId);
	if (!creature) {
		delete condition;
		return;
	}

	creature->addCondition(condition, true);
}

void Game::forceRemoveCondition(uint32_t creatureId, ConditionType_t type)
{
	Creature* creature = getCreatureByID(creatureId);
	if (!creature) {
		return;
	}

	creature->removeCondition(type, true);
}

void Game::sendOfflineTrainingDialog(Player* player)
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
	Player* player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	if (!player->hasModalWindowOpen(modalWindowId)) {
		return;
	}

	player->onModalWindowHandled(modalWindowId);

	// offline training, hardcoded
	if (modalWindowId == std::numeric_limits<uint32_t>::max()) {
		if (button == 1) {
			if (choice == SKILL_SWORD || choice == SKILL_AXE || choice == SKILL_CLUB || choice == SKILL_DISTANCE || choice == SKILL_MAGLEVEL) {
				BedItem* bedItem = player->getBedItem();
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

void Game::addPlayer(Player* player)
{
	const std::string& lowercase_name = asLowerCaseString(player->getName());
	mappedPlayerNames[lowercase_name] = player;
	wildcardTree.insert(lowercase_name);
	players[player->getID()] = player;
}

void Game::removePlayer(Player* player)
{
	const std::string& lowercase_name = asLowerCaseString(player->getName());
	mappedPlayerNames.erase(lowercase_name);
	wildcardTree.remove(lowercase_name);
	players.erase(player->getID());
}

void Game::addNpc(Npc* npc)
{
	npcs[npc->getID()] = npc;
}

void Game::removeNpc(Npc* npc)
{
	npcs.erase(npc->getID());
}

void Game::addMonster(Monster* monster)
{
	monsters[monster->getID()] = monster;
}

void Game::removeMonster(Monster* monster)
{
	monsters.erase(monster->getID());
}

Guild* Game::getGuild(uint32_t id) const
{
	auto it = guilds.find(id);
	if (it == guilds.end()) {
		return nullptr;
	}
	return it->second;
}

void Game::addGuild(Guild* guild)
{
	guilds[guild->getId()] = guild;
}

void Game::decreaseBrowseFieldRef(const Position& pos)
{
	Tile* tile = getTile(pos);
	if (!tile) {
		return;
	}

	auto it = browseFields.find(tile);
	if (it != browseFields.end()) {
		it->second->releaseThing2();
	}
}

Group* Game::getGroup(uint32_t id)
{
	return groups.getGroup(id);
}

void Game::internalRemoveItems(std::vector<Item*> itemList, uint32_t amount, bool stackable)
{
	if (stackable) {
		for (Item* item : itemList) {
			if (item->getItemCount() > amount) {
				internalRemoveItem(item, amount);
				break;
			} else {
				amount -= item->getItemCount();
				internalRemoveItem(item);
			}
		}
	} else {
		for (Item* item : itemList) {
			internalRemoveItem(item);
		}
	}
}
