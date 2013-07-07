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

	for (PlayerVector::const_iterator it = membersOnline.begin(); it != membersOnline.end(); ++it) {
		g_game.updatePlayerHelpers(*it);
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

	for (PlayerVector::const_iterator it = membersOnline.begin(); it != membersOnline.end(); ++it) {
		g_game.updatePlayerHelpers(*it);
	}

	g_game.updatePlayerHelpers(player);
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
	ranks.push_back(GuildRank(id, name, level));
}
