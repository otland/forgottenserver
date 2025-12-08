// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_IOMAP_H
#define FS_IOMAP_H

#include "configmanager.h"
#include "house.h"
#include "map.h"
#include "spawn.h"

namespace tfs::io::map {

inline constexpr char OTBM_ROOTV1 = 1;
inline constexpr char OTBM_MAP_DATA = 2;
inline constexpr char OTBM_ITEM_DEF = 3;
inline constexpr char OTBM_TILE_AREA = 4;
inline constexpr char OTBM_TILE = 5;
inline constexpr char OTBM_ITEM = 6;
inline constexpr char OTBM_TILE_SQUARE = 7;
inline constexpr char OTBM_TILE_REF = 8;
inline constexpr char OTBM_SPAWNS = 9;
inline constexpr char OTBM_SPAWN_AREA = 10;
inline constexpr char OTBM_MONSTER = 11;
inline constexpr char OTBM_TOWNS = 12;
inline constexpr char OTBM_TOWN = 13;
inline constexpr char OTBM_HOUSETILE = 14;
inline constexpr char OTBM_WAYPOINTS = 15;
inline constexpr char OTBM_WAYPOINT = 16;

struct MapAttributes
{
	std::filesystem::path spawns;
	std::filesystem::path houses;
	uint32_t width;
	uint32_t height;
};

MapAttributes loadMap(Map& map, std::filesystem::path fileName);

} // namespace tfs::io::map

#endif // FS_IOMAP_H
