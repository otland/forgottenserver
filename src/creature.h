// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_CREATURE_H
#define FS_CREATURE_H

#include "const.h"
#include "creatureevent.h"
#include "enums.h"
#include "map.h"
#include "position.h"
#include "tile.h"

class Condition;
class Container;
class Item;
class Monster;
class Npc;
class Player;

using ConditionList = std::list<Condition*>;
using CreatureEventList = std::list<CreatureEvent*>;
using CreatureIconHashMap = std::unordered_map<CreatureIcon_t, uint16_t>;

enum slots_t : uint8_t
{
	CONST_SLOT_WHEREEVER = 0,
	CONST_SLOT_HEAD = 1,
	CONST_SLOT_NECKLACE = 2,
	CONST_SLOT_BACKPACK = 3,
	CONST_SLOT_ARMOR = 4,
	CONST_SLOT_RIGHT = 5,
	CONST_SLOT_LEFT = 6,
	CONST_SLOT_LEGS = 7,
	CONST_SLOT_FEET = 8,
	CONST_SLOT_RING = 9,
	CONST_SLOT_AMMO = 10,
	CONST_SLOT_STORE_INBOX = 11,

	CONST_SLOT_FIRST = CONST_SLOT_HEAD,
	CONST_SLOT_LAST = CONST_SLOT_AMMO,
};

struct FindPathParams
{
	bool fullPathSearch = true;
	bool clearSight = true;
	bool allowDiagonal = true;
	bool keepDistance = false;
	bool summonTargetMaster = false;
	int32_t maxSearchDist = 0;
	int32_t minTargetDist = -1;
	int32_t maxTargetDist = -1;
};

static constexpr int32_t EVENT_CREATURECOUNT = 10;
static constexpr int32_t EVENT_CREATURE_THINK_INTERVAL = 1000;
static constexpr int32_t EVENT_CHECK_CREATURE_INTERVAL = (EVENT_CREATURE_THINK_INTERVAL / EVENT_CREATURECOUNT);

static constexpr uint32_t CREATURE_ID_MIN = 0x10000000;
static constexpr uint32_t CREATURE_ID_MAX = std::numeric_limits<uint32_t>::max();

class FrozenPathingConditionCall
{
public:
	explicit FrozenPathingConditionCall(Position targetPos) : targetPos(std::move(targetPos)) {}

	bool operator()(const Position& startPos, const Position& testPos, const FindPathParams& fpp,
	                int32_t& bestMatchDist) const;

	bool isInRange(const Position& startPos, const Position& testPos, const FindPathParams& fpp) const;

private:
	Position targetPos;
};

//////////////////////////////////////////////////////////////////////
// Defines the Base class for all creatures and base functions which
// every creature has

class Creature : public Thing
{
protected:
	Creature();

public:
	static double speedA, speedB, speedC;

	virtual ~Creature();

	// non-copyable
	Creature(const Creature&) = delete;
	Creature& operator=(const Creature&) = delete;

	std::shared_ptr<Creature> getCreature() override final
	{
		return std::static_pointer_cast<Creature>(shared_from_this());
	}
	std::shared_ptr<const Creature> getCreature() const override final
	{
		return std::static_pointer_cast<const Creature>(shared_from_this());
	}

	virtual std::shared_ptr<Player> getPlayer() { return nullptr; }
	virtual std::shared_ptr<const Player> getPlayer() const { return nullptr; }
	virtual std::shared_ptr<Npc> getNpc() { return nullptr; }
	virtual std::shared_ptr<const Npc> getNpc() const { return nullptr; }
	virtual std::shared_ptr<Monster> getMonster() { return nullptr; }
	virtual std::shared_ptr<const Monster> getMonster() const { return nullptr; }

	virtual const std::string& getName() const = 0;
	virtual const std::string& getNameDescription() const = 0;
	virtual std::string getDescription(int32_t lookDistance) const = 0;

	virtual CreatureType_t getType() const = 0;

	virtual void setID() = 0;
	void setRemoved() { isInternalRemoved = true; }

