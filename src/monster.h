// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_MONSTER_H
#define FS_MONSTER_H

#include "creature.h"
#include "monsters.h"
#include "position.h"

class Item;
class Spawn;
class Tile;

using CreatureHashSet = std::unordered_set<std::shared_ptr<Creature>>;
using CreatureList = std::list<std::shared_ptr<Creature>>;
using MonsterIconHashMap = std::unordered_map<MonsterIcon_t, uint16_t>;

enum TargetSearchType_t
{
	TARGETSEARCH_DEFAULT,
	TARGETSEARCH_RANDOM,
	TARGETSEARCH_ATTACKRANGE,
	TARGETSEARCH_NEAREST,
};

class Monster final : public Creature
{
public:
	static std::shared_ptr<Monster> createMonster(const std::string& name);

	static int32_t despawnRange;
	static int32_t despawnRadius;

	explicit Monster(MonsterType* mType);
	~Monster();

	// non-copyable
	Monster(const Monster&) = delete;
	Monster& operator=(const Monster&) = delete;

	std::shared_ptr<Monster> getMonster() override
	{
		assert(std::dynamic_pointer_cast<Monster>(getCreature()) != nullptr);
		return std::static_pointer_cast<Monster>(getCreature());
	}
	std::shared_ptr<const Monster> getMonster() const override
	{
		assert(std::dynamic_pointer_cast<const Monster>(getCreature()) != nullptr);
		return std::static_pointer_cast<const Monster>(getCreature());
	}

	void setID() override
	{
		if (id == 0) {
			id = monsterAutoID++;
		}
	}

	void addList() override;
	void removeList() override;

	const std::string& getName() const override;
	void setName(const std::string& name);

	const std::string& getNameDescription() const override;
	void setNameDescription(const std::string& nameDescription) { this->nameDescription = nameDescription; };

	std::string getDescription(int32_t) const override { return nameDescription + '.'; }

	CreatureType_t getType() const override { return CREATURETYPE_MONSTER; }

	const Position& getMasterPos() const { return masterPos; }
	void setMasterPos(Position pos) { masterPos = pos; }

	RaceType_t getRace() const override { return mType->info.race; }
	int32_t getArmor() const override { return mType->info.armor; }
	int32_t getDefense() const override { return mType->info.defense; }
	bool isPushable() const override { return mType->info.pushable && baseSpeed != 0; }
	bool isAttackable() const override { return mType->info.isAttackable; }

	bool canPushItems() const;
	bool canPushCreatures() const { return mType->info.canPushCreatures; }
	bool isHostile() const { return mType->info.isHostile; }
	bool canSee(const Position& pos) const override;
	bool canSeeInvisibility() const override { return isImmune(CONDITION_INVISIBLE); }
	uint32_t getManaCost() const { return mType->info.manaCost; }
	void setSpawn(Spawn* spawn) { this->spawn = spawn; }
	bool canWalkOnFieldType(CombatType_t combatType) const;

	void onAttackedCreatureDisappear(bool isLogout) override;

	void onCreatureAppear(std::shared_ptr<Creature> creature, bool isLogin) override;
	void onRemoveCreature(std::shared_ptr<Creature> creature, bool isLogout) override;
	void onCreatureMove(std::shared_ptr<Creature> creature, std::shared_ptr<const Tile> newTile, const Position& newPos,
	                    std::shared_ptr<const Tile> oldTile, const Position& oldPos, bool teleport) override;
	void onCreatureSay(std::shared_ptr<Creature> creature, SpeakClasses type, const std::string& text) override;

	void drainHealth(std::shared_ptr<Creature> attacker, int32_t damage) override;
	void changeHealth(int32_t healthChange, bool sendHealthChange = true) override;

	bool isWalkingToSpawn() const { return walkingToSpawn; }
	bool walkToSpawn();
	void onWalk() override;
	void onWalkComplete() override;
	bool getNextStep(Direction& direction, uint32_t& flags) override;
	void goToFollowCreature() override;
	void onFollowCreatureComplete();

	void onThink(uint32_t interval) override;

	bool challengeCreature(std::shared_ptr<Creature> creature, bool force = false) override;

	void setNormalCreatureLight() override;
	bool getCombatValues(int32_t& min, int32_t& max) override;

	void doAttacking(uint32_t interval) override;
	bool hasExtraSwing() override { return lastMeleeAttack == 0; }

	bool searchTarget(TargetSearchType_t searchType = TARGETSEARCH_DEFAULT);
	bool selectTarget(std::shared_ptr<Creature> creature);

	const CreatureList& getTargetList() const { return targetList; }
	const CreatureHashSet& getFriendList() const { return friendList; }

