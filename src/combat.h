// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_COMBAT_H
#define FS_COMBAT_H

#include "baseevents.h"
#include "condition.h"
#include "item.h"
#include "map.h"
#include "tools.h"

class Creature;
class MatrixArea;
class Player;
struct Position;
class Tile;

// for luascript callback
class ValueCallback final : public CallBack
{
public:
	explicit ValueCallback(formulaType_t type) : type(type) {}
	void getMinMaxValues(Player* player, CombatDamage& damage) const;

private:
	formulaType_t type;
};

class TileCallback final : public CallBack
{
public:
	void onTileCombat(Creature* creature, Tile* tile) const;
};

class TargetCallback final : public CallBack
{
public:
	void onTargetCombat(Creature* creature, Creature* target) const;
};

struct CombatParams
{
	std::forward_list<std::unique_ptr<const Condition>> conditionList = {};

	std::unique_ptr<ValueCallback> valueCallback = nullptr;
	std::unique_ptr<TileCallback> tileCallback = nullptr;
	std::unique_ptr<TargetCallback> targetCallback = nullptr;

	uint16_t itemId = 0;

	ConditionType_t dispelType = CONDITION_NONE;
	CombatType_t combatType = COMBAT_NONE;
	CombatOrigin origin = ORIGIN_SPELL;

	uint8_t impactEffect = CONST_ME_NONE;
	uint8_t distanceEffect = CONST_ANI_NONE;

	bool blockedByArmor = false;
	bool blockedByShield = false;
	bool targetCasterOrTopMost = false;
	bool aggressive = true;
	bool useCharges = false;
	bool ignoreResistances = false;
};

class AreaCombat
{
public:
	void setupArea(const std::vector<uint32_t>& vec, uint32_t rows);
	void setupArea(int32_t length, int32_t spread);
	void setupArea(int32_t radius);
	void setupAreaRing(int32_t ring);
	void setupExtArea(const std::vector<uint32_t>& vec, uint32_t rows);
	const MatrixArea& getArea(const Position& centerPos, const Position& targetPos) const;

private:
	std::vector<MatrixArea> areas;
	bool hasExtArea = false;
};

class Combat
{
public:
	Combat() = default;

	// non-copyable
	Combat(const Combat&) = delete;
	Combat& operator=(const Combat&) = delete;

	static bool isInPvpZone(const Creature* attacker, const Creature* target);
	static bool isProtected(const Player* attacker, const Player* target);
	static bool isPlayerCombat(const Creature* target);
	static CombatType_t ConditionToDamageType(ConditionType_t type);
	static ConditionType_t DamageToConditionType(CombatType_t type);
	static ReturnValue canTargetCreature(Player* attacker, Creature* target);
	static ReturnValue canDoCombat(Creature* caster, Tile* tile, bool aggressive);
	static ReturnValue canDoCombat(Creature* attacker, Creature* target);
	static void postCombatEffects(Creature* caster, const Position& pos, const CombatParams& params);

	static void addDistanceEffect(Creature* caster, const Position& fromPos, const Position& toPos, uint8_t effect);

	void doCombat(Creature* caster, Creature* target) const;
	void doCombat(Creature* caster, const Position& position) const;

	static void doTargetCombat(Creature* caster, Creature* target, CombatDamage& damage, const CombatParams& params);
	static void doAreaCombat(Creature* caster, const Position& position, const AreaCombat* area, CombatDamage& damage,
	                         const CombatParams& params);

	bool setCallback(CallBackParam_t key);
	CallBack* getCallback(CallBackParam_t key);

	bool setParam(CombatParam_t param, uint32_t value);
	int32_t getParam(CombatParam_t param);

	void setArea(AreaCombat* area);
	bool hasArea() const { return area != nullptr; }
	void addCondition(const Condition* condition) { params.conditionList.emplace_front(condition); }
	void clearConditions() { params.conditionList.clear(); }
	void setPlayerCombatValues(formulaType_t formulaType, double mina, double minb, double maxa, double maxb);
	void postCombatEffects(Creature* caster, const Position& pos) const { postCombatEffects(caster, pos, params); }

	void setOrigin(CombatOrigin origin) { params.origin = origin; }

private:
	static void combatTileEffects(const SpectatorVec& spectators, Creature* caster, Tile* tile,
	                              const CombatParams& params);
	CombatDamage getCombatDamage(Creature* creature, Creature* target) const;

	// configurable
	CombatParams params;

	// formula variables
	formulaType_t formulaType = COMBAT_FORMULA_UNDEFINED;
	double mina = 0.0;
	double minb = 0.0;
	double maxa = 0.0;
	double maxb = 0.0;

	std::unique_ptr<AreaCombat> area;
};

class MagicField final : public Item
{
public:
	explicit MagicField(uint16_t type) : Item(type), createTime(OTSYS_TIME()) {}

	MagicField* getMagicField() override { return this; }
	const MagicField* getMagicField() const override { return this; }

	bool isReplaceable() const { return Item::items[getID()].replaceable; }
	CombatType_t getCombatType() const
	{
		const ItemType& it = items[getID()];
		return it.combatType;
	}
	int32_t getDamage() const
	{
		const ItemType& it = items[getID()];
		if (it.conditionDamage) {
			return it.conditionDamage->getTotalDamage();
		}
		return 0;
	}
	void onStepInField(Creature* creature);

private:
	int64_t createTime;
};

#endif // FS_COMBAT_H
