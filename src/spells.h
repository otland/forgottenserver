/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2016  Mark Samman <mark.samman@gmail.com>
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

#ifndef FS_SPELLS_H_D78A7CCB7080406E8CAA6B1D31D3DA71
#define FS_SPELLS_H_D78A7CCB7080406E8CAA6B1D31D3DA71

#include "luascript.h"
#include "player.h"
#include "actions.h"
#include "talkaction.h"
#include "baseevents.h"

class InstantSpell;
class ConjureSpell;
class RuneSpell;
class Spell;

typedef std::map<uint16_t, bool> VocSpellMap;

class Spells final : public BaseEvents
{
	public:
		Spells();
		~Spells();

		// non-copyable
		Spells(const Spells&) = delete;
		Spells& operator=(const Spells&) = delete;

		Spell* getSpellByName(const std::string& name);
		RuneSpell* getRuneSpell(uint32_t id);
		RuneSpell* getRuneSpellByName(const std::string& name);

		InstantSpell* getInstantSpell(const std::string& words);
		InstantSpell* getInstantSpellByName(const std::string& name);

		uint32_t getInstantSpellCount(const Player* player) const;
		InstantSpell* getInstantSpellByIndex(const Player* player, uint32_t index);

		TalkActionResult_t playerSaySpell(Player* player, std::string& words);

		static Position getCasterPosition(Creature* creature, Direction dir);
		std::string getScriptBaseName() const final;

	protected:
		void clear() final;
		LuaScriptInterface& getScriptInterface() final;
		Event* getEvent(const std::string& nodeName) final;
		bool registerEvent(Event* event, const pugi::xml_node& node) final;

		std::map<uint16_t, RuneSpell*> runes;
		std::map<std::string, InstantSpell*> instants;

		friend class CombatSpell;
		LuaScriptInterface scriptInterface { "Spell Interface" };
};

typedef bool (InstantSpellFunction)(const InstantSpell* spell, Creature* creature, const std::string& param);
typedef bool (RuneSpellFunction)(const RuneSpell* spell, Player* player, const Position& posTo);

class BaseSpell
{
	public:
		constexpr BaseSpell() = default;
		virtual ~BaseSpell() = default;

		virtual bool castSpell(Creature* creature) = 0;
		virtual bool castSpell(Creature* creature, Creature* target) = 0;
};

class CombatSpell final : public Event, public BaseSpell
{
	public:
		CombatSpell(Combat* combat, bool needTarget, bool needDirection);
		~CombatSpell();

		// non-copyable
		CombatSpell(const CombatSpell&) = delete;
		CombatSpell& operator=(const CombatSpell&) = delete;

		bool castSpell(Creature* creature) final;
		bool castSpell(Creature* creature, Creature* target) final;
		bool configureEvent(const pugi::xml_node&) final {
			return true;
		}

		//scripting
		bool executeCastSpell(Creature* creature, const LuaVariant& var);

		bool loadScriptCombat();
		Combat* getCombat() {
			return combat;
		}

	protected:
		std::string getScriptEventName() const final {
			return "onCastSpell";
		}

		Combat* combat;

		bool needDirection;
		bool needTarget;
};

class Spell : public BaseSpell
{
	public:
		Spell() = default;

		bool configureSpell(const pugi::xml_node& node);
		const std::string& getName() const {
			return name;
		}

		void postCastSpell(Player* player, bool finishedSpell = true, bool payCost = true) const;
		static void postCastSpell(Player* player, uint32_t manaCost, uint32_t soulCost);

		uint32_t getManaCost(const Player* player) const;
		uint32_t getSoulCost() const {
			return soul;
		}
		uint32_t getLevel() const {
			return level;
		}
		uint32_t getMagicLevel() const {
			return magLevel;
		}
		uint32_t getManaPercent() const {
			return manaPercent;
		}
		bool isPremium() const {
			return premium;
		}

		virtual bool isInstant() const = 0;
		bool isLearnable() const {
			return learnable;
		}

		static ReturnValue CreateIllusion(Creature* creature, const Outfit_t& outfit, int32_t time);
		static ReturnValue CreateIllusion(Creature* creature, const std::string& name, int32_t time);
		static ReturnValue CreateIllusion(Creature* creature, uint32_t itemId, int32_t time);

		const VocSpellMap& getVocMap() const {
			return vocSpellMap;
		}

	protected:
		bool playerSpellCheck(Player* player) const;
		bool playerInstantSpellCheck(Player* player, const Position& toPos);
		bool playerRuneSpellCheck(Player* player, const Position& toPos);

