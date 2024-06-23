// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_BAN_H
#define FS_BAN_H

#include "connection.h"

struct BanInfo
{
	std::string bannedBy;
	std::string reason;
	time_t expiresAt;
};

class IOBan
{
public:
	static const std::optional<BanInfo> getAccountBanInfo(uint32_t accountId);
	static const std::optional<BanInfo> getIpBanInfo(const Connection::Address& clientIP);
	static bool isPlayerNamelocked(uint32_t playerId);
};

#endif // FS_BAN_H
