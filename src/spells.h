// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_SPELLS_H
#define FS_SPELLS_H

#include "actions.h"
#include "baseevents.h"
#include "creature.h"
#include "luascript.h"
#include "talkaction.h"
#include "vocation.h"

class InstantSpell;
class RuneSpell;
class Spell;

using InstantSpell_ptr = std::unique_ptr<InstantSpell>;
using RuneSpell_ptr = std::unique_ptr<RuneSpell>;

extern Vocations g_vocations;

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

	TalkActionResult_t playerSaySpell(Player* player, std::string& words);

	static Position getCasterPosition(Creature* creature, Direction dir);
	std::string_view getScriptBaseName() const override { return "spells"; }

	const std::map<std::string, InstantSpell>& getInstantSpells() const { return instants; };

	void clearMaps(bool fromLua);
	void clear(bool fromLua) override final;
	bool registerInstantLuaEvent(InstantSpell* event);
	bool registerRuneLuaEvent(RuneSpell* event);

private:
	LuaScriptInterface& getScriptInterface() override;
	Event_ptr getEvent(const std::string& nodeName) override;
	bool registerEvent(Event_ptr event, const pugi::xml_node& node) override;

	std::map<uint16_t, RuneSpell> runes;
	std::map<std::string, InstantSpell> instants;

	friend class CombatSpell;
	LuaScriptInterface scriptInterface{"Spell Interface"};
};

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
	CombatSpell(Combat_ptr combat, bool needTarget, bool needDirection);

	// non-copyable
	CombatSpell(const CombatSpell&) = delete;
	CombatSpell& operator=(const CombatSpell&) = delete;

	bool castSpell(Creature* creature) override;
	bool castSpell(Creature* creature, Creature* target) override;
	bool configureEvent(const pugi::xml_node&) override { return true; }

	// scripting
	bool executeCastSpell(Creature* creature, const LuaVariant& var);

	bool loadScriptCombat();
	Combat_ptr getCombat() { return combat; }

private:
	std::string_view getScriptEventName() const override { return "onCastSpell"; }

	Combat_ptr combat;

	bool needDirection;
	bool needTarget;
};

class Spell : public BaseSpell
{
public:
	Spell() = default;

	bool configureSpell(const pugi::xml_node& node);
	const std::string& getName() const { return name; }
	void setName(std::string n) { name = n; }
	uint8_t getId() const { return spellId; }
	void setId(uint8_t id) { spellId = id; }

	void postCastSpell(Player* player, bool finishedCast = true, bool payCost = true) const;
	static void postCastSpell(Player* player, uint32_t manaCost, uint32_t soulCost);

	uint32_t getManaCost(const Player* player) const;
	uint32_t getSoulCost() const { return soul; }
	void setSoulCost(uint32_t s) { soul = s; }
	uint32_t getLevel() const { return level; }
	void setLevel(uint32_t lvl) { level = lvl; }
	uint32_t getMagicLevel() const { return magLevel; }
	void setMagicLevel(uint32_t lvl) { magLevel = lvl; }
	uint32_t getMana() const { return mana; }
	void setMana(uint32_t m) { mana = m; }
	uint32_t getManaPercent() const { return manaPercent; }
	void setManaPercent(uint32_t m) { manaPercent = m; }
	bool isPremium() const { return premium; }
	void setPremium(bool p) { premium = p; }
	bool isEnabled() const { return enabled; }
	void setEnabled(bool e) { enabled = e; }

	virtual bool isInstant() const = 0;
	bool isLearnable() const { return learnable; }
	void setLearnable(bool l) { learnable = l; }

	const auto& getVocationSpellMap() const { return vocationSpellMap; }
	void addVocationSpellMap(std::string_view vocationName, bool showInDescription)
	{
		int32_t vocationId = g_vocations.getVocationId(vocationName);
		if (vocationId != -1) {
			vocationSpellMap[vocationId] = showInDescription;
		}
	}
	// If the set is empty, it is considered to be for all vocations.
	bool hasVocationSpellMap(uint16_t vocationId) const
	{
		return vocationSpellMap.empty() || vocationSpellMap.find(vocationId) != vocationSpellMap.end();
	}

	SpellGroup_t getGroup() const { return group; }
	void setGroup(SpellGroup_t g)
	{
		group = g;
		if (group == SPELLGROUP_NONE) {
			groupCooldown = 0;
		}
	}
	SpellGroup_t getSecondaryGroup() const { return secondaryGroup; }
	void setSecondaryGroup(SpellGroup_t g) { secondaryGroup = g; }

	uint32_t getCooldown() const { return cooldown; }
	void setCooldown(uint32_t cd) { cooldown = cd; }
	uint32_t getSecondaryCooldown() const { return secondaryGroupCooldown; }
	void setSecondaryCooldown(uint32_t cd) { secondaryGroupCooldown = cd; }
	uint32_t getGroupCooldown() const { return groupCooldown; }
	void setGroupCooldown(uint32_t cd) { groupCooldown = cd; }

	int32_t getRange() const { return range; }
	void setRange(int32_t r) { range = r; }

