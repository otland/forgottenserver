// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "otpch.h"

#include "map.h"

#include "combat.h"
#include "creature.h"
#include "game.h"
#include "iomap.h"
#include "iomapserialize.h"
#include "monster.h"
#include "spectators.h"

extern Game g_game;

bool Map::loadMap(const std::string& identifier, bool loadHouses, bool isCalledByLua)
{
	IOMap loader;
	if (!loader.loadMap(this, identifier)) {
		std::cout << "[Fatal - Map::loadMap] " << loader.getLastErrorString() << std::endl;
		return false;
	}

	if (!IOMap::loadSpawns(this, isCalledByLua)) {
		std::cout << "[Warning - Map::loadMap] Failed to load spawn data." << std::endl;
	}

	if (loadHouses && !isCalledByLua) {
		if (!IOMap::loadHouses(this)) {
			std::cout << "[Warning - Map::loadMap] Failed to load house data." << std::endl;
		}

		IOMapSerialize::loadHouseInfo();
		IOMapSerialize::loadHouseItems(this);
	}
	return true;
}

bool Map::save()
{
	bool saved = false;
	for (uint32_t tries = 0; tries < 3; tries++) {
		if (IOMapSerialize::saveHouseInfo()) {
			saved = true;
			break;
		}
	}

	if (!saved) {
		return false;
	}

	saved = false;
	for (uint32_t tries = 0; tries < 3; tries++) {
		if (IOMapSerialize::saveHouseItems()) {
			saved = true;
			break;
		}
	}
	return saved;
}

std::shared_ptr<Tile> Map::getTile(uint16_t x, uint16_t y, uint8_t z) const
{
	if (z >= MAP_MAX_LAYERS) {
		return nullptr;
	}

	const QTreeLeafNode* leaf = QTreeNode::getLeafStatic<const QTreeLeafNode*, const QTreeNode*>(&root, x, y);
	if (!leaf) {
		return nullptr;
	}

	const Floor* floor = leaf->getFloor(z);
	if (!floor) {
		return nullptr;
	}
	return floor->tiles[x & FLOOR_MASK][y & FLOOR_MASK];
}

void Map::setTile(uint16_t x, uint16_t y, uint8_t z, const std::shared_ptr<Tile>& newTile)
{
	if (z >= MAP_MAX_LAYERS) {
		std::cout << "ERROR: Attempt to set tile on invalid coordinate " << Position(x, y, z) << "!" << std::endl;
		return;
	}

	QTreeLeafNode::newLeaf = false;
	QTreeLeafNode* leaf = root.createLeaf(x, y, 15);

	if (QTreeLeafNode::newLeaf) {
		// update north
		QTreeLeafNode* northLeaf = root.getLeaf(x, y - FLOOR_SIZE);
		if (northLeaf) {
			northLeaf->leafS = leaf;
		}

		// update west leaf
		QTreeLeafNode* westLeaf = root.getLeaf(x - FLOOR_SIZE, y);
		if (westLeaf) {
			westLeaf->leafE = leaf;
		}

		// update south
		QTreeLeafNode* southLeaf = root.getLeaf(x, y + FLOOR_SIZE);
		if (southLeaf) {
			leaf->leafS = southLeaf;
		}

		// update east
		QTreeLeafNode* eastLeaf = root.getLeaf(x + FLOOR_SIZE, y);
		if (eastLeaf) {
			leaf->leafE = eastLeaf;
		}
	}

	Floor* floor = leaf->createFloor(z);
	uint32_t offsetX = x & FLOOR_MASK;
	uint32_t offsetY = y & FLOOR_MASK;

	auto& tile = floor->tiles[offsetX][offsetY];
	if (tile) {
		TileItemVector* items = newTile->getItemList();
		if (items) {
			for (auto it = items->rbegin(), end = items->rend(); it != end; ++it) {
				tile->addThing(*it);
			}
			items->clear();
		}

		if (const auto& ground = newTile->getGround()) {
			tile->addThing(ground);
			newTile->setGround(nullptr);
		}
	} else {
		tile = newTile;
	}
}

