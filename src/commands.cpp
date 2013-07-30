//////////////////////////////////////////////////////////////////////
// The Forgotten Server - a server application for the MMORPG Tibia
//////////////////////////////////////////////////////////////////////
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//////////////////////////////////////////////////////////////////////

#include "otpch.h"

#include <string>
#include <fstream>
#include <utility>
#include <cstring>
#include <cerrno>

#include "commands.h"
#include "player.h"
#include "npc.h"
#include "monsters.h"
#include "game.h"
#include "actions.h"
#include "house.h"
#include "iologindata.h"
#include "tools.h"
#include "ban.h"
#include "configmanager.h"
#include "town.h"
#include "spells.h"
#include "talkaction.h"
#include "movement.h"
#include "spells.h"
#include "weapons.h"
#include "raids.h"
#include "chat.h"
#include "quests.h"
#include "mounts.h"
#include "globalevent.h"
#ifdef __ENABLE_SERVER_DIAGNOSTIC__
#include "outputmessage.h"
#include "connection.h"
#include "admin.h"
#include "status.h"
#include "protocollogin.h"
#endif

#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

extern ConfigManager g_config;
extern Actions* g_actions;
extern Monsters g_monsters;
extern Npcs g_npcs;
extern TalkActions* g_talkActions;
extern MoveEvents* g_moveEvents;
extern Spells* g_spells;
extern Weapons* g_weapons;
extern Game g_game;
extern Chat g_chat;
extern CreatureEvents* g_creatureEvents;
extern GlobalEvents* g_globalEvents;

s_defcommands Commands::defined_commands[] = {
	//admin commands
	{"/s", &Commands::placeNpc},
	{"/m", &Commands::placeMonster},
	{"/summon", &Commands::placeSummon},
	{"/B", &Commands::broadcastMessage},
	{"/t", &Commands::teleportMasterPos},
	{"/c", &Commands::teleportHere},
	{"/i", &Commands::createItemById},
	{"/n", &Commands::createItemByName},
	{"/q", &Commands::subtractMoney},
	{"/reload", &Commands::reloadInfo},
	{"/goto", &Commands::teleportTo},
	{"/info", &Commands::getInfo},
	{"/closeserver", &Commands::closeServer},
	{"/openserver", &Commands::openServer},
	{"/a", &Commands::teleportNTiles},
	{"/kick", &Commands::kickPlayer},
	{"/owner", &Commands::setHouseOwner},
	{"/gethouse", &Commands::getHouse},
	{"/town", &Commands::teleportToTown},
	{"/up", &Commands::changeFloor},
	{"/down", &Commands::changeFloor},
	{"/pos", &Commands::showPosition},
	{"/r", &Commands::removeThing},
	{"/newtype", &Commands::newType},
	{"/newitem", &Commands::newItem},
	{"/hide", &Commands::hide},
	{"/raid", &Commands::forceRaid},
	{"/addskill", &Commands::addSkill},
	{"/ghost", &Commands::ghost},
	{"/clean", &Commands::clean},
	{"/mccheck", &Commands::multiClientCheck},
	{"/addtutor", &Commands::addTutor},
	{"/removetutor", &Commands::removeTutor},
	{"/serverdiag", &Commands::serverDiag},

	// player commands - TODO: make them talkactions
	{"!online", &Commands::whoIsOnline},
	{"!buyhouse", &Commands::buyHouse},
	{"!sellhouse", &Commands::sellHouse},
	{"!serverinfo", &Commands::serverInfo},
	{"!kills", &Commands::playerKills}
};

Commands::Commands()
{
	loaded = false;

	//setup command map
	for (uint32_t i = 0; i < sizeof(defined_commands) / sizeof(defined_commands[0]); i++) {
		Command* cmd = new Command;
		cmd->loadedGroupId = false;
		cmd->loadedAccountType = false;
		cmd->loadedLogging = false;
		cmd->logged = true;
		cmd->groupId = 1;
		cmd->f = defined_commands[i].f;
		std::string key = defined_commands[i].name;
		commandMap[key] = cmd;
	}
}

Commands::~Commands()
{
	for (CommandMap::iterator it = commandMap.begin(); it != commandMap.end(); ++it) {
		delete it->second;
	}
}

bool Commands::loadFromXml()
{
	std::string filename = "data/XML/commands.xml";

	xmlDocPtr doc = xmlParseFile(filename.c_str());

	if (doc) {
		loaded = true;
		xmlNodePtr root, p;
		root = xmlDocGetRootElement(doc);

		if (xmlStrcmp(root->name, (const xmlChar*)"commands")) {
			std::cout << "[Error - Commands::loadFromXml] Malformed commands file." << std::endl;
			xmlFreeDoc(doc);
			return false;
		}

		std::string strCmd;
		p = root->children;

		while (p) {
			if (xmlStrcmp(p->name, (const xmlChar*)"command") == 0) {
				if (readXMLString(p, "cmd", strCmd)) {
					CommandMap::iterator it = commandMap.find(strCmd);
					int32_t gId;
					int32_t aTypeLevel;
					std::string strValue;

					if (it != commandMap.end()) {
						Command* command = it->second;

						if (readXMLInteger(p, "group", gId)) {
							if (!command->loadedGroupId) {
								command->groupId = gId;
								command->loadedGroupId = true;
							} else {
								std::cout << "Duplicated command " << strCmd << std::endl;
							}
						} else {
							std::cout << "missing group tag for " << strCmd << std::endl;
						}

						if (readXMLInteger(p, "acctype", aTypeLevel)) {
							if (!command->loadedAccountType) {
								command->accountType = (AccountType_t)aTypeLevel;
								command->loadedAccountType = true;
							} else {
								std::cout << "Duplicated command " << strCmd << std::endl;
							}
						} else {
							std::cout << "missing acctype tag for " << strCmd << std::endl;
						}

						if (readXMLString(p, "log", strValue)) {
							if (!command->loadedLogging) {
								command->logged = booleanString(strValue);
								command->loadedLogging = true;
							} else {
								std::cout << "Duplicated log tag for " << strCmd << std::endl;
							}
						} else {
							std::cout << "Missing log tag for " << strCmd << std::endl;
						}
					} else {
						std::cout << "Unknown command " << strCmd << std::endl;
					}
				} else {
					std::cout << "missing cmd." << std::endl;
				}
			}

			p = p->next;
		}

		xmlFreeDoc(doc);
	}

	for (CommandMap::iterator it = commandMap.begin(), end = commandMap.end(); it != end; ++it) {
		Command* command = it->second;

		if (!command->loadedGroupId) {
			std::cout << "Warning: Missing group id for command " << it->first << std::endl;
		}

		if (!command->loadedAccountType) {
			std::cout << "Warning: Missing acctype level for command " << it->first << std::endl;
		}

		if (!command->loadedLogging) {
			std::cout << "Warning: Missing log command " << it->first << std::endl;
		}

		g_game.addCommandTag(it->first.substr(0, 1));
	}

	return loaded;
}

