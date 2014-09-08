/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2014  Mark Samman <mark.samman@gmail.com>
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

#ifndef FS_MONSTERS_H_776E8327BCE2450EB7C4A260785E6C0D
#define FS_MONSTERS_H_776E8327BCE2450EB7C4A260785E6C0D

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

	std::list<LootBlock> childLoot;
	LootBlock() {
		id = 0;
		countmax = 0;
		chance = 0;

		subType = -1;
		actionId = -1;
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

class MonsterType
{
	public:
		MonsterType();
		~MonsterType();

		void reset();

		std::map<CombatType_t, int32_t> elementMap;

		std::vector<voiceBlock_t> voiceVector;

		std::list<LootBlock> lootItems;
		std::list<std::string> scriptList;
		std::list<spellBlock_t> spellAttackList;
		std::list<spellBlock_t> spellDefenseList;
		std::list<summonBlock_t> summonList;

		std::string name;
		std::string nameDescription;

		LuaScriptInterface* scriptInterface;

		uint64_t experience;

		Outfit_t outfit;

		int32_t creatureAppearEvent;
		int32_t creatureDisappearEvent;
		int32_t creatureMoveEvent;
		int32_t creatureSayEvent;
		int32_t thinkEvent;

		uint32_t manaCost;
		uint32_t yellChance;
		uint32_t yellSpeedTicks;
		uint32_t staticAttackChance;
		uint32_t maxSummons;
		uint32_t runAwayHealth;
		int32_t targetDistance;
		int32_t baseSpeed;
		int32_t health;
		int32_t healthMax;
		int32_t changeTargetSpeed;
		int32_t changeTargetChance;
		int32_t lightLevel;
		int32_t lightColor;
		int32_t conditionImmunities;
		int32_t damageImmunities;
		int32_t defense;
		int32_t armor;

		RaceType_t race;

		uint16_t lookcorpse;

		bool canPushItems;
		bool canPushCreatures;
		bool pushable;
		bool isSummonable;
		bool isIllusionable;
		bool isConvinceable;
		bool isAttackable;
		bool isHostile;
		bool hiddenHealth;

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

		std::map<std::string, uint32_t> monsterNames;
		std::map<MonsterType*, std::string> monsterScriptList;
		std::map<uint32_t, MonsterType*> monsters;
		LuaScriptInterface* scriptInterface;

		bool loaded;
};

#endif
