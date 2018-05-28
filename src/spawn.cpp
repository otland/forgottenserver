/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2018  Mark Samman <mark.samman@gmail.com>
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

#include "spawn.h"
#include "game.h"
#include "monster.h"
#include "configmanager.h"
#include "scheduler.h"

#include "pugicast.h"

extern ConfigManager g_config;
extern Monsters g_monsters;
extern Game g_game;

static constexpr int32_t MINSPAWN_INTERVAL = 1000;

bool Spawns::loadFromXml(const std::string& filename)
{
	if (loaded) {
		return true;
	}

	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(filename.c_str());
	if (!result) {
		printXMLError("Error - Spawns::loadFromXml", filename, result);
		return false;
	}

	this->filename = filename;
	loaded = true;

	for (auto spawnNode : doc.child("spawns").children()) {
		Position centerPos(
			pugi::cast<uint16_t>(spawnNode.attribute("centerx").value()),
			pugi::cast<uint16_t>(spawnNode.attribute("centery").value()),
			pugi::cast<uint16_t>(spawnNode.attribute("centerz").value())
		);

		int32_t radius;
		pugi::xml_attribute radiusAttribute = spawnNode.attribute("radius");
		if (radiusAttribute) {
			radius = pugi::cast<int32_t>(radiusAttribute.value());
		} else {
			radius = -1;
		}

		spawnList.emplace_front(centerPos, radius);
		Spawn& spawn = spawnList.front();

		for (auto childNode : spawnNode.children()) {
			if (strcasecmp(childNode.name(), "monster") == 0) {
				pugi::xml_attribute nameAttribute = childNode.attribute("name");
				if (!nameAttribute) {
					continue;
				}

				Direction dir;

				pugi::xml_attribute directionAttribute = childNode.attribute("direction");
				if (directionAttribute) {
					dir = static_cast<Direction>(pugi::cast<uint16_t>(directionAttribute.value()));
				} else {
					dir = DIRECTION_NORTH;
				}

				Position pos(
					centerPos.x + pugi::cast<uint16_t>(childNode.attribute("x").value()),
					centerPos.y + pugi::cast<uint16_t>(childNode.attribute("y").value()),
					centerPos.z
				);
				uint32_t interval = pugi::cast<uint32_t>(childNode.attribute("spawntime").value()) * 1000;
				if (interval > MINSPAWN_INTERVAL) {
					spawn.addMonster(nameAttribute.as_string(), pos, dir, interval);
				} else {
					std::cout << "[Warning - Spawns::loadFromXml] " << nameAttribute.as_string() << ' ' << pos << " spawntime can not be less than " << MINSPAWN_INTERVAL / 1000 << " seconds." << std::endl;
				}
			} else if (strcasecmp(childNode.name(), "npc") == 0) {
				pugi::xml_attribute nameAttribute = childNode.attribute("name");
				if (!nameAttribute) {
					continue;
				}

				Npc* npc = Npc::createNpc(nameAttribute.as_string());
				if (!npc) {
					continue;
				}

				pugi::xml_attribute directionAttribute = childNode.attribute("direction");
				if (directionAttribute) {
					npc->setDirection(static_cast<Direction>(pugi::cast<uint16_t>(directionAttribute.value())));
				}

				npc->setMasterPos(Position(
					centerPos.x + pugi::cast<uint16_t>(childNode.attribute("x").value()),
					centerPos.y + pugi::cast<uint16_t>(childNode.attribute("y").value()),
					centerPos.z
				), radius);
				npcList.push_front(npc);
			}
		}
	}
	return true;
}

void Spawns::startup()
{
	if (!loaded || isStarted()) {
		return;
	}

	for (Npc* npc : npcList) {
		g_game.placeCreature(npc, npc->getMasterPos(), false, true);
	}
	npcList.clear();

	for (Spawn& spawn : spawnList) {
		spawn.startup();
	}

	started = true;
}

void Spawns::clear()
{
	for (Spawn& spawn : spawnList) {
		spawn.stopEvent();
	}
	spawnList.clear();

	loaded = false;
	started = false;
	filename.clear();
}

bool Spawns::isInZone(const Position& centerPos, int32_t radius, const Position& pos)
{
	if (radius == -1) {
		return true;
	}

	return ((pos.getX() >= centerPos.getX() - radius) && (pos.getX() <= centerPos.getX() + radius) &&
	        (pos.getY() >= centerPos.getY() - radius) && (pos.getY() <= centerPos.getY() + radius));
}

void Spawn::startSpawnCheck()
{
	if (checkSpawnEvent == 0) {
		checkSpawnEvent = g_scheduler.addEvent(createSchedulerTask(getInterval(), std::bind(&Spawn::checkSpawn, this)));
	}
}