bool Commands::reload()
{
	loaded = false;

	for (CommandMap::iterator it = commandMap.begin(), end = commandMap.end(); it != end; ++it) {
		Command* command = it->second;
		command->groupId = 1;
		command->accountType = ACCOUNT_TYPE_GOD;
		command->loadedGroupId = false;
		command->loadedAccountType = false;
		command->logged = true;
		command->loadedLogging = false;
	}

	g_game.resetCommandTag();
	return loadFromXml();
}

bool Commands::exeCommand(Player* player, const std::string& cmd)
{
	std::string str_command;
	std::string str_param;

	std::string::size_type loc = cmd.find(' ', 0);

	if (loc != std::string::npos) {
		str_command = std::string(cmd, 0, loc);
		str_param = std::string(cmd, (loc + 1), cmd.size() - loc - 1);
	} else {
		str_command = cmd;
		str_param = std::string("");
	}

	//find command
	CommandMap::iterator it = commandMap.find(str_command);

	if (it == commandMap.end()) {
		return false;
	}

	Command* command = it->second;

	if (command->groupId > player->groupId || command->accountType > player->accountType) {
		if (player->accessLevel) {
			player->sendTextMessage(MSG_STATUS_SMALL, "You can not execute this command.");
		}

		return false;
	}

	//execute command
	CommandFunc cfunc = command->f;
	(this->*cfunc)(player, str_command, str_param);

	if (command->logged) {
		player->sendTextMessage(MSG_STATUS_CONSOLE_RED, cmd);

		std::ostringstream logFile;
		logFile << "data/logs/" << player->getName() << " commands.log";
		std::ofstream out(logFile.str(), std::ios::app);
		if (out.is_open()) {
			time_t ticks = time(NULL);
			const tm* now = localtime(&ticks);
			char buf[32];
			strftime(buf, sizeof(buf), "%d/%m/%Y %H:%M", now);

			out << "[" << buf << "] " << cmd << std::endl;
			out.close();
		}
	}
	return true;
}

void Commands::placeNpc(Player* player, const std::string& cmd, const std::string& param)
{
	Npc* npc = Npc::createNpc(param);

	if (!npc) {
		return;
	}

	// Place the npc
	if (g_game.placeCreature(npc, player->getPosition())) {
		g_game.addMagicEffect(player->getPosition(), NM_ME_MAGIC_BLOOD);
		npc->setMasterPos(npc->getPosition());
	} else {
		delete npc;
		player->sendCancelMessage(RET_NOTENOUGHROOM);
		g_game.addMagicEffect(player->getPosition(), NM_ME_POFF);
	}
}

void Commands::placeMonster(Player* player, const std::string& cmd, const std::string& param)
{
	Monster* monster = Monster::createMonster(param);

	if (!monster) {
		player->sendCancelMessage(RET_NOTPOSSIBLE);
		g_game.addMagicEffect(player->getPosition(), NM_ME_POFF);
		return;
	}

	// Place the monster
	if (g_game.placeCreature(monster, player->getPosition())) {
		g_game.addMagicEffect(monster->getPosition(), NM_ME_TELEPORT);
		g_game.addMagicEffect(player->getPosition(), NM_ME_MAGIC_BLOOD);
	} else {
		delete monster;
		player->sendCancelMessage(RET_NOTENOUGHROOM);
		g_game.addMagicEffect(player->getPosition(), NM_ME_POFF);
	}
}

void Commands::placeSummon(Player* player, const std::string& cmd, const std::string& param)
{
	Monster* monster = Monster::createMonster(param);

	if (!monster) {
		player->sendCancelMessage(RET_NOTPOSSIBLE);
		g_game.addMagicEffect(player->getPosition(), NM_ME_POFF);
		return;
	}

	// Place the monster
	player->addSummon(monster);

	if (!g_game.placeCreature(monster, player->getPosition())) {
		player->removeSummon(monster);
		player->sendCancelMessage(RET_NOTENOUGHROOM);
		g_game.addMagicEffect(player->getPosition(), NM_ME_POFF);
	} else {
		g_game.addMagicEffect(monster->getPosition(), NM_ME_TELEPORT);
	}
}

void Commands::broadcastMessage(Player* player, const std::string& cmd, const std::string& param)
{
	g_game.playerBroadcastMessage(player, param);
}

void Commands::teleportMasterPos(Player* player, const std::string& cmd, const std::string& param)
{
	Position oldPosition = player->getPosition();
	Position destPos = player->getTemplePosition();
	Position newPosition = g_game.getClosestFreeTile(player, 0, destPos, true);

	if (player->getPosition() != destPos) {
		if (newPosition.x == 0) {
			player->sendCancel("You can not teleport there.");
		} else if (g_game.internalTeleport(player, newPosition) == RET_NOERROR) {
			g_game.addMagicEffect(oldPosition, NM_ME_POFF, player->isInGhostMode());
			g_game.addMagicEffect(newPosition, NM_ME_TELEPORT, player->isInGhostMode());
		}
	}
}

