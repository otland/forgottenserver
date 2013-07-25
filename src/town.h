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

#ifndef __TOWN_H__
#define __TOWN_H__

#include <string>
#include <list>
#include <map>

#include "position.h"
#include "definitions.h"

class Town
{
	public:
		Town(uint32_t _townid)
			: townid(_townid) {}

		~Town() {}

		const Position& getTemplePosition() const {
			return posTemple;
		}
		const std::string& getName() const {
			return townName;
		}

		void setTemplePos(const Position& pos) {
			posTemple = pos;
		}
		void setName(const std::string& _townName) {
			townName = _townName;
		}
		uint32_t getTownID() const {
			return townid;
		}

	private:
		uint32_t townid;
		std::string townName;
		Position posTemple;
};

typedef std::map<uint32_t, Town*> TownMap;

class Towns
{
	public:
		~Towns() {
			for (TownMap::iterator it = townMap.begin(); it != townMap.end(); ++it) {
				delete it->second;
			}
		}

		static Towns& getInstance() {
			static Towns singleton;
			return singleton;
		}

		bool addTown(uint32_t _townid, Town* town) {
			TownMap::iterator it = townMap.find(_townid);
			if (it != townMap.end()) {
				return false;
			}

			townMap[_townid] = town;
			return true;
		}

		Town* getTown(const std::string& townname) const {
			for (auto it = townMap.begin(); it != townMap.end(); ++it) {
				if (strcasecmp(townname.c_str(), it->second->getName().c_str()) == 0) {
					return it->second;
				}
			}

			return NULL;
		}

		Town* getTown(uint32_t _townid) const {
			auto it = townMap.find(_townid);
			if (it != townMap.end()) {
				return it->second;
			}

			return NULL;
		}

		TownMap::const_iterator getFirstTown() const {
			return townMap.begin();
		}
		TownMap::const_iterator getLastTown() const {
			return townMap.end();
		}

	private:
		TownMap townMap;
};

#endif
