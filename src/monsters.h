/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2015  Mark Samman <mark.samman@gmail.com>
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

class Loot
{
	public:
		Loot();

		void reset();

		Loot(const Loot&) = delete;
		Loot& operator=(const Loot&) = delete;

		uint16_t id;
		uint32_t chance;
		uint8_t countMax;
		int16_t subType;
		int32_t actionId;
		std::string text;

		std::list<Loot*> children;
};

class MonsterSpell
{
	public:
		MonsterSpell();

		void reset();

		MonsterSpell(const MonsterSpell&) = delete;
		MonsterSpell& operator=(const MonsterSpell&) = delete;
		
		uint8_t chance;
		uint16_t interval;
		uint8_t range;
		int32_t minCombatValue;
		int32_t maxCombatValue;
		int32_t attack;
		int32_t skill;
		bool combatSpell;
		bool isMelee;
		std::string name;
		std::string scriptName;
		bool isScripted;
		bool needTarget;
		bool needDirection;

		int32_t length;
		int32_t spread;
		int32_t radius;

		ConditionType_t conditionType;
		CombatType_t combatType;
		int32_t conditionMinDamage;
		int32_t conditionMaxDamage;
		int32_t conditionStartDamage;

		int32_t tickInterval;

		int32_t speedChange;
		int32_t duration;
		Outfit_t outfit;

		ShootType_t shoot;
		MagicEffectClasses effect;

};

class MonsterType
{
	public:
		MonsterType();
		~MonsterType();

		// non-copyable
		MonsterType(const MonsterType&) = delete;
		MonsterType& operator=(const MonsterType&) = delete;

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

		uint32_t manaCost;
		uint32_t yellChance;
		uint32_t yellSpeedTicks;
		uint32_t staticAttackChance;
		uint32_t maxSummons;
		uint32_t changeTargetSpeed;
		uint32_t conditionImmunities;
		uint32_t damageImmunities;
		uint32_t baseSpeed;

		int32_t creatureAppearEvent;
		int32_t creatureDisappearEvent;
		int32_t creatureMoveEvent;
		int32_t creatureSayEvent;
		int32_t thinkEvent;
		int32_t targetDistance;
		int32_t runAwayHealth;
		int32_t health;
		int32_t healthMax;
		int32_t changeTargetChance;
		int32_t defense;
		int32_t armor;

		RaceType_t race;

		uint16_t lookcorpse;

		Skulls_t skull;
		uint8_t lightLevel;
		uint8_t lightColor;

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
		void clone(MonsterType* target, MonsterType* source, const std::string& type);
		void loadLoot(Loot* loot, LootBlock& lootblock);
		void loadLoot(MonsterType* monsterType, Loot* loot);
		void loadChildLoot(Loot* loot, LootBlock& parent);
		std::list<Item*> createLootItem(const LootBlock& lootblock);
};

class Monsters
{
	public:
		Monsters();
		~Monsters();

		// non-copyable
		Monsters(const Monsters&) = delete;
		Monsters& operator=(const Monsters&) = delete;

		bool loadFromXml(bool reloading = false);
		bool isLoaded() const {
			return loaded;
		}
		bool reload();

		MonsterType* getMonsterType(const std::string& name);
		MonsterType* getMonsterType(uint32_t mid);
		uint32_t getIdByName(const std::string& name);
		void addMonsterType(const std::string& name, MonsterType* mType);
		bool deserializeSpell(MonsterSpell* spell, spellBlock_t& sb, const std::string& description = "");

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