void Commands::teleportHere(Player* player, const std::string& cmd, const std::string& param)
{
	Creature* paramCreature = g_game.getCreatureByName(param);

	if (paramCreature) {
		Position oldPosition = paramCreature->getPosition();
		Position destPos = paramCreature->getPosition();
		Position newPosition = g_game.getClosestFreeTile(player, paramCreature, player->getPosition(), false);

		if (newPosition.x == 0) {
			std::ostringstream ss;
			ss << "You can not teleport " << paramCreature->getName() << std::endl;
			player->sendCancel(ss.str());
		} else if (g_game.internalTeleport(paramCreature, newPosition) == RET_NOERROR) {
			g_game.addMagicEffect(oldPosition, NM_ME_POFF, paramCreature->isInGhostMode());
			g_game.addMagicEffect(newPosition, NM_ME_TELEPORT, paramCreature->isInGhostMode());
		}
	} else {
		player->sendCancel("A creature with that name could not be found.");
	}
}

void Commands::createItemById(Player* player, const std::string& cmd, const std::string& param)
{
	std::string tmp = param;

	std::string::size_type pos = tmp.find(' ', 0);

	if (pos == std::string::npos) {
		pos = tmp.size();
	}

	int32_t type = atoi(tmp.substr(0, pos).c_str());
	int32_t count = 1;

	if (pos < tmp.size()) {
		tmp.erase(0, pos + 1);
		count = std::max<int32_t>(1, std::min<int32_t>(atoi(tmp.c_str()), 100));
	}

	Item* newItem = Item::CreateItem(type, count);

	if (!newItem) {
		return;
	}

	ReturnValue ret = g_game.internalAddItem(player, newItem);

	if (ret != RET_NOERROR) {
		ret = g_game.internalAddItem(player->getTile(), newItem, INDEX_WHEREEVER, FLAG_NOLIMIT);

		if (ret != RET_NOERROR) {
			delete newItem;
			return;
		}
	}

	g_game.startDecay(newItem);
	g_game.addMagicEffect(player->getPosition(), NM_ME_MAGIC_POISON);
}

void Commands::createItemByName(Player* player, const std::string& cmd, const std::string& param)
{
	std::string::size_type pos1 = param.find("\"");
	pos1 = (std::string::npos == pos1 ? 0 : pos1 + 1);

	std::string::size_type pos2 = param.rfind("\"");

	if (pos2 == pos1 || pos2 == std::string::npos) {
		pos2 = param.rfind(' ');

		if (pos2 == std::string::npos) {
			pos2 = param.size();
		}
	}

	std::string itemName = param.substr(pos1, pos2 - pos1);

	int32_t count = 1;

	if (pos2 < param.size()) {
		std::string itemCount = param.substr(pos2 + 1, param.size() - (pos2 + 1));
		count = std::min<int32_t>(atoi(itemCount.c_str()), 100);
	}

	int32_t itemId = Item::items.getItemIdByName(itemName);

	if (itemId == -1) {
		player->sendTextMessage(MSG_STATUS_CONSOLE_RED, "Item could not be summoned.");
		return;
	}

	Item* newItem = Item::CreateItem(itemId, count);

	if (!newItem) {
		return;
	}

	ReturnValue ret = g_game.internalAddItem(player, newItem);

	if (ret != RET_NOERROR) {
		ret = g_game.internalAddItem(player->getTile(), newItem, INDEX_WHEREEVER, FLAG_NOLIMIT);

		if (ret != RET_NOERROR) {
			delete newItem;
			return;
		}
	}

	g_game.startDecay(newItem);
	g_game.addMagicEffect(player->getPosition(), NM_ME_MAGIC_POISON);
}

void Commands::subtractMoney(Player* player, const std::string& cmd, const std::string& param)
{
	std::ostringstream ss;
	ss << "You have " << g_game.getMoney(player) << " gold.";
	player->sendCancel(ss.str());
}

void Commands::reloadInfo(Player* player, const std::string& cmd, const std::string& param)
{
	std::string tmpParam = asLowerCaseString(param);

	if (tmpParam == "action" || tmpParam == "actions") {
		g_actions->reload();
		player->sendTextMessage(MSG_STATUS_CONSOLE_BLUE, "Reloaded actions.");
	} else if (tmpParam == "config" || tmpParam == "configuration") {
		g_config.reload();
		player->sendTextMessage(MSG_STATUS_CONSOLE_BLUE, "Reloaded config.");
	} else if (tmpParam == "command" || tmpParam == "commands") {
		reload();
		player->sendTextMessage(MSG_STATUS_CONSOLE_BLUE, "Reloaded commands.");
	} else if (tmpParam == "creaturescript" || tmpParam == "creaturescripts") {
		g_creatureEvents->reload();
		player->sendTextMessage(MSG_STATUS_CONSOLE_BLUE, "Reloaded creature scripts.");
	} else if (tmpParam == "monster" || tmpParam == "monsters") {
		g_monsters.reload();
		player->sendTextMessage(MSG_STATUS_CONSOLE_BLUE, "Reloaded monsters.");
	} else if (tmpParam == "move" || tmpParam == "movement" || tmpParam == "movements"
	           || tmpParam == "moveevents" || tmpParam == "moveevent") {
		g_moveEvents->reload();
		player->sendTextMessage(MSG_STATUS_CONSOLE_BLUE, "Reloaded movements.");
	} else if (tmpParam == "npc" || tmpParam == "npcs") {
		g_npcs.reload();
		player->sendTextMessage(MSG_STATUS_CONSOLE_BLUE, "Reloaded npcs.");
	} else if (tmpParam == "raid" || tmpParam == "raids") {
		Raids::getInstance()->reload();
		Raids::getInstance()->startup();
		player->sendTextMessage(MSG_STATUS_CONSOLE_BLUE, "Reloaded raids.");
	} else if (tmpParam == "spell" || tmpParam == "spells") {
		g_spells->reload();
		g_monsters.reload();
		player->sendTextMessage(MSG_STATUS_CONSOLE_BLUE, "Reloaded spells.");
	} else if (tmpParam == "talk" || tmpParam == "talkaction" || tmpParam == "talkactions") {
		g_talkActions->reload();
		player->sendTextMessage(MSG_STATUS_CONSOLE_BLUE, "Reloaded talk actions.");
	} else if (tmpParam == "items") {
		Item::items.reload();
		player->sendTextMessage(MSG_STATUS_CONSOLE_BLUE, "Reloaded items.");
	} else if (tmpParam == "weapon" || tmpParam == "weapons") {
		g_weapons->reload();
		player->sendTextMessage(MSG_STATUS_CONSOLE_BLUE, "Reloaded weapons.");
	} else if (tmpParam == "quest" || tmpParam == "quests") {
		Quests::getInstance()->reload();
		player->sendTextMessage(MSG_STATUS_CONSOLE_BLUE, "Reloaded quests.");
	} else if (tmpParam == "mount" || tmpParam == "mounts") {
		Mounts::getInstance()->reload();
		player->sendTextMessage(MSG_STATUS_CONSOLE_BLUE, "Reloaded mounts.");
	} else if (tmpParam == "globalevents" || tmpParam == "globalevent") {
		g_globalEvents->reload();
		player->sendTextMessage(MSG_STATUS_CONSOLE_BLUE, "Reloaded globalevents.");
	} else {
		player->sendTextMessage(MSG_STATUS_CONSOLE_BLUE, "Reload type not found.");
	}
}