	bool isTarget(std::shared_ptr<const Creature> creature) const;
	bool isFleeing() const
	{
		return !isSummon() && getHealth() <= mType->info.runAwayHealth && challengeFocusDuration <= 0;
	}

	bool getDistanceStep(const Position& targetPos, Direction& direction, bool flee = false);
	bool isTargetNearby() const { return stepDuration >= 1; }
	bool isIgnoringFieldDamage() const { return ignoreFieldDamage; }

	BlockType_t blockHit(std::shared_ptr<Creature> attacker, CombatType_t combatType, int32_t& damage,
	                     bool checkDefense = false, bool checkArmor = false, bool field = false,
	                     bool ignoreResistances = false) override;

	// monster icons
	MonsterIconHashMap& getSpecialIcons() { return monsterIcons; }
	const MonsterIconHashMap& getSpecialIcons() const { return monsterIcons; }

	static uint32_t monsterAutoID;

private:
	CreatureHashSet friendList;
	CreatureList targetList;
	MonsterIconHashMap monsterIcons;

	std::string name;
	std::string nameDescription;

	MonsterType* mType;
	Spawn* spawn = nullptr;

	int64_t lastMeleeAttack = 0;

	uint32_t attackTicks = 0;
	uint32_t targetChangeTicks = 0;
	uint32_t defenseTicks = 0;
	uint32_t yellTicks = 0;
	int32_t minCombatValue = 0;
	int32_t maxCombatValue = 0;
	int32_t targetChangeCooldown = 0;
	int32_t challengeFocusDuration = 0;
	int32_t stepDuration = 0;

	Position masterPos;

	bool ignoreFieldDamage = false;
	bool isIdle = true;
	bool isMasterInRange = false;
	bool randomStepping = false;
	bool walkingToSpawn = false;

	void onCreatureEnter(std::shared_ptr<Creature> creature);
	void onCreatureLeave(std::shared_ptr<Creature> creature);
	void onCreatureFound(std::shared_ptr<Creature> creature, bool pushFront = false);

	void updateLookDirection();

	void addFriend(std::shared_ptr<Creature> creature);
	void removeFriend(std::shared_ptr<Creature> creature);
	void addTarget(std::shared_ptr<Creature> creature, bool pushFront = false);
	void removeTarget(std::shared_ptr<Creature> creature);

	void updateTargetList();
	void clearTargetList();
	void clearFriendList();

	void death(std::shared_ptr<Creature> lastHitCreature) override;
	std::shared_ptr<Item> getCorpse(std::shared_ptr<Creature> lastHitCreature,
	                                std::shared_ptr<Creature> mostDamageCreature) override;

	void setIdle(bool idle);
	void updateIdleStatus();
	bool getIdleStatus() const { return isIdle; }

	void onAddCondition(ConditionType_t type) override;
	void onEndCondition(ConditionType_t type) override;

	bool canUseAttack(const Position& pos, std::shared_ptr<const Creature> target) const;
	bool canUseSpell(const Position& pos, const Position& targetPos, const spellBlock_t& sb, uint32_t interval,
	                 bool& inRange, bool& resetTicks);
	bool getRandomStep(const Position& creaturePos, Direction& direction) const;
	bool getDanceStep(const Position& creaturePos, Direction& direction, bool keepAttack = true,
	                  bool keepDistance = true);
	bool isInSpawnRange(const Position& pos) const;
	bool canWalkTo(Position pos, Direction direction) const;

	static bool pushItem(std::shared_ptr<Item> item);
	static void pushItems(std::shared_ptr<Tile> tile);
	static bool pushCreature(std::shared_ptr<Creature> creature);
	static void pushCreatures(std::shared_ptr<Tile> tile);

	void onThinkTarget(uint32_t interval);
	void onThinkYell(uint32_t interval);
	void onThinkDefense(uint32_t interval);

	bool isFriend(std::shared_ptr<const Creature> creature) const;
	bool isOpponent(std::shared_ptr<const Creature> creature) const;

	uint64_t getLostExperience() const override { return skillLoss ? mType->info.experience : 0; }
	uint16_t getLookCorpse() const override { return mType->info.lookcorpse; }
	void dropLoot(std::shared_ptr<Container> corpse, std::shared_ptr<Creature> lastHitCreature) override;
	uint32_t getDamageImmunities() const override { return mType->info.damageImmunities; }
	uint32_t getConditionImmunities() const override { return mType->info.conditionImmunities; }
	void getPathSearchParams(std::shared_ptr<const Creature> creature, FindPathParams& fpp) const override;

	friend class LuaScriptInterface;
};

#endif // FS_MONSTER_H
