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

#include "otpch.h"

#include "definitions.h"

#include <string>
#include <map>
#include <algorithm>

#include <boost/config.hpp>
#include <boost/bind.hpp>

#include "iomap.h"

#include "iomapserialize.h"

#include <cstdio>

#include "items.h"
#include "map.h"
#include "tile.h"
#include "combat.h"
#include "creature.h"

#include "player.h"
#include "configmanager.h"
#include "game.h"

extern Game g_game;
extern ConfigManager g_config;
IOMapSerialize IOMapSerialize;

Map::Map()
{
	mapWidth = 0;
	mapHeight = 0;
}

Map::~Map()
{
	//
}

bool Map::loadMap(const std::string& identifier)
{
	IOMap loader;
	if (!loader.loadMap(this, identifier)) {
		std::cout << "[Fatal - Map::loadMap] " << loader.getLastErrorString() << std::endl;
		return false;
	}

	if (!loader.loadSpawns(this)) {
		std::cout << "[Warning - Map::loadMap] Failed to load spawn data." << std::endl;
	}

	if (!loader.loadHouses(this)) {
		std::cout << "[Warning - Map::loadMap] Failed to load house data." << std::endl;
	}

	IOMapSerialize.loadHouseInfo(this);
	IOMapSerialize.loadMap(this);
	return true;
}

bool Map::saveMap()
{
	bool saved = false;

	for (uint32_t tries = 0; tries < 3; tries++) {
		if (IOMapSerialize.saveHouseInfo(this)) {
			saved = true;
			break;
		}
	}

	if (!saved) {
		return false;
	}

	saved = false;

	for (uint32_t tries = 0; tries < 3; tries++) {
		if (IOMapSerialize.saveMap(this)) {
			saved = true;
			break;
		}
	}

	return saved;
}

Tile* Map::getTile(int32_t x, int32_t y, int32_t z)
{
	if (x < 0 || x >= 0xFFFF || y < 0 || y >= 0xFFFF || z < 0 || z >= MAP_MAX_LAYERS) {
		return nullptr;
	}

	QTreeLeafNode* leaf = QTreeNode::getLeafStatic(&root, x, y);
	if (leaf) {
		Floor* floor = leaf->getFloor(z);
		if (floor) {
			return floor->tiles[x & FLOOR_MASK][y & FLOOR_MASK];
		}
	}
	return nullptr;
}

Tile* Map::getTile(const Position& pos)
{
	return getTile(pos.x, pos.y, pos.z);
}

void Map::setTile(int32_t x, int32_t y, int32_t z, Tile* newTile)
{
	if (x < 0 || x >= 0xFFFF || y < 0 || y >= 0xFFFF || z < 0 || z >= MAP_MAX_LAYERS) {
		std::cout << "ERROR: Attempt to set tile on invalid coordinate " << Position(x, y, z) << "!" << std::endl;
		return;
	}

	QTreeLeafNode::newLeaf = false;
	QTreeLeafNode* leaf = root.createLeaf(x, y, 15);

	if (QTreeLeafNode::newLeaf) {
		//update north
		QTreeLeafNode* northLeaf = root.getLeaf(x, y - FLOOR_SIZE);
		if (northLeaf) {
			northLeaf->m_leafS = leaf;
		}

		//update west leaf
		QTreeLeafNode* westLeaf = root.getLeaf(x - FLOOR_SIZE, y);
		if (westLeaf) {
			westLeaf->m_leafE = leaf;
		}

		//update south
		QTreeLeafNode* southLeaf = root.getLeaf(x, y + FLOOR_SIZE);
		if (southLeaf) {
			leaf->m_leafS = southLeaf;
		}

		//update east
		QTreeLeafNode* eastLeaf = root.getLeaf(x + FLOOR_SIZE, y);
		if (eastLeaf) {
			leaf->m_leafE = eastLeaf;
		}
	}

	Floor* floor = leaf->createFloor(z);
	uint32_t offsetX = x & FLOOR_MASK;
	uint32_t offsetY = y & FLOOR_MASK;

	Tile*& tile = floor->tiles[offsetX][offsetY];
	if (tile) {
		if (tile->hasFlag(TILESTATE_REFRESH)) {
			refreshTileMap.erase(tile);
		}
		delete tile;
	}

	tile = newTile;
	newTile->qt_node = leaf;

	if (newTile->hasFlag(TILESTATE_REFRESH)) {
		RefreshBlock_t rb;
		rb.lastRefresh = OTSYS_TIME();

		if (TileItemVector* newTileItems = newTile->getItemList()) {
			for (ItemVector::iterator it = newTileItems->getBeginDownItem(); it != newTileItems->getEndDownItem(); ++it) {
				rb.list.push_back((*it)->clone());
			}
		}

		refreshTileMap[newTile] = rb;
	}
}