void Commands::teleportToTown(Player* player, const std::string& cmd, const std::string& param)
{
	std::string tmp = param;
	Town* town = Towns::getInstance().getTown(tmp);

	if (town) {
		Position oldPosition = player->getPosition();
		Position newPosition = g_game.getClosestFreeTile(player, 0, town->getTemplePosition(), true);

		if (player->getPosition() != town->getTemplePosition()) {
			if (newPosition.x == 0) {
				player->sendCancel("You can not teleport there.");
			} else if (g_game.internalTeleport(player, newPosition) == RET_NOERROR) {
				g_game.addMagicEffect(oldPosition, NM_ME_POFF, player->isInGhostMode());
				g_game.addMagicEffect(newPosition, NM_ME_TELEPORT, player->isInGhostMode());
			}
		}
	} else {
		player->sendCancel("Could not find the town.");
	}
}

void Commands::teleportTo(Player* player, const std::string& cmd, const std::string& param)
{
	Creature* paramCreature = g_game.getCreatureByName(param);

	if (paramCreature) {
		Position oldPosition = player->getPosition();
		Position newPosition = g_game.getClosestFreeTile(player, 0, paramCreature->getPosition(), true);

		if (newPosition.x > 0) {
			if (g_game.internalTeleport(player, newPosition) == RET_NOERROR) {
				bool ghostMode = false;

				if (player->isInGhostMode() || paramCreature->isInGhostMode()) {
					ghostMode = true;
				}

				g_game.addMagicEffect(oldPosition, NM_ME_POFF, ghostMode);
				g_game.addMagicEffect(player->getPosition(), NM_ME_TELEPORT, ghostMode);
			}
		} else {
			std::ostringstream ss;
			ss << "You can not teleport to " << paramCreature->getName() << ".";
			player->sendCancel(ss.str());
		}
	}
}

void Commands::getInfo(Player* player, const std::string& cmd, const std::string& param)
{
	Player* paramPlayer = g_game.getPlayerByName(param);

	if (!paramPlayer) {
		player->sendTextMessage(MSG_STATUS_CONSOLE_BLUE, "Player not found.");
		return;
	}

	if (player->getAccountType() < paramPlayer->getAccountType()) {
		player->sendTextMessage(MSG_STATUS_CONSOLE_BLUE, "You can not get info about this player.");
		return;
	}

	uint32_t playerIp = paramPlayer->getIP();

	std::ostringstream info;
	info << "Name: " << paramPlayer->name << std::endl <<
	     "Access: " << paramPlayer->accessLevel << std::endl <<
	     "Level: " << paramPlayer->level << std::endl <<
	     "Magic Level: " << paramPlayer->magLevel << std::endl <<
	     "Speed: " << paramPlayer->getSpeed() << std::endl <<
	     "Position: " << paramPlayer->getPosition() << std::endl <<
	     "IP: " << convertIPToString(playerIp) << std::endl;
	player->sendTextMessage(MSG_STATUS_CONSOLE_BLUE, info.str());

	if (playerIp != 0) {
		PlayerVector vec;

		const std::map<uint32_t, Player*>& players = g_game.getPlayers();
		for (auto it = players.begin(); it != players.end(); ++it) {
			if (it->second->getIP() != playerIp || it->second == paramPlayer) {
				continue;
			}

			vec.push_back(it->second);
		}

		if (!vec.empty()) {
			std::ostringstream ss;

			Player* tmpPlayer = vec[0];
			ss << "Other players on same IP: " << tmpPlayer->getName() << " [" << tmpPlayer->getLevel() << "]";

			for (PlayerVector::size_type i = 1, size = vec.size(); i < size; ++i) {
				tmpPlayer = vec[i];
				ss << ", " << tmpPlayer->getName() << " [" << tmpPlayer->getLevel() << "]";
			}

			ss << ".";
			player->sendTextMessage(MSG_STATUS_CONSOLE_BLUE, ss.str());
		}
	}
}

void Commands::closeServer(Player* player, const std::string& cmd, const std::string& param)
{
	if (param == "shutdown") {
		g_game.setGameState(GAME_STATE_SHUTDOWN);
	} else {
		g_game.setGameState(GAME_STATE_CLOSED);
		player->sendTextMessage(MSG_STATUS_CONSOLE_BLUE, "Server is now closed.");
	}
}

void Commands::openServer(Player* player, const std::string& cmd, const std::string& param)
{
	g_game.setGameState(GAME_STATE_NORMAL);
	player->sendTextMessage(MSG_STATUS_CONSOLE_BLUE, "Server is now open.");
}

void Commands::teleportNTiles(Player* player, const std::string& cmd, const std::string& param)
{
	int32_t ntiles = atoi(param.c_str());

	if (ntiles != 0) {
		Position oldPosition = player->getPosition();
		Position newPos = player->getPosition();

		switch (player->direction) {
			case NORTH:
				newPos.y -= ntiles;
				break;
			case SOUTH:
				newPos.y += ntiles;
				break;
			case EAST:
				newPos.x += ntiles;
				break;
			case WEST:
				newPos.x -= ntiles;
				break;
			default:
				break;
		}

		Position newPosition = g_game.getClosestFreeTile(player, 0, newPos, true);
		if (newPosition.x == 0) {
			player->sendCancel("You can not teleport there.");
		} else if (g_game.internalTeleport(player, newPosition) == RET_NOERROR) {
			if (ntiles != 1) {
				g_game.addMagicEffect(oldPosition, NM_ME_POFF, player->isInGhostMode());
				g_game.addMagicEffect(newPosition, NM_ME_TELEPORT, player->isInGhostMode());
			}
		}
	}
}

