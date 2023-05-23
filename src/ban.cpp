// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "otpch.h"

#include "ban.h"

#include "connection.h"
#include "database.h"
#include "databasetasks.h"
#include "tools.h"

bool Ban::acceptConnection(const Connection::Address& clientIP)
{
	std::lock_guard<std::recursive_mutex> lockClass(lock);

	uint64_t currentTime = OTSYS_TIME();

	auto it = ipConnectMap.find(clientIP);
	if (it == ipConnectMap.end()) {
		ipConnectMap.emplace(clientIP, ConnectBlock(currentTime, 0, 1));
		return true;
	}

	ConnectBlock& connectBlock = it->second;
	if (connectBlock.blockTime > currentTime) {
		connectBlock.blockTime += 250;
		return false;
	}

	int64_t timeDiff = currentTime - connectBlock.lastAttempt;
	connectBlock.lastAttempt = currentTime;
	if (timeDiff <= 5000) {
		if (++connectBlock.count > 5) {
			connectBlock.count = 0;
			if (timeDiff <= 500) {
				connectBlock.blockTime = currentTime + 3000;
				return false;
			}
		}
	} else {
		connectBlock.count = 1;
	}
	return true;
}

bool IOBan::isAccountBanned(uint32_t accountId, BanInfo& banInfo)
{
	Database& db = Database::getInstance();

	DBResult_ptr result = db.storeQuery(fmt::format(
	    "SELECT `reason`, `expires_at`, `banned_at`, `banned_by`, (SELECT `name` FROM `players` WHERE `id` = `banned_by`) AS `name` FROM `account_bans` WHERE `account_id` = {:d}",
	    accountId));
	if (!result) {
		return false;
	}

	int64_t expiresAt = result->getNumber<int64_t>("expires_at");
	if (expiresAt != 0 && time(nullptr) > expiresAt) {
		// Move the ban to history if it has expired
		g_databaseTasks.addTask(fmt::format(
		    "INSERT INTO `account_ban_history` (`account_id`, `reason`, `banned_at`, `expired_at`, `banned_by`) VALUES ({:d}, {:s}, {:d}, {:d}, {:d})",
		    accountId, db.escapeString(result->getString("reason")), result->getNumber<time_t>("banned_at"), expiresAt,
		    result->getNumber<uint32_t>("banned_by")));
		g_databaseTasks.addTask(fmt::format("DELETE FROM `account_bans` WHERE `account_id` = {:d}", accountId));
		return false;
	}

	banInfo.expiresAt = expiresAt;
	banInfo.reason = result->getString("reason");
	banInfo.bannedBy = result->getString("name");
	return true;
}

bool IOBan::isIpBanned(const Connection::Address& clientIP, BanInfo& banInfo)
{
	if (clientIP.is_unspecified()) {
		return false;
	}

	Database& db = Database::getInstance();

	DBResult_ptr result = db.storeQuery(fmt::format(
	    "SELECT `reason`, `expires_at`, (SELECT `name` FROM `players` WHERE `id` = `banned_by`) AS `name` FROM `ip_bans` WHERE `ip` = INET6_ATON('{:s}')",
	    clientIP.to_string()));
	if (!result) {
		return false;
	}

	int64_t expiresAt = result->getNumber<int64_t>("expires_at");
	if (expiresAt != 0 && time(nullptr) > expiresAt) {
		g_databaseTasks.addTask(
		    fmt::format("DELETE FROM `ip_bans` WHERE `ip` = INET6_ATON('{:s}')", clientIP.to_string()));
		return false;
	}

	banInfo.expiresAt = expiresAt;
	banInfo.reason = result->getString("reason");
	banInfo.bannedBy = result->getString("name");
	return true;
}

bool IOBan::isPlayerNamelocked(uint32_t playerId)
{
	return Database::getInstance()
	    .storeQuery(fmt::format("SELECT 1 FROM `player_namelocks` WHERE `player_id` = {:d}", playerId))
	    .get();
}
