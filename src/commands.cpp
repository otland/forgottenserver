/**
 * The Forgotten Server - a server application for the MMORPG Tibia
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
#include "protocolstatus.h"
#include "protocollogin.h"
#endif

#include "pugicast.h"

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
	{"/c", &Commands::teleportHere},
	{"/i", &Commands::createItemById},
	{"/n", &Commands::createItemByName},
	{"/reload", &Commands::reloadInfo},
	{"/goto", &Commands::teleportTo},
	{"/info", &Commands::getInfo},
	{"/closeserver", &Commands::closeServer},
	{"/openserver", &Commands::openServer},
	{"/a", &Commands::teleportNTiles},
	{"/kick", &Commands::kickPlayer},
	{"/owner", &Commands::setHouseOwner},
	{"/town", &Commands::teleportToTown},
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
	{"!buyhouse", &Commands::buyHouse},
	{"!sellhouse", &Commands::sellHouse},
	{"!serverinfo", &Commands::serverInfo}
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
	for (const auto& it : commandMap) {
		delete it.second;
	}
}

bool Commands::loadFromXml()
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file("data/XML/commands.xml");
	if (!result) {
		std::cout << "[Error - Commands::loadFromXml] Failed to load data/XML/commands.xml: " << result.description() << std::endl;
		return false;
	}

	loaded = true;

	for (pugi::xml_node commandNode = doc.child("commands").first_child(); commandNode; commandNode = commandNode.next_sibling()) {
		pugi::xml_attribute cmdAttribute = commandNode.attribute("cmd");
		if (!cmdAttribute) {
			std::cout << "[Warning - Commands::loadFromXml] Missing cmd" << std::endl;
			continue;
		}

		auto it = commandMap.find(cmdAttribute.as_string());
		if (it == commandMap.end()) {
			std::cout << "[Warning - Commands::loadFromXml] Unknown command " << cmdAttribute.as_string() << std::endl;
			continue;
		}

		Command* command = it->second;

		pugi::xml_attribute groupAttribute = commandNode.attribute("group");
		if (groupAttribute) {
			if (!command->loadedGroupId) {
				command->groupId = pugi::cast<uint32_t>(groupAttribute.value());
				command->loadedGroupId = true;
			} else {
				std::cout << "[Notice - Commands::loadFromXml] Duplicate command: " << it->first << std::endl;
			}
		}

		pugi::xml_attribute acctypeAttribute = commandNode.attribute("acctype");
		if (acctypeAttribute) {
			if (!command->loadedAccountType) {
				command->accountType = (AccountType_t)pugi::cast<uint32_t>(acctypeAttribute.value());
				command->loadedAccountType = true;
			} else {
				std::cout << "[Notice - Commands::loadFromXml] Duplicate command: " << it->first << std::endl;
			}
		}

		pugi::xml_attribute logAttribute = commandNode.attribute("log");
		if (logAttribute) {
			if (!command->loadedLogging) {
				command->logged = booleanString(logAttribute.as_string());
				command->loadedLogging = true;
			} else {
				std::cout << "[Notice - Commands::loadFromXml] Duplicate log tag for: " << it->first << std::endl;
			}
		}
	}

	for (const auto& it : commandMap) {
		Command* command = it.second;
		if (!command->loadedGroupId) {
			std::cout << "[Warning - Commands::loadFromXml] Missing group id for command " << it.first << std::endl;
		}

		if (!command->loadedAccountType) {
			std::cout << "[Warning - Commands::loadFromXml] Missing acctype level for command " << it.first << std::endl;
		}

		if (!command->loadedLogging) {
			std::cout << "[Warning - Commands::loadFromXml] Missing log command " << it.first << std::endl;
		}

		g_game.addCommandTag(it.first[0]);
	}
	return loaded;
}

bool Commands::reload()
{
	loaded = false;

	for (const auto& it : commandMap) {
		Command* command = it.second;
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

Command* Commands::getCommand(const std::string& cmd)
{
	auto it = commandMap.find(cmd);
	if (it == commandMap.end()) {
		return nullptr;
	}
	return it->second;
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
	}

	//find command
	auto it = commandMap.find(str_command);
	if (it == commandMap.end()) {
		return false;
	}

	Command* command = it->second;
	if (command->groupId > player->group->id || command->accountType > player->accountType) {
		if (player->group->access) {
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
			time_t ticks = time(nullptr);
			const tm* now = localtime(&ticks);
			char buf[32];
			strftime(buf, sizeof(buf), "%d/%m/%Y %H:%M", now);

			out << '[' << buf << "] " << cmd << std::endl;
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

void Commands::teleportHere(Player* player, const std::string& cmd, const std::string& param)
{
	Creature* paramCreature = g_game.getCreatureByName(param);
	if (paramCreature) {
		Position oldPosition = paramCreature->getPosition();
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
	std::string::size_type pos1 = param.find('"');
	pos1 = (pos1 == std::string::npos ? 0 : pos1 + 1);

	std::string::size_type pos2 = param.rfind('"');
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
		g_weapons->loadDefaults();
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
	Town* town = Towns::getInstance().getTown(param);
	if (town) {
		Position oldPosition = player->getPosition();
		Position newPosition = g_game.getClosestFreeTile(player, 0, town->getTemplePosition(), true);
		if (oldPosition != newPosition) {
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
				bool ghostMode = player->isInGhostMode() || paramCreature->isInGhostMode();
				g_game.addMagicEffect(oldPosition, NM_ME_POFF, ghostMode);
				g_game.addMagicEffect(player->getPosition(), NM_ME_TELEPORT, ghostMode);
			}
		} else {
			std::ostringstream ss;
			ss << "You can not teleport to " << paramCreature->getName() << '.';
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
	     "Access: " << paramPlayer->group->access << std::endl <<
	     "Level: " << paramPlayer->level << std::endl <<
	     "Magic Level: " << paramPlayer->magLevel << std::endl <<
	     "Speed: " << paramPlayer->getSpeed() << std::endl <<
	     "Position: " << paramPlayer->getPosition() << std::endl <<
	     "IP: " << convertIPToString(playerIp) << std::endl;
	player->sendTextMessage(MSG_STATUS_CONSOLE_BLUE, info.str());

	if (playerIp != 0) {
		PlayerVector vec;
		for (const auto& it : g_game.getPlayers()) {
			if (it.second->getIP() != playerIp || it.second == paramPlayer) {
				continue;
			}

			vec.push_back(it.second);
		}

		if (!vec.empty()) {
			std::ostringstream ss;

			Player* tmpPlayer = vec[0];
			ss << "Other players on same IP: " << tmpPlayer->getName() << " [" << tmpPlayer->getLevel() << ']';

			for (PlayerVector::size_type i = 1, size = vec.size(); i < size; ++i) {
				tmpPlayer = vec[i];
				ss << ", " << tmpPlayer->getName() << " [" << tmpPlayer->getLevel() << ']';
			}

			ss << '.';
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
	if (ntiles == 0) {
		return;
	}

	Position oldPosition = player->getPosition();
	Position newPos = oldPosition;

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

void Commands::kickPlayer(Player* player, const std::string& cmd, const std::string& param)
{
	Player* playerKick = g_game.getPlayerByName(param);
	if (playerKick) {
		if (playerKick->group->access) {
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
			} else if (IOLoginData::getGuidByName(guid, name)) {
				houseTile->getHouse()->setHouseOwner(guid);
			} else {
				player->sendTextMessage(MSG_STATUS_CONSOLE_BLUE, "Player not found.");
			}
		}
	}
}

void Commands::sellHouse(Player* player, const std::string& cmd, const std::string& param)
{
	Player* tradePartner = g_game.getPlayerByName(param);
	if (!tradePartner || tradePartner == player) {
		player->sendCancel("Trade player not found.");
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

	HouseTile* houseTile = dynamic_cast<HouseTile*>(player->getTile());
	if (!houseTile) {
		player->sendCancel("You must stand in your house to initiate the trade.");
		return;
	}

	House* house = houseTile->getHouse();
	if (!house || house->getHouseOwner() != player->guid) {
		player->sendCancel("You don't own this house.");
		return;
	}

	if (Houses::getInstance().getHouseByPlayerId(tradePartner->guid)) {
		player->sendCancel("Trade player already owns a house.");
		return;
	}

	if (IOLoginData::hasBiddedOnHouse(tradePartner->guid)) {
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

	Tile* tile = g_game.getTile(pos);
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

	for (const auto& it : Houses::getInstance().getHouses()) {
		if (it.second->getHouseOwner() == player->guid) {
			player->sendCancel("You are already the owner of a house.");
			return;
		}
	}

	uint64_t price = house->getTiles().size() * g_config.getNumber(ConfigManager::HOUSE_PRICE);
	if (!g_game.removeMoney(player, price)) {
		player->sendCancel("You do not have enough money.");
		return;
	}

	house->setHouseOwner(player->guid);
	player->sendTextMessage(MSG_INFO_DESCR, "You have successfully bought this house, be sure to have the money for the rent in the bank.");
}

void Commands::showPosition(Player* player, const std::string& cmd, const std::string& param)
{
	if (!param.empty() && player->isAccessPlayer()) {
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

	if (lookType >= 0 && lookType != 1 && lookType != 135 && lookType != 411 && lookType != 415 && lookType != 424 && (lookType <= 160 || lookType >= 192) && lookType != 439 && lookType != 440 && lookType != 468 && lookType != 469 && (lookType < 474 || lookType > 485) && lookType != 518 && lookType != 519 && lookType != 520 && lookType != 524 && lookType != 525 && lookType != 536 && lookType != 543 && lookType != 549 && lookType <= 575) {
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
	Tokenizer tokens(param, boost::char_separator<char>(","));

	auto it = tokens.begin();
	std::string playerName = parseNextParam(it, tokens.end());

	Player* paramPlayer = g_game.getPlayerByName(playerName);
	if (!paramPlayer) {
		player->sendTextMessage(MSG_STATUS_SMALL, "Couldn't find target.");
		return;
	}

	std::string skill = asLowerCaseString(parseNextParam(it, tokens.end()));
	if (!skill.empty() && (skill[0] == 'l' || skill[0] == 'e')) {
		paramPlayer->addExperience(Player::getExpForLevel(paramPlayer->getLevel() + 1) - paramPlayer->experience, false, false);
	} else if (!skill.empty() && skill[0] == 'm') {
		paramPlayer->addManaSpent(paramPlayer->vocation->getReqMana(paramPlayer->getBaseMagicLevel() + 1) - paramPlayer->manaSpent, false);
	} else {
		skills_t skillId = getSkillId(skill);
		paramPlayer->addSkillAdvance(skillId, paramPlayer->vocation->getReqSkillTries(skillId, paramPlayer->getSkill(skillId, SKILL_LEVEL) + 1));
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
	text << "World:\n";
	text << "Player: " << g_game.getPlayersOnline() << " (" << Player::playerCount << ")\n";
	text << "Npc: " << g_game.getNpcsOnline() << " (" << Npc::npcCount << ")\n";
	text << "Monster: " << g_game.getMonstersOnline() << " (" << Monster::monsterCount << ")\n";

	text << "\nProtocols:\n";
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
	text << "lua: " << LUA_VERSION << "\n";
	text << "pugixml: " << PUGIXML_VERSION << "\n";

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
	for (Creature* spectator : list) {
		Player* tmpPlayer = spectator->getPlayer();
		if (tmpPlayer != player && !tmpPlayer->isAccessPlayer()) {
			if (player->isInGhostMode()) {
				tmpPlayer->sendCreatureDisappear(player, player->getTile()->getClientIndexOfThing(tmpPlayer, player));
			} else {
				tmpPlayer->sendCreatureAppear(player, player->getPosition(), true);
			}
		} else {
			tmpPlayer->sendCreatureChangeVisible(player, !player->isInGhostMode());
		}
	}

	if (player->isInGhostMode()) {
		for (const auto& it : g_game.getPlayers()) {
			if (!it.second->isAccessPlayer()) {
				it.second->notifyStatusChange(player, VIPSTATUS_OFFLINE);
			}
		}

		IOLoginData::updateOnlineStatus(player->getGUID(), false);
		player->sendTextMessage(MSG_INFO_DESCR, "You are now invisible.");
		g_game.addMagicEffect(list, player->getPosition(), NM_ME_YALAHARIGHOST);
	} else {
		for (const auto& it : g_game.getPlayers()) {
			if (!it.second->isAccessPlayer()) {
				it.second->notifyStatusChange(player, VIPSTATUS_ONLINE);
			}
		}

		IOLoginData::updateOnlineStatus(player->getGUID(), true);
		player->sendTextMessage(MSG_INFO_DESCR, "You are visible again.");
		Position pos = player->getPosition();
		pos.x += 1;
		g_game.addMagicEffect(list, pos, NM_ME_SMOKE);
	}
}

void Commands::multiClientCheck(Player* player, const std::string& cmd, const std::string& param)
{
	player->sendTextMessage(MSG_STATUS_CONSOLE_BLUE, "Multiclient Check List:");
	std::map<uint32_t, std::vector<Player*>> ipMap;

	for (const auto& it : g_game.getPlayers()) {
		Player* tmpPlayer = it.second;
		if (tmpPlayer->getIP() == 0) {
			continue;
		}

		ipMap[tmpPlayer->getIP()].push_back(tmpPlayer);
	}

	for (const auto& it : ipMap) {
		if (it.second.size() < 2) {
			continue;
		}

		Player* tmpPlayer = it.second[0];
		std::ostringstream ss;
		ss << convertIPToString(it.first) << ": " << tmpPlayer->getName() << " [" << tmpPlayer->getLevel() << ']';

		for (std::vector<Player*>::size_type i = 1, size = it.second.size(); i < size; ++i) {
			tmpPlayer = it.second[i];
			ss << ", " << tmpPlayer->getName() << " [" << tmpPlayer->getLevel() << ']';
		}

		ss << '.';
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
		accountId = IOLoginData::getAccountNumberByName(characterName);
		uint32_t guid;
		IOLoginData::getGuidByName(guid, characterName);
	}

	if (accountId != 0 && IOLoginData::getAccountType(accountId) == ACCOUNT_TYPE_NORMAL) {
		IOLoginData::setAccountType(accountId, ACCOUNT_TYPE_TUTOR);
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
		accountId = IOLoginData::getAccountNumberByName(characterName);
		uint32_t guid;
		IOLoginData::getGuidByName(guid, characterName);
	}

	if (accountId != 0 && IOLoginData::getAccountType(accountId) == ACCOUNT_TYPE_TUTOR) {
		IOLoginData::setAccountType(accountId, ACCOUNT_TYPE_NORMAL);
		player->sendTextMessage(MSG_INFO_DESCR, characterName + (" is no longer a tutor."));
	} else {
		player->sendTextMessage(MSG_INFO_DESCR, "A character with that name does not exist, or is not a tutor.");
	}
}