	uint32_t getID() const { return id; }
	virtual void removeList() = 0;
	virtual void addList() = 0;

	virtual bool canSee(const Position& pos) const;
	virtual bool canSeeCreature(const std::shared_ptr<const Creature>& creature) const;

	virtual RaceType_t getRace() const { return RACE_NONE; }
	virtual Skulls_t getSkull() const { return skull; }
	virtual Skulls_t getSkullClient(const std::shared_ptr<const Creature>& creature) const
	{
		return creature->getSkull();
	}
	void setSkull(Skulls_t newSkull);
	Direction getDirection() const { return direction; }
	void setDirection(Direction dir) { direction = dir; }

	bool isHealthHidden() const { return hiddenHealth; }
	void setHiddenHealth(bool b) { hiddenHealth = b; }

	int32_t getThrowRange() const override final { return 1; }
	bool isPushable() const override { return getWalkDelay() <= 0; }
	bool isRemoved() const override final { return isInternalRemoved; }
	virtual bool canSeeInvisibility() const { return false; }
	virtual bool isInGhostMode() const { return false; }
	virtual bool canSeeGhostMode(const std::shared_ptr<const Creature>&) const { return false; }

	int32_t getWalkDelay(Direction dir) const;
	int32_t getWalkDelay() const;
	int64_t getTimeSinceLastMove() const;

	int64_t getEventStepTicks(bool onlyDelay = false) const;
	int64_t getStepDuration(Direction dir) const;
	int64_t getStepDuration() const;
	virtual int32_t getStepSpeed() const { return getSpeed(); }
	int32_t getSpeed() const { return baseSpeed + varSpeed; }
	void setSpeed(int32_t varSpeedDelta)
	{
		int32_t oldSpeed = getSpeed();
		varSpeed = varSpeedDelta;

		if (getSpeed() <= 0) {
			stopEventWalk();
			cancelNextWalk = true;
		} else if (oldSpeed <= 0 && !listWalkDir.empty()) {
			addEventWalk();
		}
	}

	void setBaseSpeed(uint32_t newBaseSpeed) { baseSpeed = newBaseSpeed; }
	uint32_t getBaseSpeed() const { return baseSpeed; }

	int32_t getHealth() const { return health; }
	virtual int32_t getMaxHealth() const { return healthMax; }
	bool isDead() const { return health <= 0; }

	void setDrunkenness(uint8_t newDrunkenness) { drunkenness = newDrunkenness; }
	uint8_t getDrunkenness() const { return drunkenness; }

	const Outfit_t getCurrentOutfit() const { return currentOutfit; }
	void setCurrentOutfit(Outfit_t outfit) { currentOutfit = outfit; }
	const Outfit_t getDefaultOutfit() const { return defaultOutfit; }
	bool isInvisible() const;
	ZoneType_t getZone() const
	{
		const auto& tile = getTile();
		if (!tile) {
			return ZONE_NORMAL;
		}
		return tile->getZone();
	}

	// creature icons
	CreatureIconHashMap& getIcons() { return creatureIcons; }
	const CreatureIconHashMap& getIcons() const { return creatureIcons; }
	void updateIcons() const;

	// walk functions
	void startAutoWalk();
	void startAutoWalk(Direction direction);
	void startAutoWalk(const std::vector<Direction>& listDir);
	void addEventWalk(bool firstStep = false);
	void stopEventWalk();
	virtual void goToFollowCreature() = 0;
	void updateFollowCreaturePath(FindPathParams& fpp);

	// walk events
	virtual void onWalk(Direction& dir);
	virtual void onWalkAborted() {}
	virtual void onWalkComplete() {}

	// follow functions
	std::shared_ptr<Creature> getFollowCreature() const { return followCreature; }
	virtual void setFollowCreature(const std::shared_ptr<Creature>& creature);
	virtual void removeFollowCreature();
	virtual bool canFollowCreature(const std::shared_ptr<Creature>& creature);
	virtual bool isFollowingCreature(const std::shared_ptr<Creature>& creature) { return followCreature == creature; }