void Commands::kickPlayer(Player* player, const std::string& cmd, const std::string& param)
{
	Player* playerKick = g_game.getPlayerByName(param);

	if (playerKick) {
		if (playerKick->accessLevel) {
			player->sendTextMessage(MSG_STATUS_CONSOLE_BLUE, "You cannot kick this player.");
			return;
		}

		playerKick->kickPlayer(true);
	}
}

void Commands::setHouseOwner(Player* player, const std::string& cmd, const std::string& param)
{
	if (player->getTile()->hasFlag(TILESTATE_HOUSE)) {
		HouseTile* houseTile = dynamic_cast<HouseTile*>(player->getTile());

		if (houseTile) {
			uint32_t guid;
			std::string name = param;

			if (name == "none") {
				houseTile->getHouse()->setHouseOwner(0);
			} else if (IOLoginData::getInstance()->getGuidByName(guid, name)) {
				houseTile->getHouse()->setHouseOwner(guid);
			} else {
				player->sendTextMessage(MSG_STATUS_CONSOLE_BLUE, "Player not found.");
			}
		}
	}
}

void Commands::sellHouse(Player* player, const std::string& cmd, const std::string& param)
{
	House* house = Houses::getInstance().getHouseByPlayerId(player->guid);

	if (!house) {
		player->sendCancel("You do not own any house.");
		return;
	}

	Player* tradePartner = g_game.getPlayerByName(param);

	if (!(tradePartner && tradePartner != player)) {
		player->sendCancel("Trade player not found.");
		return;
	}

	if (tradePartner->level < 1) {
		player->sendCancel("Trade player level is too low.");
		return;
	}

	if (!Position::areInRange<2, 2, 0>(tradePartner->getPosition(), player->getPosition())) {
		player->sendCancel("Trade player is too far away.");
		return;
	}

	if (!tradePartner->isPremium()) {
		player->sendCancel("Trade player does not have a premium account.");
		return;
	}

	if (Houses::getInstance().getHouseByPlayerId(tradePartner->guid)) {
		player->sendCancel("Trade player already owns a house.");
		return;
	}

	if (IOLoginData::getInstance()->hasBiddedOnHouse(tradePartner->guid)) {
		player->sendCancel("Trade player is currently the highest bidder of an auctioned house.");
		return;
	}

	Item* transferItem = house->getTransferItem();

	if (!transferItem) {
		player->sendCancel("You can not trade this house.");
		return;
	}

	transferItem->getParent()->setParent(player);

	if (!g_game.internalStartTrade(player, tradePartner, transferItem)) {
		house->resetTransferItem();
	}
}

void Commands::getHouse(Player* player, const std::string& cmd, const std::string& param)
{
	std::string name = param;
	uint32_t guid;

	if (!IOLoginData::getInstance()->getGuidByName(guid, name)) {
		return;
	}

	std::ostringstream str;
	str << name;

	House* house = Houses::getInstance().getHouseByPlayerId(guid);

	if (house) {
		str << " owns house: " << house->getName() << ".";
	} else {
		str << " does not own any house.";
	}

	player->sendTextMessage(MSG_STATUS_CONSOLE_BLUE, str.str());
}

void Commands::serverInfo(Player* player, const std::string& cmd, const std::string& param)
{
	std::ostringstream text;
	text << "Server Info:";
	text << "\nExp Rate: " << g_game.getExperienceStage(player->level);
	text << "\nSkill Rate: " << g_config.getNumber(ConfigManager::RATE_SKILL);
	text << "\nMagic Rate: " << g_config.getNumber(ConfigManager::RATE_MAGIC);
	text << "\nLoot Rate: " << g_config.getNumber(ConfigManager::RATE_LOOT);
	player->sendTextMessage(MSG_STATUS_CONSOLE_BLUE, text.str());
}

void Commands::buyHouse(Player* player, const std::string& cmd, const std::string& param)
{
	if (!player->isPremium()) {
		player->sendCancelMessage(RET_YOUNEEDPREMIUMACCOUNT);
		return;
	}

	Position pos = player->getPosition();
	pos = getNextPosition(player->direction, pos);

	Tile* tile = g_game.getTile(pos.x, pos.y, pos.z);

	if (!tile) {
		player->sendCancel("You have to be looking at the door of the house you would like to buy.");
		return;
	}

	HouseTile* houseTile = dynamic_cast<HouseTile*>(tile);

	if (!houseTile) {
		player->sendCancel("You have to be looking at the door of the house you would like to buy.");
		return;
	}

	House* house = houseTile->getHouse();

	if (!house || !house->getDoorByPosition(pos)) {
		player->sendCancel("You have to be looking at the door of the house you would like to buy.");
		return;
	}

	if (house->getHouseOwner()) {
		player->sendCancel("This house alreadly has an owner.");
		return;
	}

	for (HouseMap::iterator it = Houses::getInstance().getHouseBegin(), end = Houses::getInstance().getHouseEnd(); it != end; ++it) {
		if (it->second->getHouseOwner() == player->guid) {
			player->sendCancel("You are already the owner of a house.");
			return;
		}
	}

	uint64_t price = 0;

	for (HouseTileList::iterator it = house->getHouseTileBegin(), end = house->getHouseTileEnd(); it != end; ++it) {
		price += g_config.getNumber(ConfigManager::HOUSE_PRICE);
	}

	if (!g_game.removeMoney(player, price)) {
		player->sendCancel("You do not have enough money.");
		return;
	}

	house->setHouseOwner(player->guid);
	player->sendTextMessage(MSG_INFO_DESCR, "You have successfully bought this house, be sure to have the money for the rent in the bank.");
}

