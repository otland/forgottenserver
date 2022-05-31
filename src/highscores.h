// Copyright 2022 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_HIGHSCORES_H
#define FS_HIGHSCORES_H

#include "configmanager.h"

enum HighscoresType : uint8_t
{
	HIGHSCORES_TYPE_SKILLS = 0x00,
	HIGHSCORES_TYPE_POINTS = 0x01,
	HIGHSCORES_TYPE_SCORE = 0x02
};

enum HighscoresCategoryTypes
{
	HIGHSCORES_CATEGORY_LEVEL = 0,
	HIGHSCORES_CATEGORY_MAGLEVEL = 1,
	HIGHSCORES_CATEGORY_FIST_FIGHTING = 2,
	HIGHSCORES_CATEGORY_AXE_FIGHTING = 3,
	HIGHSCORES_CATEGORY_CLUB_FIGHTING = 4,
	HIGHSCORES_CATEGORY_SWORD_FIGHTING = 5,
	HIGHSCORES_CATEGORY_DISTANCE_FIGHTING = 6,
	HIGHSCORES_CATEGORY_SHIELDING = 7,
	HIGHSCORES_CATEGORY_FISHING = 8,
	HIGHSCORES_CATEGORY_ACHIEVEMENTS = 9,
	HIGHSCORES_CATEGORY_LOYALTY = 10,
	// HIGHSCORES_CATEGORY_DEATHS = 10,
	HIGHSCORES_CATEGORY_FIRST = HIGHSCORES_CATEGORY_LEVEL,
	HIGHSCORES_CATEGORY_LAST = HIGHSCORES_CATEGORY_ACHIEVEMENTS
};

struct Highscores
{
	static std::map<uint8_t, std::string> init()
	{
		std::map<uint8_t, std::string> map;
		map[HIGHSCORES_CATEGORY_ACHIEVEMENTS] = "Achievement Points";
		map[HIGHSCORES_CATEGORY_AXE_FIGHTING] = "Axe Fighting";
		map[HIGHSCORES_CATEGORY_CLUB_FIGHTING] = "Club Fighting";
		map[HIGHSCORES_CATEGORY_DISTANCE_FIGHTING] = "Distance Fighting";
		map[HIGHSCORES_CATEGORY_LEVEL] = "Experience Points";
		map[HIGHSCORES_CATEGORY_FISHING] = "Fishing";
		map[HIGHSCORES_CATEGORY_FIST_FIGHTING] = "Fist Fighting";
		map[HIGHSCORES_CATEGORY_MAGLEVEL] = "Magic Level";
		map[HIGHSCORES_CATEGORY_SHIELDING] = "Shielding";
		map[HIGHSCORES_CATEGORY_SWORD_FIGHTING] = "Sword Fighting";
		// map[HIGHSCORES_CATEGORY_LOYALTY] = "Loyalty";
		// map[HIGHSCORES_CATEGORY_DEATHS] = "Most Deaths";
		return map;
	}

	static uint8_t getType(HighscoresCategoryTypes type)
	{
		std::map<uint8_t, uint8_t> map;
		map[HIGHSCORES_CATEGORY_LEVEL] = HIGHSCORES_TYPE_POINTS;
		map[HIGHSCORES_CATEGORY_MAGLEVEL] = HIGHSCORES_TYPE_SKILLS;
		map[HIGHSCORES_CATEGORY_FIST_FIGHTING] = HIGHSCORES_TYPE_SKILLS;
		map[HIGHSCORES_CATEGORY_AXE_FIGHTING] = HIGHSCORES_TYPE_SKILLS;
		map[HIGHSCORES_CATEGORY_CLUB_FIGHTING] = HIGHSCORES_TYPE_SKILLS;
		map[HIGHSCORES_CATEGORY_SWORD_FIGHTING] = HIGHSCORES_TYPE_SKILLS;
		map[HIGHSCORES_CATEGORY_DISTANCE_FIGHTING] = HIGHSCORES_TYPE_SKILLS;
		map[HIGHSCORES_CATEGORY_SHIELDING] = HIGHSCORES_TYPE_SKILLS;
		map[HIGHSCORES_CATEGORY_FISHING] = HIGHSCORES_TYPE_SKILLS;
		map[HIGHSCORES_CATEGORY_ACHIEVEMENTS] = HIGHSCORES_TYPE_POINTS;
		// map[HIGHSCORES_CATEGORY_LOYALTY] = HIGHSCORES_TYPE_POINTS;
		// map[HIGHSCORES_CATEGORY_DEATHS] = HIGHSCORES_TYPE_POINTS;
		return map[type];
	}
};

enum HighscoresAction : uint8_t
{
	HIGHSCORES_ACTION_BROWSE = 0x00,
	HIGHSCORES_ACTION_OWN = 0x01
};

struct HighscoresParams
{
	std::string world;
	uint32_t vocation = 0xFFFFFFFF;
	uint32_t category = HIGHSCORES_CATEGORY_FIRST;
	uint8_t type = 0;
	uint8_t battlEye = 0xFF;
	uint16_t page = 1;
	uint16_t totalPages = 0;
	uint64_t timestamp = 0;
};

struct HighscoresEntry
{
	HighscoresEntry(uint32_t id, uint32_t rank, std::string name, std::string title, uint8_t vocation,
	                std::string world, uint16_t level, uint64_t points) :
	    id(id),
	    rank(rank),
	    name(std::move(name)),
	    title(std::move(title)),
	    vocation(vocation),
	    world(std::move(world)),
	    level(level),
	    points(points)
	{}

	uint32_t id = 0;
	uint32_t rank = 0;
	std::string name;
	std::string title;
	uint8_t vocation = VOCATION_NONE;
	std::string world;
	uint16_t level = 1;
	uint64_t points = 0;
};

#endif // FS_HIGHSCORES_H
