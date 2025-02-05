// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "otpch.h"

#include "ban.h"

#include "connection.h"
#include "database.h"
#include "databasetasks.h"

namespace IOBan {

const std::optional<BanInfo> getAccountBanInfo(uint32_t accountId)
{
	Database& db = Database::getInstance();

	DBResult_ptr result = db.storeQuery(fmt::format(
	    "SELECT `reason`, `expires_at`, `banned_at`, `banned_by`, (SELECT `name` FROM `players` WHERE `id` = `banned_by`) AS `name` FROM `account_bans` WHERE `account_id` = {:d}",
	    accountId));
	if (!result) {
		return std::nullopt;
	}

	int64_t expiresAt = result->getNumber<int64_t>("expires_at");
	if (expiresAt != 0 && time(nullptr) > expiresAt) {
		// Move the ban to history if it has expired
		g_databaseTasks.addTask(fmt::format(
		    "INSERT INTO `account_ban_history` (`account_id`, `reason`, `banned_at`, `expired_at`, `banned_by`) VALUES ({:d}, {:s}, {:d}, {:d}, {:d})",
		    accountId, db.escapeString(result->getString("reason")), result->getNumber<time_t>("banned_at"), expiresAt,
		    result->getNumber<uint32_t>("banned_by")));
		g_databaseTasks.addTask(fmt::format("DELETE FROM `account_bans` WHERE `account_id` = {:d}", accountId));
		return std::nullopt;
	}

	auto banInfo = std::make_optional<BanInfo>();
	banInfo->expiresAt = expiresAt;

	banInfo->reason = result->getString("reason");
	if (banInfo->reason.empty()) {
		banInfo->reason = "(none)";
	}

	banInfo->bannedBy = result->getString("name");
	return banInfo;
}

const std::optional<BanInfo> getIpBanInfo(const Connection::Address& clientIP)
{
	if (clientIP.is_unspecified()) {
		return std::nullopt;
	}

	Database& db = Database::getInstance();

	DBResult_ptr result = db.storeQuery(fmt::format(
	    "SELECT `reason`, `expires_at`, (SELECT `name` FROM `players` WHERE `id` = `banned_by`) AS `name` FROM `ip_bans` WHERE `ip` = INET6_ATON('{:s}')",
	    clientIP.to_string()));
	if (!result) {
		return std::nullopt;
	}

	int64_t expiresAt = result->getNumber<int64_t>("expires_at");
	if (expiresAt != 0 && time(nullptr) > expiresAt) {
		g_databaseTasks.addTask(
		    fmt::format("DELETE FROM `ip_bans` WHERE `ip` = INET6_ATON('{:s}')", clientIP.to_string()));
		return std::nullopt;
	}

	auto banInfo = std::make_optional<BanInfo>();
	banInfo->expiresAt = expiresAt;

	banInfo->reason = result->getString("reason");
	if (banInfo->reason.empty()) {
		banInfo->reason = "(none)";
	}

	banInfo->bannedBy = result->getString("name");
	return banInfo;
}

bool isPlayerNamelocked(uint32_t playerId)
{
	return Database::getInstance()
	    .storeQuery(fmt::format("SELECT 1 FROM `player_namelocks` WHERE `player_id` = {:d}", playerId))
	    .get();
}

} // namespace IOBan
