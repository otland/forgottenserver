// Copyright 2022 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

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

using GuildRank_ptr = std::shared_ptr<GuildRank>;

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

		const std::vector<GuildRank_ptr>& getRanks() const {
			return ranks;
		}
		GuildRank_ptr getRankById(uint32_t rankId);
		GuildRank_ptr getRankByName(const std::string& name) const;
		GuildRank_ptr getRankByLevel(uint8_t level) const;
		void addRank(uint32_t rankId, const std::string& rankName, uint8_t level);

		const std::string& getMotd() const {
			return motd;
		}
		void setMotd(const std::string& motd) {
			this->motd = motd;
		}

	private:
		std::list<Player*> membersOnline;
		std::vector<GuildRank_ptr> ranks;
		std::string name;
		std::string motd;
		uint32_t id;
		uint32_t memberCount = 0;
};

using GuildWarVector = std::vector<uint32_t>;

namespace IOGuild
{
	Guild* loadGuild(uint32_t guildId);
	uint32_t getGuildIdByName(const std::string& name);
};

#endif
