/**
 * The Forgotten Server - a server application for the MMORPG Tibia
 * Copyright (C) 2013  Mark Samman <mark.samman@gmail.com>
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

#ifndef _MOUNTS_H_
#define _MOUNTS_H_

#include "player.h"
#include "networkmessage.h"

struct Mount
{
	Mount(uint8_t id, uint16_t clientId, const std::string& name, int32_t speed, bool premium)
		: name(name), speed(speed), clientId(clientId), id(id), premium(premium) {}

	bool isTamed(Player* player) const;

	std::string name;
	int32_t speed;
	uint16_t clientId;
	uint8_t id;
	bool premium;
};

typedef std::list<Mount> MountsList;

class Mounts
{
	public:
		static Mounts* getInstance() {
			static Mounts instance;
			return &instance;
		}

		bool reload();
		bool loadFromXml();
		Mount* getMountByID(uint8_t id);
		Mount* getMountByClientID(uint16_t clientId);

		const MountsList& getMounts() const {
			return mounts;
		}

	private:
		std::list<Mount> mounts;
};

#endif