	// follow events
	virtual void onFollowCreature(const std::shared_ptr<const Creature>&);
	virtual void onUnfollowCreature();

	// Pathfinding functions
	bool isFollower(const std::shared_ptr<const Creature>& creature);
	void addFollower(const std::shared_ptr<Creature>& creature);
	void removeFollower(const std::shared_ptr<Creature>& creature);
	void removeFollowers();
	void releaseFollowers();

	// Pathfinding events
	void updateFollowersPaths();

	// combat functions
	std::shared_ptr<Creature> getAttackedCreature() { return attackedCreature; }
	virtual void setAttackedCreature(const std::shared_ptr<Creature>& creature);
	virtual void removeAttackedCreature();
	virtual bool canAttackCreature(const std::shared_ptr<Creature>& creature);
	virtual bool isAttackingCreature(const std::shared_ptr<Creature>& creature) { return attackedCreature == creature; }

	virtual BlockType_t blockHit(const std::shared_ptr<Creature>& attacker, CombatType_t combatType, int32_t& damage,
	                             bool checkDefense = false, bool checkArmor = false, bool field = false,
	                             bool ignoreResistances = false);

	bool setMaster(const std::shared_ptr<Creature>& newMaster);
	void removeMaster() { master = nullptr; }

	bool isSummon() const { return master != nullptr; }
	std::shared_ptr<Creature> getMaster() const { return master; }

	// Summons are owned by the master via shared_ptr to keep them alive while summoned.
	const std::list<std::shared_ptr<Creature>>& getSummons() const { return summons; }

	virtual int32_t getArmor() const { return 0; }
	virtual int32_t getDefense() const { return 0; }
	virtual float getAttackFactor() const { return 1.0f; }
	virtual float getDefenseFactor() const { return 1.0f; }

	virtual uint8_t getSpeechBubble() const { return SPEECHBUBBLE_NONE; }

	bool addCondition(Condition* condition, bool force = false);
	bool addCombatCondition(Condition* condition);
	void removeCondition(ConditionType_t type, ConditionId_t conditionId, bool force = false);
	void removeCondition(ConditionType_t type, bool force = false);
	void removeCondition(Condition* condition, bool force = false);
	void removeCombatCondition(ConditionType_t type);
	Condition* getCondition(ConditionType_t type) const;
	Condition* getCondition(ConditionType_t type, ConditionId_t conditionId, uint32_t subId = 0) const;
	void executeConditions(uint32_t interval);
	bool hasCondition(ConditionType_t type, uint32_t subId = 0) const;
	virtual bool isImmune(ConditionType_t type) const;
	virtual bool isImmune(CombatType_t type) const;
	virtual bool isSuppress(ConditionType_t type) const;
	virtual uint32_t getDamageImmunities() const { return 0; }
	virtual uint32_t getConditionImmunities() const { return 0; }
	virtual uint32_t getConditionSuppressions() const { return 0; }
	virtual bool isAttackable() const { return true; }

	virtual void changeHealth(int32_t healthChange, bool sendHealthChange = true);

	void gainHealth(const std::shared_ptr<Creature>& healer, int32_t healthGain);
	virtual void drainHealth(const std::shared_ptr<Creature>& attacker, int32_t damage);

	virtual bool challengeCreature(const std::shared_ptr<Creature>&, bool) { return false; }

	CreatureVector getKillers();
	void onDeath();
	virtual uint64_t getGainedExperience(const std::shared_ptr<Creature>& attacker) const;
	void addDamagePoints(const std::shared_ptr<Creature>& attacker, int32_t damagePoints);
	bool hasBeenAttacked(uint32_t attackerId);

