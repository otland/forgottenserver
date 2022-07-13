// Copyright 2022 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_QUESTS_H
#define FS_QUESTS_H

class Mission;
class Quest;
class Player;

using MissionsList = std::list<Mission>;
using QuestsList = std::list<Quest>;

//for luascript callback
class MissionCallback final : public CallBack
{
	public:
		MissionCallback() = default;
		std::string getDescription(Player* player) const;
};

class Mission
{
public:
	Mission(std::string name, uint16_t id, int32_t storageID, int32_t startValue, int32_t endValue,
	        bool ignoreEndValue) :
	    name(std::move(name)),
	    storageID(storageID),
	    startValue(startValue),
	    endValue(endValue),
	    ignoreEndValue(ignoreEndValue),
	    id(id)
	{}

	bool isCompleted(Player* player) const;
	bool isStarted(Player* player) const;
	std::string getName(Player* player) const;
	std::string getDescription(Player* player) const;
	uint16_t getID() const { return id; }
	uint32_t getStorageId() const { return storageID; }
	int32_t getStartStorageValue() const { return startValue; }
	int32_t getEndStorageValue() const { return endValue; }

	std::map<int32_t, std::string> descriptions;
	std::string mainDescription;

	CallBack* getCallback() {
		if (!callback) {
			callback.reset(new MissionCallback());
		}

		return callback.get();
	}

private:
	std::string name;
	uint32_t storageID;
	int32_t startValue, endValue;
	bool ignoreEndValue;
	uint16_t id;

	std::unique_ptr<MissionCallback> callback;

	friend class Quest;
};

class Quest
{
public:
	Quest(std::string name, uint16_t id, int32_t startStorageID, int32_t startStorageValue) :
	    name(std::move(name)), startStorageID(startStorageID), startStorageValue(startStorageValue), id(id)
	{}

	bool isCompleted(Player* player) const;
	bool isStarted(Player* player) const;
	uint16_t getID() const { return id; }
	std::string getName() const { return name; }
	uint16_t getMissionsCount(Player* player) const;

	uint32_t getStartStorageId() const { return startStorageID; }
	int32_t getStartStorageValue() const { return startStorageValue; }

	const MissionsList& getMissions() const { return missions; }

	const Mission* getMissionById(uint16_t id) const;

	bool isTracking(const uint32_t key, const int32_t value) const;

	Mission& createMission(const uint16_t id, const std::string& name, const int32_t storageId, const int32_t startValue, const int32_t endValue, const bool ignoreEndValue);

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
	const QuestsList& getQuests() const { return quests; }

	bool loadFromXml();
	Quest* getQuestByID(uint16_t id);
	bool isQuestStorage(const uint32_t key, const int32_t value, const int32_t oldValue) const;
	uint16_t getQuestsCount(Player* player) const;
	bool reload();

	void createQuest(Quest* quest);

	static uint32_t questAutoID;
	static uint32_t missionAutoID;
private:
	QuestsList quests;
};

class TrackedQuest
{
public:
	TrackedQuest(uint16_t questId, uint16_t missionId) : questId(questId), missionId(missionId) {}

	uint16_t getQuestId() const { return questId; }

	uint16_t getMissionId() const { return missionId; }

private:
	uint16_t questId = 0;
	uint16_t missionId = 0;
};

#endif // FS_QUESTS_H
