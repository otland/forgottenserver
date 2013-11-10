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

#ifndef __TOWN_H__
#define __TOWN_H__

#include "position.h"

class Town
{
	public:
		Town(uint32_t _id)
			: id(_id) {}
		~Town() {}

		const Position& getTemplePosition() const {
			return templePosition;
		}
		const std::string& getName() const {
			return name;
		}

		void setTemplePos(const Position& pos) {
			templePosition = pos;
		}
		void setName(const std::string& _name) {
			name = _name;
		}
		uint32_t getID() const {
			return id;
		}

	private:
		uint32_t id;
		std::string name;
		Position templePosition;
};

class Towns
{
	public:
		~Towns() {
			for (const auto& it : townMap) {
				delete it.second;
			}
		}

		static Towns& getInstance() {
			static Towns singleton;
			return singleton;
		}

		bool addTown(uint32_t townId, Town* town) {
			auto it = townMap.find(townId);
			if (it != townMap.end()) {
				return false;
			}

			townMap[townId] = town;
			return true;
		}

		Town* getTown(const std::string& townName) const {
			for (const auto& it : townMap) {
				if (strcasecmp(townName.c_str(), it.second->getName().c_str()) == 0) {
					return it.second;
				}
			}
			return nullptr;
		}

		Town* getTown(uint32_t townId) const {
			auto it = townMap.find(townId);
			if (it == townMap.end()) {
				return nullptr;
			}
			return it->second;
		}

	private:
		std::map<uint32_t, Town*> townMap;
};

#endif
