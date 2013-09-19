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

#ifndef __OTSERV_MONSTERS_H__
#define __OTSERV_MONSTERS_H__

#include <string>
#include "creature.h"

#define MAX_LOOTCHANCE 100000
#define MAX_STATICWALK 100

struct LootBlock {
	uint16_t id;
	uint32_t countmax;
	uint32_t chance;

	//optional
	int32_t subType;
	int32_t actionId;
	std::string text;

	typedef std::list<LootBlock> LootItems;
	LootItems childLoot;
	LootBlock() {
		id = 0;
		countmax = 0;
		chance = 0;

		subType = -1;
		actionId = -1;
		text = "";
	}
};

struct summonBlock_t {
	std::string name;
	uint32_t chance;
	uint32_t speed;
};

class BaseSpell;

struct spellBlock_t {
	BaseSpell* spell;
	uint32_t chance;
	uint32_t speed;
	uint32_t range;
	int32_t minCombatValue;
	int32_t maxCombatValue;
	bool combatSpell;
	bool isMelee;
};

struct voiceBlock_t {
	std::string text;
	bool yellText;
};

typedef std::list<LootBlock> LootItems;
typedef std::list<summonBlock_t> SummonList;
typedef std::list<spellBlock_t> SpellList;
typedef std::vector<voiceBlock_t> VoiceVector;
typedef std::list<std::string> MonsterScriptList;
typedef std::map<CombatType_t, int32_t> ElementMap;

class MonsterType
{
	public:
		MonsterType();
		~MonsterType();

		void reset();

		std::string name;
		std::string nameDescription;
		uint64_t experience;

		int32_t defense;
		int32_t armor;

		bool canPushItems;
		bool canPushCreatures;
		uint32_t staticAttackChance;
		int32_t maxSummons;
		int32_t targetDistance;
		int32_t runAwayHealth;
		bool pushable;
		int32_t baseSpeed;
		int32_t health;
		int32_t healthMax;

		Outfit_t outfit;
		uint16_t lookcorpse;
		int32_t conditionImmunities;
		int32_t damageImmunities;
		RaceType_t race;
		bool isSummonable;
		bool isIllusionable;
		bool isConvinceable;
		bool isAttackable;
		bool isHostile;
		bool hiddenHealth;

		int32_t lightLevel;
		int32_t lightColor;

		uint32_t manaCost;
		SummonList summonList;
		LootItems lootItems;
		ElementMap elementMap;
		SpellList spellAttackList;
		SpellList spellDefenseList;

		uint32_t yellChance;
		uint32_t yellSpeedTicks;
		VoiceVector voiceVector;

		int32_t changeTargetSpeed;
		int32_t changeTargetChance;

		MonsterScriptList scriptList;

		void createLoot(Container* corpse);
		bool createLootContainer(Container* parent, const LootBlock& lootblock);
		std::list<Item*> createLootItem(const LootBlock& lootblock);
};

class Monsters
{
	public:
		Monsters();
		~Monsters();

		bool loadFromXml(bool reloading = false);
		bool isLoaded() const {
			return loaded;
		}
		bool reload();

		MonsterType* getMonsterType(const std::string& name);
		MonsterType* getMonsterType(uint32_t mid);
		uint32_t getIdByName(const std::string& name);

		static uint32_t getLootRandom();

	private:
		ConditionDamage* getDamageCondition(ConditionType_t conditionType,
		                                    int32_t maxDamage, int32_t minDamage, int32_t startDamage, uint32_t tickInterval);
		bool deserializeSpell(const pugi::xml_node& node, spellBlock_t& sb, const std::string& description = "");

		bool loadMonster(const std::string& file, const std::string& monster_name, bool reloading = false);

		void loadLootContainer(const pugi::xml_node& node, LootBlock&);
		bool loadLootItem(const pugi::xml_node& node, LootBlock&);

		typedef std::map<std::string, uint32_t> MonsterNameMap;
		MonsterNameMap monsterNames;

		typedef std::map<uint32_t, MonsterType*> MonsterMap;
		MonsterMap monsters;

		bool loaded;
};

#endif