bool Map::placeCreature(const Position& centerPos, Creature* creature, bool extendedPos /*=false*/, bool forceLogin /*=false*/)
{
	bool foundTile;
	bool placeInPZ;

	Tile* tile = getTile(centerPos);
	if (tile) {
		placeInPZ = tile->hasFlag(TILESTATE_PROTECTIONZONE);
		ReturnValue ret = tile->__queryAdd(0, creature, 1, FLAG_IGNOREBLOCKITEM);
		foundTile = forceLogin || ret == RET_NOERROR || ret == RET_PLAYERISNOTINVITED;
	} else {
		placeInPZ = false;
		foundTile = false;
	}

	if (!foundTile) {
		static std::vector<std::pair<int32_t, int32_t>> extendedRelList {
			{0, -2}, {-2, 0}, {0, 2}, {2, 0},

			{-1, -1}, {0, -1}, {1, -1},
			{-1,  0},          {1,  0},
			{-1,  1}, {0,  1}, {1,  1}
		};

		static std::vector<std::pair<int32_t, int32_t>> normalRelList {
			{-1, -1}, {0, -1}, {1, -1},
			{-1,  0},          {1,  0},
			{-1,  1}, {0,  1}, {1,  1}
		};

		std::vector<std::pair<int32_t, int32_t>>& relList = (extendedPos ? extendedRelList : normalRelList);

		if (extendedPos) {
			std::random_shuffle(relList.begin(), relList.begin() + 4);
			std::random_shuffle(relList.begin() + 4, relList.end());
		} else {
			std::random_shuffle(relList.begin(), relList.end());
		}

		for (const auto& it : relList) {
			Position tryPos(centerPos.x + it.first, centerPos.y + it.second, centerPos.z);

			tile = getTile(tryPos);
			if (!tile || (placeInPZ && !tile->hasFlag(TILESTATE_PROTECTIONZONE))) {
				continue;
			}

			if (tile->__queryAdd(0, creature, 1, 0) == RET_NOERROR) {
				if (extendedPos) {
					if (isSightClear(centerPos, tryPos, false)) {
						foundTile = true;
						break;
					}
				} else {
					foundTile = true;
					break;
				}
			}
		}

		if (!foundTile) {
			return false;
		}
	}

	int32_t index = 0;
	Item* toItem = nullptr;
	uint32_t flags = 0;
	Cylinder* toCylinder = tile->__queryDestination(index, creature, &toItem, flags);
	toCylinder->__internalAddThing(creature);
	Tile* toTile = toCylinder->getTile();
	toTile->qt_node->addCreature(creature);
	return true;
}

bool Map::removeCreature(Creature* creature)
{
	Tile* tile = creature->getTile();
	if (!tile) {
		return false;
	}

	tile->qt_node->removeCreature(creature);
	tile->__removeThing(creature, 0);
	return true;
}

void Map::getSpectatorsInternal(SpectatorVec& list, const Position& centerPos, int32_t minRangeX, int32_t maxRangeX, int32_t minRangeY, int32_t maxRangeY, int32_t minRangeZ, int32_t maxRangeZ, bool onlyPlayers)
{
	int_fast16_t min_y = centerPos.y + minRangeY;
	int_fast16_t min_x = centerPos.x + minRangeX;
	int_fast16_t max_y = centerPos.y + maxRangeY;
	int_fast16_t max_x = centerPos.x + maxRangeX;

	int32_t minoffset = centerPos.getZ() - maxRangeZ;
	int32_t x1 = std::min<int32_t>(0xFFFF, std::max<int32_t>(0, (min_x + minoffset)));
	int32_t y1 = std::min<int32_t>(0xFFFF, std::max<int32_t>(0, (min_y + minoffset)));

	int32_t maxoffset = centerPos.getZ() - minRangeZ;
	int32_t x2 = std::min<int32_t>(0xFFFF, std::max<int32_t>(0, (max_x + maxoffset)));
	int32_t y2 = std::min<int32_t>(0xFFFF, std::max<int32_t>(0, (max_y + maxoffset)));

	int32_t startx1 = x1 - (x1 % FLOOR_SIZE);
	int32_t starty1 = y1 - (y1 % FLOOR_SIZE);
	int32_t endx2 = x2 - (x2 % FLOOR_SIZE);
	int32_t endy2 = y2 - (y2 % FLOOR_SIZE);

	QTreeLeafNode* startLeaf = getLeaf(startx1, starty1);
	QTreeLeafNode* leafS = startLeaf;
	QTreeLeafNode* leafE;

	for (int32_t ny = starty1; ny <= endy2; ny += FLOOR_SIZE) {
		leafE = leafS;
		for (int32_t nx = startx1; nx <= endx2; nx += FLOOR_SIZE) {
			if (leafE) {
				const CreatureVector& node_list = (onlyPlayers ? leafE->player_list : leafE->creature_list);
				for (Creature* creature : node_list) {
					const Position& cpos = creature->getPosition();
					if (cpos.z < minRangeZ || cpos.z > maxRangeZ) {
						continue;
					}

					int_fast16_t offsetZ = Position::getOffsetZ(centerPos, cpos);
					if (cpos.y < (min_y + offsetZ) || cpos.y > (max_y + offsetZ)) {
						continue;
					}

					if (cpos.x < (min_x + offsetZ) || cpos.x > (max_x + offsetZ)) {
						continue;
					}

					list.insert(creature);
				}

				leafE = leafE->stepEast();
			} else {
				leafE = getLeaf(nx + FLOOR_SIZE, ny);
			}
		}

		if (leafS) {
			leafS = leafS->stepSouth();
		} else {
			leafS = getLeaf(startx1, ny + FLOOR_SIZE);
		}
	}
}