void Commands::whoIsOnline(Player* player, const std::string& cmd, const std::string& param)
{
	std::ostringstream ss;
	ss << g_game.getPlayersOnline() << " players online.";
	player->sendTextMessage(MSG_STATUS_CONSOLE_BLUE, ss.str());
	ss.str("");

	uint32_t i = 0;

	const std::map<uint32_t, Player*>& players = g_game.getPlayers();

	auto it = players.begin();
	if(!g_config.getBoolean(ConfigManager::SHOW_GAMEMASTERS_ONLINE))
	{
		while(it != players.end())
		{
			Player* tmpPlayer = it->second;
			if(!tmpPlayer->isAccessPlayer() || player->isAccessPlayer())
			{
				ss << (i > 0 ? ", " : "") << tmpPlayer->name << " [" << tmpPlayer->level << "]";
				++i;
			}
			++it;

			if(i == 10)
			{
				ss << (it != players.end() ? "," : ".");
				player->sendTextMessage(MSG_STATUS_CONSOLE_BLUE, ss.str());
				ss.str("");
				i = 0;
			}
		}
	}
	else
	{
		while(it != players.end())
		{
			ss << (i > 0 ? ", " : "") << it->second->name << " [" << it->second->level << "]";
			++it;
			++i;

			if(i == 10)
			{
				ss << (it != players.end() ? "," : ".");
				player->sendTextMessage(MSG_STATUS_CONSOLE_BLUE, ss.str());
				ss.str("");
				i = 0;
			}
		}
	}

	if(i > 0)
	{
		ss << ".";
		player->sendTextMessage(MSG_STATUS_CONSOLE_BLUE, ss.str());
	}
}

void Commands::changeFloor(Player* player, const std::string& cmd, const std::string& param)
{
	Position newPos = player->getPosition();

	if (cmd[1] == 'u') {
		newPos.z--;
	} else {
		newPos.z++;
	}

	Position newPosition = g_game.getClosestFreeTile(player, 0, newPos, true);

	if (newPosition.x != 0) {
		Position oldPosition = player->getPosition();

		if (g_game.internalTeleport(player, newPosition) == RET_NOERROR) {
			g_game.addMagicEffect(oldPosition, NM_ME_POFF, player->isInGhostMode());
			g_game.addMagicEffect(player->getPosition(), NM_ME_TELEPORT, player->isInGhostMode());
			return;
		}
	}

	player->sendCancel("You can not teleport there.");
}

void Commands::showPosition(Player* player, const std::string& cmd, const std::string& param)
{
	if (param != "" && player->isAccessPlayer()) {
		StringVec exploded = explodeString(param, ", ", 2);

		if (!exploded.size() || exploded.size() < 3) {
			player->sendTextMessage(MSG_STATUS_CONSOLE_BLUE, "Not enough params.");
			return;
		}

		uint16_t x = atoi(exploded[0].c_str());
		uint16_t y = atoi(exploded[1].c_str());
		uint8_t z = atoi(exploded[2].c_str());

		Position oldPosition = player->getPosition();

		if (g_game.internalTeleport(player, Position(x, y, z)) == RET_NOERROR) {
			g_game.addMagicEffect(oldPosition, NM_ME_POFF, player->isInGhostMode());
			g_game.addMagicEffect(player->getPosition(), NM_ME_TELEPORT, player->isInGhostMode());
			return;
		}
	}

	const Position& pos = player->getPosition();

	std::ostringstream ss;
	ss << "Your current position is [X: " << pos.x << " | Y: " << pos.y << " | Z: " << pos.getZ() << "].";
	player->sendTextMessage(MSG_STATUS_CONSOLE_BLUE, ss.str());
}

void Commands::removeThing(Player* player, const std::string& cmd, const std::string& param)
{
	Position pos = player->getPosition();
	pos = getNextPosition(player->direction, pos);
	Tile* removeTile = g_game.getMap()->getTile(pos);

	if (!removeTile) {
		player->sendTextMessage(MSG_STATUS_SMALL, "Tile not found.");
		g_game.addMagicEffect(pos, NM_ME_POFF);
		return;
	}

	Thing* thing = removeTile->getTopVisibleThing(player);

	if (!thing) {
		player->sendTextMessage(MSG_STATUS_SMALL, "Object not found.");
		g_game.addMagicEffect(pos, NM_ME_POFF);
		return;
	}

	if (Creature* creature = thing->getCreature()) {
		g_game.removeCreature(creature, true);
	} else {
		Item* item = thing->getItem();
		if (item) {
			if (item->isGroundTile()) {
				player->sendTextMessage(MSG_STATUS_SMALL, "You may not remove a ground tile.");
				g_game.addMagicEffect(pos, NM_ME_POFF);
				return;
			}

			g_game.internalRemoveItem(item, std::max<int32_t>(1, std::min<int32_t>(atoi(param.c_str()), item->getItemCount())));
			g_game.addMagicEffect(pos, NM_ME_MAGIC_BLOOD);
		}
	}
}

void Commands::newType(Player* player, const std::string& cmd, const std::string& param)
{
	int32_t lookType = atoi(param.c_str());

	if (lookType >= 0 && lookType != 1 && lookType != 135 && lookType != 411 && lookType != 415 && lookType != 424 && (lookType <= 160 || lookType >= 192) && lookType != 439 && lookType != 440 && lookType != 468 && lookType != 469 && lookType <= 573 && (lookType < 474 || lookType > 485) && lookType != 518 && lookType != 519 && lookType != 520 && lookType != 524 && lookType != 525 && lookType != 536 && lookType != 543 && lookType != 549) {
		Outfit_t newOutfit = player->getDefaultOutfit();
		newOutfit.lookType = lookType;
		g_game.internalCreatureChangeOutfit(player, newOutfit);
	} else {
		player->sendTextMessage(MSG_STATUS_SMALL, "This looktype does not exist.");
	}
}

void Commands::hide(Player* player, const std::string& cmd, const std::string& param)
{
	player->setHiddenHealth(!player->isHealthHidden());
	g_game.addCreatureHealth(player);
}

