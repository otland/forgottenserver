// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_IOMAP_H
#define FS_IOMAP_H

#include "configmanager.h"
#include "house.h"
#include "map.h"
#include "spawn.h"

enum OTBM_AttrTypes_t
{
	OTBM_ATTR_DESCRIPTION = 1,
	OTBM_ATTR_EXT_FILE = 2,
	OTBM_ATTR_TILE_FLAGS = 3,
	OTBM_ATTR_ACTION_ID = 4,
	OTBM_ATTR_UNIQUE_ID = 5,
	OTBM_ATTR_TEXT = 6,
	OTBM_ATTR_DESC = 7,
	OTBM_ATTR_TELE_DEST = 8,
	OTBM_ATTR_ITEM = 9,
	OTBM_ATTR_DEPOT_ID = 10,
	OTBM_ATTR_EXT_SPAWN_FILE = 11,
	OTBM_ATTR_RUNE_CHARGES = 12,
	OTBM_ATTR_EXT_HOUSE_FILE = 13,
	OTBM_ATTR_HOUSEDOORID = 14,
	OTBM_ATTR_COUNT = 15,
	OTBM_ATTR_DURATION = 16,
	OTBM_ATTR_DECAYING_STATE = 17,
	OTBM_ATTR_WRITTENDATE = 18,
	OTBM_ATTR_WRITTENBY = 19,
	OTBM_ATTR_SLEEPERGUID = 20,
	OTBM_ATTR_SLEEPSTART = 21,
	OTBM_ATTR_CHARGES = 22,
};

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

enum OTBM_TileFlag_t : uint32_t
{
	OTBM_TILEFLAG_PROTECTIONZONE = 1 << 0,
	OTBM_TILEFLAG_NOPVPZONE = 1 << 2,
	OTBM_TILEFLAG_NOLOGOUT = 1 << 3,
	OTBM_TILEFLAG_PVPZONE = 1 << 4
};

#pragma pack(1)

struct OTBM_root_header
{
	uint32_t version;
	uint16_t width;
	uint16_t height;
	uint32_t majorVersionItems;
	uint32_t minorVersionItems;
};

struct OTBM_Destination_coords
{
	uint16_t x;
	uint16_t y;
	uint8_t z;
};

struct OTBM_Tile_coords
{
	uint8_t x;
	uint8_t y;
};

#pragma pack()

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

	void setLastErrorString(std::string error) { errorString = std::move(error); }

private:
	std::string errorString;
};

#endif // FS_IOMAP_H
