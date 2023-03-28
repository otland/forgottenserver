// Copyright 2022 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "otpch.h"

#include <stdexcept>
#include <iostream>

#include "bestiary.h"
#include "player.h"
#include "monsters.h"

void Bestiary::addBestiaryMonster(std::string className, MonsterType* mType)
{
	bestiaryMap[className][mType->name] = mType;
	bestiaryMapRaceId[className][mType->info.bestiary.raceId] = mType;
}

RaceMap& Bestiary::getRaceBestiary(std::string className) { return bestiaryMap[className]; }

size_t Bestiary::getBestiaryMonsterCount()
{
	size_t monsterCount = 0;
	for (auto it = getBestiaryMapByRaceId().begin(); it != getBestiaryMapByRaceId().end(); it++) {
		monsterCount += it->second.size();
	}
	return monsterCount;
}

bool Bestiary::isValidBestiaryRecord(BestiaryBlock_t& bestiaryBlock)
{
	constexpr int maxBestiaryStars = 5;
	constexpr int maxBestiaryOccurrence = 4;

	if (bestiaryBlock.raceId == 0) {
		std::cout << "[Warning - Bestiary::isValidBestiaryRecord] race id can't be 0." << std::endl;
		return false;
	}

	if (bestiaryBlock.className.size() == 0) {
		std::cout << "[Warning - Bestiary::isValidBestiaryRecord] class can't be empty string." << std::endl;
		return false;
	}

	if (bestiaryBlock.prowess == 0 || bestiaryBlock.expertise == 0 || bestiaryBlock.mastery == 0) {
		std::cout << "[Warning - Bestiary::isValidBestiaryRecord] unlock data value can't be 0." << std::endl;
		return false;
	}

	if (bestiaryBlock.prowess == 0  || bestiaryBlock.prowess >= bestiaryBlock.expertise ||
	    bestiaryBlock.expertise >= bestiaryBlock.mastery) {
		std::cout << "[Warning - Bestiary::isValidBestiaryRecord] invalid unlock data value - "
		          << "prowess == 0 or "
		          << "prowess >= expertise (" << bestiaryBlock.prowess << ">=" << bestiaryBlock.expertise
		          << ") or expertise >= expertise(" << bestiaryBlock.expertise << ">=" << bestiaryBlock.mastery << ")"
		          << std::endl;
		return false;
	}

	if (bestiaryBlock.stars > maxBestiaryStars) {
		std::cout << "[Warning - Bestiary::isValidBestiaryRecord] invalid stars value: " << bestiaryBlock.stars << "."
		          << std::endl;
		return false;
	}

	if (bestiaryBlock.occurrence > maxBestiaryOccurrence) {
		std::cout << "[Warning - Bestiary::isValidBestiaryRecord] invalid occurrence value: "
		          << bestiaryBlock.occurrence << "." << std::endl;
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
	if (kills >= mType->info.bestiary.mastery) {
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
	} else if (killAmount < mType->info.bestiary.prowess) {
		return 1;
	} else if (killAmount < mType->info.bestiary.expertise) {
		return 2;
	} else if (killAmount < mType->info.bestiary.mastery) {
		return 3;
	}
	return 4;
}

uint16_t Bestiary::calculateDifficult(uint32_t chance)
{
	if (chance < 200) { // 0.2%
		return 4;
	} else if (chance < 1000) { // 1%
		return 3;
	} else if (chance < 5000) { // 5%
		return 2;
	} else if (chance < 25000) { // 25%
		return 1;
	}
	return 0; 
}

std::array<int16_t, 8> Bestiary::getMonsterElements(MonsterType* mtype) const
{
	std::array<int16_t, 8> deffArray = {};
	deffArray.fill(100);

	for (const auto& [damageType, damaveValue] : mtype->info.elementMap) {
		switch (damageType) {
			case COMBAT_PHYSICALDAMAGE:
				deffArray[0] -= static_cast<int16_t>(damaveValue);
				break;
			case COMBAT_FIREDAMAGE:
				deffArray[1] -= static_cast<int16_t>(damaveValue);
				break;
			case COMBAT_EARTHDAMAGE:
				deffArray[2] -= static_cast<int16_t>(damaveValue);
				break;
			case COMBAT_ENERGYDAMAGE:
				deffArray[3] -= static_cast<int16_t>(damaveValue);
				break;
			case COMBAT_ICEDAMAGE:
				deffArray[4] -= static_cast<int16_t>(damaveValue);
				break;
			case COMBAT_HOLYDAMAGE:
				deffArray[5] -= static_cast<int16_t>(damaveValue);
				break;
			case COMBAT_DEATHDAMAGE:
				deffArray[6] -= static_cast<int16_t>(damaveValue);
				break;
			case COMBAT_HEALING:
				deffArray[7] -= static_cast<int16_t>(damaveValue);
				break;
			default:
				break;
		}
	}
	return deffArray;
}
