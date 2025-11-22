// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_WEAPONS_H
#define FS_WEAPONS_H

#include "baseevents.h"
#include "combat.h"
#include "const.h"
#include "luascript.h"
#include "vocation.h"

class Weapon;

using Weapon_ptr = std::unique_ptr<Weapon>;

extern Vocations g_vocations;

class Weapons final : public BaseEvents
{
public:
	Weapons();
	~Weapons();

	// non-copyable
	Weapons(const Weapons&) = delete;
	Weapons& operator=(const Weapons&) = delete;

	void loadDefaults();
	const Weapon* getWeapon(const std::shared_ptr<const Item>& item) const;

	static int32_t getMaxMeleeDamage(int32_t attackSkill, int32_t attackValue);
	static int32_t getMaxWeaponDamage(uint32_t level, int32_t attackSkill, int32_t attackValue, float attackFactor);

	bool registerLuaEvent(Weapon* event);
	void clear(bool fromLua) override final;

private:
	LuaScriptInterface& getScriptInterface() override;
	std::string_view getScriptBaseName() const override { return "weapons"; }
	Event_ptr getEvent(const std::string& nodeName) override;
	bool registerEvent(Event_ptr event, const pugi::xml_node& node) override;

	std::map<uint32_t, Weapon*> weapons;

	LuaScriptInterface scriptInterface{"Weapon Interface"};
};

class Weapon : public Event
{
public:
	explicit Weapon(LuaScriptInterface* interface) : Event(interface) {}

	bool configureEvent(const pugi::xml_node& node) override;
	bool loadFunction(const pugi::xml_attribute&, bool) final { return true; }
	virtual void configureWeapon(const ItemType& it);
	virtual bool interruptSwing() const { return false; }

	int32_t playerWeaponCheck(const std::shared_ptr<const Player>& player,
	                          const std::shared_ptr<const Creature>& target, uint8_t shootRange) const;
	bool ammoCheck(const std::shared_ptr<const Player>& player) const;
	static bool useFist(const std::shared_ptr<Player>& player, const std::shared_ptr<Creature>& target);
	virtual bool useWeapon(const std::shared_ptr<Player>& player, const std::shared_ptr<Item>& item,
	                       const std::shared_ptr<Creature>& target) const;

	virtual int32_t getWeaponDamage(const std::shared_ptr<const Player>& player,
	                                const std::shared_ptr<const Creature>& target,
	                                const std::shared_ptr<const Item>& item, bool maxDamage = false) const = 0;
	virtual int32_t getElementDamage(const std::shared_ptr<const Player>& player,
	                                 const std::shared_ptr<const Creature>& target,
	                                 const std::shared_ptr<const Item>& item) const = 0;
	virtual CombatType_t getElementType() const = 0;

	uint16_t getID() const { return id; }
	void setID(uint16_t newId) { id = newId; }

	uint32_t getReqLevel() const { return level; }
	void setRequiredLevel(uint32_t reqlvl) { level = reqlvl; }

	uint32_t getReqMagLv() const { return magLevel; }
	void setRequiredMagLevel(uint32_t reqlvl) { magLevel = reqlvl; }

	bool isPremium() const { return premium; }
	void setNeedPremium(bool prem) { premium = prem; }

	bool isWieldedUnproperly() const { return wieldUnproperly; }
	void setWieldUnproperly(bool unproperly) { wieldUnproperly = unproperly; }

	uint32_t getMana() const { return mana; }
	void setMana(uint32_t m) { mana = m; }

	uint32_t getManaPercent() const { return manaPercent; }
	void setManaPercent(uint32_t m) { manaPercent = m; }

	int32_t getHealth() const { return health; }
	void setHealth(int32_t h) { health = h; }

	uint32_t getHealthPercent() const { return healthPercent; }
	void setHealthPercent(uint32_t m) { healthPercent = m; }

	uint32_t getSoul() const { return soul; }
	void setSoul(uint32_t s) { soul = s; }

	uint8_t getBreakChance() const { return breakChance; }
	void setBreakChance(uint8_t b) { breakChance = b; }

	bool isEnabled() const { return enabled; }
	void setIsEnabled(bool e) { enabled = e; }

	uint32_t getWieldInfo() const { return wieldInfo; }
	void setWieldInfo(uint32_t info) { wieldInfo |= info; }

	const auto& getVocationWeaponSet() const { return vocationWeaponSet; }
	void addVocationWeaponSet(const std::string& vocationName)
	{
		int32_t vocationId = g_vocations.getVocationId(vocationName);
		if (vocationId != -1) {
			vocationWeaponSet.insert(vocationId);
		}
	}
	// If the set is empty, it is considered to be for all vocations.
	bool hasVocationWeaponSet(uint16_t vocationId) const
	{
		return vocationWeaponSet.empty() || vocationWeaponSet.find(vocationId) != vocationWeaponSet.end();
	}

	const std::string& getVocationString() const { return vocationString; }
	void setVocationString(const std::string& str) { vocationString = str; }