void Map::getSpectators(SpectatorVec& list, const Position& centerPos, bool multifloor /*= false*/, bool onlyPlayers /*= false*/,
                        int32_t minRangeX /*= 0*/, int32_t maxRangeX /*= 0*/,
                        int32_t minRangeY /*= 0*/, int32_t maxRangeY /*= 0*/)
{
	if (centerPos.z >= MAP_MAX_LAYERS) {
		return;
	}

	bool foundCache = false;
	bool cacheResult = false;

	minRangeX = (minRangeX == 0 ? -maxViewportX : -minRangeX);
	maxRangeX = (maxRangeX == 0 ? maxViewportX : maxRangeX);
	minRangeY = (minRangeY == 0 ? -maxViewportY : -minRangeY);
	maxRangeY = (maxRangeY == 0 ? maxViewportY : maxRangeY);

	if (minRangeX == -maxViewportX && maxRangeX == maxViewportX && minRangeY == -maxViewportY && maxRangeY == maxViewportY && multifloor) {
		if (onlyPlayers) {
			auto it = playersSpectatorCache.find(centerPos);
			if (it != playersSpectatorCache.end()) {
				if (!list.empty()) {
					const SpectatorVec& cachedList = *it->second;
					list.insert(cachedList.begin(), cachedList.end());
				} else {
					list = *it->second;
				}

				foundCache = true;
			}
		}

		if (!foundCache) {
			auto it = spectatorCache.find(centerPos);
			if (it != spectatorCache.end()) {
				if (!onlyPlayers) {
					if (!list.empty()) {
						const SpectatorVec& cachedList = *it->second;
						list.insert(cachedList.begin(), cachedList.end());
					} else {
						list = *it->second;
					}
				} else {
					const SpectatorVec& cachedList = *it->second;
					for (Creature* spectator : cachedList) {
						if (spectator->getPlayer()) {
							list.insert(spectator);
						}
					}
				}

				foundCache = true;
			} else {
				cacheResult = true;
			}
		}
	}

	if (!foundCache) {
		int32_t minRangeZ;
		int32_t maxRangeZ;

		if (multifloor) {
			if (centerPos.z > 7) {
				//underground

				//8->15
				minRangeZ = std::max<int32_t>(centerPos.getZ() - 2, 0);
				maxRangeZ = std::min<int32_t>(centerPos.getZ() + 2, MAP_MAX_LAYERS - 1);
			}
			//above ground
			else if (centerPos.z == 6) {
				minRangeZ = 0;
				maxRangeZ = 8;
			} else if (centerPos.z == 7) {
				minRangeZ = 0;
				maxRangeZ = 9;
			} else {
				minRangeZ = 0;
				maxRangeZ = 7;
			}
		} else {
			minRangeZ = centerPos.z;
			maxRangeZ = centerPos.z;
		}

		getSpectatorsInternal(list, centerPos, minRangeX, maxRangeX, minRangeY, maxRangeY, minRangeZ, maxRangeZ, onlyPlayers);

		if (cacheResult) {
			if (onlyPlayers) {
				playersSpectatorCache[centerPos].reset(new SpectatorVec(list));
			} else {
				spectatorCache[centerPos].reset(new SpectatorVec(list));
			}
		}
	}
}

