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

std::shared_ptr<Tile> createTile(const std::shared_ptr<Item>& ground, const std::shared_ptr<Item>& item, uint16_t x,
                                 uint16_t y, uint8_t z)
{
	if (!ground) {
		return std::make_shared<StaticTile>(x, y, z);
	}

	std::shared_ptr<Tile> tile;
	if ((item && item->isBlocking()) || ground->isBlocking()) {
		tile = std::make_shared<StaticTile>(x, y, z);
	} else {
		tile = std::make_shared<DynamicTile>(x, y, z);
	}

	tile->internalAddThing(ground);
	ground->startDecaying();
	return tile;
}

std::optional<std::string> parseMapDataAttributes(OTB::Loader& loader, const OTB::Node& mapNode, Map& map,
                                                  const std::filesystem::path& fileName)
{
	PropStream propStream;
	if (!loader.getProps(mapNode, propStream)) {
		return std::make_optional("Could not read map data attributes.");
	}

	uint8_t attribute;
	while (propStream.read<uint8_t>(attribute)) {
		switch (attribute) {
			case OTBM_ATTR_DESCRIPTION: {
				auto [mapDescription, ok] = propStream.readString();
				if (!ok) {
					return std::make_optional("Invalid description tag.");
				}
				break;
			}

			case OTBM_ATTR_EXT_SPAWN_FILE: {
				auto [spawnFile, ok] = propStream.readString();
				if (!ok) {
					return std::make_optional("Invalid spawn tag.");
				}

				map.spawnfile = fileName.parent_path() / spawnFile;
				break;
			}

			case OTBM_ATTR_EXT_HOUSE_FILE: {
				auto [houseFile, ok] = propStream.readString();
				if (!ok) {
					return std::make_optional("Invalid house tag.");
				}

				map.housefile = fileName.parent_path() / houseFile;
				break;
			}

			default:
				return std::make_optional("Unknown header node.");
		}
	}
	return std::nullopt;
}