	WeaponAction_t action = WEAPONACTION_NONE;
	CombatParams params;
	WeaponType_t weaponType;
	std::unordered_set<uint16_t> vocationWeaponSet;

protected:
	void internalUseWeapon(const std::shared_ptr<Player>& player, const std::shared_ptr<Item>& item,
	                       const std::shared_ptr<Creature>& target, int32_t damageModifier) const;
	void internalUseWeapon(const std::shared_ptr<Player>& player, const std::shared_ptr<Item>& item,
	                       const std::shared_ptr<Tile>& tile) const;

	uint16_t id = 0;

private:
	virtual bool getSkillType(const std::shared_ptr<const Player>&, const std::shared_ptr<const Item>&, skills_t&,
	                          uint32_t&) const
	{
		return false;
	}

	uint32_t getManaCost(const std::shared_ptr<const Player>& player) const;
	int32_t getHealthCost(const std::shared_ptr<const Player>& player) const;

	uint32_t level = 0;
	uint32_t magLevel = 0;
	uint32_t mana = 0;
	uint32_t manaPercent = 0;
	uint32_t health = 0;
	uint32_t healthPercent = 0;
	uint32_t soul = 0;
	uint32_t wieldInfo = WIELDINFO_NONE;
	uint8_t breakChance = 0;
	bool enabled = true;
	bool premium = false;
	bool wieldUnproperly = false;
	std::string vocationString = "";

	std::string_view getScriptEventName() const override final { return "onUseWeapon"; }

	bool executeUseWeapon(const std::shared_ptr<Player>& player, const LuaVariant& var) const;
	void onUsedWeapon(const std::shared_ptr<Player>& player, const std::shared_ptr<Item>& item,
	                  const std::shared_ptr<Tile>& destTile) const;

	static void decrementItemCount(const std::shared_ptr<Item>& item);

	friend class Combat;
};

class WeaponMelee final : public Weapon
{
public:
	explicit WeaponMelee(LuaScriptInterface* interface);

	void configureWeapon(const ItemType& it) override;

	bool useWeapon(const std::shared_ptr<Player>& player, const std::shared_ptr<Item>& item,
	               const std::shared_ptr<Creature>& target) const override;

	int32_t getWeaponDamage(const std::shared_ptr<const Player>& player, const std::shared_ptr<const Creature>& target,
	                        const std::shared_ptr<const Item>& item, bool maxDamage = false) const override;
	int32_t getElementDamage(const std::shared_ptr<const Player>& player, const std::shared_ptr<const Creature>& target,
	                         const std::shared_ptr<const Item>& item) const override;
	CombatType_t getElementType() const override { return elementType; }

private:
	bool getSkillType(const std::shared_ptr<const Player>& player, const std::shared_ptr<const Item>& item,
	                  skills_t& skill, uint32_t& skillpoint) const override;

	CombatType_t elementType = COMBAT_NONE;
	uint16_t elementDamage = 0;
};

class WeaponDistance final : public Weapon
{
public:
	explicit WeaponDistance(LuaScriptInterface* interface);

	void configureWeapon(const ItemType& it) override;
	bool interruptSwing() const override { return true; }

	bool useWeapon(const std::shared_ptr<Player>& player, const std::shared_ptr<Item>& item,
	               const std::shared_ptr<Creature>& target) const override;

	int32_t getWeaponDamage(const std::shared_ptr<const Player>& player, const std::shared_ptr<const Creature>& target,
	                        const std::shared_ptr<const Item>& item, bool maxDamage = false) const override;
	int32_t getElementDamage(const std::shared_ptr<const Player>& player, const std::shared_ptr<const Creature>& target,
	                         const std::shared_ptr<const Item>& item) const override;
	CombatType_t getElementType() const override { return elementType; }

private:
	bool getSkillType(const std::shared_ptr<const Player>& player, const std::shared_ptr<const Item>& item,
	                  skills_t& skill, uint32_t& skillpoint) const override;

	CombatType_t elementType = COMBAT_NONE;
	uint16_t elementDamage = 0;
};

class WeaponWand final : public Weapon
{
public:
	explicit WeaponWand(LuaScriptInterface* interface) : Weapon(interface) {}

	bool configureEvent(const pugi::xml_node& node) override;
	void configureWeapon(const ItemType& it) override;

	int32_t getWeaponDamage(const std::shared_ptr<const Player>& player, const std::shared_ptr<const Creature>& target,
	                        const std::shared_ptr<const Item>& item, bool maxDamage = false) const override;
	int32_t getElementDamage(const std::shared_ptr<const Player>&, const std::shared_ptr<const Creature>&,
	                         const std::shared_ptr<const Item>&) const override
	{
		return 0;
	}
	CombatType_t getElementType() const override { return COMBAT_NONE; }

	void setMinChange(int32_t change) { minChange = change; }

	void setMaxChange(int32_t change) { maxChange = change; }

private:
	bool getSkillType(const std::shared_ptr<const Player>&, const std::shared_ptr<const Item>&, skills_t&,
	                  uint32_t&) const override
	{
		return false;
	}

	int32_t minChange = 0;
	int32_t maxChange = 0;
};

#endif // FS_WEAPONS_H
