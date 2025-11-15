// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_GUILD_H
#define FS_GUILD_H

class Player;

using GuildWarVector = std::vector<uint32_t>;

struct GuildRank
{
	uint32_t id;
	std::string name;
	uint8_t level;

	GuildRank(uint32_t id, std::string_view name, uint8_t level) : id{id}, name{name}, level{level} {}
};

using GuildRank_ptr = std::shared_ptr<GuildRank>;

class Guild
{
public:
	static constexpr uint8_t MEMBER_RANK_LEVEL_DEFAULT = 1;

	Guild(uint32_t id, std::string_view name) : name{name}, id{id} {}

	uint32_t getId() const { return id; }
	const std::string& getName() const { return name; }

	void addMember(Player* player);
	void removeMember(Player* player);
	const auto& getMembersOnline() const { return membersOnline; }
	uint32_t getMemberCount() const { return memberCount; }
	void setMemberCount(uint32_t count) { memberCount = count; }

	void addRank(uint32_t rankId, std::string_view rankName, uint8_t level);
	const std::vector<GuildRank_ptr>& getRanks() const { return ranks; }
	GuildRank_ptr getRankById(uint32_t rankId);
	GuildRank_ptr getRankByName(const std::string& name) const;
	GuildRank_ptr getRankByLevel(uint8_t level) const;

	const std::string& getMotd() const { return motd; }
	void setMotd(const std::string& motd) { this->motd = motd; }

private:
	boost::container::flat_set<Player*> membersOnline;
	std::vector<GuildRank_ptr> ranks;
	std::string name;
	std::string motd;
	uint32_t id;
	uint32_t memberCount = 0;
};

using Guild_ptr = std::shared_ptr<Guild>;

namespace IOGuild {
Guild_ptr loadGuild(uint32_t guildId);
uint32_t getGuildIdByName(const std::string& name);
}; // namespace IOGuild

#endif // FS_GUILD_H
