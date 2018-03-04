/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2018  Mark Samman <mark.samman@gmail.com>
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

#ifndef FS_IOGUILD_H_EF9ACEBA0B844C388B70FF52E69F1AFF
#define FS_IOGUILD_H_EF9ACEBA0B844C388B70FF52E69F1AFF

class Guild;
using GuildWarVector = std::vector<uint32_t>;

class IOGuild
{
	public:
		static Guild* loadGuild(uint32_t guildId);
		static uint32_t getGuildIdByName(const std::string& name);
		static void getWarList(uint32_t guildId, GuildWarVector& guildWarVector);
};

#endif
