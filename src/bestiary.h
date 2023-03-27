// Copyright 2022 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_BESTIARY_H
#define FS_BESTIARY_H
#include <map>
#include <string>

class Player;
class MonsterType;

struct BestiaryBlock_t;

typedef std::map<std::string, MonsterType*> RaceMap;
typedef std::map<uint16_t, MonsterType*> RaceMapRaceId;
typedef std::map<std::string, RaceMap> BestiaryMap;
typedef std::map<std::string, RaceMapRaceId> BestiaryMapRaceId;

class Bestiary
{
public:
	void addBestiaryMonster(std::string race, MonsterType* mType);

	RaceMap& getRaceBestiary(std::string race);

	bool isValidBestiaryRecord(BestiaryBlock_t& bestiaryBlock);

	size_t getRaceCount() { return bestiaryMap.size(); };

	size_t getBestiaryMonsterCount();

	bool isBestiaryFinished(const Player* player, const MonsterType* mType);

	bool isBestiaryInProgress(const Player* player, const MonsterType* mType);

	uint16_t getProgressStatus(const Player* player, const MonsterType* mType);

	uint16_t calculateDifficult(uint32_t chance);

	std::map<uint8_t, int16_t> getMonsterElements(MonsterType* mtype) const;

	void clear();

	BestiaryMap& getBestiaryMapByName() { return bestiaryMap; };

	BestiaryMapRaceId& getBestiaryMapByRaceId() { return bestiaryMapRaceId; };

	RaceMap& getRaceMapByName(std::string race) { return bestiaryMap[race]; };

	RaceMapRaceId& getRaceMapByRaceId(std::string race) { return bestiaryMapRaceId[race]; };

private:
	BestiaryMap bestiaryMap;
	BestiaryMapRaceId bestiaryMapRaceId;
};
#endif // FS_BAN_H
