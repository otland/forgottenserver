// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "otpch.h"

#include "guild.h"

#include "database.h"
#include "game.h"
#include "tools.h"

extern Game g_game;

void Guild::removeMember(const std::shared_ptr<Player>& player)
{
	membersOnline.erase(player);

	if (membersOnline.empty()) {
		g_game.removeGuild(id);
	}
}

void Guild::addRank(uint32_t rankId, std::string_view rankName, uint8_t level)
{
	ranks.emplace_back(std::make_shared<GuildRank>(rankId, rankName, level));
}

std::shared_ptr<GuildRank> Guild::getRankById(uint32_t rankId)
{
	for (const auto& rank : ranks) {
		if (rank->id == rankId) {
			return rank;
		}
	}
	return nullptr;
}

std::shared_ptr<GuildRank> Guild::getRankByName(const std::string& name) const
{
	for (const auto& rank : ranks) {
		if (caseInsensitiveEqual(rank->name, name)) {
			return rank;
		}
	}
	return nullptr;
}

std::shared_ptr<GuildRank> Guild::getRankByLevel(uint8_t level) const
{
	for (const auto& rank : ranks) {
		if (rank->level == level) {
			return rank;
		}
	}
	return nullptr;
}

std::shared_ptr<Guild> IOGuild::loadGuild(uint32_t guildId)
{
	Database& db = Database::getInstance();
	DBResult_ptr result = db.storeQuery(fmt::format("SELECT `name` FROM `guilds` WHERE `id` = {:d}", guildId));
	if (!result) {
		return nullptr;
	}

	const auto& guild = std::make_shared<Guild>(guildId, result->getString("name"));
	if ((result = db.storeQuery(
	         fmt::format("SELECT `id`, `name`, `level` FROM `guild_ranks` WHERE `guild_id` = {:d}", guildId)))) {
		do {
			guild->addRank(result->getNumber<uint32_t>("id"), result->getString("name"),
			               result->getNumber<uint16_t>("level"));
		} while (result->next());
	}
	return guild;
}

uint32_t IOGuild::getGuildIdByName(const std::string& name)
{
	Database& db = Database::getInstance();

	DBResult_ptr result =
	    db.storeQuery(fmt::format("SELECT `id` FROM `guilds` WHERE `name` = {:s}", db.escapeString(name)));
	if (!result) {
		return 0;
	}
	return result->getNumber<uint32_t>("id");
}