	bool getNeedTarget() const { return needTarget; }
	void setNeedTarget(bool n) { needTarget = n; }
	bool getNeedWeapon() const { return needWeapon; }
	void setNeedWeapon(bool n) { needWeapon = n; }
	bool getNeedLearn() const { return learnable; }
	void setNeedLearn(bool n) { learnable = n; }
	bool getSelfTarget() const { return selfTarget; }
	void setSelfTarget(bool s) { selfTarget = s; }
	bool getBlockingSolid() const { return blockingSolid; }
	void setBlockingSolid(bool b) { blockingSolid = b; }
	bool getBlockingCreature() const { return blockingCreature; }
	void setBlockingCreature(bool b) { blockingCreature = b; }
	bool getAggressive() const { return aggressive; }
	void setAggressive(bool a) { aggressive = a; }
	bool getPzLock() const { return pzLock; }
	void setPzLock(bool pzLock) { this->pzLock = pzLock; }

	SpellType_t spellType = SPELL_UNDEFINED;

protected:
	bool playerSpellCheck(Player* player) const;
	bool playerInstantSpellCheck(Player* player, const Position& toPos);
	bool playerRuneSpellCheck(Player* player, const Position& toPos);
	void addCooldowns(Player* player) const;

	std::map<uint16_t, bool> vocationSpellMap;

	SpellGroup_t group = SPELLGROUP_NONE;
	SpellGroup_t secondaryGroup = SPELLGROUP_NONE;

	uint32_t cooldown = 1000;
	uint32_t groupCooldown = 1000;
	uint32_t secondaryGroupCooldown = 0;
	uint32_t level = 0;
	uint32_t magLevel = 0;
	int32_t range = -1;

	uint8_t spellId = 0;

	bool selfTarget = false;
	bool needTarget = false;

private:
	uint32_t mana = 0;
	uint32_t manaPercent = 0;
	uint32_t soul = 0;

	bool needWeapon = false;
	bool blockingSolid = false;
	bool blockingCreature = false;
	bool aggressive = true;
	bool pzLock = false;
	bool learnable = false;
	bool enabled = true;
	bool premium = false;

	std::string name;
};

class InstantSpell final : public TalkAction, public Spell
{
public:
	explicit InstantSpell(LuaScriptInterface* interface) : TalkAction(interface) {}

	bool configureEvent(const pugi::xml_node& node) override;

	virtual bool playerCastInstant(Player* player, std::string& param);

	bool castSpell(Creature* creature) override;
	bool castSpell(Creature* creature, Creature* target) override;

	// scripting
	bool executeCastSpell(Creature* creature, const LuaVariant& var);

	bool isInstant() const override { return true; }
	bool getHasParam() const { return hasParam; }
	void setHasParam(bool p) { hasParam = p; }
	bool getHasPlayerNameParam() const { return hasPlayerNameParam; }
	void setHasPlayerNameParam(bool p) { hasPlayerNameParam = p; }
	bool getNeedDirection() const { return needDirection; }
	void setNeedDirection(bool n) { needDirection = n; }
	bool getNeedCasterTargetOrDirection() const { return casterTargetOrDirection; }
	void setNeedCasterTargetOrDirection(bool d) { casterTargetOrDirection = d; }
	bool getBlockWalls() const { return checkLineOfSight; }
	void setBlockWalls(bool w) { checkLineOfSight = w; }
	bool canCast(const Player* player) const;
	bool canThrowSpell(const Creature* creature, const Creature* target) const;

private:
	std::string_view getScriptEventName() const override { return "onCastSpell"; }

	bool internalCastSpell(Creature* creature, const LuaVariant& var);

	bool needDirection = false;
	bool hasParam = false;
	bool hasPlayerNameParam = false;
	bool checkLineOfSight = true;
	bool casterTargetOrDirection = false;
};

class RuneSpell final : public Action, public Spell
{
public:
	explicit RuneSpell(LuaScriptInterface* interface) : Action(interface) {}

	bool configureEvent(const pugi::xml_node& node) override;

	ReturnValue canExecuteAction(const Player* player, const Position& toPos) override;
	bool hasOwnErrorHandler() override { return true; }
	Thing* getTarget(Player*, Creature* targetCreature, const Position&, uint8_t) const override
	{
		return targetCreature;
	}

	bool executeUse(Player* player, Item* item, const Position& fromPosition, Thing* target, const Position& toPosition,
	                bool isHotkey) override;

	bool castSpell(Creature* creature) override;
	bool castSpell(Creature* creature, Creature* target) override;

	// scripting
	bool executeCastSpell(Creature* creature, const LuaVariant& var, bool isHotkey);

	bool isInstant() const override { return false; }
	uint16_t getRuneItemId() const { return runeId; }
	void setRuneItemId(uint16_t i) { runeId = i; }
	uint32_t getCharges() const { return charges; }
	void setCharges(uint32_t c)
	{
		if (c > 0) {
			hasCharges = true;
		}
		charges = c;
	}

private:
	std::string_view getScriptEventName() const override { return "onCastSpell"; }

	bool internalCastSpell(Creature* creature, const LuaVariant& var, bool isHotkey);

	uint16_t runeId = 0;
	uint32_t charges = 0;
	bool hasCharges = false;
};

#endif // FS_SPELLS_H
