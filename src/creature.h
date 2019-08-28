/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2018  Mark Samman <mark.samman@gmail.com>
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

#ifndef FS_CREATURE_H_5363C04015254E298F84E6D59A139508
#define FS_CREATURE_H_5363C04015254E298F84E6D59A139508

#include "map.h"
#include "position.h"
#include "condition.h"
#include "const.h"
#include "tile.h"
#include "enums.h"
#include "creatureevent.h"

using ConditionList = std::list<Condition*>;
using CreatureEventList = std::list<CreatureEvent*>;

enum slots_t : uint8_t {
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

	CONST_SLOT_FIRST = CONST_SLOT_HEAD,
	CONST_SLOT_LAST = CONST_SLOT_AMMO,
};

struct FindPathParams {
	bool fullPathSearch = true;
	bool clearSight = true;
	bool allowDiagonal = true;
	bool keepDistance = false;
	int32_t maxSearchDist = 0;
	int32_t minTargetDist = -1;
	int32_t maxTargetDist = -1;
};

class Map;
class Thing;
class Container;
class Player;
class Monster;
class Npc;
class Item;
class Tile;

static constexpr int32_t EVENT_CREATURECOUNT = 10;
static constexpr int32_t EVENT_CREATURE_THINK_INTERVAL = 1000;
static constexpr int32_t EVENT_CHECK_CREATURE_INTERVAL = (EVENT_CREATURE_THINK_INTERVAL / EVENT_CREATURECOUNT);

class FrozenPathingConditionCall
{
	public:
		explicit FrozenPathingConditionCall(Position targetPos) : targetPos(std::move(targetPos)) {}

		bool operator()(const Position& startPos, const Position& testPos,
		                const FindPathParams& fpp, int32_t& bestMatchDist) const;

		bool isInRange(const Position& startPos, const Position& testPos,
		               const FindPathParams& fpp) const;

	private:
		Position targetPos;
};

//////////////////////////////////////////////////////////////////////
// Defines the Base class for all creatures and base functions which
// every creature has

class Creature : virtual public Thing
{
	protected:
		Creature();

	public:
		static double speedA, speedB, speedC;

		virtual ~Creature();

		// non-copyable
		Creature(const Creature&) = delete;
		Creature& operator=(const Creature&) = delete;

		Creature* getCreature() override final {
			return this;
		}
		const Creature* getCreature() const override final {
			return this;
		}
		virtual Player* getPlayer() {
			return nullptr;
		}
		virtual const Player* getPlayer() const {
			return nullptr;
		}
		virtual Npc* getNpc() {
			return nullptr;
		}
		virtual const Npc* getNpc() const {
			return nullptr;
		}
		virtual Monster* getMonster() {
			return nullptr;
		}
		virtual const Monster* getMonster() const {
			return nullptr;
		}

		virtual const std::string& getName() const = 0;
		virtual const std::string& getNameDescription() const = 0;

		virtual CreatureType_t getType() const = 0;

		virtual void setID() = 0;
		void setRemoved() {
			isInternalRemoved = true;
		}

		uint32_t getID() const {
			return id;
		}
		virtual void removeList() = 0;
		virtual void addList() = 0;

		virtual bool canSee(const Position& pos) const;
		virtual bool canSeeCreature(const Creature* creature) const;

		virtual RaceType_t getRace() const {
			return RACE_NONE;
		}
		virtual Skulls_t getSkull() const {
			return skull;
		}
		virtual Skulls_t getSkullClient(const Creature* creature) const {
			return creature->getSkull();
		}
		void setSkull(Skulls_t newSkull);
		Direction getDirection() const {
			return direction;
		}
		void setDirection(Direction dir) {
			direction = dir;
		}

		bool isHealthHidden() const {
			return hiddenHealth;
		}
		void setHiddenHealth(bool b) {
			hiddenHealth = b;
		}

		int32_t getThrowRange() const override final {
			return 1;
		}
		bool isPushable() const override {
			return getWalkDelay() <= 0;
		}
		bool isRemoved() const override final {
			return isInternalRemoved;
		}
		virtual bool canSeeInvisibility() const {
			return false;
		}
		virtual bool isInGhostMode() const {
			return false;
		}

		int32_t getWalkDelay(Direction dir) const;
		int32_t getWalkDelay() const;
		int64_t getTimeSinceLastMove() const;

		int64_t getEventStepTicks(bool onlyDelay = false) const;
		int64_t getStepDuration(Direction dir) const;
		int64_t getStepDuration() const;
		virtual int32_t getStepSpeed() const {
			return getSpeed();
		}
		int32_t getSpeed() const {
			return baseSpeed + varSpeed;
		}
		void setSpeed(int32_t varSpeedDelta) {
			int32_t oldSpeed = getSpeed();
			varSpeed = varSpeedDelta;

			if (getSpeed() <= 0) {
				stopEventWalk();
				cancelNextWalk = true;
			} else if (oldSpeed <= 0 && !listWalkDir.empty()) {
				addEventWalk();
			}
		}

