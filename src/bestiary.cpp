// Copyright 2022 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "otpch.h"

#include <stdexcept>
#include <iostream>

#include "bestiary.h"
#include "player.h"
#include "monsters.h"

void Bestiary::addBestiaryMonster(std::string race, MonsterType* mType)
{
	bestiaryMap[race][mType->name] = mType;
	bestiaryMapRaceId[race][mType->info.bestiary.raceId] = mType;
}

RaceMap& Bestiary::getRaceBestiary(std::string race) { return bestiaryMap[race]; }

size_t Bestiary::getBestiaryMonsterCount()
{
	size_t monsterCount = 0;
	for (auto& it = getBestiaryMapByRaceId().begin(); it != getBestiaryMapByRaceId().end(); it++) {
		monsterCount += it->second.size();
	}
	return monsterCount;
}


bool Bestiary::isValidBestiaryRecord(BestiaryBlock_t& bestiaryBlock)
{
	const int maxBestiaryStars = 5;
	const int maxBestiaryOccurrence = 4;

	if (bestiaryBlock.className.size() == 0) {
		std::cout << "[Warning - Monsters::loadMonster] invalid class name." << std::endl;
		return false;
	}

	if (bestiaryBlock.race.size() == 0) {
		std::cout << "[Warning - Monsters::loadMonster] invalid race." << std::endl;
		return false;
	}

	if (bestiaryBlock.firstUnlock == 0 || bestiaryBlock.secondUnlock == 0 || bestiaryBlock.finishUnlock == 0) {
		std::cout << "[Warning - Monsters::loadMonster] unlock data value can't be 0." << std::endl;
		return false;
	}

	if (bestiaryBlock.firstUnlock >= bestiaryBlock.secondUnlock &&
	    bestiaryBlock.finishUnlock >= bestiaryBlock.secondUnlock) {
		std::cout << "[Warning - Monsters::loadMonster] invalid unlock data value." << std::endl;
		return false;
	}

	if (bestiaryBlock.stars == 0 || bestiaryBlock.stars > maxBestiaryStars) {
		std::cout << "[Warning - Monsters::loadMonster] invalid stars value ." << std::endl;
		return false;
	}

	if (bestiaryBlock.occurrence == 0 || bestiaryBlock.occurrence > maxBestiaryOccurrence) {
		std::cout << "[Warning - Monsters::loadMonster] invalid occurrence value ." << std::endl;
		return false;
	}

	return true;
}

void Bestiary::clear()
{
	bestiaryMap.clear();
	bestiaryMapRaceId.clear();
}

bool Bestiary::isBestiaryFinished(const Player* player, const MonsterType* mType) {
	if (!player || !mType || mType->info.bestiary.raceId == 0) {
		return false;
	}

	uint32_t kills = player->getBestiaryKills(mType->info.bestiary.raceId);
	if (kills >= mType->info.bestiary.finishUnlock) {
		return true;
	}
	return false;
}

bool Bestiary::isBestiaryInProgress(const Player* player, const MonsterType* mType)
{
	if (!player || !mType || mType->info.bestiary.raceId == 0) {
		return false;
	}

	uint32_t kills = player->getBestiaryKills(mType->info.bestiary.raceId);
	if (kills > 0) {
		return true;
	}
	return false;
}

uint16_t Bestiary::getProgressStatus(const Player* player, const MonsterType* mType)
{
	if (!player || !mType || mType->info.bestiary.raceId == 0) {
		return false;
	}

	uint32_t killAmount = player->getBestiaryKills(mType->info.bestiary.raceId);

	if (killAmount == 0) {
		return 0;
	} else if (killAmount < mType->info.bestiary.firstUnlock) {
		return 1;
	} else if (killAmount < mType->info.bestiary.secondUnlock) {
		return 2;
	} else if (killAmount < mType->info.bestiary.finishUnlock) {
		return 3;
	}
	return 4;
}

uint16_t Bestiary::calculateDifficult(uint32_t chance)
{
	double chanceInPercent = static_cast<double>(chance) / 1000;

	if (chanceInPercent < 0.2) { // 0.2%
		return 4;
	} else if (chanceInPercent < 1) { // 1%
		return 3;
	} else if (chanceInPercent < 5) { // 5%
		return 2;
	} else if (chanceInPercent < 25) { // 25%
		return 1;
	}
	return 0; 
}

std::map<uint8_t, int16_t> Bestiary::getMonsterElements(MonsterType* mtype) const
{
	std::map<uint8_t, int16_t> defaultMap = {};
	for (uint8_t i = 0; i <= 7; i++) {
		defaultMap[i] = 100;
	}
	for (const auto& elementEntry : mtype->info.elementMap) {
		switch (elementEntry.first) {
			case COMBAT_PHYSICALDAMAGE:
				defaultMap[0] -= static_cast<int16_t>(elementEntry.second);
				break;
			case COMBAT_FIREDAMAGE:
				defaultMap[1] -= static_cast<int16_t>(elementEntry.second);
				break;
			case COMBAT_EARTHDAMAGE:
				defaultMap[2] -= static_cast<int16_t>(elementEntry.second);
				break;
			case COMBAT_ENERGYDAMAGE:
				defaultMap[3] -= static_cast<int16_t>(elementEntry.second);
				break;
			case COMBAT_ICEDAMAGE:
				defaultMap[4] -= static_cast<int16_t>(elementEntry.second);
				break;
			case COMBAT_HOLYDAMAGE:
				defaultMap[5] -= static_cast<int16_t>(elementEntry.second);
				break;
			case COMBAT_DEATHDAMAGE:
				defaultMap[6] -= static_cast<int16_t>(elementEntry.second);
				break;
			case COMBAT_HEALING:
				defaultMap[7] -= static_cast<int16_t>(elementEntry.second);
				break;
			default:
				break;
		}
	}
	return defaultMap;
}