	// combat event functions
	virtual void onAddCondition(ConditionType_t type);
	virtual void onAddCombatCondition(ConditionType_t type);
	virtual void onEndCondition(ConditionType_t type);
	void onTickCondition(ConditionType_t type, bool& bRemove);
	virtual void onCombatRemoveCondition(Condition* condition);
	virtual void onAttackedCreature(const std::shared_ptr<Creature>&, bool = true) {}
	virtual void onAttacked();
	virtual void onAttackedCreatureDrainHealth(const std::shared_ptr<Creature>& target, int32_t points);
	virtual void onTargetCreatureGainHealth(const std::shared_ptr<Creature>&, int32_t) {}
	virtual bool onKilledCreature(const std::shared_ptr<Creature>& target, bool lastHit = true);
	virtual void onGainExperience(uint64_t gainExp, const std::shared_ptr<Creature>& target);
	virtual void onAttackedCreatureBlockHit(BlockType_t) {}
	virtual void onBlockHit() {}
	virtual void onChangeZone(ZoneType_t zone);
	virtual void onAttackedCreatureChangeZone(ZoneType_t zone);
	virtual void onIdleStatus();

	virtual LightInfo getCreatureLight() const;
	virtual void setNormalCreatureLight();
	void setCreatureLight(LightInfo lightInfo);

	virtual void onThink(uint32_t interval);
	virtual void forceUpdatePath();
	void onAttacking(uint32_t interval);
	virtual void onWalk();
	virtual bool getNextStep(Direction& dir, uint32_t& flags);

	virtual void onAddTileItem(const std::shared_ptr<const Tile>&, const Position&) {}
	virtual void onUpdateTileItem(const std::shared_ptr<const Tile>&, const Position&,
	                              const std::shared_ptr<const Item>&, const ItemType&,
	                              const std::shared_ptr<const Item>&, const ItemType&)
	{}
	virtual void onRemoveTileItem(const std::shared_ptr<const Tile>&, const Position&, const ItemType&,
	                              const std::shared_ptr<const Item>&)
	{}

	virtual void onCreatureAppear(const std::shared_ptr<Creature>& creature, bool isLogin);
	virtual void onRemoveCreature(const std::shared_ptr<Creature>& creature, bool isLogout);
	virtual void onCreatureMove(const std::shared_ptr<Creature>& creature, const std::shared_ptr<const Tile>& newTile,
	                            const Position& newPos, const std::shared_ptr<const Tile>& oldTile,
	                            const Position& oldPos, bool teleport);

	virtual void onAttackedCreatureDisappear(bool) {}
	virtual void onFollowCreatureDisappear(bool) {}

	virtual void onCreatureSay(const std::shared_ptr<Creature>&, SpeakClasses, const std::string&) {}

	virtual void onPlacedCreature() {}

	virtual bool getCombatValues(int32_t&, int32_t&) { return false; }

	size_t getSummonCount() const { return summons.size(); }
	void setDropLoot(bool lootDrop) { this->lootDrop = lootDrop; }
	void setSkillLoss(bool skillLoss) { this->skillLoss = skillLoss; }
	void setUseDefense(bool useDefense) { canUseDefense = useDefense; }
	void setMovementBlocked(bool state)
	{
		movementBlocked = state;
		cancelNextWalk = true;
	}
	bool isMovementBlocked() const { return movementBlocked; }

	// creature script events
	bool registerCreatureEvent(const std::string& name);
	bool unregisterCreatureEvent(const std::string& name);

	bool hasParent() const override { return getParent() != nullptr; }
	std::shared_ptr<Thing> getParent() const override final { return tile; }
	void setParent(const std::shared_ptr<Thing>& thing) override final
	{
		tile = thing->getTile();
		position = tile->getPosition();
	}

	const Position& getPosition() const override final { return position; }

	std::shared_ptr<Tile> getTile() override final { return tile; }
	std::shared_ptr<const Tile> getTile() const override final { return tile; }

	const Position& getLastPosition() const { return lastPosition; }
	void setLastPosition(Position newLastPos) { lastPosition = newLastPos; }

	static bool canSee(const Position& myPos, const Position& pos, int32_t viewRangeX, int32_t viewRangeY);

	double getDamageRatio(const std::shared_ptr<Creature>& attacker) const;

