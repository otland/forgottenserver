// Copyright 2022 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "otpch.h"

#include <stdexcept>
#include <iostream>

#include "bestiary.h"
#include "player.h"
#include "monsters.h"

void Bestiary::addBestiaryMonster(std::string race, const MonsterType* mType)
{
	bestiaryMap[race][mType->name] = mType;
}

RaceMap& Bestiary::getRaceBestiary(std::string race) { return bestiaryMap[race]; }

const MonsterType* Bestiary::getBestiaryRecordData(const MonsterType* mType) const
{
	/*
	if (!mType) {
		return nullptr;
	}
	MonsterType* mt = nullptr;

	try {
		mt = bestiaryMap.at(mType->info.bestiary.race).at(mType->name);
		// do stuff
	} catch (const std::out_of_range& oor) {
	// TODO KRET 
		std::cout << "[Warning - Monsters::getRaceBestiary] can't bind record on " << std::endl;
	}
	*/
	return mType;
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

void Bestiary::clear() { bestiaryMap.clear(); }


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
	} else if (killAmount < mType->info.bestiary.finishUnlock) {
		return 1;
	} else if (killAmount < mType->info.bestiary.secondUnlock) {
		return 2;
	} else if (killAmount < mType->info.bestiary.finishUnlock) {
		return 3;
	}
	return 4;
}
