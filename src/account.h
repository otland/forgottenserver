/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2019  Mark Samman <mark.samman@gmail.com>
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

#ifndef FS_ACCOUNT_H_34817537BA2B4CB7B71AA562AFBB118F
#define FS_ACCOUNT_H_34817537BA2B4CB7B71AA562AFBB118F

#include "enums.h"

struct Account {
	std::vector<std::string> characters;
	std::string name;
	std::string key;
	time_t lastDay = 0;
	uint32_t id = 0;
	uint16_t premiumDays = 0;
	AccountType_t accountType = ACCOUNT_TYPE_NORMAL;

	Account() = default;
};

#endif
