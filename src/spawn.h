//////////////////////////////////////////////////////////////////////
// The Forgotten Server - a server application for the MMORPG Tibia
//////////////////////////////////////////////////////////////////////
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//////////////////////////////////////////////////////////////////////

#ifndef __OTSERV_SPAWN_H__
#define __OTSERV_SPAWN_H__

#include "tile.h"
#include "position.h"
#include "monster.h"

#include <vector>
#include <map>

class Spawn;
typedef std::list<Spawn*> SpawnList;

class Spawns
{
	private:
		Spawns();

	public:
		static Spawns* getInstance() {
			static Spawns instance;
			return &instance;
		}

		bool isInZone(const Position& centerPos, int32_t radius, const Position& pos);

		~Spawns();

		bool loadFromXml(const std::string& _filename);
		void startup();
		void clear();

		bool isLoaded() const {
			return loaded;
		}
		bool isStarted() const {
			return started;
		}

	private:
		typedef std::list<Npc*> NpcList;
		NpcList npcList;
		SpawnList spawnList;
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
		Spawn(const Position& _pos, int32_t _radius);
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
		Position centerPos;
		int32_t radius;
		int32_t despawnRange;
		int32_t despawnRadius;

		//map of creatures in the spawn
		typedef std::map<uint32_t, spawnBlock_t> SpawnMap;
		SpawnMap spawnMap;

		//map of the spawned creatures
		typedef std::multimap<uint32_t, Monster*, std::less<uint32_t> > SpawnedMap;
		typedef SpawnedMap::value_type spawned_pair;
		SpawnedMap spawnedMap;

		uint32_t interval;
		uint32_t checkSpawnEvent;

		bool findPlayer(const Position& pos);
		bool spawnMonster(uint32_t spawnId, MonsterType* mType, const Position& pos, Direction dir, bool startup = false);
		void checkSpawn();
};

#endif