const SpectatorVec& Map::getSpectators(const Position& centerPos)
{
	if (centerPos.z >= MAP_MAX_LAYERS) {
		std::shared_ptr<SpectatorVec> p(new SpectatorVec());
		SpectatorVec& list = *p;
		return list;
	}

	auto it = spectatorCache.find(centerPos);
	if (it != spectatorCache.end()) {
		return *it->second;
	}

	std::shared_ptr<SpectatorVec> p(new SpectatorVec());
	spectatorCache[centerPos] = p;
	SpectatorVec& list = *p;

	int32_t minRangeX = -maxViewportX;
	int32_t maxRangeX = maxViewportX;
	int32_t minRangeY = -maxViewportY;
	int32_t maxRangeY = maxViewportY;
	int32_t minRangeZ, maxRangeZ;

	if (centerPos.z > 7) {
		//underground

		//8->15
		minRangeZ = std::max<int32_t>(centerPos.z - 2, 0);
		maxRangeZ = std::min<int32_t>(centerPos.z + 2, MAP_MAX_LAYERS - 1);
	}
	//above ground
	else if (centerPos.z == 6) {
		minRangeZ = 0;
		maxRangeZ = 8;
	} else if (centerPos.z == 7) {
		minRangeZ = 0;
		maxRangeZ = 9;
	} else {
		minRangeZ = 0;
		maxRangeZ = 7;
	}

	getSpectatorsInternal(list, centerPos, minRangeX, maxRangeX, minRangeY, maxRangeY, minRangeZ, maxRangeZ, false);
	return list;
}

void Map::clearSpectatorCache()
{
	spectatorCache.clear();
	playersSpectatorCache.clear();
}

bool Map::canThrowObjectTo(const Position& fromPos, const Position& toPos, bool checkLineOfSight /*= true*/,
                           int32_t rangex /*= Map::maxClientViewportX*/, int32_t rangey /*= Map::maxClientViewportY*/)
{
	//z checks
	//underground 8->15
	//ground level and above 7->0
	if ((fromPos.z >= 8 && toPos.z < 8) || (toPos.z >= 8 && fromPos.z < 8)) {
		return false;
	}

	int32_t deltaz = Position::getDistanceZ(fromPos, toPos);
	if (deltaz > 2) {
		return false;
	}

	if ((Position::getDistanceX(fromPos, toPos) - deltaz) > rangex) {
		return false;
	}

	//distance checks
	if ((Position::getDistanceY(fromPos, toPos) - deltaz) > rangey) {
		return false;
	}

	if (!checkLineOfSight) {
		return true;
	}
	return isSightClear(fromPos, toPos, false);
}

bool Map::checkSightLine(const Position& fromPos, const Position& toPos) const
{
	if (fromPos == toPos) {
		return true;
	}

	Position start(fromPos.z > toPos.z ? toPos : fromPos);
	Position destination(fromPos.z > toPos.z ? fromPos : toPos);

	const int8_t mx = start.x < destination.x ? 1 : start.x == destination.x ? 0 : -1;
	const int8_t my = start.y < destination.y ? 1 : start.y == destination.y ? 0 : -1;

	int32_t A = Position::getOffsetY(destination, start);
	int32_t B = Position::getOffsetX(start, destination);
	int32_t C = -(A * destination.x + B * destination.y);

	while (!Position::areInRange<0, 0, 15>(start, destination)) {
		int32_t move_hor = std::abs(A * (start.x + mx) + B * (start.y) + C);
		int32_t move_ver = std::abs(A * (start.x) + B * (start.y + my) + C);
		int32_t move_cross = std::abs(A * (start.x + mx) + B * (start.y + my) + C);

		if (start.y != destination.y && (start.x == destination.x || move_hor > move_ver || move_hor > move_cross)) {
			start.y += my;
		}

		if (start.x != destination.x && (start.y == destination.y || move_ver > move_hor || move_ver > move_cross)) {
			start.x += mx;
		}

		const Tile* tile = const_cast<Map*>(this)->getTile(start.x, start.y, start.z);
		if (tile && tile->hasProperty(BLOCKPROJECTILE)) {
			return false;
		}
	}

	// now we need to perform a jump between floors to see if everything is clear (literally)
	while (start.z != destination.z) {
		const Tile* tile = const_cast<Map*>(this)->getTile(start.x, start.y, start.z);
		if (tile && tile->getThingCount() > 0) {
			return false;
		}

		start.z++;
	}

	return true;
}

bool Map::isSightClear(const Position& fromPos, const Position& toPos, bool floorCheck) const
{
	if (floorCheck && fromPos.z != toPos.z) {
		return false;
	}

	// Cast two converging rays and see if either yields a result.
	return checkSightLine(fromPos, toPos) || checkSightLine(toPos, fromPos);
}

const Tile* Map::canWalkTo(const Creature* creature, const Position& pos)
{
	int32_t walkCache = creature->getWalkCache(pos);
	if (walkCache == 0) {
		return nullptr;
	} else if (walkCache == 1) {
		return getTile(pos);
	}

	//used for none-cached tiles
	Tile* tile = getTile(pos);
	if (creature->getTile() != tile) {
		if (!tile || tile->__queryAdd(0, creature, 1, FLAG_PATHFINDING | FLAG_IGNOREFIELDDAMAGE) != RET_NOERROR) {
			return nullptr;
		}
	}
	return tile;
}