std::optional<std::string> parseTileArea(OTB::Loader& loader, const OTB::Node& tileAreaNode, Map& map)
{
	PropStream propStream;
	if (!loader.getProps(tileAreaNode, propStream)) {
		return std::make_optional("Invalid map node.");
	}

	OTBM_Destination_coords area_coord;
	if (!propStream.read(area_coord)) {
		return std::make_optional("Invalid map node.");
	}

	uint16_t base_x = area_coord.x;
	uint16_t base_y = area_coord.y;
	uint16_t z = area_coord.z;

	for (auto& tileNode : tileAreaNode.children) {
		if (tileNode.type != OTBM_TILE && tileNode.type != OTBM_HOUSETILE) {
			return std::make_optional("Unknown tile node.");
		}

		if (!loader.getProps(tileNode, propStream)) {
			return std::make_optional("Could not read node data.");
		}

		OTBM_Tile_coords tile_coord;
		if (!propStream.read(tile_coord)) {
			return std::make_optional("Could not read tile position.");
		}

		uint16_t x = base_x + tile_coord.x;
		uint16_t y = base_y + tile_coord.y;

		bool isHouseTile = false;
		House* house = nullptr;
		std::shared_ptr<Tile> tile = nullptr;
		std::shared_ptr<Item> ground_item = nullptr;
		uint32_t tileflags = TILESTATE_NONE;

		if (tileNode.type == OTBM_HOUSETILE) {
			uint32_t houseId;
			if (!propStream.read<uint32_t>(houseId)) {
				return std::make_optional(fmt::format("[x:{:d}, y:{:d}, z:{:d}] Could not read house id.", x, y, z));
			}

			house = map.houses.addHouse(houseId);
			if (!house) {
				return std::make_optional(
				    fmt::format("[x:{:d}, y:{:d}, z:{:d}] Could not create house id: {:d}", x, y, z, houseId));
			}

			tile = std::make_shared<HouseTile>(x, y, z, house);
			house->addTile(std::static_pointer_cast<HouseTile>(tile));
			isHouseTile = true;
		}

		uint8_t attribute;
		// read tile attributes
		while (propStream.read<uint8_t>(attribute)) {
			switch (attribute) {
				case OTBM_ATTR_TILE_FLAGS: {
					uint32_t flags;
					if (!propStream.read<uint32_t>(flags)) {
						return std::make_optional(
						    fmt::format("[x:{:d}, y:{:d}, z:{:d}] Failed to read tile flags.", x, y, z));
					}

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
					auto item = Item::CreateItem(propStream);
					if (!item) {
						return std::make_optional(
						    fmt::format("[x:{:d}, y:{:d}, z:{:d}] Failed to create item.", x, y, z));
					}

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
							ground_item = std::move(item);
						} else {
							tile = createTile(std::move(ground_item), item, x, y, z);
							tile->internalAddThing(item);
							item->startDecaying();
							item->setLoadedFromMap(true);
						}
					}
					break;
				}

				default:
					return std::make_optional(fmt::format("[x:{:d}, y:{:d}, z:{:d}] Unknown tile attribute.", x, y, z));
			}
		}

		for (auto& itemNode : tileNode.children) {
			if (itemNode.type != OTBM_ITEM) {
				return std::make_optional(fmt::format("[x:{:d}, y:{:d}, z:{:d}] Unknown node type.", x, y, z));
			}

			PropStream stream;
			if (!loader.getProps(itemNode, stream)) {
				return std::make_optional("Invalid item node.");
			}

			auto item = Item::CreateItem(stream);
			if (!item) {
				return std::make_optional(fmt::format("[x:{:d}, y:{:d}, z:{:d}] Failed to create item.", x, y, z));
			}

			if (!item->unserializeItemNode(loader, itemNode, stream)) {
				return std::make_optional(
				    fmt::format("[x:{:d}, y:{:d}, z:{:d}] Failed to load item {:d}.", x, y, z, item->getID()));
			}

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
					ground_item = std::move(item);
				} else {
					tile = createTile(std::move(ground_item), item, x, y, z);
					tile->internalAddThing(item);
					item->startDecaying();
					item->setLoadedFromMap(true);
				}
			}
		}

		if (!tile) {
			tile = createTile(std::move(ground_item), nullptr, x, y, z);
		}

		tile->setFlag(tileflags);

		map.setTile(x, y, z, tile);
	}
	return std::nullopt;
}

std::optional<std::string> parseTowns(OTB::Loader& loader, const OTB::Node& townsNode, Map& map)
{
	for (auto& townNode : townsNode.children) {
		PropStream propStream;
		if (townNode.type != OTBM_TOWN) {
			return std::make_optional("Unknown town node.");
		}

		if (!loader.getProps(townNode, propStream)) {
			return std::make_optional("Could not read town data.");
		}

		uint32_t townId;
		if (!propStream.read<uint32_t>(townId)) {
			return std::make_optional("Could not read town id.");
		}

		auto [townName, ok] = propStream.readString();
		if (!ok) {
			return std::make_optional("Could not read town name.");
		}

		OTBM_Destination_coords town_coords;
		if (!propStream.read(town_coords)) {
			return std::make_optional("Could not read town coordinates.");
		}

		map.towns.setTown(townId, new Town{.id = townId,
		                                   .name = std::string{townName},
		                                   .templePosition = {town_coords.x, town_coords.y, town_coords.z}});
	}
	return std::nullopt;
}