void Commands::newItem(Player* player, const std::string& cmd, const std::string& param)
{
	int32_t itemId = atoi(param.c_str());

	for (uint32_t i = 0; i < param.length(); i++) {
		if (!isNumber(param[i])) {
			itemId = Item::items.getItemIdByName(param);
			break;
		}
	}

	if (itemId <= 0) {
		return;
	}

	const ItemType& it = Item::items[itemId];
	if (it.id == 0) {
		return;
	}

	Outfit_t outfit;
	outfit.lookTypeEx = itemId;

	ConditionOutfit* outfitCondition = new ConditionOutfit(CONDITIONID_COMBAT, CONDITION_OUTFIT, -1);
	outfitCondition->addOutfit(outfit);
	player->addCondition(outfitCondition);
}

void Commands::forceRaid(Player* player, const std::string& cmd, const std::string& param)
{
	Raid* raid = Raids::getInstance()->getRaidByName(param);

	if (!raid || !raid->isLoaded()) {
		player->sendTextMessage(MSG_STATUS_CONSOLE_BLUE, "No such raid exists.");
		return;
	}

	if (Raids::getInstance()->getRunning()) {
		player->sendTextMessage(MSG_STATUS_CONSOLE_BLUE, "Another raid is already being executed.");
		return;
	}

	Raids::getInstance()->setRunning(raid);

	RaidEvent* event = raid->getNextRaidEvent();

	if (!event) {
		player->sendTextMessage(MSG_STATUS_CONSOLE_BLUE, "The raid does not contain any data.");
		return;
	}

	raid->setState(RAIDSTATE_EXECUTING);

	uint32_t ticks = event->getDelay();

	if (ticks > 0) {
		g_scheduler.addEvent(createSchedulerTask(ticks,
		                     boost::bind(&Raid::executeRaidEvent, raid, event)));
	} else {
		g_dispatcher.addTask(createTask(
		                         boost::bind(&Raid::executeRaidEvent, raid, event)));
	}

	player->sendTextMessage(MSG_STATUS_CONSOLE_BLUE, "Raid started.");
}

void Commands::addSkill(Player* player, const std::string& cmd, const std::string& param)
{
	boost::char_separator<char> sep(",");
	tokenizer cmdtokens(param, sep);
	tokenizer::iterator cmdit = cmdtokens.begin();
	std::string param1, param2;
	param1 = parseParams(cmdit, cmdtokens.end());
	param2 = parseParams(cmdit, cmdtokens.end());
	trimString(param1);
	trimString(param2);
	toLowerCaseString(param2);

	Player* paramPlayer = g_game.getPlayerByName(param1);

	if (!paramPlayer) {
		player->sendTextMessage(MSG_STATUS_SMALL, "Couldn't find target.");
		return;
	}

	if (param2[0] == 'l' || param2[0] == 'e') {
		paramPlayer->addExperience(Player::getExpForLevel(paramPlayer->getLevel() + 1) - paramPlayer->experience, false, false);
	} else if (param2[0] == 'm') {
		paramPlayer->addManaSpent(paramPlayer->vocation->getReqMana(paramPlayer->getBaseMagicLevel() + 1) - paramPlayer->manaSpent, false);
	} else {
		skills_t skillId = getSkillId(param2);
		paramPlayer->addSkillAdvance(skillId, paramPlayer->vocation->getReqSkillTries(skillId, paramPlayer->getSkill(skillId, SKILL_LEVEL) + 1));
	}
}

void Commands::playerKills(Player* player, const std::string& cmd, const std::string& param)
{
	int32_t fragTime = g_config.getNumber(ConfigManager::FRAG_TIME);

	if (player->skullTicks && fragTime > 0) {
		int32_t frags = (int32_t)ceil(player->skullTicks / (double)fragTime);
		int32_t remainingTime = (player->skullTicks % fragTime) / 1000;
		int32_t hours = remainingTime / 3600;
		int32_t minutes = (remainingTime % 3600) / 60;

		std::ostringstream ss;
		ss << "You have " << frags << " unjustified kill" << (frags > 1 ? "s" : "") << ". The amount of unjustified kills will decrease after: " << hours << " hour" << (hours != 1 ? "s" : "") << " and " << minutes << " minute" << (minutes != 1 ? "s" : "") << ".";
		player->sendTextMessage(MSG_STATUS_CONSOLE_BLUE, ss.str());
	} else {
		player->sendTextMessage(MSG_STATUS_CONSOLE_BLUE, "You do not have any unjustified frag.");
	}
}

void Commands::clean(Player* player, const std::string& cmd, const std::string& param)
{
	uint32_t count = g_game.getMap()->clean();

	if (count != 1) {
		std::ostringstream ss;
		ss << "Cleaned " << count << " items from the map.";
		g_game.broadcastMessage(ss.str(), MSG_STATUS_WARNING);
	} else {
		g_game.broadcastMessage("Cleaned 1 item from the map.", MSG_STATUS_WARNING);
	}
}

void Commands::serverDiag(Player* player, const std::string& cmd, const std::string& param)
{
#ifdef __ENABLE_SERVER_DIAGNOSTIC__
	std::ostringstream text;
	text << "Server diagonostic:\n";
	text << "World:" << "\n";
	text << "Player: " << g_game.getPlayersOnline() << " (" << Player::playerCount << ")\n";
	text << "Npc: " << g_game.getNpcsOnline() << " (" << Npc::npcCount << ")\n";
	text << "Monster: " << g_game.getMonstersOnline() << " (" << Monster::monsterCount << ")\n";

	text << "\nProtocols:" << "\n";
	text << "--------------------\n";
	text << "ProtocolGame: " << ProtocolGame::protocolGameCount << "\n";
	text << "ProtocolLogin: " << ProtocolLogin::protocolLoginCount << "\n";
	text << "ProtocolAdmin: " << ProtocolAdmin::protocolAdminCount << "\n";
	text << "ProtocolStatus: " << ProtocolStatus::protocolStatusCount << "\n\n";

	text << "\nConnections:\n";
	text << "--------------------\n";
	text << "Active connections: " << Connection::connectionCount << "\n";
	text << "Total message pool: " << OutputMessagePool::getInstance()->getTotalMessageCount() << "\n";
	text << "Auto message pool: " << OutputMessagePool::getInstance()->getAutoMessageCount() << "\n";
	text << "Free message pool: " << OutputMessagePool::getInstance()->getAvailableMessageCount() << "\n";

	text << "\nLibraries:\n";
	text << "--------------------\n";
	text << "asio: " << BOOST_ASIO_VERSION << "\n";
	text << "libxml: " << XML_DEFAULT_VERSION << "\n";
	text << "lua: " << LUA_VERSION << "\n";

	player->sendTextMessage(MSG_STATUS_CONSOLE_BLUE, text.str().c_str());
#else
	player->sendTextMessage(MSG_STATUS_CONSOLE_BLUE, "This command requires the server to be compiled with the __ENABLE_SERVER_DIAGNOSTIC__ flag.");
#endif
}