void Map::removeTile(uint16_t x, uint16_t y, uint8_t z)
{
	if (z >= MAP_MAX_LAYERS) {
		return;
	}

	const QTreeLeafNode* leaf = QTreeNode::getLeafStatic<const QTreeLeafNode*, const QTreeNode*>(&root, x, y);
	if (!leaf) {
		return;
	}

	const Floor* floor = leaf->getFloor(z);
	if (!floor) {
		return;
	}

	if (const auto& tile = floor->tiles[x & FLOOR_MASK][y & FLOOR_MASK]) {
		if (const CreatureVector* creatures = tile->getCreatures()) {
			for (int32_t i = creatures->size(); --i >= 0;) {
				if (const auto& player = (*creatures)[i]->getPlayer()) {
					g_game.internalTeleport(player, player->getTown()->templePosition, false, FLAG_NOLIMIT);
				} else {
					g_game.removeCreature((*creatures)[i]);
				}
			}
		}

		if (TileItemVector* items = tile->getItemList()) {
			for (auto it = items->begin(), end = items->end(); it != end; ++it) {
				g_game.internalRemoveItem(*it);
			}
		}

		if (const auto& ground = tile->getGround()) {
			g_game.internalRemoveItem(ground);
			tile->setGround(nullptr);
		}
	}
}

