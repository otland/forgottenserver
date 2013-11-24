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

#ifndef __OTSERV_SPAWN_H__
#define __OTSERV_SPAWN_H__

#include "tile.h"
#include "position.h"
#include "monster.h"

class Spawn;

class Spawns
{
	private:
		Spawns();

	public:
		static Spawns* getInstance() {
			static Spawns instance;
			return &instance;
		}

		static bool isInZone(const Position& centerPos, int32_t radius, const Position& pos);

		~Spawns();

		bool loadFromXml(const std::string& _filename);
		void startup();
		void clear();

		bool isStarted() const {
			return started;
		}

	private:
		std::list<Npc*> npcList;
		std::list<Spawn*> spawnList;
		bool loaded, started;
		std::string filename;
};

struct spawnBlock_t {
	MonsterType* mType;
	Direction direction;
	Position pos;
	uint32_t interval;
	int64_t lastSpawn;
};

class Spawn
{
	public:
		Spawn(const Position& pos, int32_t radius) : centerPos(pos), radius(radius), interval(60000), checkSpawnEvent() {}
		~Spawn();

		bool addMonster(const std::string& _name, const Position& _pos, Direction _dir, uint32_t _interval);
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
		typedef std::multimap<uint32_t, Monster*, std::less<uint32_t>> SpawnedMap;
		typedef SpawnedMap::value_type spawned_pair;
		SpawnedMap spawnedMap;

		//map of creatures in the spawn
		std::map<uint32_t, spawnBlock_t> spawnMap;

		Position centerPos;
		int32_t radius;

		uint32_t interval;
		uint32_t checkSpawnEvent;

		static bool findPlayer(const Position& pos);
		bool spawnMonster(uint32_t spawnId, MonsterType* mType, const Position& pos, Direction dir, bool startup = false);
		void checkSpawn();
};

#endif