std::optional<std::string> parseWaypoints(OTB::Loader& loader, const OTB::Node& waypointsNode, Map& map)
{
	PropStream propStream;
	for (auto& node : waypointsNode.children) {
		if (node.type != OTBM_WAYPOINT) {
			return std::make_optional("Unknown waypoint node.");
		}

		if (!loader.getProps(node, propStream)) {
			return std::make_optional("Could not read waypoint data.");
		}

		auto [name, ok] = propStream.readString();
		if (!ok) {
			return std::make_optional("Could not read waypoint name.");
		}

		OTBM_Destination_coords waypoint_coords;
		if (!propStream.read(waypoint_coords)) {
			return std::make_optional("Could not read waypoint coordinates.");
		}

		map.waypoints[std::string{name}] = Position(waypoint_coords.x, waypoint_coords.y, waypoint_coords.z);
	}
	return std::nullopt;
}

} // namespace

bool IOMap::loadMap(Map* map, const std::filesystem::path& fileName)
{
	int64_t start = OTSYS_TIME();
	try {
		OTB::Loader loader{fileName.string(), OTB::Identifier{{'O', 'T', 'B', 'M'}}};
		auto& root = loader.parseTree();

		PropStream propStream;
		if (!loader.getProps(root, propStream)) {
			setLastErrorString("Could not read root property.");
			return false;
		}

		OTBM_root_header root_header;
		if (!propStream.read(root_header)) {
			setLastErrorString("Could not read header.");
			return false;
		}

		uint32_t headerVersion = root_header.version;
		if (headerVersion == 0) {
			// In otbm version 1 the count variable after splashes/fluidcontainers and stackables are saved as
			// attributes instead, this solves a lot of problems with items that are changed
			// (stackable/charges/fluidcontainer/splash) during an update.
			setLastErrorString(
			    "This map need to be upgraded by using the latest map editor version to be able to load correctly.");
			return false;
		}

		if (headerVersion > 2) {
			setLastErrorString("Unknown OTBM version detected.");
			return false;
		}

		if (root_header.majorVersionItems < 3) {
			setLastErrorString(
			    "This map need to be upgraded by using the latest map editor version to be able to load correctly.");
			return false;
		}

		if (root_header.majorVersionItems > Item::items.majorVersion) {
			setLastErrorString(
			    "The map was saved with a different items.otb version, an upgraded items.otb is required.");
			return false;
		}

		if (root_header.minorVersionItems < CLIENT_VERSION_810) {
			setLastErrorString("This map needs to be updated.");
			return false;
		}

		if (root_header.minorVersionItems > Item::items.minorVersion) {
			std::cout << "[Warning - IOMap::loadMap] This map needs an updated items.otb." << std::endl;
		}

		std::cout << "> Map size: " << root_header.width << "x" << root_header.height << '.' << std::endl;
		map->width = root_header.width;
		map->height = root_header.height;

		if (root.children.size() != 1 || root.children[0].type != OTBM_MAP_DATA) {
			setLastErrorString("Could not read data node.");
			return false;
		}

		auto& mapNode = root.children[0];
		if (auto errorString = parseMapDataAttributes(loader, mapNode, *map, fileName)) {
			setLastErrorString(*errorString);
			return false;
		}

		for (auto& mapDataNode : mapNode.children) {
			if (mapDataNode.type == OTBM_TILE_AREA) {
				if (auto errorString = parseTileArea(loader, mapDataNode, *map)) {
					setLastErrorString(*errorString);
					return false;
				}
			} else if (mapDataNode.type == OTBM_TOWNS) {
				if (auto errorString = parseTowns(loader, mapDataNode, *map)) {
					setLastErrorString(*errorString);
					return false;
				}
			} else if (mapDataNode.type == OTBM_WAYPOINTS && headerVersion > 1) {
				if (auto errorString = parseWaypoints(loader, mapDataNode, *map)) {
					setLastErrorString(*errorString);
					return false;
				}
			} else {
				setLastErrorString("Unknown map node.");
				return false;
			}
		}
	} catch (const OTB::InvalidOTBFormat& err) {
		setLastErrorString(err.what());
		return false;
	}

	std::cout << "> Map loading time: " << (OTSYS_TIME() - start) / (1000.) << " seconds." << std::endl;
	return true;
}