		uint8_t spellId = 0;
		SpellGroup_t group = SPELLGROUP_NONE;
		uint32_t groupCooldown = 1000;
		SpellGroup_t secondaryGroup = SPELLGROUP_NONE;
		uint32_t secondaryGroupCooldown = 0;

		uint32_t mana = 0;
		uint32_t manaPercent = 0;
		uint32_t soul = 0;
		uint32_t cooldown = 1000;
		uint32_t level = 0;
		uint32_t magLevel = 0;
		int32_t range = -1;

		bool needTarget = false;
		bool needWeapon = false;
		bool selfTarget = false;
		bool blockingSolid = false;
		bool blockingCreature = false;
		bool aggressive = true;
		bool learnable = false;
		bool enabled = true;
		bool premium = false;

		VocSpellMap vocSpellMap;

	private:
		std::string name;
};

class InstantSpell : public TalkAction, public Spell
{
	public:
		explicit InstantSpell(LuaScriptInterface* interface) : TalkAction(interface) {}

		bool configureEvent(const pugi::xml_node& node) override;
		bool loadFunction(const pugi::xml_attribute& attr) override;

		virtual bool playerCastInstant(Player* player, std::string& param);

		bool castSpell(Creature* creature) override;
		bool castSpell(Creature* creature, Creature* target) override;

		//scripting
		bool executeCastSpell(Creature* creature, const LuaVariant& var);

		bool isInstant() const final {
			return true;
		}
		bool getHasParam() const {
			return hasParam;
		}
		bool getHasPlayerNameParam() const {
			return hasPlayerNameParam;
		}
		bool canCast(const Player* player) const;
		bool canThrowSpell(const Creature* creature, const Creature* target) const;

	protected:
		std::string getScriptEventName() const override;

		static InstantSpellFunction HouseGuestList;
		static InstantSpellFunction HouseSubOwnerList;
		static InstantSpellFunction HouseDoorList;
		static InstantSpellFunction HouseKick;
		static InstantSpellFunction SearchPlayer;
		static InstantSpellFunction SummonMonster;
		static InstantSpellFunction Levitate;
		static InstantSpellFunction Illusion;

		static House* getHouseFromPos(Creature* creature);

		bool internalCastSpell(Creature* creature, const LuaVariant& var);

		InstantSpellFunction* function = nullptr;

		bool needDirection = false;
		bool hasParam = false;
		bool hasPlayerNameParam = false;
		bool checkLineOfSight = true;
		bool casterTargetOrDirection = false;
};

class ConjureSpell final : public InstantSpell
{
	public:
		explicit ConjureSpell(LuaScriptInterface* interface) : InstantSpell(interface) {}

		bool configureEvent(const pugi::xml_node& node) final;
		bool loadFunction(const pugi::xml_attribute& attr) final;

		bool playerCastInstant(Player* player, std::string& param) final;

		bool castSpell(Creature*) final {
			return false;
		}
		bool castSpell(Creature*, Creature*) final {
			return false;
		}

	protected:
		std::string getScriptEventName() const final;

		bool conjureItem(Creature* creature) const;

		uint32_t conjureId = 0;
		uint32_t conjureCount = 1;
		uint32_t reagentId = 0;
};

class RuneSpell final : public Action, public Spell
{
	public:
		explicit RuneSpell(LuaScriptInterface* interface) : Action(interface) {}

		bool configureEvent(const pugi::xml_node& node) final;
		bool loadFunction(const pugi::xml_attribute& attr) final;

		ReturnValue canExecuteAction(const Player* player, const Position& toPos) final;
		bool hasOwnErrorHandler() final {
			return true;
		}
		Thing* getTarget(Player*, Creature* targetCreature, const Position&, uint8_t) const final {
			return targetCreature;
		}

		bool executeUse(Player* player, Item* item, const Position& fromPosition, Thing* target, const Position& toPosition, bool isHotkey) final;

		bool castSpell(Creature* creature) final;
		bool castSpell(Creature* creature, Creature* target) final;

		//scripting
		bool executeCastSpell(Creature* creature, const LuaVariant& var, bool isHotkey);

		bool isInstant() const final {
			return false;
		}
		uint16_t getRuneItemId() const {
			return runeId;
		}

	protected:
		std::string getScriptEventName() const final;

		static RuneSpellFunction Illusion;
		static RuneSpellFunction Convince;

		bool internalCastSpell(Creature* creature, const LuaVariant& var, bool isHotkey);

		RuneSpellFunction* runeFunction = nullptr;
		uint16_t runeId = 0;
		bool hasCharges = true;
};

#endif
