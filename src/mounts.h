//////////////////////////////////////////////////////////////////////
// The Forgotten Server - a server application for the MMORPG Tibia
//////////////////////////////////////////////////////////////////////
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//////////////////////////////////////////////////////////////////////

#ifndef _MOUNTS_H_
#define _MOUNTS_H_

#include <list>
#include <string>
#include "player.h"
#include "networkmessage.h"

class Mount;

typedef std::list<Mount*> MountsList;

class Mount
{
	public:
		Mount(uint8_t _id, uint16_t _clientId, const std::string& _name, int32_t _speed, bool _premium);
		~Mount() {}

		bool isTamed(Player* player) const;
		uint8_t getID() const {
			return id;
		}
		uint16_t getClientID() const {
			return clientId;
		}
		std::string getName() const {
			return name;
		}
		int32_t getSpeed() const {
			return speed;
		}
		bool isPremium() const {
			return premium;
		}

	private:
		uint8_t id;
		uint16_t clientId;
		std::string name;
		int32_t speed;
		bool premium;
};

class Mounts
{
	public:
		Mounts() {}
		~Mounts();

		static Mounts* getInstance() {
			static Mounts instance;
			return &instance;
		}

		bool reload();
		bool loadFromXml();
		Mount* getMountByID(uint8_t id);
		Mount* getMountByClientID(uint16_t clientId);

		MountsList::const_iterator getFirstMount() const {
			return mounts.begin();
		}
		MountsList::const_iterator getLastMount() const {
			return mounts.end();
		}

	private:
		MountsList mounts;
};

#endif
