// Copyright 2022 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_SPAWN_H_1A86089E080846A9AE53ED12E7AE863B
#define FS_SPAWN_H_1A86089E080846A9AE53ED12E7AE863B

#include "tile.h"
#include "position.h"

#include <utility>
#include <vector>

class Monster;
class MonsterType;
class Npc;

struct spawnBlock_t {
	Position pos;
	std::vector<std::pair<MonsterType*, uint16_t>> mTypes;
	int64_t lastSpawn;
	uint32_t interval;
	Direction direction;
};

class Spawn
{
	public:
		Spawn(Position pos, int32_t radius) : centerPos(std::move(pos)), radius(radius) {}
		~Spawn();

		// non-copyable
		Spawn(const Spawn&) = delete;
		Spawn& operator=(const Spawn&) = delete;

		bool addBlock(spawnBlock_t sb);
		bool addMonster(const std::string& name, const Position& pos, Direction dir, uint32_t interval);
		void removeMonster(Monster* monster);

		uint32_t getInterval() const {
			return interval;
		}
		void startup();

		void startSpawnCheck();
		void stopEvent();

		bool isInSpawnZone(const Position& pos);
		void cleanup();

	private:
		//map of the spawned creatures
		using SpawnedMap = std::multimap<uint32_t, Monster*>;
		SpawnedMap spawnedMap;

		//map of creatures in the spawn
		std::map<uint32_t, spawnBlock_t> spawnMap;

		Position centerPos;
		int32_t radius;

		uint32_t interval = 60000;
		uint32_t checkSpawnEvent = 0;

		static bool findPlayer(const Position& pos);
		bool spawnMonster(uint32_t spawnId, spawnBlock_t sb, bool startup = false);
		bool spawnMonster(uint32_t spawnId, MonsterType* mType, const Position& pos, Direction dir, bool startup = false);
		void checkSpawn();
};

class Spawns
{
	public:
		static bool isInZone(const Position& centerPos, int32_t radius, const Position& pos);

		bool loadFromXml(const std::string& filename);
		void startup();
		void clear();

		bool isStarted() const {
			return started;
		}

	private:
		std::forward_list<Npc*> npcList;
		std::forward_list<Spawn> spawnList;
		std::string filename;
		bool loaded = false;
		bool started = false;
};

#endif
