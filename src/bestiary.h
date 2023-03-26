// Copyright 2022 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_BESTIARY_H
#define FS_BESTIARY_H
#include <unordered_map>
#include <string>

class Player;
class MonsterType;

struct BestiaryBlock_t;

typedef std::unordered_map<std::string, const MonsterType*> RaceMap;
typedef std::unordered_map<std::string, RaceMap> BestiaryMap;

class Bestiary
{
public:
	void addBestiaryMonster(std::string race, const MonsterType* mType);

	RaceMap& getRaceBestiary(std::string race);

	const MonsterType* getBestiaryRecordData(const MonsterType* mType) const;

	bool isValidBestiaryRecord(BestiaryBlock_t& bestiaryBlock);

	size_t getRaceCount() { return bestiaryMap.size(); };

	bool isBestiaryFinished(const Player* player, const MonsterType* mType);

	bool isBestiaryInProgress(const Player* player, const MonsterType* mType);

	uint16_t getProgressStatus(const Player* player, const MonsterType* mType);

	void clear();

	BestiaryMap bestiaryMap;
};
#endif // FS_BAN_H
