// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_ACCOUNT_H
#define FS_ACCOUNT_H

#include "enums.h"

struct Account
{
	std::vector<std::string> characters;
	std::string name;
	std::string key;
	uint32_t id = 0;
	time_t premiumEndsAt = 0;
	AccountType_t accountType = ACCOUNT_TYPE_NORMAL;

	Account() = default;
};

#endif // FS_ACCOUNT_H