Spawn::~Spawn()
{
	for (const auto& it : spawnedMap) {
		Monster* monster = it.second;
		monster->setSpawn(nullptr);
		monster->decrementReferenceCounter();
	}
}

bool Spawn::findPlayer(const Position& pos)
{
	SpectatorHashSet spectators;
	g_game.map.getSpectators(spectators, pos, false, true);
	for (Creature* spectator : spectators) {
		if (!spectator->getPlayer()->hasFlag(PlayerFlag_IgnoredByMonsters)) {
			return true;
		}
	}
	return false;
}

bool Spawn::isInSpawnZone(const Position& pos)
{
	return Spawns::isInZone(centerPos, radius, pos);
}

bool Spawn::spawnMonster(uint32_t spawnId, MonsterType* mType, const Position& pos, Direction dir, bool startup /*= false*/)
{
	std::unique_ptr<Monster> monster_ptr(new Monster(mType));
	if (startup) {
		//No need to send out events to the surrounding since there is no one out there to listen!
		if (!g_game.internalPlaceCreature(monster_ptr.get(), pos, true)) {
			return false;
		}
	} else {
		if (!g_game.placeCreature(monster_ptr.get(), pos, false, true)) {
			return false;
		}
	}

	Monster* monster = monster_ptr.release();
	monster->setDirection(dir);
	monster->setSpawn(this);
	monster->setMasterPos(pos);
	monster->incrementReferenceCounter();

	spawnedMap.insert(spawned_pair(spawnId, monster));
	spawnMap[spawnId].lastSpawn = OTSYS_TIME();
	return true;
}

void Spawn::startup()
{
	for (const auto& it : spawnMap) {
		uint32_t spawnId = it.first;
		const spawnBlock_t& sb = it.second;
		spawnMonster(spawnId, sb.mType, sb.pos, sb.direction, true);
	}
}

void Spawn::checkSpawn()
{
	checkSpawnEvent = 0;

	cleanup();

	uint32_t spawnCount = 0;

	for (auto& it : spawnMap) {
		uint32_t spawnId = it.first;
		if (spawnedMap.find(spawnId) != spawnedMap.end()) {
			continue;
		}

		spawnBlock_t& sb = it.second;
		if (OTSYS_TIME() >= sb.lastSpawn + sb.interval) {
			if (findPlayer(sb.pos)) {
				sb.lastSpawn = OTSYS_TIME();
				continue;
			}

			spawnMonster(spawnId, sb.mType, sb.pos, sb.direction);
			if (++spawnCount >= static_cast<uint32_t>(g_config.getNumber(ConfigManager::RATE_SPAWN))) {
				break;
			}
		}
	}

	if (spawnedMap.size() < spawnMap.size()) {
		checkSpawnEvent = g_scheduler.addEvent(createSchedulerTask(getInterval(), std::bind(&Spawn::checkSpawn, this)));
	}
}

void Spawn::cleanup()
{
	auto it = spawnedMap.begin();
	while (it != spawnedMap.end()) {
		uint32_t spawnId = it->first;
		Monster* monster = it->second;
		if (monster->isRemoved()) {
			if (spawnId != 0) {
				spawnMap[spawnId].lastSpawn = OTSYS_TIME();
			}

			monster->decrementReferenceCounter();
			it = spawnedMap.erase(it);
		} else if (!isInSpawnZone(monster->getPosition()) && spawnId != 0) {
			spawnedMap.insert(spawned_pair(0, monster));
			it = spawnedMap.erase(it);
		} else {
			++it;
		}
	}
}

bool Spawn::addMonster(const std::string& name, const Position& pos, Direction dir, uint32_t interval)
{
	MonsterType* mType = g_monsters.getMonsterType(name);
	if (!mType) {
		std::cout << "[Spawn::addMonster] Can not find " << name << std::endl;
		return false;
	}

	this->interval = std::min(this->interval, interval);

	spawnBlock_t sb;
	sb.mType = mType;
	sb.pos = pos;
	sb.direction = dir;
	sb.interval = interval;
	sb.lastSpawn = 0;

	uint32_t spawnId = spawnMap.size() + 1;
	spawnMap[spawnId] = sb;
	return true;
}

void Spawn::removeMonster(Monster* monster)
{
	for (auto it = spawnedMap.begin(), end = spawnedMap.end(); it != end; ++it) {
		if (it->second == monster) {
			monster->decrementReferenceCounter();
			spawnedMap.erase(it);
			break;
		}
	}
}

void Spawn::stopEvent()
{
	if (checkSpawnEvent != 0) {
		g_scheduler.stopEvent(checkSpawnEvent);
		checkSpawnEvent = 0;
	}
}
