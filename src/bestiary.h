// Copyright 2022 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_BESTIARY_H
#define FS_BESTIARY_H
#include <map>
#include <string>

class Player;
class MonsterType;

struct BestiaryBlock_t;

using  RaceMap = std::map<std::string, MonsterType*>;
using  RaceMapRaceId = std::map<uint16_t, MonsterType*>;
using  BestiaryMap = std::map<std::string, RaceMap>;
using  BestiaryMapRaceId = std::map<std::string, RaceMapRaceId>;

class Bestiary
{
public:
	void addBestiaryMonster(std::string race, MonsterType* mType);

	RaceMap& getRaceBestiary(std::string race);

	bool isValidBestiaryRecord(BestiaryBlock_t& bestiaryBlock);

	size_t getRaceCount() const { return bestiaryMap.size(); };

	size_t getBestiaryMonsterCount();

	bool isBestiaryFinished(const Player* player, const MonsterType* mType);

	bool isBestiaryInProgress(const Player* player, const MonsterType* mType);

	uint16_t getProgressStatus(const Player* player, const MonsterType* mType);

	uint16_t calculateDifficult(uint32_t chance);

	std::array<int16_t, 8> getMonsterElements(MonsterType* mtype) const;

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