bool Map::placeCreature(const Position& centerPos, const std::shared_ptr<Creature>& creature,
                        bool extendedPos /* = false*/, bool forceLogin /* = false*/)
{
	bool foundTile = false;
	bool placeInPZ = false;

	auto tile = getTile(centerPos.x, centerPos.y, centerPos.z);
	if (tile) {
		placeInPZ = tile->hasFlag(TILESTATE_PROTECTIONZONE);
		ReturnValue ret = tile->queryAdd(0, creature, 1, FLAG_IGNOREBLOCKITEM);
		foundTile = forceLogin || ret == RETURNVALUE_NOERROR || ret == RETURNVALUE_PLAYERISNOTINVITED;
	}

	if (!foundTile) {
		static auto extendedRelList = std::vector{std::pair{0, -2}, {2, 0},  {0, 2}, {-2, 0}, {-1, -1}, {0, -1},
		                                          {1, -1},          {-1, 0}, {1, 0}, {-1, 1}, {0, 1},   {1, 1}};

		static auto normalRelList =
		    std::vector{std::pair{-1, -1}, {0, -1}, {1, -1}, {-1, 0}, {1, 0}, {-1, 1}, {0, 1}, {1, 1}};

		auto& relList = extendedPos ? extendedRelList : normalRelList;

		if (extendedPos) {
			std::shuffle(relList.begin(), relList.begin() + 4, getRandomGenerator());
			std::shuffle(relList.begin() + 4, relList.end(), getRandomGenerator());
		} else {
			std::shuffle(relList.begin(), relList.end(), getRandomGenerator());
		}

		for (const auto& it : relList) {
			Position tryPos(centerPos.x + it.first, centerPos.y + it.second, centerPos.z);

			tile = getTile(tryPos.x, tryPos.y, tryPos.z);
			if (!tile || (placeInPZ && !tile->hasFlag(TILESTATE_PROTECTIONZONE))) {
				continue;
			}

			if (tile->queryAdd(0, creature, 1, 0) == RETURNVALUE_NOERROR) {
				if (!extendedPos || isSightClear(centerPos, tryPos, false)) {
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
	uint32_t flags = 0;
	std::shared_ptr<Item> toItem = nullptr;

	const auto& toThing = tile->queryDestination(index, creature, toItem, flags);
	toThing->internalAddThing(creature);

	const Position& dest = toThing->getPosition();
	getQTNode(dest.x, dest.y)->addCreature(creature);
	return true;
}

void Map::moveCreature(const std::shared_ptr<Creature>& creature, const std::shared_ptr<Tile>& newTile,
                       bool forceTeleport /* = false*/)
{
	const auto& oldTile = creature->getTile();

	// If the tile does not have the creature it means that the creature is ready for elimination, we skip the move.
	if (!oldTile->hasCreature(creature)) {
		return;
	}

	Position oldPos = oldTile->getPosition();
	Position newPos = newTile->getPosition();

	bool teleport = forceTeleport || !newTile->getGround() || !oldPos.isInRange(newPos, 1, 1, 0);

	SpectatorVec spectators, newPosSpectators;
	getSpectators(spectators, oldPos, true);
	getSpectators(newPosSpectators, newPos, true);
	spectators.addSpectators(newPosSpectators);

	std::vector<int32_t> oldStackPosVector;
	for (const auto& spectator : spectators) {
		if (const auto& tmpPlayer = spectator->getPlayer()) {
			if (tmpPlayer->canSeeCreature(creature)) {
				oldStackPosVector.push_back(oldTile->getClientIndexOfCreature(tmpPlayer, creature));
			} else {
				oldStackPosVector.push_back(-1);
			}
		}
	}

	// remove the creature
	oldTile->removeThing(creature, 0);

	QTreeLeafNode* leaf = getQTNode(oldPos.x, oldPos.y);
	QTreeLeafNode* new_leaf = getQTNode(newPos.x, newPos.y);

	// Switch the node ownership
	if (leaf != new_leaf) {
		leaf->removeCreature(creature);
		new_leaf->addCreature(creature);
	}

	// add the creature
	newTile->addThing(creature);

	if (!teleport) {
		if (oldPos.y > newPos.y) {
			creature->setDirection(DIRECTION_NORTH);
		} else if (oldPos.y < newPos.y) {
			creature->setDirection(DIRECTION_SOUTH);
		}

		if (oldPos.x < newPos.x) {
			creature->setDirection(DIRECTION_EAST);
		} else if (oldPos.x > newPos.x) {
			creature->setDirection(DIRECTION_WEST);
		}
	}

	// send to client
	size_t i = 0;
	for (const auto& spectator : spectators) {
		if (const auto& tmpPlayer = spectator->getPlayer()) {
			// Use the correct stackpos
			int32_t stackpos = oldStackPosVector[i++];
			if (stackpos != -1) {
				tmpPlayer->sendCreatureMove(creature, newPos, newTile->getClientIndexOfCreature(tmpPlayer, creature),
				                            oldPos, stackpos, teleport);
			}
		}
	}

	// event method
	for (const auto& spectator : spectators) {
		spectator->onCreatureMove(creature, newTile, newPos, oldTile, oldPos, teleport);
	}

	oldTile->postRemoveNotification(creature, newTile, 0);
	newTile->postAddNotification(creature, oldTile, 0);
}

void Map::getSpectatorsInternal(SpectatorVec& spectators, const Position& centerPos, int32_t minRangeX,
                                int32_t maxRangeX, int32_t minRangeY, int32_t maxRangeY, int32_t minRangeZ,
                                int32_t maxRangeZ, bool onlyPlayers) const
{
	auto min_y = centerPos.y + minRangeY;
	auto min_x = centerPos.x + minRangeX;
	auto max_y = centerPos.y + maxRangeY;
	auto max_x = centerPos.x + maxRangeX;

	int32_t minoffset = centerPos.getZ() - maxRangeZ;
	uint16_t x1 = std::min<uint32_t>(0xFFFF, std::max<int32_t>(0, (min_x + minoffset)));
	uint16_t y1 = std::min<uint32_t>(0xFFFF, std::max<int32_t>(0, (min_y + minoffset)));

	int32_t maxoffset = centerPos.getZ() - minRangeZ;
	uint16_t x2 = std::min<uint32_t>(0xFFFF, std::max<int32_t>(0, (max_x + maxoffset)));
	uint16_t y2 = std::min<uint32_t>(0xFFFF, std::max<int32_t>(0, (max_y + maxoffset)));

	int32_t startx1 = x1 - (x1 % FLOOR_SIZE);
	int32_t starty1 = y1 - (y1 % FLOOR_SIZE);
	int32_t endx2 = x2 - (x2 % FLOOR_SIZE);
	int32_t endy2 = y2 - (y2 % FLOOR_SIZE);

	const QTreeLeafNode* startLeaf =
	    QTreeNode::getLeafStatic<const QTreeLeafNode*, const QTreeNode*>(&root, startx1, starty1);
	const QTreeLeafNode* leafS = startLeaf;
	const QTreeLeafNode* leafE;

	for (int_fast32_t ny = starty1; ny <= endy2; ny += FLOOR_SIZE) {
		leafE = leafS;
		for (int_fast32_t nx = startx1; nx <= endx2; nx += FLOOR_SIZE) {
			if (leafE) {
				const CreatureVector& node_list = (onlyPlayers ? leafE->player_list : leafE->creature_list);
				for (const auto& creature : node_list) {
					const Position& cpos = creature->getPosition();
					if (minRangeZ > cpos.z || maxRangeZ < cpos.z) {
						continue;
					}

					int16_t offsetZ = centerPos.getOffsetZ(cpos);
					if ((min_y + offsetZ) > cpos.y || (max_y + offsetZ) < cpos.y || (min_x + offsetZ) > cpos.x ||
					    (max_x + offsetZ) < cpos.x) {
						continue;
					}

					spectators.emplace_back(creature);
				}
				leafE = leafE->leafE;
			} else {
				leafE = QTreeNode::getLeafStatic<const QTreeLeafNode*, const QTreeNode*>(&root, nx + FLOOR_SIZE, ny);
			}
		}

		if (leafS) {
			leafS = leafS->leafS;
		} else {
			leafS = QTreeNode::getLeafStatic<const QTreeLeafNode*, const QTreeNode*>(&root, startx1, ny + FLOOR_SIZE);
		}
	}
}

void Map::getSpectators(SpectatorVec& spectators, const Position& centerPos, bool multifloor /*= false*/,
                        bool onlyPlayers /*= false*/, int32_t minRangeX /*= 0*/, int32_t maxRangeX /*= 0*/,
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

	if (minRangeX == -maxViewportX && maxRangeX == maxViewportX && minRangeY == -maxViewportY &&
	    maxRangeY == maxViewportY && multifloor) {
		if (onlyPlayers) {
			auto it = playersSpectatorCache.find(centerPos);
			if (it != playersSpectatorCache.end()) {
				if (!spectators.empty()) {
					spectators.addSpectators(it->second);
				} else {
					spectators = it->second;
				}

				foundCache = true;
			}
		}

		if (!foundCache) {
			auto it = spectatorCache.find(centerPos);
			if (it != spectatorCache.end()) {
				if (!onlyPlayers) {
					if (!spectators.empty()) {
						const SpectatorVec& cachedSpectators = it->second;
						spectators.addSpectators(cachedSpectators);
					} else {
						spectators = it->second;
					}
				} else {
					const SpectatorVec& cachedSpectators = it->second;
					for (const auto& spectator : cachedSpectators) {
						if (spectator->getPlayer()) {
							spectators.emplace_back(spectator);
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
				// underground (8->15)
				minRangeZ = std::max(centerPos.getZ() - 2, 0);
				maxRangeZ = std::min(centerPos.getZ() + 2, MAP_MAX_LAYERS - 1);
			} else if (centerPos.z == 6) {
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

		getSpectatorsInternal(spectators, centerPos, minRangeX, maxRangeX, minRangeY, maxRangeY, minRangeZ, maxRangeZ,
		                      onlyPlayers);

		if (cacheResult) {
			if (onlyPlayers) {
				playersSpectatorCache[centerPos] = spectators;
			} else {
				spectatorCache[centerPos] = spectators;
			}
		}
	}
}

void Map::clearSpectatorCache() { spectatorCache.clear(); }

void Map::clearPlayersSpectatorCache() { playersSpectatorCache.clear(); }

bool Map::canThrowObjectTo(const Position& fromPos, const Position& toPos, bool checkLineOfSight /*= true*/,
                           bool sameFloor /*= false*/, int32_t rangex /*= Map::maxClientViewportX*/,
                           int32_t rangey /*= Map::maxClientViewportY*/) const
{
	if (fromPos.getDistanceX(toPos) > rangex || fromPos.getDistanceY(toPos) > rangey) {
		return false;
	}

	return !checkLineOfSight || isSightClear(fromPos, toPos, sameFloor);
}

bool Map::isTileClear(uint16_t x, uint16_t y, uint8_t z, bool blockFloor /*= false*/,
                      bool pathfinding /*= false*/) const
{
	const auto& tile = getTile(x, y, z);
	if (!tile) {
		return true;
	}

	if (blockFloor && tile->getGround()) {
		return false;
	}

	if (pathfinding) {
		return !tile->hasProperty(CONST_PROP_BLOCKPROJECTILE) && !tile->hasProperty(CONST_PROP_BLOCKPATH) &&
		       !tile->hasProperty(CONST_PROP_BLOCKSOLID) && !tile->hasProperty(CONST_PROP_IMMOVABLEBLOCKPATH) &&
		       !tile->hasProperty(CONST_PROP_IMMOVABLEBLOCKSOLID) && !tile->getTopCreature();
	}

	return !tile->hasProperty(CONST_PROP_BLOCKPROJECTILE);
}

namespace {

bool checkSteepLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t z, bool pathfinding /*= false*/)
{
	float dx = x1 - x0;
	float slope = (dx == 0) ? 1 : (y1 - y0) / dx;
	float yi = y0 + slope;

	for (uint16_t x = x0 + 1; x < x1; ++x) {
		// 0.1 is necessary to avoid loss of precision during calculation
		if (!g_game.map.isTileClear(std::floor(yi + 0.1), x, z, false, pathfinding)) {
			return false;
		}
		yi += slope;
	}

	return true;
}

bool checkSlightLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t z, bool pathfinding /*= false*/)
{
	float dx = x1 - x0;
	float slope = (dx == 0) ? 1 : (y1 - y0) / dx;
	float yi = y0 + slope;

	for (uint16_t x = x0 + 1; x < x1; ++x) {
		// 0.1 is necessary to avoid loss of precision during calculation
		if (!g_game.map.isTileClear(x, std::floor(yi + 0.1), z, false, pathfinding)) {
			return false;
		}
		yi += slope;
	}

	return true;
}

} // namespace

bool Map::checkSightLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t z,
                         bool pathfinding /*= false*/) const
{
	if (x0 == x1 && y0 == y1) {
		return true;
	}

	if (std::abs(y1 - y0) > std::abs(x1 - x0)) {
		if (y1 > y0) {
			return checkSteepLine(y0, x0, y1, x1, z, pathfinding);
		}
		return checkSteepLine(y1, x1, y0, x0, z, pathfinding);
	}

	if (x0 > x1) {
		return checkSlightLine(x1, y1, x0, y0, z, pathfinding);
	}

	return checkSlightLine(x0, y0, x1, y1, z, pathfinding);
}

bool Map::isSightClear(const Position& fromPos, const Position& toPos, bool sameFloor /*= false*/,
                       bool pathfinding /*= false*/) const
{
	// target is on the same floor
	if (fromPos.z == toPos.z) {
		// skip checks if toPos is next to us
		if (fromPos.getDistanceX(toPos) < 2 && fromPos.getDistanceY(toPos) < 2) {
			return true;
		}

		// Check for additional tile properties when pathfinding
		if (pathfinding) {
			return checkSightLine(fromPos.x, fromPos.y, toPos.x, toPos.y, fromPos.z, true);
		}

		// sight is clear or sameFloor is enabled
		bool sightClear = checkSightLine(fromPos.x, fromPos.y, toPos.x, toPos.y, fromPos.z);
		if (sightClear || sameFloor) {
			return sightClear;
		}

		// no obstacles above floor 0 so we can throw above the obstacle
		if (fromPos.z == 0) {
			return true;
		}

		// check if tiles above us and the target are clear and check for a clear sight between them
		uint8_t newZ = fromPos.z - 1;
		return isTileClear(fromPos.x, fromPos.y, newZ, true) && isTileClear(toPos.x, toPos.y, newZ, true) &&
		       checkSightLine(fromPos.x, fromPos.y, toPos.x, toPos.y, newZ);
	}

	// target is on a different floor
	if (sameFloor) {
		return false;
	}

	// skip checks for sight line in case fromPos and toPos cross the ground floor
	if ((fromPos.z < 8 && toPos.z > 7) || (fromPos.z > 7 && toPos.z < 8)) {
		return false;
	}

	// target is above us
	if (fromPos.z > toPos.z) {
		if (fromPos.getDistanceZ(toPos) > 1) {
			return false;
		}

		// check a tile above us and the path to the target
		uint8_t newZ = fromPos.z - 1;
		return isTileClear(fromPos.x, fromPos.y, newZ, true) &&
		       checkSightLine(fromPos.x, fromPos.y, toPos.x, toPos.y, newZ);
	}

	// target is below us check if tiles above the target are clear
	for (uint8_t z = fromPos.z; z < toPos.z; ++z) {
		if (!isTileClear(toPos.x, toPos.y, z, true)) {
			return false;
		}
	}

	// check if we can throw to the tile above the target
	return checkSightLine(fromPos.x, fromPos.y, toPos.x, toPos.y, fromPos.z);
}

const std::shared_ptr<Tile> Map::canWalkTo(const std::shared_ptr<const Creature>& creature, const Position& pos) const
{
	const auto& tile = getTile(pos.x, pos.y, pos.z);
	if (creature->getTile() != tile) {
		if (!tile) {
			return nullptr;
		}

		uint32_t flags = FLAG_PATHFINDING;
		if (!creature->getPlayer()) {
			flags |= FLAG_IGNOREFIELDDAMAGE;
		}

		if (tile->queryAdd(0, creature, 1, flags) != RETURNVALUE_NOERROR) {
			return nullptr;
		}
	}
	return tile;
}

static uint16_t calculateHeuristic(const Position& p1, const Position& p2)
{
	uint16_t dx = std::abs(p1.getX() - p2.getX());
	uint16_t dy = std::abs(p1.getY() - p2.getY());
	return dx * dx + dy * dy;
}

bool Map::getPathMatching(const std::shared_ptr<const Creature>& creature, const Position& targetPos,
                          std::vector<Direction>& dirList, const FrozenPathingConditionCall& pathCondition,
                          const FindPathParams& fpp) const
{
	Position pos = creature->getPosition();
	const Position startPos = pos;

	// We can't walk, no need to create path.
	if (creature->getSpeed() <= 0) {
		return false;
	}

	// We can't get paths up or down floors.
	if (startPos.getZ() != targetPos.getZ()) {
		return false;
	}

	int32_t distanceX = startPos.getDistanceX(targetPos);
	int32_t distanceY = startPos.getDistanceY(targetPos);
	// We are next to our target. Let dance step decide.
	if (fpp.maxTargetDist <= 1 && distanceX <= 1 && distanceY <= 1) {
		return true;
	}

	// Don't update path. The target is too far away.
	int32_t maxDistanceX = fpp.maxSearchDist ? fpp.maxSearchDist : Map::maxViewportX + 1;
	int32_t maxDistanceY = fpp.maxSearchDist ? fpp.maxSearchDist : Map::maxViewportY + 1;
	if (distanceX > maxDistanceX || distanceY > maxDistanceY) {
		return false;
	}

	static constexpr std::array<std::pair<int, int>, 8> allNeighbors = {
	    {{-1, 0}, {0, 1}, {1, 0}, {0, -1}, {-1, -1}, {1, -1}, {1, 1}, {-1, 1}}};

	bool sightClear = isSightClear(startPos, targetPos, true, true);

	Position endPos;
	AStarNodes nodes(pos.x, pos.y);

	AStarNode* found = nullptr;
	int32_t bestMatch = 0;
	uint16_t iterations = 0;
	AStarNode* n = nodes.getBestNode();
	while (n) {
		iterations++;

		if (iterations >= Map::maxViewportX * Map::maxViewportY) {
			return false;
		}

		const int32_t x = n->x;
		const int32_t y = n->y;
		pos.x = x;
		pos.y = y;
		if (pathCondition(startPos, pos, fpp, bestMatch)) {
			found = n;
			endPos = pos;
			if (bestMatch == 0) {
				break;
			}
		}

		for (uint8_t i = 0; i < 8; ++i) {
			pos.x = x + allNeighbors[i].first;
			pos.y = y + allNeighbors[i].second;

			int32_t startDistanceToNode = startPos.getDistanceX(pos) + startPos.getDistanceY(pos);
			if (fpp.maxSearchDist != 0 && startDistanceToNode > fpp.maxSearchDist) {
				continue;
			} else if (fpp.maxSearchDist == 0 && (startDistanceToNode > (Map::maxViewportX + Map::maxViewportY))) {
				continue;
			}

			if (fpp.keepDistance && !pathCondition.isInRange(startPos, pos, fpp)) {
				continue;
			}

			// If sight is clear we can ignore a lot of nodes.
			if (sightClear && !fpp.keepDistance && !fpp.summonTargetMaster && fpp.minTargetDist <= 1) {
				int32_t startX = startPos.getX();
				int32_t startY = startPos.getY();
				int32_t targetX = targetPos.getX();
				int32_t targetY = targetPos.getY();

				// We don't need nodes behind us.
				// We also do not need nodes on a different x or y if target and start is same x/y.
				if (startX > targetX && pos.x > startX) {
					continue;
				} else if (startX == targetX && pos.x != startX) {
					continue;
				} else if (startX < targetX && pos.x < startX) {
					continue;
				}
				if (startY > targetY && pos.y > startY) {
					continue;
				} else if (startY == targetY && pos.y != startY) {
					continue;
				} else if (startY < targetY && pos.y < startY) {
					continue;
				}

				// We don't need nodes past the targetPos
				if (startX > targetX && pos.x < targetX) {
					continue;
				} else if (startX < targetX && pos.x > targetX) {
					continue;
				}
				if (startY > targetY && pos.y < targetY) {
					continue;
				} else if (startY < targetY && pos.y > targetY) {
					continue;
				}
			}

			std::shared_ptr<const Tile> tile;
			AStarNode* neighborNode = nodes.getNodeByPosition(pos.x, pos.y);
			if (neighborNode) {
				tile = getTile(pos.x, pos.y, pos.z);
			} else {
				tile = canWalkTo(creature, pos);
				if (!tile) {
					continue;
				}
			}

			// The cost to walk to this neighbor
			const uint16_t g = n->g + AStarNodes::getMapWalkCost(n, pos) + AStarNodes::getTileWalkCost(creature, tile);
			const uint16_t h = calculateHeuristic(pos, targetPos);
			const uint16_t newf = h + g;

			if (neighborNode) {
				if (neighborNode->f <= newf) {
					// The node on the closed/open list is cheaper than this one
					continue;
				}

				neighborNode->g = g;
				neighborNode->f = newf;
				neighborNode->parent = n;
			} else {
				// Does not exist in the open/closed list, create a new node
				if (!nodes.createNode(n, pos.x, pos.y, g, newf)) {
					// Limit of nodes reached
					return false;
				}
			}
		}

		n = nodes.getBestNode();
	}

	if (!found) {
		return false;
	}

	int32_t prevx = endPos.getX();
	int32_t prevy = endPos.getY();

	found = found->parent;
	while (found) {
		pos.x = found->x;
		pos.y = found->y;

		int32_t dx = pos.getX() - prevx;
		int32_t dy = pos.getY() - prevy;

		prevx = pos.x;
		prevy = pos.y;

		if (dx == 1 && dy == 1) {
			dirList.push_back(DIRECTION_NORTHWEST);
		} else if (dx == -1 && dy == 1) {
			dirList.push_back(DIRECTION_NORTHEAST);
		} else if (dx == 1 && dy == -1) {
			dirList.push_back(DIRECTION_SOUTHWEST);
		} else if (dx == -1 && dy == -1) {
			dirList.push_back(DIRECTION_SOUTHEAST);
		} else if (dx == 1) {
			dirList.push_back(DIRECTION_WEST);
		} else if (dx == -1) {
			dirList.push_back(DIRECTION_EAST);
		} else if (dy == 1) {
			dirList.push_back(DIRECTION_NORTH);
		} else if (dy == -1) {
			dirList.push_back(DIRECTION_SOUTH);
		}

		found = found->parent;
	}

	return true;
}

// AStarNodes
AStarNodes::AStarNodes(uint16_t x, uint16_t y)
{
	nodes.reserve(Map::nodeReserveSize);
	nodeMap.reserve(Map::nodeReserveSize);
	visited.reserve(Map::nodeReserveSize);
	createNode(nullptr, x, y, 0, 0);
}

AStarNode* AStarNodes::createNode(AStarNode* parent, uint16_t x, uint16_t y, uint16_t g, uint16_t f)
{
	if (nodes.size() == Map::nodeReserveSize) {
		return nullptr;
	}

	uint32_t key = hashCoord(x, y);
	nodes.emplace_back(AStarNode{parent, x, y, g, f});
	AStarNode* node = &nodes.back();
	nodeMap[key] = node;
	openSet.push(node);
	return node;
}

AStarNode* AStarNodes::getBestNode()
{
	while (!openSet.empty()) {
		AStarNode* node = openSet.top();
		openSet.pop();
		uint32_t key = hashCoord(node->x, node->y);
		if (visited.find(key) == visited.end()) {
			visited.insert(key);
			return node;
		}
	}
	return nullptr;
}

AStarNode* AStarNodes::getNodeByPosition(uint16_t x, uint16_t y)
{
	auto it = nodeMap.find(hashCoord(x, y));
	return (it != nodeMap.end()) ? it->second : nullptr;
}

uint16_t AStarNodes::getMapWalkCost(AStarNode* node, const Position& neighborPos)
{
	if (std::abs(node->x - neighborPos.x) == std::abs(node->y - neighborPos.y)) {
		// diagonal movement extra cost
		return MAP_DIAGONALWALKCOST;
	}
	return MAP_NORMALWALKCOST;
}

uint16_t AStarNodes::getTileWalkCost(const std::shared_ptr<const Creature>& creature,
                                     const std::shared_ptr<const Tile>& tile)
{
	uint16_t cost = 0;
	if (tile->getTopVisibleCreature(creature)) {
		cost += MAP_NORMALWALKCOST * 3;
	}

	if (const auto& field = tile->getFieldItem()) {
		CombatType_t combatType = field->getCombatType();
		const auto& monster = creature->getMonster();
		if (!creature->isImmune(combatType) && !creature->hasCondition(Combat::DamageToConditionType(combatType)) &&
		    (monster && !monster->canWalkOnFieldType(combatType))) {
			cost += MAP_NORMALWALKCOST * 18;
		}
	}
	return cost;
}

// QTreeNode
QTreeNode::~QTreeNode()
{
	for (auto* ptr : child) {
		delete ptr;
	}
}

QTreeLeafNode* QTreeNode::getLeaf(uint32_t x, uint32_t y)
{
	if (leaf) {
		return static_cast<QTreeLeafNode*>(this);
	}

	auto node = child[((x & 0x8000) >> 15) | ((y & 0x8000) >> 14)];
	if (!node) {
		return nullptr;
	}
	return node->getLeaf(x << 1, y << 1);
}

QTreeLeafNode* QTreeNode::createLeaf(uint32_t x, uint32_t y, uint32_t level)
{
	if (!isLeaf()) {
		uint32_t index = ((x & 0x8000) >> 15) | ((y & 0x8000) >> 14);
		if (!child[index]) {
			if (level != FLOOR_BITS) {
				child[index] = new QTreeNode();
			} else {
				child[index] = new QTreeLeafNode();
				QTreeLeafNode::newLeaf = true;
			}
		}
		return child[index]->createLeaf(x * 2, y * 2, level - 1);
	}
	return static_cast<QTreeLeafNode*>(this);
}

// QTreeLeafNode
bool QTreeLeafNode::newLeaf = false;

QTreeLeafNode::~QTreeLeafNode()
{
	for (auto* ptr : array) {
		delete ptr;
	}
}

Floor* QTreeLeafNode::createFloor(uint32_t z)
{
	if (!array[z]) {
		array[z] = new Floor();
	}
	return array[z];
}

void QTreeLeafNode::addCreature(const std::shared_ptr<Creature>& c)
{
	creature_list.push_back(c);

	if (c->getPlayer()) {
		player_list.push_back(c);
	}
}

void QTreeLeafNode::removeCreature(const std::shared_ptr<Creature>& c)
{
	auto iter = std::find(creature_list.begin(), creature_list.end(), c);
	assert(iter != creature_list.end());
	std::iter_swap(iter, creature_list.end() - 1);
	creature_list.pop_back();

	if (c->getPlayer()) {
		iter = std::find(player_list.begin(), player_list.end(), c);
		assert(iter != player_list.end());
		std::iter_swap(iter, player_list.end() - 1);
		player_list.pop_back();
	}
}

uint32_t Map::clean() const
{
	uint64_t start = OTSYS_TIME();
	size_t tiles = 0;

	if (g_game.getGameState() == GAME_STATE_NORMAL) {
		g_game.setGameState(GAME_STATE_MAINTAIN);
	}

	std::vector<std::shared_ptr<Item>> toRemove;

	for (const auto& tile : g_game.getTilesToClean()) {
		if (!tile) {
			continue;
		}

		if (const auto& items = tile->getItemList()) {
			++tiles;
			for (const auto& item : *items) {
				if (item && item->isCleanable()) {
					toRemove.emplace_back(item);
				}
			}
		}
	}

	for (const auto& item : toRemove) {
		g_game.internalRemoveItem(item, -1);
	}

	size_t count = toRemove.size();
	g_game.clearTilesToClean();

	if (g_game.getGameState() == GAME_STATE_MAINTAIN) {
		g_game.setGameState(GAME_STATE_NORMAL);
	}

	std::cout << "> CLEAN: Removed " << count << " item" << (count != 1 ? "s" : "") << " from " << tiles << " tile"
	          << (tiles != 1 ? "s" : "") << " in " << (OTSYS_TIME() - start) / (1000.) << " seconds." << std::endl;
	return count;
}