		void setBaseSpeed(uint32_t newBaseSpeed) {
			baseSpeed = newBaseSpeed;
		}
		uint32_t getBaseSpeed() const {
			return baseSpeed;
		}

		int32_t getHealth() const {
			return health;
		}
		virtual int32_t getMaxHealth() const {
			return healthMax;
		}

		const Outfit_t getCurrentOutfit() const {
			return currentOutfit;
		}
		void setCurrentOutfit(Outfit_t outfit) {
			currentOutfit = outfit;
		}
		const Outfit_t getDefaultOutfit() const {
			return defaultOutfit;
		}
		bool isInvisible() const;
		ZoneType_t getZone() const {
			return getTile()->getZone();
		}

		//walk functions
		void startAutoWalk(const std::forward_list<Direction>& listDir);
		void addEventWalk(bool firstStep = false);
		void stopEventWalk();
		virtual void goToFollowCreature();

		//walk events
		virtual void onWalk(Direction& dir);
		virtual void onWalkAborted() {}
		virtual void onWalkComplete() {}

		//follow functions
		Creature* getFollowCreature() const {
			return followCreature;
		}
		virtual bool setFollowCreature(Creature* creature);

		//follow events
		virtual void onFollowCreature(const Creature*) {}
		virtual void onFollowCreatureComplete(const Creature*) {}

		//combat functions
		Creature* getAttackedCreature() {
			return attackedCreature;
		}
		virtual bool setAttackedCreature(Creature* creature);
		virtual BlockType_t blockHit(Creature* attacker, CombatType_t combatType, int32_t& damage,
		                             bool checkDefense = false, bool checkArmor = false, bool field = false);

		bool setMaster(Creature* newMaster);

		void removeMaster() {
			if (master) {
				master = nullptr;
				decrementReferenceCounter();
			}
		}

		bool isSummon() const {
			return master != nullptr;
		}
		Creature* getMaster() const {
			return master;
		}

		const std::list<Creature*>& getSummons() const {
			return summons;
		}

		virtual int32_t getArmor() const {
			return 0;
		}
		virtual int32_t getDefense() const {
			return 0;
		}
		virtual float getAttackFactor() const {
			return 1.0f;
		}
		virtual float getDefenseFactor() const {
			return 1.0f;
		}

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
		virtual uint32_t getDamageImmunities() const {
			return 0;
		}
		virtual uint32_t getConditionImmunities() const {
			return 0;
		}
		virtual uint32_t getConditionSuppressions() const {
			return 0;
		}
		virtual bool isAttackable() const {
			return true;
		}

		virtual void changeHealth(int32_t healthChange, bool sendHealthChange = true);

		void gainHealth(Creature* healer, int32_t healthGain);
		virtual void drainHealth(Creature* attacker, int32_t damage);

		virtual bool challengeCreature(Creature*) {
			return false;
		}

		void onDeath();
		virtual uint64_t getGainedExperience(Creature* attacker) const;
		void addDamagePoints(Creature* attacker, int32_t damagePoints);
		bool hasBeenAttacked(uint32_t attackerId);

		//combat event functions
		virtual void onAddCondition(ConditionType_t type);
		virtual void onAddCombatCondition(ConditionType_t type);
		virtual void onEndCondition(ConditionType_t type);
		void onTickCondition(ConditionType_t type, bool& bRemove);
		virtual void onCombatRemoveCondition(Condition* condition);
		virtual void onAttackedCreature(Creature*) {}
		virtual void onAttacked();
		virtual void onAttackedCreatureDrainHealth(Creature* target, int32_t points);
		virtual void onTargetCreatureGainHealth(Creature*, int32_t) {}
		virtual bool onKilledCreature(Creature* target, bool lastHit = true);
		virtual void onGainExperience(uint64_t gainExp, Creature* target);
		virtual void onAttackedCreatureBlockHit(BlockType_t) {}
		virtual void onBlockHit() {}
		virtual void onChangeZone(ZoneType_t zone);
		virtual void onAttackedCreatureChangeZone(ZoneType_t zone);
		virtual void onIdleStatus();

		virtual LightInfo getCreatureLight() const;
		virtual void setNormalCreatureLight();
		void setCreatureLight(LightInfo lightInfo);

		virtual void onThink(uint32_t interval);
		void onAttacking(uint32_t interval);
		virtual void onWalk();
		virtual bool getNextStep(Direction& dir, uint32_t& flags);

		void onAddTileItem(const Tile* tile, const Position& pos);
		virtual void onUpdateTileItem(const Tile* tile, const Position& pos, const Item* oldItem,
		                              const ItemType& oldType, const Item* newItem, const ItemType& newType);
		virtual void onRemoveTileItem(const Tile* tile, const Position& pos, const ItemType& iType,
		                              const Item* item);

		virtual void onCreatureAppear(Creature* creature, bool isLogin);
		virtual void onRemoveCreature(Creature* creature, bool isLogout);
		virtual void onCreatureMove(Creature* creature, const Tile* newTile, const Position& newPos,
		                            const Tile* oldTile, const Position& oldPos, bool teleport);

		virtual void onAttackedCreatureDisappear(bool) {}
		virtual void onFollowCreatureDisappear(bool) {}