bool Map::getPathTo(const Creature* creature, const Position& destPos,
                    std::list<Direction>& listDir, int32_t maxSearchDist /*= -1*/)
{
	if (canWalkTo(creature, destPos) == nullptr) {
		return false;
	}

	listDir.clear();

	Position endPos = creature->getPosition();
	if (destPos.z != endPos.z) {
		return false;
	}

	std::unordered_map<uint32_t, AStarNode*> nodeTable;

	AStarNodes nodes;
	AStarNode* startNode = nodes.createOpenNode();

	nodeTable[(destPos.x * 0xFFFF) + destPos.y] = startNode;

	startNode->x = destPos.x;
	startNode->y = destPos.y;

	startNode->g = 0;
	startNode->h = nodes.getEstimatedDistance(destPos.x, destPos.y, endPos.x, endPos.y);
	startNode->f = startNode->h;
	startNode->parent = nullptr;

	Position pos;
	pos.z = destPos.z;

	static int32_t neighbourOrderList[8][2] = {
		{ -1, 0},
		{0, 1},
		{1, 0},
		{0, -1},

		//diagonal
		{ -1, -1},
		{1, -1},
		{1, 1},
		{ -1, 1},
	};

	AStarNode* found = nullptr;

	while (maxSearchDist != -1 || nodes.countClosedNodes() < 100) {
		AStarNode* n = nodes.getBestNode();
		if (!n) {
			listDir.clear();
			return false; //no path found
		}

		if (n->x == endPos.x && n->y == endPos.y) {
			found = n;
			break;
		} else {
			for (int i = 0; i < 8; ++i) {
				pos.x = n->x + neighbourOrderList[i][0];
				pos.y = n->y + neighbourOrderList[i][1];

				bool outOfRange = false;
				if (maxSearchDist != -1 && (Position::getDistanceX(endPos, pos) > maxSearchDist ||
				                            Position::getDistanceY(endPos, pos) > maxSearchDist)) {
					outOfRange = true;
				}

				if (!outOfRange) {
					const Tile* tile = canWalkTo(creature, pos);
					if (tile) {
						//The cost (g) for this neighbour
						const int_fast32_t cost = nodes.getMapWalkCost(creature, n, tile, pos);
						const int_fast32_t extraCost = nodes.getTileWalkCost(creature, tile);
						const int_fast32_t newg = n->g + cost + extraCost;
						const uint32_t tableIndex = (pos.x * 0xFFFF) + pos.y;

						//Check if the node is already in the closed/open list
						//If it exists and the nodes already on them has a lower cost (g) then we can ignore this neighbour node

						AStarNode* neighbourNode;
						auto it = nodeTable.find(tableIndex);
						if (it != nodeTable.end()) {
							neighbourNode = it->second;
						} else {
							neighbourNode = nullptr;
						}

						if (neighbourNode) {
							if (neighbourNode->g <= newg) {
								continue;    //The node on the closed/open list is cheaper than this one
							}

							nodes.openNode(neighbourNode);
						} else {
							//Does not exist in the open/closed list, create a new node
							neighbourNode = nodes.createOpenNode();

							if (!neighbourNode) {
								//seems we ran out of nodes
								listDir.clear();
								return false;
							}

							nodeTable[tableIndex] = neighbourNode;

							neighbourNode->x = pos.x;
							neighbourNode->y = pos.y;
						}

						//This node is the best node so far with this state
						neighbourNode->parent = n;
						neighbourNode->g = newg;
						neighbourNode->h = nodes.getEstimatedDistance(pos.x, pos.y, endPos.x, endPos.y);
						neighbourNode->f = newg + neighbourNode->h;
					}
				}
			}

			nodes.closeNode(n);
		}
	}

	int_fast32_t prevx = endPos.x;
	int_fast32_t prevy = endPos.y;
	int_fast32_t dx, dy;

	while (found) {
		pos.x = found->x;
		pos.y = found->y;

		found = found->parent;

		dx = pos.getX() - prevx;
		dy = pos.getY() - prevy;

		prevx = pos.x;
		prevy = pos.y;

		if (dx == -1 && dy == -1) {
			listDir.push_back(NORTHWEST);
		} else if (dx == 1 && dy == -1) {
			listDir.push_back(NORTHEAST);
		} else if (dx == -1 && dy == 1) {
			listDir.push_back(SOUTHWEST);
		} else if (dx == 1 && dy == 1) {
			listDir.push_back(SOUTHEAST);
		} else if (dx == -1) {
			listDir.push_back(WEST);
		} else if (dx == 1) {
			listDir.push_back(EAST);
		} else if (dy == -1) {
			listDir.push_back(NORTH);
		} else if (dy == 1) {
			listDir.push_back(SOUTH);
		}
	}

	return !listDir.empty();
}

