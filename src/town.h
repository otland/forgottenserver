// Copyright 2022 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_TOWN_H_3BE21D2293B44AA4A3D22D25BE1B9350
#define FS_TOWN_H_3BE21D2293B44AA4A3D22D25BE1B9350

#include "position.h"

class Town
{
	public:
		explicit Town(uint32_t id) : id(id) {}

		const Position& getTemplePosition() const {
			return templePosition;
		}
		const std::string& getName() const {
			return name;
		}

		void setTemplePos(Position pos) {
			templePosition = pos;
		}
		void setName(std::string name) {
			this->name = std::move(name);
		}
		uint32_t getID() const {
			return id;
		}

	private:
		uint32_t id;
		std::string name;
		Position templePosition;
};

using TownMap = std::map<uint32_t, Town*>;

class Towns
{
	public:
		Towns() = default;
		~Towns() {
			for (const auto& it : townMap) {
				delete it.second;
			}
		}

		// non-copyable
		Towns(const Towns&) = delete;
		Towns& operator=(const Towns&) = delete;

		bool addTown(uint32_t townId, Town* town) {
			return townMap.emplace(townId, town).second;
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

		const TownMap& getTowns() const {
			return townMap;
		}

	private:
		TownMap townMap;
};

#endif
