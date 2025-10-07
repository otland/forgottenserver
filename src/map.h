// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_MAP_H
#define FS_MAP_H

#include "house.h"
#include "position.h"
#include "spawn.h"
#include "spectators.h"
#include "town.h"

class Creature;

inline constexpr int32_t MAP_MAX_LAYERS = 16;

static constexpr uint16_t MAP_NORMALWALKCOST = 10;
static constexpr uint16_t MAP_DIAGONALWALKCOST = 25;

struct FindPathParams;
struct AStarNode
{
	AStarNode* parent;
	uint16_t x, y;
	uint16_t g, f;
};

inline uint32_t hashCoord(uint16_t x, uint16_t y) { return (static_cast<uint32_t>(x) << 16) | y; }

class AStarNodes
{
public:
	AStarNodes(uint16_t x, uint16_t y);

	AStarNode* createNode(AStarNode* parent, uint16_t x, uint16_t y, uint16_t g, uint16_t f);
	AStarNode* getBestNode();
	AStarNode* getNodeByPosition(uint16_t x, uint16_t y);

	static uint16_t getMapWalkCost(AStarNode* node, const Position& neighborPos);
	static uint16_t getTileWalkCost(const Creature& creature, const Tile* tile);

private:
	std::vector<AStarNode> nodes;
	std::unordered_map<uint32_t, AStarNode*> nodeMap;
	std::unordered_set<uint32_t> visited;

	struct NodeCompare
	{
		bool operator()(AStarNode* a, AStarNode* b) const
		{
			return a->f > b->f; // Min-heap based on f score
		}
	};

	std::priority_queue<AStarNode*, std::vector<AStarNode*>, NodeCompare> openSet;
};

using SpectatorCache = std::map<Position, SpectatorVec>;

class FrozenPathingConditionCall;

/**
 * Map class.
 * Holds all the actual map-data
 */

class Map
{
public:
	static constexpr int32_t maxViewportX = 11; // min value: maxClientViewportX + 1
	static constexpr int32_t maxViewportY = 11; // min value: maxClientViewportY + 1
	static constexpr int32_t maxClientViewportX = 8;
	static constexpr int32_t maxClientViewportY = 6;
	static constexpr int16_t nodeReserveSize = static_cast<int16_t>((maxViewportX * maxViewportY * 3) / 2);

	uint32_t clean() const;

	/**
	 * Load a map.
	 * \returns true if the map was loaded successfully
	 */
	bool loadMap(const std::string& identifier, bool loadHouses, bool isCalledByLua = true);

	/**
	 * Save a map.
	 * \returns true if the map was saved successfully
	 */
	static bool save();

	/**
	 * Get a single tile.
	 * \returns A pointer to that tile.
	 */
	Tile* getTile(uint16_t x, uint16_t y, uint8_t z) const;
	Tile* getTile(const Position& pos) const { return getTile(pos.x, pos.y, pos.z); }

	/**
	 * Set a single tile.
	 */
	void setTile(uint16_t x, uint16_t y, uint8_t z, Tile* newTile);
	void setTile(const Position& pos, Tile* newTile) { setTile(pos.x, pos.y, pos.z, newTile); }

	/**
	 * Removes a single tile.
	 */
	void removeTile(uint16_t x, uint16_t y, uint8_t z);
	void removeTile(const Position& pos) { removeTile(pos.x, pos.y, pos.z); }

	/**
	 * Place a creature on the map
	 * \param centerPos The position to place the creature
	 * \param creature Creature to place on the map
	 * \param extendedPos If true, the creature will in first-hand be placed 2
	 * tiles away \param forceLogin If true, placing the creature will not fail
	 * because of obstacles (creatures/chests)
	 */
	bool placeCreature(const Position& centerPos, Creature* creature, bool extendedPos = false,
	                   bool forceLogin = false);

	void moveCreature(Creature& creature, Tile& newTile, bool forceTeleport = false);

	void getSpectators(SpectatorVec& spectators, const Position& centerPos, bool multifloor = false,
	                   bool onlyPlayers = false, int32_t minRangeX = 0, int32_t maxRangeX = 0, int32_t minRangeY = 0,
	                   int32_t maxRangeY = 0);

	void clearSpectatorCache();
	void clearPlayersSpectatorCache();

	/**
	 * Checks if you can throw an object to that position
	 *	\param fromPos from Source point
	 *	\param toPos Destination point
	 *	\param rangex maximum allowed range horizontally
	 *	\param rangey maximum allowed range vertically
	 *	\param checkLineOfSight checks if there is any blocking objects in the
	 *way \param sameFloor checks if the destination is on same floor \returns
	 *The result if you can throw there or not
	 */
	bool canThrowObjectTo(const Position& fromPos, const Position& toPos, bool checkLineOfSight = true,
	                      bool sameFloor = false, int32_t rangex = Map::maxClientViewportX,
	                      int32_t rangey = Map::maxClientViewportY) const;

	/**
	 * Checks if there are no obstacles on that position
	 *	\param blockFloor counts the ground tile as an obstacle
	 *	\returns The result if there is an obstacle or not
	 */
	bool isTileClear(uint16_t x, uint16_t y, uint8_t z, bool blockFloor = false, bool pathfinding = false) const;

	/**
	 * Checks if path is clear from fromPos to toPos
	 * Notice: This only checks a straight line if the path is clear, for path
	 *finding use getPathTo. \param fromPos from Source point \param toPos
	 *Destination point \param sameFloor checks if the destination is on same
	 *floor \returns The result if there is no obstacles
	 */
	bool isSightClear(const Position& fromPos, const Position& toPos, bool sameFloor = false,
	                  bool pathfinding = false) const;
	bool checkSightLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t z, bool pathfinding = false) const;

	const Tile* canWalkTo(const Creature& creature, const Position& pos) const;

	bool getPathMatching(const Creature& creature, const Position& targetPos, std::vector<Direction>& dirList,
	                     const FrozenPathingConditionCall& pathCondition, const FindPathParams& fpp) const;

	std::map<std::string, Position> waypoints;

	Spawns spawns;
	Towns towns;
	Houses houses;

private:
	SpectatorCache spectatorCache;
	SpectatorCache playersSpectatorCache;

	std::filesystem::path spawnfile;
	std::filesystem::path housefile;

	uint32_t width = 0;
	uint32_t height = 0;

	friend class Game;
	friend class IOMap;
};

#endif // FS_MAP_H