bool Map::getPathMatching(const Creature* creature, std::list<Direction>& dirList,
                          const FrozenPathingConditionCall& pathCondition, const FindPathParams& fpp)
{
	dirList.clear();

	Position startPos = creature->getPosition();
	Position endPos;

	std::unordered_map<uint32_t, AStarNode*> nodeTable;

	AStarNodes nodes;
	AStarNode* startNode = nodes.createOpenNode();

	nodeTable[(startPos.x * 0xFFFF) + startPos.y] = startNode;

	startNode->x = startPos.x;
	startNode->y = startPos.y;

	startNode->f = 0;
	startNode->parent = nullptr;

	Position pos(0, 0, startPos.z);
	int32_t bestMatch = 0;

	static int32_t neighbourOrderList[8][2] = {
		{ -1, 0},
		{0, 1},
		{1, 0},
		{0, -1},

		//diagonal
		{ -1, -1},
		{1, -1},
		{1, 1},
		{ -1, 1},
	};

	AStarNode* found = nullptr;

	while (fpp.maxSearchDist != -1 || nodes.countClosedNodes() < 100) {
		AStarNode* n = nodes.getBestNode();
		if (!n) {
			if (found) {
				//not quite what we want, but we found something
				break;
			}

			dirList.clear();
			return false; //no path found
		}

		if (pathCondition(startPos, Position(n->x, n->y, startPos.z), fpp, bestMatch)) {
			found = n;
			endPos = Position(n->x, n->y, startPos.z);

			if (bestMatch == 0) {
				break;
			}
		}

		int32_t dirCount = (fpp.allowDiagonal ? 8 : 4);

		for (int32_t i = 0; i < dirCount; ++i) {
			pos.x = n->x + neighbourOrderList[i][0];
			pos.y = n->y + neighbourOrderList[i][1];

			bool inRange = true;
			if (fpp.maxSearchDist != -1 && (Position::getDistanceX(startPos, pos) > fpp.maxSearchDist ||
			                                Position::getDistanceY(startPos, pos) > fpp.maxSearchDist)) {
				inRange = false;
			}

			if (fpp.keepDistance) {
				if (!pathCondition.isInRange(startPos, pos, fpp)) {
					inRange = false;
				}
			}

			if (inRange) {
				const Tile* tile = canWalkTo(creature, pos);
				if (tile) {
					//The cost (g) for this neighbour
					const int_fast32_t cost = nodes.getMapWalkCost(creature, n, tile, pos);
					const int_fast32_t extraCost = nodes.getTileWalkCost(creature, tile);
					const int_fast32_t newf = n->f + cost + extraCost;
					const uint32_t tableIndex = (pos.x * 0xFFFF) + pos.y;

					//Check if the node is already in the closed/open list
					//If it exists and the nodes already on them has a lower cost (g) then we can ignore this neighbour node

					AStarNode* neighbourNode;
					auto it = nodeTable.find(tableIndex);
					if (it != nodeTable.end()) {
						neighbourNode = it->second;
					} else {
						neighbourNode = nullptr;
					}

					if (neighbourNode) {
						if (neighbourNode->f <= newf) {
							//The node on the closed/open list is cheaper than this one
							continue;
						}

						nodes.openNode(neighbourNode);
					} else {
						//Does not exist in the open/closed list, create a new node
						neighbourNode = nodes.createOpenNode();

						if (!neighbourNode) {
							if (found) {
								//not quite what we want, but we found something
								break;
							}

							//seems we ran out of nodes
							dirList.clear();
							return false;
						}

						nodeTable[tableIndex] = neighbourNode;

						neighbourNode->x = pos.x;
						neighbourNode->y = pos.y;
					}

					//This node is the best node so far with this state
					neighbourNode->parent = n;
					neighbourNode->f = newf;
				}
			}
		}

		nodes.closeNode(n);
	}

	if (!found) {
		return false;
	}

	int_fast32_t prevx = endPos.x;
	int_fast32_t prevy = endPos.y;
	int_fast32_t dx, dy;

	found = found->parent;
	while (found) {
		pos.x = found->x;
		pos.y = found->y;

		dx = pos.getX() - prevx;
		dy = pos.getY() - prevy;

		prevx = pos.x;
		prevy = pos.y;

		if (dx == 1 && dy == 1) {
			dirList.push_front(NORTHWEST);
		} else if (dx == -1 && dy == 1) {
			dirList.push_front(NORTHEAST);
		} else if (dx == 1 && dy == -1) {
			dirList.push_front(SOUTHWEST);
		} else if (dx == -1 && dy == -1) {
			dirList.push_front(SOUTHEAST);
		} else if (dx == 1) {
			dirList.push_front(WEST);
		} else if (dx == -1) {
			dirList.push_front(EAST);
		} else if (dy == 1) {
			dirList.push_front(NORTH);
		} else if (dy == -1) {
			dirList.push_front(SOUTH);
		}

		found = found->parent;
	}
	return true;
}