		virtual void onCreatureSay(Creature*, SpeakClasses, const std::string&) {}

		virtual void onPlacedCreature() {}

		virtual bool getCombatValues(int32_t&, int32_t&) {
			return false;
		}

		size_t getSummonCount() const {
			return summons.size();
		}
		void setDropLoot(bool lootDrop) {
			this->lootDrop = lootDrop;
		}
		void setSkillLoss(bool skillLoss) {
			this->skillLoss = skillLoss;
		}
		void setUseDefense(bool useDefense) {
			canUseDefense = useDefense;
		}

		//creature script events
		bool registerCreatureEvent(const std::string& name);
		bool unregisterCreatureEvent(const std::string& name);

		Cylinder* getParent() const override final {
			return tile;
		}
		void setParent(Cylinder* cylinder) override final {
			tile = static_cast<Tile*>(cylinder);
			position = tile->getPosition();
		}

		const Position& getPosition() const override final {
			return position;
		}

		Tile* getTile() override final {
			return tile;
		}
		const Tile* getTile() const override final {
			return tile;
		}

		int32_t getWalkCache(const Position& pos) const;

		const Position& getLastPosition() const {
			return lastPosition;
		}
		void setLastPosition(Position newLastPos) {
			lastPosition = newLastPos;
		}

		static bool canSee(const Position& myPos, const Position& pos, int32_t viewRangeX, int32_t viewRangeY);

		double getDamageRatio(Creature* attacker) const;

		bool getPathTo(const Position& targetPos, std::forward_list<Direction>& dirList, const FindPathParams& fpp) const;
		bool getPathTo(const Position& targetPos, std::forward_list<Direction>& dirList, int32_t minTargetDist, int32_t maxTargetDist, bool fullPathSearch = true, bool clearSight = true, int32_t maxSearchDist = 0) const;

		void incrementReferenceCounter() {
			++referenceCounter;
		}
		void decrementReferenceCounter() {
			if (--referenceCounter == 0) {
				delete this;
			}
		}

	protected:
		virtual bool useCacheMap() const {
			return false;
		}

		struct CountBlock_t {
			int32_t total;
			int64_t ticks;
		};

		static constexpr int32_t mapWalkWidth = Map::maxViewportX * 2 + 1;
		static constexpr int32_t mapWalkHeight = Map::maxViewportY * 2 + 1;
		static constexpr int32_t maxWalkCacheWidth = (mapWalkWidth - 1) / 2;
		static constexpr int32_t maxWalkCacheHeight = (mapWalkHeight - 1) / 2;

		Position position;

		using CountMap = std::map<uint32_t, CountBlock_t>;
		CountMap damageMap;

		std::list<Creature*> summons;
		CreatureEventList eventsList;
		ConditionList conditions;

		std::forward_list<Direction> listWalkDir;

		Tile* tile = nullptr;
		Creature* attackedCreature = nullptr;
		Creature* master = nullptr;
		Creature* followCreature = nullptr;

		uint64_t lastStep = 0;
		uint32_t referenceCounter = 0;
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

		Outfit_t currentOutfit;
		Outfit_t defaultOutfit;

		Position lastPosition;
		LightInfo internalLight;

		Direction direction = DIRECTION_SOUTH;
		Skulls_t skull = SKULL_NONE;

		bool localMapCache[mapWalkHeight][mapWalkWidth] = {{ false }};
		bool isInternalRemoved = false;
		bool isMapLoaded = false;
		bool isUpdatingPath = false;
		bool creatureCheck = false;
		bool inCheckCreaturesVector = false;
		bool skillLoss = true;
		bool lootDrop = true;
		bool cancelNextWalk = false;
		bool hasFollowPath = false;
		bool forceUpdateFollowPath = false;
		bool hiddenHealth = false;
		bool canUseDefense = true;

		//creature script events
		bool hasEventRegistered(CreatureEventType_t event) const {
			return (0 != (scriptEventsBitField & (static_cast<uint32_t>(1) << event)));
		}
		CreatureEventList getCreatureEvents(CreatureEventType_t type);

		void updateMapCache();
		void updateTileCache(const Tile* tile, int32_t dx, int32_t dy);
		void updateTileCache(const Tile* tile, const Position& pos);
		void onCreatureDisappear(const Creature* creature, bool isLogout);
		virtual void doAttacking(uint32_t) {}
		virtual bool hasExtraSwing() {
			return false;
		}

		virtual uint64_t getLostExperience() const {
			return 0;
		}
		virtual void dropLoot(Container*, Creature*) {}
		virtual uint16_t getLookCorpse() const {
			return 0;
		}
		virtual void getPathSearchParams(const Creature* creature, FindPathParams& fpp) const;
		virtual void death(Creature*) {}
		virtual bool dropCorpse(Creature* lastHitCreature, Creature* mostDamageCreature, bool lastHitUnjustified, bool mostDamageUnjustified);
		virtual Item* getCorpse(Creature* lastHitCreature, Creature* mostDamageCreature);

		friend class Game;
		friend class Map;
		friend class LuaScriptInterface;
};

#endif
