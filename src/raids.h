/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2016  Mark Samman <mark.samman@gmail.com>
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

#ifndef FS_RAIDS_H_3583C7C054584881856D55765DEDAFA9
#define FS_RAIDS_H_3583C7C054584881856D55765DEDAFA9

#include "const.h"
#include "position.h"
#include "baseevents.h"

enum RaidState_t {
	RAIDSTATE_IDLE,
	RAIDSTATE_EXECUTING,
};

struct MonsterSpawn {
	MonsterSpawn(const char* name, uint32_t minAmount, uint32_t maxAmount) : name(name), minAmount(minAmount), maxAmount(maxAmount) {}

	std::string name;
	uint32_t minAmount;
	uint32_t maxAmount;
};

//How many times it will try to find a tile to add the monster to before giving up
#define MAXIMUM_TRIES_PER_MONSTER 10
#define CHECK_RAIDS_INTERVAL 60
#define RAID_MINTICKS 1000

class Raid;
class RaidEvent;

class Raids
{
	public:
		Raids();
		~Raids();

		// non-copyable
		Raids(const Raids&) = delete;
		Raids& operator=(const Raids&) = delete;

		bool loadFromXml();
		bool startup();

		void clear();
		bool reload();

		bool isLoaded() const {
			return loaded;
		}
		bool isStarted() const {
			return started;
		}

		Raid* getRunning() {
			return running;
		}
		void setRunning(Raid* newRunning) {
			running = newRunning;
		}

		Raid* getRaidByName(const std::string& name);

		uint64_t getLastRaidEnd() const {
			return lastRaidEnd;
		}
		void setLastRaidEnd(uint64_t newLastRaidEnd) {
			lastRaidEnd = newLastRaidEnd;
		}

		void checkRaids();

		LuaScriptInterface& getScriptInterface() {
			return scriptInterface;
		}

	private:
		LuaScriptInterface scriptInterface;

		std::list<Raid*> raidList;
		Raid* running;
		uint64_t lastRaidEnd;
		uint32_t checkRaidsEvent;
		bool loaded, started;
};

class Raid
{
	public:
		Raid(std::string name, uint32_t interval, uint32_t marginTime, bool repeat)
			: name(name), interval(interval), nextEvent(0), margin(marginTime), state(RAIDSTATE_IDLE), nextEventEvent(0), loaded(false), repeat(repeat) {}
		~Raid();

		// non-copyable
		Raid(const Raid&) = delete;
		Raid& operator=(const Raid&) = delete;

		bool loadFromXml(const std::string& _filename);

		void startRaid();

		void executeRaidEvent(RaidEvent* raidEvent);
		void resetRaid();

		RaidEvent* getNextRaidEvent();
		void setState(RaidState_t newState) {
			state = newState;
		}
		std::string getName() const {
			return name;
		}

		bool isLoaded() const {
			return loaded;
		}
		uint64_t getMargin() const {
			return margin;
		}
		uint32_t getInterval() const {
			return interval;
		}
		bool canBeRepeated() const {
			return repeat;
		}

		void stopEvents();

	private:
		std::vector<RaidEvent*> raidEvents;
		std::string name;
		uint32_t interval;
		uint32_t nextEvent;
		uint64_t margin;
		RaidState_t state;
		uint32_t nextEventEvent;
		bool loaded;
		bool repeat;
};

class RaidEvent
{
	public:
		virtual ~RaidEvent() = default;

		virtual bool configureRaidEvent(const pugi::xml_node& eventNode);

		virtual bool executeEvent() = 0;
		uint32_t getDelay() const {
			return delay;
		}
		void setDelay(uint32_t newDelay) {
			delay = newDelay;
		}

		static bool compareEvents(const RaidEvent* lhs, const RaidEvent* rhs) {
			return lhs->getDelay() < rhs->getDelay();
		}

	private:
		uint32_t delay;
};

class AnnounceEvent final : public RaidEvent
{
	public:
		AnnounceEvent() : messageType(MESSAGE_EVENT_ADVANCE) {}

		bool configureRaidEvent(const pugi::xml_node& eventNode) final;

		bool executeEvent() final;

	private:
		std::string message;
		MessageClasses messageType;
};

class SingleSpawnEvent final : public RaidEvent
{
	public:
		bool configureRaidEvent(const pugi::xml_node& eventNode) final;

		bool executeEvent() final;

	private:
		std::string monsterName;
		Position position;
};

class AreaSpawnEvent final : public RaidEvent
{
	public:
		bool configureRaidEvent(const pugi::xml_node& eventNode) final;

		void addMonster(const std::string& monsterName, uint32_t minAmount, uint32_t maxAmount);

		bool executeEvent() final;

	private:
		std::list<MonsterSpawn> spawnList;
		Position fromPos, toPos;
};

class ScriptEvent final : public RaidEvent, public Event
{
	public:
		explicit ScriptEvent(LuaScriptInterface* _interface);
		explicit ScriptEvent(const ScriptEvent* copy);

		bool configureRaidEvent(const pugi::xml_node& eventNode) final;
		bool configureEvent(const pugi::xml_node&) final {
			return false;
		}

		bool executeEvent() final;

	protected:
		std::string getScriptEventName() const final;
};

#endif