//*********** AStarNodes *************

AStarNodes::AStarNodes()
{
	curNode = 0;
	openNodes.reset();
}

AStarNode* AStarNodes::createOpenNode()
{
	if (curNode >= MAX_NODES) {
		return nullptr;
	}

	uint32_t ret_node = curNode++;
	openNodes[ret_node] = 1;
	return &nodes[ret_node];
}

AStarNode* AStarNodes::getBestNode()
{
	if (curNode == 0) {
		return nullptr;
	}

	int32_t best_node_f = std::numeric_limits<int32_t>::max();
	int32_t best_node = -1;
	for (uint32_t i = 0; i < curNode; i++) {
		if (nodes[i].f < best_node_f && openNodes[i] == 1) {
			best_node_f = nodes[i].f;
			best_node = i;
		}
	}

	if (best_node >= 0) {
		return &nodes[best_node];
	}
	return nullptr;
}

void AStarNodes::closeNode(AStarNode* node)
{
	uint32_t pos = GET_NODE_INDEX(node);
	if (pos >= MAX_NODES) {
		assert(pos >= MAX_NODES);
		std::cout << "AStarNodes. trying to close node out of range" << std::endl;
		return;
	}

	openNodes[pos] = 0;
}

void AStarNodes::openNode(AStarNode* node)
{
	uint32_t pos = GET_NODE_INDEX(node);
	if (pos >= MAX_NODES) {
		assert(pos >= MAX_NODES);
		std::cout << "AStarNodes. trying to open node out of range" << std::endl;
		return;
	}

	openNodes[pos] = 1;
}

uint32_t AStarNodes::countClosedNodes()
{
	uint32_t counter = 0;
	for (uint32_t i = 0; i < curNode; i++) {
		if (openNodes[i] == 0) {
			counter++;
		}
	}
	return counter;
}

uint32_t AStarNodes::countOpenNodes()
{
	uint32_t counter = 0;
	for (uint32_t i = 0; i < curNode; i++) {
		if (openNodes[i] == 1) {
			counter++;
		}
	}
	return counter;
}

int32_t AStarNodes::getMapWalkCost(const Creature* creature, AStarNode* node,
                                   const Tile* neighbourTile, const Position& neighbourPos)
{
	if (std::abs(node->x - neighbourPos.x) == std::abs(node->y - neighbourPos.y)) {
		//diagonal movement extra cost
		return MAP_DIAGONALWALKCOST;
	}
	return MAP_NORMALWALKCOST;
}

int32_t AStarNodes::getTileWalkCost(const Creature* creature, const Tile* tile)
{
	int cost = 0;
	if (tile->getTopVisibleCreature(creature) != nullptr) {
		//destroy creature cost
		cost += MAP_NORMALWALKCOST * 3;
	}

	if (const MagicField* field = tile->getFieldItem()) {
		CombatType_t combatType = field->getCombatType();
		if (!creature->isImmune(combatType) && !creature->hasCondition(Combat::DamageToConditionType(combatType))) {
			cost += MAP_NORMALWALKCOST * 18;
		}
	}
	return cost;
}

int32_t AStarNodes::getEstimatedDistance(int32_t x, int32_t y, int32_t xGoal, int32_t yGoal)
{
	int32_t h_diagonal = std::min<int32_t>(std::abs(x - xGoal), std::abs(y - yGoal));
	int32_t h_straight = (std::abs(x - xGoal) + std::abs(y - yGoal));
	return MAP_DIAGONALWALKCOST * h_diagonal + MAP_NORMALWALKCOST * (h_straight - 2 * h_diagonal);
}

//*********** Floor **************

Floor::~Floor()
{
	for (uint32_t i = 0; i < FLOOR_SIZE; ++i) {
		for (uint32_t j = 0; j < FLOOR_SIZE; ++j) {
			delete tiles[i][j];
		}
	}
}

//**************** QTreeNode **********************
QTreeNode::QTreeNode()
{
	m_isLeaf = false;
	m_child[0] = nullptr;
	m_child[1] = nullptr;
	m_child[2] = nullptr;
	m_child[3] = nullptr;
}

QTreeNode::~QTreeNode()
{
	delete m_child[0];
	delete m_child[1];
	delete m_child[2];
	delete m_child[3];
}

