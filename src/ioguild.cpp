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

#include "ioguild.h"
#include "database.h"

bool IOGuild::getGuildIdByName(uint32_t& guildId, const std::string& guildName)
{
	Database* db = Database::getInstance();

	DBQuery query;
	query << "SELECT `id` FROM `guilds` WHERE `name` = " << db->escapeString(guildName);

	DBResult* result = db->storeQuery(query.str());
	if (!result) {
		return false;
	}

	guildId = result->getDataInt("id");
	db->freeResult(result);
	return true;
}

void IOGuild::getWarList(uint32_t guildId, GuildWarList& guildWarList)
{
	Database* db = Database::getInstance();

	DBQuery query;
	query << "SELECT `guild1`, `guild2` FROM `guild_wars` WHERE (`guild1` = " << guildId << " OR `guild2` = " << guildId << ") AND `ended` = 0 AND `status` = 1";

	DBResult* result = db->storeQuery(query.str());

	if (!result) {
		return;
	}

	do {
		uint32_t guild1 = result->getDataInt("guild1");

		if (guildId == guild1) {
			guildWarList.push_back(guild1);
		} else {
			guildWarList.push_back(result->getDataInt("guild2"));
		}
	} while (result->next());

	db->freeResult(result);
}