void Commands::ghost(Player* player, const std::string& cmd, const std::string& param)
{
	player->switchGhostMode();

	SpectatorVec list;
	g_game.getSpectators(list, player->getPosition(), true, true);

	for (SpectatorVec::const_iterator it = list.begin(), end = list.end(); it != end; ++it) {
		Player* tmpPlayer = (*it)->getPlayer();
		tmpPlayer->sendCreatureChangeVisible(player, !player->isInGhostMode());

		if (tmpPlayer != player && !tmpPlayer->isAccessPlayer()) {
			if (player->isInGhostMode()) {
				tmpPlayer->sendCreatureDisappear(player, player->getTile()->getClientIndexOfThing(tmpPlayer, player), true);
			} else {
				tmpPlayer->sendCreatureAppear(player, player->getPosition(), true);
			}

			tmpPlayer->sendUpdateTile(player->getTile(), player->getPosition());
		}
	}

	if (player->isInGhostMode()) {
		const std::map<uint32_t, Player*>& players = g_game.getPlayers();
		for (auto it = players.begin(); it != players.end(); ++it) {
			if (!it->second->isAccessPlayer()) {
				it->second->notifyStatusChange(player, VIPSTATUS_OFFLINE);
			}
		}

		IOLoginData::getInstance()->updateOnlineStatus(player->getGUID(), false);
		player->sendTextMessage(MSG_INFO_DESCR, "You are now invisible.");
		g_game.addMagicEffect(list, player->getPosition(), NM_ME_YALAHARIGHOST);
	} else {
		const std::map<uint32_t, Player*>& players = g_game.getPlayers();
		for (auto it = players.begin(); it != players.end(); ++it) {
			if (!it->second->isAccessPlayer()) {
				it->second->notifyStatusChange(player, VIPSTATUS_ONLINE);
			}
		}

		IOLoginData::getInstance()->updateOnlineStatus(player->getGUID(), true);
		player->sendTextMessage(MSG_INFO_DESCR, "You are visible again.");
		Position pos = player->getPosition();
		pos.x += 1;
		g_game.addMagicEffect(list, pos, NM_ME_SMOKE);
	}
}

void Commands::multiClientCheck(Player* player, const std::string& cmd, const std::string& param)
{
	player->sendTextMessage(MSG_STATUS_CONSOLE_BLUE, "Multiclient Check List:");
	std::map< uint32_t, std::vector<Player*> > ipMap;

	const std::map<uint32_t, Player*>& players = g_game.getPlayers();
	for (auto it = players.begin(); it != players.end(); ++it) {
		Player* tmpPlayer = it->second;
		if (tmpPlayer->isRemoved() || tmpPlayer->getIP() == 0) {
			continue;
		}

		ipMap[tmpPlayer->getIP()].push_back(tmpPlayer);
	}

	for (auto it = ipMap.begin(), end = ipMap.end(); it != end; ++it) {
		if (it->second.size() < 2) {
			continue;
		}

		Player* tmpPlayer = it->second[0];
		std::ostringstream ss;
		ss << convertIPToString(it->first) << ": " << tmpPlayer->getName() << " [" << tmpPlayer->getLevel() << "]";

		for (std::vector<Player*>::size_type i = 1, size = it->second.size(); i < size; ++i) {
			tmpPlayer = it->second[i];
			ss << ", " << tmpPlayer->getName() << " [" << tmpPlayer->getLevel() << "]";
		}

		ss << ".";
		player->sendTextMessage(MSG_STATUS_CONSOLE_BLUE, ss.str());
	}
}

void Commands::addTutor(Player* player, const std::string& cmd, const std::string& param)
{
	uint32_t accountId = 0;
	std::string characterName = param;

	Player* targetPlayer = g_game.getPlayerByName(characterName);

	if (targetPlayer) {
		targetPlayer->accountType = ACCOUNT_TYPE_TUTOR;
		accountId = targetPlayer->getAccount();
		characterName = targetPlayer->getName();
	} else {
		accountId = IOLoginData::getInstance()->getAccountNumberByName(characterName);
		uint32_t guid;
		IOLoginData::getInstance()->getGuidByName(guid, characterName);
	}

	if (accountId != 0 && IOLoginData::getInstance()->getAccountType(accountId) == ACCOUNT_TYPE_NORMAL) {
		IOLoginData::getInstance()->setAccountType(accountId, ACCOUNT_TYPE_TUTOR);
		player->sendTextMessage(MSG_INFO_DESCR, characterName + (" is now a tutor."));
	} else {
		player->sendTextMessage(MSG_INFO_DESCR, "A character with that name does not exist, or is already a tutor.");
	}
}

void Commands::removeTutor(Player* player, const std::string& cmd, const std::string& param)
{
	uint32_t accountId = 0;
	std::string characterName = param;

	Player* targetPlayer = g_game.getPlayerByName(characterName);

	if (targetPlayer) {
		targetPlayer->accountType = ACCOUNT_TYPE_NORMAL;
		accountId = targetPlayer->getAccount();
		characterName = targetPlayer->getName();
	} else {
		accountId = IOLoginData::getInstance()->getAccountNumberByName(characterName);
		uint32_t guid;
		IOLoginData::getInstance()->getGuidByName(guid, characterName);
	}

	if (accountId != 0 && IOLoginData::getInstance()->getAccountType(accountId) == ACCOUNT_TYPE_TUTOR) {
		IOLoginData::getInstance()->setAccountType(accountId, ACCOUNT_TYPE_NORMAL);
		player->sendTextMessage(MSG_INFO_DESCR, characterName + (" is no longer a tutor."));
	} else {
		player->sendTextMessage(MSG_INFO_DESCR, "A character with that name does not exist, or is not a tutor.");
	}
}
