// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_BAN_H
#define FS_BAN_H

#include "connection.h"

namespace IOBan {

struct BanInfo
{
	std::string bannedBy;
	std::string reason;
	time_t expiresAt;
};

const std::optional<BanInfo> getAccountBanInfo(uint32_t accountId);
const std::optional<BanInfo> getIpBanInfo(const Connection::Address& clientIP);
bool isPlayerNamelocked(uint32_t playerId);

}; // namespace IOBan

#endif // FS_BAN_H