QTreeLeafNode* QTreeNode::getLeaf(uint32_t x, uint32_t y)
{
	if (!isLeaf()) {
		uint32_t index = ((x & 0x8000) >> 15) | ((y & 0x8000) >> 14);
		if (m_child[index]) {
			return m_child[index]->getLeaf(x * 2, y * 2);
		} else {
			return nullptr;
		}
	} else {
		return static_cast<QTreeLeafNode*>(this);
	}
}

QTreeLeafNode* QTreeNode::getLeafStatic(QTreeNode* root, uint32_t x, uint32_t y)
{
	QTreeNode* currentNode = root;
	while (currentNode) {
		if (!currentNode->isLeaf()) {
			uint32_t index = ((x & 0x8000) >> 15) | ((y & 0x8000) >> 14);
			if (currentNode->m_child[index]) {
				currentNode = currentNode->m_child[index];
				x *= 2;
				y *= 2;
			} else {
				return nullptr;
			}
		} else {
			return static_cast<QTreeLeafNode*>(currentNode);
		}
	}
	return nullptr;
}

QTreeLeafNode* QTreeNode::createLeaf(uint32_t x, uint32_t y, uint32_t level)
{
	if (!isLeaf()) {
		uint32_t index = ((x & 0x8000) >> 15) | ((y & 0x8000) >> 14);
		if (!m_child[index]) {
			if (level != FLOOR_BITS) {
				m_child[index] = new QTreeNode();
			} else {
				m_child[index] = new QTreeLeafNode();
				QTreeLeafNode::newLeaf = true;
			}
		}
		return m_child[index]->createLeaf(x * 2, y * 2, level - 1);
	}
	return static_cast<QTreeLeafNode*>(this);
}

//************ LeafNode ************************
bool QTreeLeafNode::newLeaf = false;
QTreeLeafNode::QTreeLeafNode()
{
	for (uint32_t i = 0; i < MAP_MAX_LAYERS; ++i) {
		m_array[i] = nullptr;
	}

	m_isLeaf = true;
	m_leafS = nullptr;
	m_leafE = nullptr;
}

QTreeLeafNode::~QTreeLeafNode()
{
	for (uint32_t i = 0; i < MAP_MAX_LAYERS; ++i) {
		delete m_array[i];
	}
}

Floor* QTreeLeafNode::createFloor(uint32_t z)
{
	if (!m_array[z]) {
		m_array[z] = new Floor();
	}
	return m_array[z];
}

void QTreeLeafNode::addCreature(Creature* c)
{
	creature_list.push_back(c);

	if (c->getPlayer()) {
		player_list.push_back(c);
	}
}

void QTreeLeafNode::removeCreature(Creature* c)
{
	CreatureVector::iterator iter = std::find(creature_list.begin(), creature_list.end(), c);
	assert(iter != creature_list.end());
	std::swap(*iter, creature_list.back());
	creature_list.pop_back();

	if (c->getPlayer()) {
		iter = std::find(player_list.begin(), player_list.end(), c);
		assert(iter != player_list.end());
		std::swap(*iter, player_list.back());
		player_list.pop_back();
	}
}

uint32_t Map::clean()
{
	uint64_t start = OTSYS_TIME();
	uint32_t count = 0, tiles = 0;

	if (g_game.getGameState() == GAME_STATE_NORMAL) {
		g_game.setGameState(GAME_STATE_MAINTAIN);
	}

	for (int32_t z = 0; z < (int32_t)MAP_MAX_LAYERS; z++) {
		for (uint32_t y = 1; y <= mapHeight; y++) {
			for (uint32_t x = 1; x <= mapWidth; x++) {
				Tile* tile = getTile(x, y, z);
				if (!tile || tile->hasFlag(TILESTATE_PROTECTIONZONE) || !tile->getItemList()) {
					continue;
				}

				++tiles;
				TileItemVector* itemList = tile->getItemList();
				ItemVector::iterator it = itemList->begin(), end = itemList->end();
				while (it != end) {
					if ((*it)->isCleanable()) {
						g_game.internalRemoveItem(*it, -1);
						it = itemList->begin();
						end = itemList->end();
						++count;
					} else {
						++it;
					}
				}
			}
		}
	}

	if (g_game.getGameState() == GAME_STATE_MAINTAIN) {
		g_game.setGameState(GAME_STATE_NORMAL);
	}

	std::cout << "> CLEAN: Removed " << count << " item" << (count != 1 ? "s" : "")
	          << " from " << tiles << " tile" << (tiles != 1 ? "s" : "") << " in "
	          << (OTSYS_TIME() - start) / (1000.) << " seconds." << std::endl;
	return count;
}
