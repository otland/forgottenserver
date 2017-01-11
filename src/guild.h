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

#ifndef FS_GUILD_H_C00F0A1D732E4BA88FF62ACBE74D76BC
#define FS_GUILD_H_C00F0A1D732E4BA88FF62ACBE74D76BC

class Player;

struct GuildRank {
	uint32_t id;
	std::string name;
	uint8_t level;

	GuildRank(uint32_t id, std::string name, uint8_t level) :
		id(id), name(std::move(name)), level(level) {}
};

class Guild
{
	public:
		Guild(uint32_t id, std::string name) : name(std::move(name)), id(id) {}

		void addMember(Player* player);
		void removeMember(Player* player);

		uint32_t getId() const {
			return id;
		}
		const std::string& getName() const {
			return name;
		}
		const std::list<Player*>& getMembersOnline() const {
			return membersOnline;
		}
		uint32_t getMemberCount() const {
			return memberCount;
		}
		void setMemberCount(uint32_t count) {
			memberCount = count;
		}

		GuildRank* getRankById(uint32_t id);
		const GuildRank* getRankByLevel(uint8_t level) const;
		void addRank(uint32_t id, const std::string& name, uint8_t level);

		const std::string& getMotd() const {
			return motd;
		}
		void setMotd(const std::string& motd) {
			this->motd = motd;
		}

	private:
		std::list<Player*> membersOnline;
		std::vector<GuildRank> ranks;
		std::string name;
		std::string motd;
		uint32_t id;
		uint32_t memberCount = 0;
};

#endif