	bool getPathTo(const Position& targetPos, std::vector<Direction>& dirList, const FindPathParams& fpp) const;
	bool getPathTo(const Position& targetPos, std::vector<Direction>& dirList, int32_t minTargetDist,
	               int32_t maxTargetDist, bool fullPathSearch = true, bool clearSight = true,
	               int32_t maxSearchDist = 0) const;

	virtual void setStorageValue(uint32_t key, std::optional<int32_t> value, bool isSpawn = false);
	virtual std::optional<int32_t> getStorageValue(uint32_t key) const;
	decltype(auto) getStorageMap() const { return storageMap; }

protected:
	struct CountBlock_t
	{
		int32_t total;
		int64_t ticks;
	};

	Position position;

	using CountMap = std::map<uint32_t, CountBlock_t>;
	CountMap damageMap;

	std::list<std::shared_ptr<Creature>> summons;
	CreatureEventList eventsList;
	ConditionList conditions;
	CreatureIconHashMap creatureIcons;

	std::vector<Direction> listWalkDir;

	std::shared_ptr<Tile> tile = nullptr;
	std::shared_ptr<Creature> attackedCreature = nullptr;
	std::shared_ptr<Creature> master = nullptr;
	std::shared_ptr<Creature> followCreature = nullptr;
	std::vector<std::shared_ptr<Creature>> followers;

	uint64_t lastStep = 0;
	int64_t lastPathUpdate = 0;
	uint32_t id = 0;
	uint32_t scriptEventsBitField = 0;
	uint32_t eventWalk = 0;
	uint32_t walkUpdateTicks = 0;
	uint32_t lastHitCreatureId = 0;
	uint32_t blockCount = 0;
	uint32_t blockTicks = 0;
	uint32_t lastStepCost = 1;
	uint32_t baseSpeed = 220;
	int32_t varSpeed = 0;
	int32_t health = 1000;
	int32_t healthMax = 1000;
	uint8_t drunkenness = 0;

	Outfit_t currentOutfit;
	Outfit_t defaultOutfit;
	uint16_t currentMount;

	Position lastPosition;
	LightInfo internalLight;

	Direction direction = DIRECTION_SOUTH;
	Skulls_t skull = SKULL_NONE;

	bool isInternalRemoved = false;
	bool creatureCheck = false;
	bool inCheckCreaturesVector = false;
	bool skillLoss = true;
	bool lootDrop = true;
	bool cancelNextWalk = false;
	bool hasFollowPath = false;
	bool hiddenHealth = false;
	bool canUseDefense = true;
	bool movementBlocked = false;

	// creature script events
	bool hasEventRegistered(CreatureEventType_t event) const
	{
		return (0 != (scriptEventsBitField & (static_cast<uint32_t>(1) << event)));
	}
	CreatureEventList getCreatureEvents(CreatureEventType_t type);

	void onCreatureDisappear(const std::shared_ptr<const Creature>& creature, bool isLogout);
	virtual void doAttacking(uint32_t) {}
	virtual bool hasExtraSwing() { return false; }

	virtual uint64_t getLostExperience() const { return 0; }
	virtual void dropLoot(const std::shared_ptr<Container>&, const std::shared_ptr<Creature>&) {}
	virtual uint16_t getLookCorpse() const { return 0; }
	virtual void getPathSearchParams(const std::shared_ptr<const Creature>& creature, FindPathParams& fpp) const;
	virtual void death(const std::shared_ptr<Creature>&) {}
	virtual bool dropCorpse(const std::shared_ptr<Creature>& lastHitCreature,
	                        const std::shared_ptr<Creature>& mostDamageCreature, bool lastHitUnjustified,
	                        bool mostDamageUnjustified);
	virtual std::shared_ptr<Item> getCorpse(const std::shared_ptr<Creature>& lastHitCreature,
	                                        const std::shared_ptr<Creature>& mostDamageCreature);

	friend class Game;
	friend class Map;
	friend class LuaScriptInterface;

private:
	std::map<uint32_t, int32_t> storageMap;
};

#endif // FS_CREATURE_H
