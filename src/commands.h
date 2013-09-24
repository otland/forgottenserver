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

#ifndef __OTSERV_COMMANDS_H__
#define __OTSERV_COMMANDS_H__

#include <string>
#include <map>
#include "creature.h"

struct Command;
struct s_defcommands;

class Commands
{
	public:
		Commands();
		~Commands();

		bool loadFromXml();
		bool reload();

		Command* getCommand(const std::string& cmd);
		bool exeCommand(Player* player, const std::string& cmd);

	protected:
		bool loaded;

		//commands
		void placeNpc(Player* player, const std::string& cmd, const std::string& param);
		void placeMonster(Player* player, const std::string& cmd, const std::string& param);
		void placeSummon(Player* player, const std::string& cmd, const std::string& param);
		void broadcastMessage(Player* player, const std::string& cmd, const std::string& param);
		void teleportHere(Player* player, const std::string& cmd, const std::string& param);
		void teleportToTown(Player* player, const std::string& cmd, const std::string& param);
		void teleportTo(Player* player, const std::string& cmd, const std::string& param);
		void createItemById(Player* player, const std::string& cmd, const std::string& param);
		void createItemByName(Player* player, const std::string& cmd, const std::string& param);
		void reloadInfo(Player* player, const std::string& cmd, const std::string& param);
		void getInfo(Player* player, const std::string& cmd, const std::string& param);
		void closeServer(Player* player, const std::string& cmd, const std::string& param);
		void openServer(Player* player, const std::string& cmd, const std::string& param);
		void teleportNTiles(Player* player, const std::string& cmd, const std::string& param);
		void kickPlayer(Player* player, const std::string& cmd, const std::string& param);
		void setHouseOwner(Player* player, const std::string& cmd, const std::string& param);
		void sellHouse(Player* player, const std::string& cmd, const std::string& param);
		void serverInfo(Player* player, const std::string& cmd, const std::string& param);
		void whoIsOnline(Player* player, const std::string& cmd, const std::string& param);
		void showPosition(Player* player, const std::string& cmd, const std::string& param);
		void removeThing(Player* player, const std::string& cmd, const std::string& param);
		void buyHouse(Player* player, const std::string& cmd, const std::string& param);
		void newType(Player* player, const std::string& cmd, const std::string& param);
		void forceRaid(Player* player, const std::string& cmd, const std::string& param);
		void addSkill(Player* player, const std::string& cmd, const std::string& param);
		void clean(Player* player, const std::string& cmd, const std::string& param);
		void serverDiag(Player* player, const std::string& cmd, const std::string& param);
		void ghost(Player* player, const std::string& cmd, const std::string& param);
		void multiClientCheck(Player* player, const std::string& cmd, const std::string& param);
		void newItem(Player* player, const std::string& cmd, const std::string& param);
		void hide(Player* player, const std::string& cmd, const std::string& param);
		void addTutor(Player* player, const std::string& cmd, const std::string& param);
		void removeTutor(Player* player, const std::string& cmd, const std::string& param);

		//table of commands
		static s_defcommands defined_commands[];

		std::map<std::string, Command*> commandMap;
};

typedef void (Commands::*CommandFunc)(Player*, const std::string&, const std::string&);

struct Command {
	CommandFunc f;
	uint32_t groupId;
	AccountType_t accountType;
	bool loadedGroupId;
	bool loadedAccountType;
	bool logged;
	bool loadedLogging;
};

struct s_defcommands {
	const char* name;
	CommandFunc f;
};

#endif
