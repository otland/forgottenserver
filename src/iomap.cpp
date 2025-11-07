// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "otpch.h"

#include "iomap.h"

#include "housetile.h"

/*
        OTBM_ROOTV1
        |
        |--- OTBM_MAP_DATA
        |	|
        |	|--- OTBM_TILE_AREA
        |	|	|--- OTBM_TILE
        |	|	|--- OTBM_TILE_SQUARE (not implemented)
        |	|	|--- OTBM_TILE_REF (not implemented)
        |	|	|--- OTBM_HOUSETILE
        |	|
        |	|--- OTBM_SPAWNS (not implemented)
        |	|	|--- OTBM_SPAWN_AREA (not implemented)
        |	|	|--- OTBM_MONSTER (not implemented)
        |	|
        |	|--- OTBM_TOWNS
        |	|	|--- OTBM_TOWN
        |	|
        |	|--- OTBM_WAYPOINTS
        |		|--- OTBM_WAYPOINT
        |
        |--- OTBM_ITEM_DEF (not implemented)
*/

namespace tfs::io::map {

namespace {

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

enum OTBM_TileFlag_t : uint32_t
{
	OTBM_TILEFLAG_PROTECTIONZONE = 1 << 0,
	OTBM_TILEFLAG_NOPVPZONE = 1 << 2,
	OTBM_TILEFLAG_NOLOGOUT = 1 << 3,
	OTBM_TILEFLAG_PVPZONE = 1 << 4
};

auto parseMapAttributes(const OTB::Node& mapNode)
{
	std::string spawns = {}, houses = {};

	auto first = mapNode.propsBegin;

	while (first != mapNode.propsEnd) {
		switch (auto attr = OTB::read<uint8_t>(first, mapNode.propsEnd)) {
			case OTBM_ATTR_DESCRIPTION: {
				auto len = OTB::read<uint16_t>(first, mapNode.propsEnd);
				OTB::skip(first, mapNode.propsEnd, len);
				// out.description = OTB::readString(first, mapNode.propsEnd);
				break;
			}

			case OTBM_ATTR_EXT_SPAWN_FILE: {
				spawns = OTB::readString(first, mapNode.propsEnd);
				break;
			}

			case OTBM_ATTR_EXT_HOUSE_FILE: {
				houses = OTB::readString(first, mapNode.propsEnd);
				break;
			}

			default:
				throw std::invalid_argument(fmt::format("Unknown map attribute {:d}\n", attr));
		}
	}

	return std::make_pair(spawns, houses);
}

auto read_coords(auto& first, const auto last)
{
	auto x = OTB::read<uint16_t>(first, last);
	auto y = OTB::read<uint16_t>(first, last);
	auto z = OTB::read<uint8_t>(first, last);
	return std::make_tuple(x, y, z);
}

Tile* createTile(Item*& ground, Item* item, uint16_t x, uint16_t y, uint8_t z)
{
	if (!ground) {
		return new StaticTile(x, y, z);
	}

	Tile* tile;
	if ((item && item->isBlocking()) || ground->isBlocking()) {
		tile = new StaticTile(x, y, z);
	} else {
		tile = new DynamicTile(x, y, z);
	}

	tile->internalAddThing(ground);
	ground->startDecaying();
	ground = nullptr;
	return tile;
}

void parseTileArea(const OTB::Node& node, Map& map)
{
	auto first = node.propsBegin;
	auto [base_x, base_y, z] = read_coords(first, node.propsEnd);

	for (const auto& tileNode : node.children) {
		if (tileNode.type != OTBM_TILE && tileNode.type != OTBM_HOUSETILE) [[unlikely]] {
			throw std::invalid_argument(fmt::format("Unknown tile node: {:d}.", static_cast<uint16_t>(node.type)));
		}

		auto first = tileNode.propsBegin;
		auto x = base_x + OTB::read<uint8_t>(first, tileNode.propsEnd);
		auto y = base_y + OTB::read<uint8_t>(first, tileNode.propsEnd);

		bool isHouseTile = false;
		House* house = nullptr;
		Tile* tile = nullptr;
		Item* ground_item = nullptr;
		uint32_t tileflags = TILESTATE_NONE;

		if (tileNode.type == OTBM_HOUSETILE) {
			auto house_id = OTB::read<uint32_t>(first, tileNode.propsEnd);

			house = map.houses.addHouse(house_id);

			auto houseTile = new HouseTile(x, y, z, house);
			house->addTile(houseTile);
			tile = houseTile;
			isHouseTile = true;
		}

		// read tile attributes
		while (first != tileNode.propsEnd) {
			switch (OTB::read<char>(first, tileNode.propsEnd)) {
				case OTBM_ATTR_TILE_FLAGS: {
					auto flags = OTB::read<uint32_t>(first, tileNode.propsEnd);

					if ((flags & OTBM_TILEFLAG_PROTECTIONZONE) != 0) {
						tileflags |= TILESTATE_PROTECTIONZONE;
					} else if ((flags & OTBM_TILEFLAG_NOPVPZONE) != 0) {
						tileflags |= TILESTATE_NOPVPZONE;
					} else if ((flags & OTBM_TILEFLAG_PVPZONE) != 0) {
						tileflags |= TILESTATE_PVPZONE;
					}

					if ((flags & OTBM_TILEFLAG_NOLOGOUT) != 0) {
						tileflags |= TILESTATE_NOLOGOUT;
					}
					break;
				}

				case OTBM_ATTR_ITEM: {
					auto item = Item::CreateItem(Item::getPersistentId(OTB::read<uint16_t>(first, tileNode.propsEnd)));

					if (isHouseTile && item->isMoveable()) {
						std::cout << "[Warning - IOMap::loadMap] Moveable item with ID: " << item->getID()
						          << ", in house: " << house->getId() << ", at position [x: " << x << ", y: " << y
						          << ", z: " << z << "].\n";
					} else {
						if (item->getItemCount() == 0) {
							item->setItemCount(1);
						}

						if (tile) {
							tile->internalAddThing(item);
							item->startDecaying();
							item->setLoadedFromMap(true);
						} else if (item->isGroundTile()) {
							delete ground_item;
							ground_item = item;
							item = nullptr;
						} else {
							tile = createTile(ground_item, item, x, y, z);
							tile->internalAddThing(item);
							item->startDecaying();
							item->setLoadedFromMap(true);
						}
					}
					break;
				}

				default:
					throw std::invalid_argument(
					    fmt::format("[x:{:d}, y:{:d}, z:{:d}] Unknown tile attribute.", x, y, z));
			}
		}

		for (const auto& itemNode : tileNode.children) {
			if (itemNode.type != OTBM_ITEM) [[unlikely]] {
				throw std::invalid_argument(fmt::format("Unknown item node: {:d}.", static_cast<uint16_t>(node.type)));
			}

			auto first = itemNode.propsBegin;
			auto id = OTB::read<uint16_t>(first, itemNode.propsEnd);
			Item* item = Item::CreateItem(Item::getPersistentId(id));
			item->unserializeItemNode(first, itemNode.propsEnd, itemNode);

			if (isHouseTile && item->isMoveable()) {
				std::cout << "[Warning - IOMap::loadMap] Moveable item with ID: " << item->getID()
				          << ", in house: " << house->getId() << ", at position [x: " << x << ", y: " << y
				          << ", z: " << z << "]." << std::endl;
			} else {
				if (item->getItemCount() == 0) {
					item->setItemCount(1);
				}

				if (tile) {
					tile->internalAddThing(item);
					item->startDecaying();
					item->setLoadedFromMap(true);
				} else if (item->isGroundTile()) {
					delete ground_item;
					ground_item = item;
					item = nullptr;
				} else {
					tile = createTile(ground_item, item, x, y, z);
					tile->internalAddThing(item);
					item->startDecaying();
					item->setLoadedFromMap(true);
				}
			}
		}

		if (!tile) {
			tile = createTile(ground_item, nullptr, x, y, z);
		}

		tile->setFlag(static_cast<tileflags_t>(tileflags));

		map.setTile(x, y, z, tile);
	}
}

void parseTowns(const OTB::Node& townsNode, Map& map)
{
	for (const auto& node : townsNode.children) {
		if (node.type != OTBM_TOWN) [[unlikely]] {
			throw std::invalid_argument(fmt::format("Unknown town node: {:d}.", static_cast<uint16_t>(node.type)));
		}

		auto first = node.propsBegin;
		auto townId = OTB::read<uint32_t>(first, node.propsEnd);
		auto townName = OTB::readString(first, node.propsEnd);
		auto [x, y, z] = read_coords(first, node.propsEnd);

		map.towns.setTown(townId, new Town{.id = townId, .name = std::move(townName), .templePosition = {x, y, z}});
	}
}

void parseWaypoints(const OTB::Node& waypointsNode, Map& map)
{
	for (const auto& node : waypointsNode.children) {
		if (node.type != OTBM_WAYPOINT) [[unlikely]] {
			throw std::invalid_argument(fmt::format("Unknown waypoint node: {:d}.", static_cast<uint16_t>(node.type)));
		}

		auto first = node.propsBegin;

		auto name = OTB::readString(first, node.propsEnd);
		auto [x, y, z] = read_coords(first, node.propsEnd);

		map.waypoints[name] = Position{x, y, z};
	}
}

} // namespace

MapAttributes loadMap(Map& map, std::filesystem::path fileName)
{
	auto start = std::chrono::steady_clock::now();

	auto loader = OTB::load(fileName.string(), "OTBM");
	auto first = loader.begin(), last = loader.end();

	auto version = OTB::read<uint32_t>(first, last);
	if (version == 0) {
		// In otbm version 1 the count variable after splashes/fluidcontainers and stackables are saved as
		// attributes instead, this solves a lot of problems with items that are changed
		// (stackable/charges/fluidcontainer/splash) during an update.
		throw std::invalid_argument(
		    "This map need to be upgraded by using the latest map editor version to be able to load correctly.");
	}

	if (version > 2) {
		throw std::invalid_argument("Unknown OTBM version detected.");
	}

	auto width = OTB::read<uint16_t>(first, last);
	auto height = OTB::read<uint16_t>(first, last);
	auto majorVersionItems = OTB::read<uint32_t>(first, last);
	auto minorVersionItems = OTB::read<uint32_t>(first, last);

	if (majorVersionItems < 3) {
		throw std::invalid_argument(
		    "This map need to be upgraded by using the latest map editor version to be able to load correctly.");
	}

	if (majorVersionItems > Item::items.majorVersion) {
		throw std::invalid_argument(
		    "The map was saved with a different items.otb version, an upgraded items.otb is required.");
	}

	if (minorVersionItems < CLIENT_VERSION_810) {
		throw std::invalid_argument("This map needs to be updated.");
	}

	if (minorVersionItems > Item::items.minorVersion) {
		fmt::print("[Warning - IOMap::loadMap] This map needs an updated items.otb.\n");
	}

	fmt::print("> Map size: {:d}x{:d}\n.", width, height);

	const auto& rootNodes = loader.children();
	if (rootNodes.size() != 1 || rootNodes.front().type != OTBM_MAP_DATA) {
		throw std::invalid_argument("Could not read data node.");
	}

	const auto& mapNode = rootNodes.front();
	auto [spawns, houses] = parseMapAttributes(mapNode);

	if (spawns.empty()) {
		// OTBM file doesn't tell us about the spawnfile, lets guess it is mapname-spawn.xml.
		spawns = fileName.stem().concat("-spawn.xml").string();
	}

	if (houses.empty()) {
		// OTBM file doesn't tell us about the housefile, lets guess it is mapname-house.xml.
		houses = fileName.stem().concat("-house.xml").string();
	}

	for (const auto& node : mapNode.children) {
		switch (node.type) {
			case OTBM_TILE_AREA:
				parseTileArea(node, map);
				break;

			case OTBM_TOWNS:
				parseTowns(node, map);
				break;

			case OTBM_WAYPOINTS:
				if (version <= 1) [[unlikely]] {
					throw std::invalid_argument(fmt::format(
					    "Waypoints are not supported in OTBM version {:d}, please update your map.", version));
				}

				parseWaypoints(node, map);
				break;

			default:
				throw std::invalid_argument(fmt::format("Unknown map node: {:d}.", static_cast<uint16_t>(node.type)));
		}
	}

	auto end = std::chrono::steady_clock::now();

	std::cout << "> Map loading time: " << duration_cast<std::chrono::milliseconds>(end - start).count() << "ms.\n";

	return {
	    .spawns = fileName.parent_path() / spawns,
	    .houses = fileName.parent_path() / houses,
	    .width = width,
	    .height = height,
	};
}

} // namespace tfs::io::map
