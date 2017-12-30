/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2017  Mark Samman <mark.samman@gmail.com>
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

class Commands
{
	public:
		Commands();
		~Commands();

		// non-copyable
		Commands(const Commands&) = delete;
		Commands& operator=(const Commands&) = delete;

		bool loadFromXml();
		bool reload();

		bool exeCommand(Player& player, const std::string& cmd);

	protected:
		std::map<std::string, Command*> commandMap;
};

using CommandFunction = std::function<void(Player&, const std::string&)>;

struct Command {
	Command(CommandFunction f, uint32_t groupId, AccountType_t accountType, bool log)
		: f(f), groupId(groupId), accountType(accountType), log(log) {}

	CommandFunction f;
	uint32_t groupId;
	AccountType_t accountType;
	bool log;
};

#endif
