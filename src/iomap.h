// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_IOMAP_H
#define FS_IOMAP_H

#include "configmanager.h"
#include "house.h"
#include "map.h"
#include "spawn.h"

enum OTBM_NodeTypes_t
{
	OTBM_ROOTV1 = 1,
	OTBM_MAP_DATA = 2,
	OTBM_ITEM_DEF = 3,
	OTBM_TILE_AREA = 4,
	OTBM_TILE = 5,
	OTBM_ITEM = 6,
	OTBM_TILE_SQUARE = 7,
	OTBM_TILE_REF = 8,
	OTBM_SPAWNS = 9,
	OTBM_SPAWN_AREA = 10,
	OTBM_MONSTER = 11,
	OTBM_TOWNS = 12,
	OTBM_TOWN = 13,
	OTBM_HOUSETILE = 14,
	OTBM_WAYPOINTS = 15,
	OTBM_WAYPOINT = 16,
};

class IOMap
{
public:
	bool loadMap(Map* map, const std::filesystem::path& fileName);

	/* Load the spawns
	 * \param map pointer to the Map class
	 * \returns Returns true if the spawns were loaded successfully
	 */
	static bool loadSpawns(Map* map, bool isCalledByLua)
	{
		if (map->spawnfile.empty()) {
			// OTBM file doesn't tell us about the spawnfile, lets guess it is mapname-spawn.xml.
			map->spawnfile = getString(ConfigManager::MAP_NAME);
			map->spawnfile += "-spawn.xml";
		}

		return map->spawns.loadFromXml(map->spawnfile.string(), isCalledByLua);
	}

	/* Load the houses (not house tile-data)
	 * \param map pointer to the Map class
	 * \returns Returns true if the houses were loaded successfully
	 */
	static bool loadHouses(Map* map)
	{
		if (map->housefile.empty()) {
			// OTBM file doesn't tell us about the housefile, lets guess it is mapname-house.xml.
			map->housefile = getString(ConfigManager::MAP_NAME);
			map->housefile += "-house.xml";
		}

		return map->houses.loadHousesXML(map->housefile.string());
	}

	const std::string& getLastErrorString() const { return errorString; }

	void setLastErrorString(std::string error) { errorString = error; }

private:
	std::string errorString;
};

#endif // FS_IOMAP_H
