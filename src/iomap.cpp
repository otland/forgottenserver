/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2014  Mark Samman <mark.samman@gmail.com>
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

#include "otpch.h"

#include "iomap.h"
#include "map.h"

#include "tile.h"
#include "item.h"
#include "container.h"
#include "teleport.h"
#include "fileloader.h"
#include "town.h"

#include "beds.h"

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

Tile* IOMap::createTile(Item*& ground, Item* item, int px, int py, int pz)
{
	if (!ground) {
		return new StaticTile(px, py, pz);
	}

	Tile* tile;
	if ((item && item->isBlocking()) || ground->isBlocking()) {
		tile = new StaticTile(px, py, pz);
	} else {
		tile = new DynamicTile(px, py, pz);
	}

	tile->__internalAddThing(ground);
	ground->__startDecaying();
	ground = nullptr;
	return tile;
}

bool IOMap::loadMap(Map* map, const std::string& identifier)
{
	int64_t start = OTSYS_TIME();

	FileLoader f;

	if (!f.openFile(identifier.c_str(), "OTBM")) {
		std::ostringstream ss;
		ss << "Could not open the file " << identifier << '.';
		setLastErrorString(ss.str());
		return false;
	}

	uint32_t type;
	PropStream propStream;

	NODE root = f.getChildNode(nullptr, type);
	if (!f.getProps(root, propStream)) {
		setLastErrorString("Could not read root property.");
		return false;
	}

	OTBM_root_header* root_header;
	if (!propStream.GET_STRUCT(root_header)) {
		setLastErrorString("Could not read header.");
		return false;
	}

	uint32_t headerVersion = root_header->version;
	if (headerVersion <= 0) {
		//In otbm version 1 the count variable after splashes/fluidcontainers and stackables
		//are saved as attributes instead, this solves alot of problems with items
		//that is changed (stackable/charges/fluidcontainer/splash) during an update.
		setLastErrorString("This map need to be upgraded by using the latest map editor version to be able to load correctly.");
		return false;
	}

	if (headerVersion > 2) {
		setLastErrorString("Unknown OTBM version detected.");
		return false;
	}

	if (root_header->majorVersionItems < 3) {
		setLastErrorString("This map need to be upgraded by using the latest map editor version to be able to load correctly.");
		return false;
	}

	if (root_header->majorVersionItems > Items::dwMajorVersion) {
		setLastErrorString("The map was saved with a different items.otb version, an upgraded items.otb is required.");
		return false;
	}

	if (root_header->minorVersionItems < CLIENT_VERSION_810) {
		setLastErrorString("This map needs to be updated.");
		return false;
	}

	if (root_header->minorVersionItems > Items::dwMinorVersion) {
		std::cout << "[Warning - IOMap::loadMap] This map needs an updated items.otb." << std::endl;
	}

	std::cout << "> Map size: " << root_header->width << "x" << root_header->height << '.' << std::endl;
	map->mapWidth = root_header->width;
	map->mapHeight = root_header->height;

	NODE nodeMap = f.getChildNode(root, type);

	if (type != OTBM_MAP_DATA) {
		setLastErrorString("Could not read data node.");
		return false;
	}

	if (!f.getProps(nodeMap, propStream)) {
		setLastErrorString("Could not read map data attributes.");
		return false;
	}

	std::string mapDescription;
	std::string tmp;

	uint8_t attribute;
	while (propStream.GET_UCHAR(attribute)) {
		switch (attribute) {
			case OTBM_ATTR_DESCRIPTION:
				if (!propStream.GET_STRING(mapDescription)) {
					setLastErrorString("Invalid description tag.");
					return false;
				}
				break;

			case OTBM_ATTR_EXT_SPAWN_FILE:
				if (!propStream.GET_STRING(tmp)) {
					setLastErrorString("Invalid spawn tag.");
					return false;
				}

				map->spawnfile = identifier.substr(0, identifier.rfind('/') + 1);
				map->spawnfile += tmp;
				break;

			case OTBM_ATTR_EXT_HOUSE_FILE:
				if (!propStream.GET_STRING(tmp)) {
					setLastErrorString("Invalid house tag.");
					return false;
				}

				map->housefile = identifier.substr(0, identifier.rfind('/') + 1);
				map->housefile += tmp;
				break;

			default:
				setLastErrorString("Unknown header node.");
				return false;
		}
	}

	NODE nodeMapData = f.getChildNode(nodeMap, type);
	while (nodeMapData != NO_NODE) {
		if (f.getError() != ERROR_NONE) {
			setLastErrorString("Invalid map node.");
			return false;
		}

		if (type == OTBM_TILE_AREA) {
			if (!f.getProps(nodeMapData, propStream)) {
				setLastErrorString("Invalid map node.");
				return false;
			}

			OTBM_Destination_coords* area_coord;
			if (!propStream.GET_STRUCT(area_coord)) {
				setLastErrorString("Invalid map node.");
				return false;
			}

			int32_t base_x = area_coord->_x;
			int32_t base_y = area_coord->_y;
			int32_t base_z = area_coord->_z;

			NODE nodeTile = f.getChildNode(nodeMapData, type);
			while (nodeTile != NO_NODE) {
				if (f.getError() != ERROR_NONE) {
					setLastErrorString("Could not read node data.");
					return false;
				}

				if (type != OTBM_TILE && type != OTBM_HOUSETILE) {
					setLastErrorString("Unknown tile node.");
					return false;
				}

				if (!f.getProps(nodeTile, propStream)) {
					setLastErrorString("Could not read node data.");
					return false;
				}

				OTBM_Tile_coords* tile_coord;
				if (!propStream.GET_STRUCT(tile_coord)) {
					setLastErrorString("Could not read tile position.");
					return false;
				}

				uint16_t px = base_x + tile_coord->_x;
				uint16_t py = base_y + tile_coord->_y;
				uint16_t pz = base_z;

				bool isHouseTile = false;
				House* house = nullptr;
				Tile* tile = nullptr;
				Item* ground_item = nullptr;
				uint32_t tileflags = TILESTATE_NONE;

				if (type == OTBM_HOUSETILE) {
					uint32_t _houseid;
					if (!propStream.GET_ULONG(_houseid)) {
						std::ostringstream ss;
						ss << "[x:" << px << ", y:" << py << ", z:" << pz << "] Could not read house id.";
						setLastErrorString(ss.str());
						return false;
					}

					house = Houses::getInstance().addHouse(_houseid);
					if (!house) {
						std::ostringstream ss;
						ss << "[x:" << px << ", y:" << py << ", z:" << pz << "] Could not create house id: " << _houseid;
						setLastErrorString(ss.str());
						return false;
					}

					tile = new HouseTile(px, py, pz, house);
					house->addTile(static_cast<HouseTile*>(tile));
					isHouseTile = true;
				}

				//read tile attributes
				while (propStream.GET_UCHAR(attribute)) {
					switch (attribute) {
						case OTBM_ATTR_TILE_FLAGS: {
							uint32_t flags;
							if (!propStream.GET_ULONG(flags)) {
								std::ostringstream ss;
								ss << "[x:" << px << ", y:" << py << ", z:" << pz << "] Failed to read tile flags.";
								setLastErrorString(ss.str());
								return false;
							}

							if ((flags & TILESTATE_PROTECTIONZONE) == TILESTATE_PROTECTIONZONE) {
								tileflags |= TILESTATE_PROTECTIONZONE;
							} else if ((flags & TILESTATE_NOPVPZONE) == TILESTATE_NOPVPZONE) {
								tileflags |= TILESTATE_NOPVPZONE;
							} else if ((flags & TILESTATE_PVPZONE) == TILESTATE_PVPZONE) {
								tileflags |= TILESTATE_PVPZONE;
							}

							if ((flags & TILESTATE_NOLOGOUT) == TILESTATE_NOLOGOUT) {
								tileflags |= TILESTATE_NOLOGOUT;
							}
							break;
						}

						case OTBM_ATTR_ITEM: {
							Item* item = Item::CreateItem(propStream);
							if (!item) {
								std::ostringstream ss;
								ss << "[x:" << px << ", y:" << py << ", z:" << pz << "] Failed to create item.";
								setLastErrorString(ss.str());
								return false;
							}

							if (isHouseTile && item->isMoveable()) {
								std::cout << "[Warning - IOMap::loadMap] Moveable item with ID: " << item->getID() << ", in house: " << house->getId() << ", at position [x: " << px << ", y: " << py << ", z: " << pz << "]." << std::endl;
								delete item;
								item = nullptr;
							} else {
								if (item->getItemCount() <= 0) {
									item->setItemCount(1);
								}

								if (tile) {
									tile->__internalAddThing(item);
									item->__startDecaying();
									item->setLoadedFromMap(true);
								} else if (item->isGroundTile()) {
									delete ground_item;
									ground_item = item;
								} else {
									tile = createTile(ground_item, item, px, py, pz);
									tile->__internalAddThing(item);
									item->__startDecaying();
									item->setLoadedFromMap(true);
								}
							}

							break;
						}

						default:
							std::ostringstream ss;
							ss << "[x:" << px << ", y:" << py << ", z:" << pz << "] Unknown tile attribute.";
							setLastErrorString(ss.str());
							return false;
					}
				}

				NODE nodeItem = f.getChildNode(nodeTile, type);
				while (nodeItem) {
					if (type != OTBM_ITEM) {
						std::ostringstream ss;
						ss << "[x:" << px << ", y:" << py << ", z:" << pz << "] Unknown node type.";
						setLastErrorString(ss.str());
						return false;
					}

					PropStream stream;
					if (!f.getProps(nodeItem, stream)) {
						setLastErrorString("Invalid item node.");
						return false;
					}

					Item* item = Item::CreateItem(stream);
					if (!item) {
						std::ostringstream ss;
						ss << "[x:" << px << ", y:" << py << ", z:" << pz << "] Failed to create item.";
						setLastErrorString(ss.str());
						return false;
					}

					if (!item->unserializeItemNode(f, nodeItem, stream)) {
						std::ostringstream ss;
						ss << "[x:" << px << ", y:" << py << ", z:" << pz << "] Failed to load item " << item->getID() << '.';
						setLastErrorString(ss.str());
						delete item;
						return false;
					}

					if (isHouseTile && item->isMoveable()) {
						std::cout << "[Warning - IOMap::loadMap] Moveable item with ID: " << item->getID() << ", in house: " << house->getId() << ", at position [x: " << px << ", y: " << py << ", z: " << pz << "]." << std::endl;
						delete item;
					} else {
						if (item->getItemCount() <= 0) {
							item->setItemCount(1);
						}

						if (tile) {
							tile->__internalAddThing(item);
							item->__startDecaying();
							item->setLoadedFromMap(true);
						} else if (item->isGroundTile()) {
							delete ground_item;
							ground_item = item;
						} else {
							tile = createTile(ground_item, item, px, py, pz);
							tile->__internalAddThing(item);
							item->__startDecaying();
							item->setLoadedFromMap(true);
						}
					}

					nodeItem = f.getNextNode(nodeItem, type);
				}

				if (!tile) {
					tile = createTile(ground_item, nullptr, px, py, pz);
				}

				tile->setFlag((tileflags_t)tileflags);

				map->setTile(px, py, pz, tile);

				nodeTile = f.getNextNode(nodeTile, type);
			}
		} else if (type == OTBM_TOWNS) {
			NODE nodeTown = f.getChildNode(nodeMapData, type);
			while (nodeTown != NO_NODE) {
				if (type != OTBM_TOWN) {
					setLastErrorString("Unknown town node.");
					return false;
				}

				if (!f.getProps(nodeTown, propStream)) {
					setLastErrorString("Could not read town data.");
					return false;
				}

				uint32_t townid;
				if (!propStream.GET_ULONG(townid)) {
					setLastErrorString("Could not read town id.");
					return false;
				}

				Town* town = Towns::getInstance().getTown(townid);
				if (!town) {
					town = new Town(townid);
					Towns::getInstance().addTown(townid, town);
				}

				std::string townName;
				if (!propStream.GET_STRING(townName)) {
					setLastErrorString("Could not read town name.");
					return false;
				}

				town->setName(townName);

				OTBM_Destination_coords* town_coords;
				if (!propStream.GET_STRUCT(town_coords)) {
					setLastErrorString("Could not read town coordinates.");
					return false;
				}

				town->setTemplePos(Position(town_coords->_x, town_coords->_y, town_coords->_z));

				nodeTown = f.getNextNode(nodeTown, type);
			}
		} else if (type == OTBM_WAYPOINTS && headerVersion > 1) {
			NODE nodeWaypoint = f.getChildNode(nodeMapData, type);
			while (nodeWaypoint != NO_NODE) {
				if (type != OTBM_WAYPOINT) {
					setLastErrorString("Unknown waypoint node.");
					return false;
				}

				if (!f.getProps(nodeWaypoint, propStream)) {
					setLastErrorString("Could not read waypoint data.");
					return false;
				}

				std::string name;
				if (!propStream.GET_STRING(name)) {
					setLastErrorString("Could not read waypoint name.");
					return false;
				}

				OTBM_Destination_coords* waypoint_coords;
				if (!propStream.GET_STRUCT(waypoint_coords)) {
					setLastErrorString("Could not read waypoint coordinates.");
					return false;
				}

				map->waypoints[name] = Position(waypoint_coords->_x, waypoint_coords->_y, waypoint_coords->_z);

				nodeWaypoint = f.getNextNode(nodeWaypoint, type);
			}
		} else {
			setLastErrorString("Unknown map node.");
			return false;
		}

		nodeMapData = f.getNextNode(nodeMapData, type);
	}

	std::cout << "> Map loading time: " << (OTSYS_TIME() - start) / (1000.) << " seconds." << std::endl;
	return true;
}
