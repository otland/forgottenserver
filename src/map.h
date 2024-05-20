// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_MAP_H
#define FS_MAP_H

#include "house.h"
#include "position.h"
#include "spawn.h"
#include "town.h"

class Creature;

inline constexpr int32_t MAP_MAX_LAYERS = 16;

struct FindPathParams;

using SpectatorCache = std::map<Position, SpectatorVec>;

inline constexpr int32_t FLOOR_BITS = 3;
inline constexpr int32_t FLOOR_SIZE = (1 << FLOOR_BITS);
using Floor = std::array<std::array<std::unique_ptr<Tile>, FLOOR_SIZE>, FLOOR_SIZE>;

class FrozenPathingConditionCall;
class QTreeLeafNode;

class QTreeNode
{
public:
	constexpr QTreeNode() = default;
	virtual ~QTreeNode() = default;

	virtual constexpr bool isLeaf() const { return false; }

	QTreeLeafNode* getLeaf(uint32_t x, uint32_t y);
	QTreeLeafNode* createLeaf(uint32_t x, uint32_t y, uint32_t level);
	constexpr auto& operator[](uint32_t index) const { return child[index]; }

private:
	std::array<std::unique_ptr<QTreeNode>, 4> child = {};
};

class QTreeLeafNode final : public QTreeNode
{
public:
	QTreeLeafNode();

	constexpr bool isLeaf() const override { return true; }

	auto& createFloor(uint32_t z)
	{
		if (!array[z]) {
			array[z] = std::make_unique<Floor>();
		}
		return array[z];
	}

	const auto& getFloor(uint8_t z) const { return array[z]; }

	void addCreature(Creature* c);
	void removeCreature(Creature* c);

private:
	QTreeLeafNode* leafS = nullptr;
	QTreeLeafNode* leafE = nullptr;
	std::array<std::unique_ptr<Floor>, MAP_MAX_LAYERS> array = {};
	CreatureVector creature_list;
	CreatureVector player_list;

	friend class Map;
	friend class QTreeNode;
};

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

	static uint32_t clean();

	/**
	 * Load a map.
	 * \returns true if the map was loaded successfully
	 */
	bool loadMap(const std::string& identifier, bool loadHouses);

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
	bool isTileClear(uint16_t x, uint16_t y, uint8_t z, bool blockFloor = false) const;

	/**
	 * Checks if path is clear from fromPos to toPos
	 * Notice: This only checks a straight line if the path is clear, for path
	 *finding use getPathTo. \param fromPos from Source point \param toPos
	 *Destination point \param sameFloor checks if the destination is on same
	 *floor \returns The result if there is no obstacles
	 */
	bool isSightClear(const Position& fromPos, const Position& toPos, bool sameFloor = false) const;

	const Tile* canWalkTo(const Creature& creature, const Position& pos) const;

	bool getPathMatching(const Creature& creature, std::vector<Direction>& dirList,
	                     const FrozenPathingConditionCall& pathCondition, const FindPathParams& fpp) const;

	std::map<std::string, Position> waypoints;

	QTreeLeafNode* getQTNode(uint16_t x, uint16_t y);

	Spawns spawns;
	Towns towns;
	Houses houses;

private:
	SpectatorCache spectatorCache;
	SpectatorCache playersSpectatorCache;

	QTreeNode root;

	std::filesystem::path spawnfile;
	std::filesystem::path housefile;

	uint32_t width = 0;
	uint32_t height = 0;

	// Actually scans the map for spectators
	void getSpectatorsInternal(SpectatorVec& spectators, const Position& centerPos, int32_t minRangeX,
	                           int32_t maxRangeX, int32_t minRangeY, int32_t maxRangeY, int32_t minRangeZ,
	                           int32_t maxRangeZ, bool onlyPlayers) const;

	friend class Game;
	friend class IOMap;
};

#endif // FS_MAP_H
