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

	auto first = mapNode.props_begin;
	auto last = mapNode.props_end;

	while (first != last) {
		switch (auto attr = OTB::read<uint8_t>(first, last)) {
			case OTBM_ATTR_DESCRIPTION: {
				auto len = OTB::read<uint16_t>(first, last);
				OTB::skip(first, last, len);
				// out.description = OTB::readString(first, last);
				break;
			}

			case OTBM_ATTR_EXT_SPAWN_FILE: {
				spawns = OTB::readString(first, last);
				break;
			}

			case OTBM_ATTR_EXT_HOUSE_FILE: {
				houses = OTB::readString(first, last);
				break;
			}

			default:
				throw std::invalid_argument(fmt::format("Unknown map attribute {:d}\n", attr));
		}
	}

	return std::make_pair(spawns, houses);
}

auto read_coords(auto& first, auto const last)
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
	auto node_begin = node.props_begin;
	auto [base_x, base_y, z] = read_coords(node_begin, node.props_end);

	for (auto& tileNode : node.children) {
		if (tileNode.type != OTBM_TILE && tileNode.type != OTBM_HOUSETILE) {
			throw std::invalid_argument(fmt::format("Unknown tile node {:d}.", tileNode.type));
		}

		auto tile_begin = tileNode.props_begin;
		auto tile_end = tileNode.props_end;

		uint16_t x = base_x + OTB::read<uint8_t>(tile_begin, tile_end);
		uint16_t y = base_y + OTB::read<uint8_t>(tile_begin, tile_end);

		bool isHouseTile = false;
		House* house = nullptr;
		Tile* tile = nullptr;
		Item* ground_item = nullptr;
		uint32_t tileflags = TILESTATE_NONE;

		if (tileNode.type == OTBM_HOUSETILE) {
			uint32_t house_id = OTB::read<uint32_t>(tile_begin, tile_end);

			house = map.houses.addHouse(house_id);
			if (!house) {
				throw std::invalid_argument(
				    fmt::format("[x:{:d}, y:{:d}, z:{:d}] Could not create house id: {:d}", x, y, z, house_id));
			}

			auto houseTile = new HouseTile(x, y, z, house);
			house->addTile(houseTile);
			tile = houseTile;
			isHouseTile = true;
		}

		// read tile attributes
		while (tile_begin != tile_end) {
			switch (OTB::read<char>(tile_begin, tile_end)) {
				case OTBM_ATTR_TILE_FLAGS: {
					uint32_t flags = OTB::read<uint32_t>(tile_begin, tile_end);

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
					auto item = Item::CreateItem(Item::getPersistentId(OTB::read<uint16_t>(tile_begin, tile_end)));
					if (!item) {
						throw std::invalid_argument(
						    fmt::format("[x:{:d}, y:{:d}, z:{:d}] Failed to create item.", x, y, z));
					}

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
							ground_item = std::move(item);
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
			if (itemNode.type != OTBM_ITEM) {
				throw std::invalid_argument(fmt::format("[x:{:d}, y:{:d}, z:{:d}] Unknown node type.", x, y, z));
			}

			auto item_begin = itemNode.props_begin;
			auto item_end = itemNode.props_end;
			auto id = OTB::read<uint16_t>(item_begin, item_end);
			Item* item = Item::CreateItem(Item::getPersistentId(id));
			if (!item) {
				throw std::invalid_argument(fmt::format("[x:{:d}, y:{:d}, z:{:d}] Failed to create item.", x, y, z));
			}

			item->unserializeItemNode(item_begin, item_end, itemNode);
			// throw std::invalid_argument(fmt::format("[x:{:d}, y:{:d}, z:{:d}] Failed to load item {:d}: {:s}.", x, y,
			// z, item->getID(), e.what()));

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
					std::swap(ground_item, item);
					delete item;
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
	for (auto& node : townsNode.children) {
		if (node.type != OTBM_TOWN) {
			throw std::invalid_argument(fmt::format("Unknown town node: {:d}", node.type));
		}

		auto first = node.props_begin, last = node.props_end;
		auto townId = OTB::read<uint32_t>(first, last);
		auto townName = OTB::readString(first, last);
		auto [x, y, z] = read_coords(first, last);

		map.towns.setTown(townId, new Town{.id = townId, .name = std::move(townName), .templePosition = {x, y, z}});
	}
}

void parseWaypoints(const OTB::Node& waypointsNode, Map& map)
{
	PropStream propStream;
	for (auto& node : waypointsNode.children) {
		if (node.type != OTBM_WAYPOINT) {
			throw std::invalid_argument(fmt::format("Unknown waypoint node: {:d}", node.type));
		}

		auto first = node.props_begin;
		auto last = node.props_end;

		auto name = OTB::readString(first, last);

		auto [x, y, z] = read_coords(first, last);
		map.waypoints[name] = Position{x, y, z};
	}
}

} // namespace

bool IOMap::loadMap(Map* map, const std::filesystem::path& fileName)
{
	auto start = std::chrono::steady_clock::now();

	try {
		auto loader = OTB::load(fileName.string(), "OTBM");
		auto first = loader.begin(), last = loader.end();

		uint32_t version = OTB::read<uint32_t>(first, last);
		if (version == 0) {
			// In otbm version 1 the count variable after splashes/fluidcontainers and stackables are saved as
			// attributes instead, this solves a lot of problems with items that are changed
			// (stackable/charges/fluidcontainer/splash) during an update.
			setLastErrorString(
			    "This map need to be upgraded by using the latest map editor version to be able to load correctly.");
			return false;
		}

		if (version > 2) {
			setLastErrorString("Unknown OTBM version detected.");
			return false;
		}

		map->width = OTB::read<uint16_t>(first, last);
		map->height = OTB::read<uint16_t>(first, last);
		uint32_t majorVersionItems = OTB::read<uint32_t>(first, last);
		uint32_t minorVersionItems = OTB::read<uint32_t>(first, last);

		if (majorVersionItems < 3) {
			setLastErrorString(
			    "This map need to be upgraded by using the latest map editor version to be able to load correctly.");
			return false;
		}

		if (majorVersionItems > Item::items.majorVersion) {
			setLastErrorString(
			    "The map was saved with a different items.otb version, an upgraded items.otb is required.");
			return false;
		}

		if (minorVersionItems < CLIENT_VERSION_810) {
			setLastErrorString("This map needs to be updated.");
			return false;
		}

		if (minorVersionItems > Item::items.minorVersion) {
			fmt::print("[Warning - IOMap::loadMap] This map needs an updated items.otb.\n");
		}

		fmt::print("> Map size: {:d}x{:d}\n.", map->width, map->height);

		const auto& rootNodes = loader.children();
		if (rootNodes.size() != 1 or rootNodes.front().type != OTBM_MAP_DATA) {
			setLastErrorString("Could not read data node.");
			return false;
		}

		const auto& mapNode = rootNodes.front();
		auto [spawns, houses] = parseMapAttributes(mapNode);

		map->spawnfile = fileName.parent_path() / std::move(spawns);
		map->housefile = fileName.parent_path() / std::move(houses);

		for (auto& node : mapNode.children) {
			if (node.type == OTBM_TILE_AREA) {
				parseTileArea(node, *map);
			} else if (node.type == OTBM_TOWNS) {
				parseTowns(node, *map);
			} else if (node.type == OTBM_WAYPOINTS && version > 1) {
				parseWaypoints(node, *map);
			} else {
				setLastErrorString("Unknown map node.");
				return false;
			}
		}
	} catch (const std::invalid_argument& err) {
		setLastErrorString(err.what());
		return false;
	}

	auto end = std::chrono::steady_clock::now();

	std::cout << "> Map loading time: " << duration_cast<std::chrono::milliseconds>(end - start).count() << "ms.\n";
	return true;
}
