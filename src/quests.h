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

#ifndef _QUESTS_H_
#define _QUESTS_H_

#include "player.h"
#include "networkmessage.h"

class MissionState;
class Mission;
class Quest;

typedef std::list<Mission> MissionsList;
typedef std::list<Quest> QuestsList;

class MissionState
{
	public:
		MissionState() {}
		MissionState(const std::string& description, int32_t missionID)
			: description(description), missionID(missionID) {}

		int32_t getMissionID() const {
			return missionID;
		}
		std::string getMissionDescription() const {
			return description;
		}

	private:
		std::string description;
		int32_t missionID;
};

class Mission
{
	public:
		Mission(const std::string& name, int32_t storageID, int32_t startValue, int32_t endValue, bool ignoreEndValue)
			: name(name), storageID(storageID), startValue(startValue), endValue(endValue), ignoreEndValue(ignoreEndValue) {
			mainState = nullptr;
		}

		bool isCompleted(Player* player) const;
		bool isStarted(Player* player) const;
		std::string getName(Player* player) const;
		std::string getDescription(Player* player) const;

		uint32_t getStorageId() const {
			return storageID;
		}
		int32_t getStartStorageValue() const {
			return startValue;
		}
		int32_t getEndStorageValue() const {
			return endValue;
		}

		MissionState* mainState;
		std::map<int32_t, MissionState> state;

	private:
		std::string name;
		uint32_t storageID;
		int32_t startValue, endValue;
		bool ignoreEndValue;
};

class Quest
{
	public:
		Quest(const std::string& name, uint16_t id, int32_t startStorageID, int32_t startStorageValue)
			: name(name), startStorageID(startStorageID), startStorageValue(startStorageValue), id(id) {}

		bool isCompleted(Player* player) const;
		bool isStarted(Player* player) const;
		uint16_t getID() const {
			return id;
		}
		std::string getName() const {
			return name;
		}
		uint16_t getMissionsCount(Player* player) const;

		uint32_t getStartStorageId() const {
			return startStorageID;
		}
		int32_t getStartStorageValue() const {
			return startStorageValue;
		}

		const MissionsList& getMissions() const {
			return missions;
		}

	private:
		std::string name;

		uint32_t startStorageID;
		int32_t startStorageValue;
		uint16_t id;

		MissionsList missions;

	friend class Quests;
};

class Quests
{
	public:
		static Quests* getInstance() {
			static Quests instance;
			return &instance;
		}

		const QuestsList& getQuests() const {
			return quests;
		}

		bool loadFromXml();
		Quest* getQuestByID(uint16_t id);
		bool isQuestStorage(const uint32_t key, const int32_t value, const int32_t oldValue) const;
		uint16_t getQuestsCount(Player* player) const;
		bool reload();

	private:
		QuestsList quests;
};

#endif
