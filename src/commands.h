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

#ifndef FS_COMMANDS_H_C95A575CCADF434699D26CD042690970
#define FS_COMMANDS_H_C95A575CCADF434699D26CD042690970

#include "enums.h"

class Player;

struct Command;
struct s_defcommands;

class Commands
{
	public:
		Commands();
		~Commands();

		bool loadFromXml();
		bool reload();

		bool exeCommand(Player& player, const std::string& cmd);

	protected:
		//commands
		void reloadInfo(Player& player, const std::string& param);
		void sellHouse(Player& player, const std::string& param);
		void forceRaid(Player& player, const std::string& param);

		//table of commands
		static s_defcommands defined_commands[];

		std::map<std::string, Command*> commandMap;
		bool loaded;
};

typedef void (Commands::*CommandFunc)(Player&, const std::string&);

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
