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

#include "guild.h"

#include "game.h"

extern Game g_game;

void Guild::addMember(Player* player)
{
	membersOnline.push_back(player);
	for (Player* member : membersOnline) {
		g_game.updatePlayerHelpers(*member);
	}
}

void Guild::removeMember(Player* player)
{
	membersOnline.remove(player);
	for (Player* member : membersOnline) {
		g_game.updatePlayerHelpers(*member);
	}
	g_game.updatePlayerHelpers(*player);

	if (membersOnline.empty()) {
		g_game.removeGuild(id);
		delete this;
	}
}

GuildRank_ptr Guild::getRankById(uint32_t rankId)
{
	for (auto rank : ranks) {
		if (rank->id == rankId) {
			return rank;
		}
	}
	return nullptr;
}

GuildRank_ptr Guild::getRankByName(const std::string& name) const
{
	for (auto rank : ranks) {
		if (rank->name == name) {
			return rank;
		}
	}
	return nullptr;
}

GuildRank_ptr Guild::getRankByLevel(uint8_t level) const
{
	for (auto rank : ranks) {
		if (rank->level == level) {
			return rank;
		}
	}
	return nullptr;
}

void Guild::addRank(uint32_t rankId, const std::string& rankName, uint8_t level)
{
	ranks.emplace_back(std::make_shared<GuildRank>(rankId, rankName, level));
}
