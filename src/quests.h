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

#ifndef _QUESTS_H_
#define _QUESTS_H_

#include <list>
#include <string>
#include "player.h"
#include "networkmessage.h"

class MissionState;
class Mission;
class Quest;

typedef std::map<int32_t, MissionState*> StateList;
typedef std::list<Mission*> MissionsList;
typedef std::list<Quest*> QuestsList;

class MissionState
{
	public:
		MissionState(const std::string& _description, int32_t _missionID);

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
		Mission(const std::string& _missionName, int32_t _storageID, int32_t _startValue, int32_t _endValue, bool _ignoreEndValue);
		~Mission();
		bool isCompleted(Player* player) const;
		bool isStarted(Player* player) const;
		std::string getName(Player* player);
		std::string getDescription(Player* player);

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
		StateList state;

	private:
		std::string missionName;
		uint32_t storageID;
		int32_t startValue, endValue;
		bool ignoreEndValue;
};

class Quest
{
	public:
		Quest(const std::string& _name, uint16_t _id, int32_t _startStorageID, int32_t _startStorageValue);
		~Quest();

		bool isCompleted(Player* player);
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

		void addMission(Mission* mission) {
			missions.push_back(mission);
		}

		MissionsList::const_iterator getFirstMission() const {
			return missions.begin();
		}
		MissionsList::const_iterator getLastMission() const {
			return missions.end();
		}

	private:
		std::string name;

		uint32_t startStorageID;
		int32_t startStorageValue;
		uint16_t id;

		MissionsList missions;
};

class Quests
{
	public:
		Quests();
		~Quests();

		static Quests* getInstance() {
			static Quests instance;
			return &instance;
		}

		QuestsList::const_iterator getFirstQuest() const {
			return quests.begin();
		}
		QuestsList::const_iterator getLastQuest() const {
			return quests.end();
		}

		bool loadFromXml();
		Quest* getQuestByID(uint16_t id);
		bool isQuestStorage(const uint32_t key, const int32_t value, const int32_t oldValue);
		uint16_t getQuestsCount(Player* player);
		bool reload();

	private:
		QuestsList quests;
};

#endif
