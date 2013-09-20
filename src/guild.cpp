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

#include "guild.h"

#include "game.h"

extern Game g_game;

Guild::Guild(uint32_t id, const std::string& name)
{
	this->id = id;
	this->name = name;
	this->memberCount = 0;
}

void Guild::addMember(Player* player)
{
	membersOnline.push_back(player);
	for (Player* member : membersOnline) {
		g_game.updatePlayerHelpers(*member);
	}
}

void Guild::removeMember(Player* player)
{
	for (PlayerVector::iterator it = membersOnline.begin(); it != membersOnline.end(); ++it) {
		if (*it == player) {
			membersOnline.erase(it);
			break;
		}
	}

	for (Player* member : membersOnline) {
		g_game.updatePlayerHelpers(*member);
	}

	g_game.updatePlayerHelpers(*player);
}

GuildRank* Guild::getRankById(uint32_t id)
{
	for (size_t i = 0; i < ranks.size(); ++i) {
		if (ranks[i].id == id) {
			return &ranks[i];
		}
	}
	return NULL;
}

GuildRank* Guild::getRankByLevel(uint8_t level)
{
	for (size_t i = 0; i < ranks.size(); ++i) {
		if (ranks[i].level == level) {
			return &ranks[i];
		}
	}
	return NULL;
}

void Guild::addRank(uint32_t id, const std::string& name, uint8_t level)
{
	ranks.emplace_back(id, name, level);
}
