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

#ifndef __GUILD_H__
#define __GUILD_H__

#include "player.h"

typedef std::vector<Player*> PlayerVector;

struct GuildRank {
	uint32_t id;
	std::string name;
	uint8_t level;

	GuildRank(uint32_t id, const std::string& name, uint8_t level) {
		this->id = id;
		this->name = name;
		this->level = level;
	}
};

class Guild
{
	public:
		Guild(uint32_t id, const std::string& name);
		~Guild() {}

		void addMember(Player* player);
		void removeMember(Player* player);

		uint32_t getId() const {
			return id;
		}
		const std::string& getName() const {
			return name;
		}
		const PlayerVector& getMembersOnline() const {
			return membersOnline;
		}
		size_t getMembersOnlineCount() const {
			return membersOnline.size();
		}

		uint32_t getMemberCount() const {
			return memberCount;
		}
		void setMemberCount(uint32_t count) {
			memberCount = count;
		}

		GuildRank* getRankById(uint32_t id);
		GuildRank* getRankByLevel(uint8_t level);
		void addRank(uint32_t id, const std::string& name, uint8_t level);

		const std::string& getMotd() const {
			return motd;
		}
		void setMotd(const std::string& motd) {
			this->motd = motd;
		}

	private:
		PlayerVector membersOnline;
		std::vector<GuildRank> ranks;
		std::string name;
		std::string motd;
		uint32_t id;
		uint32_t memberCount;
};

#endif
