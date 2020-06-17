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

#ifndef FS_WAITLIST_H_7E4299E552E44F10BC4F4E50BF3D7241
#define FS_WAITLIST_H_7E4299E552E44F10BC4F4E50BF3D7241

#include "player.h"

struct WaitListInfo;

class WaitingList
{
	public:
		static WaitingList& getInstance();

		std::size_t clientLogin(const Player* player);
		static uint8_t getTime(std::size_t slot);

	private:
		WaitingList();

		std::unique_ptr<WaitListInfo> info;
};

#endif
