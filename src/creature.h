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

#ifndef FS_CREATURE_H_5363C04015254E298F84E6D59A139508
#define FS_CREATURE_H_5363C04015254E298F84E6D59A139508

#include "map.h"
#include "position.h"
#include "condition.h"
#include "const.h"
#include "tile.h"
#include "enums.h"
#include "creatureevent.h"

typedef std::list<Condition*> ConditionList;
typedef std::list<CreatureEvent*> CreatureEventList;

enum slots_t {
	CONST_SLOT_WHEREEVER = 0,
	CONST_SLOT_FIRST = 1,
	CONST_SLOT_HEAD = CONST_SLOT_FIRST,
	CONST_SLOT_NECKLACE = 2,
	CONST_SLOT_BACKPACK = 3,
	CONST_SLOT_ARMOR = 4,
	CONST_SLOT_RIGHT = 5,
	CONST_SLOT_LEFT = 6,
	CONST_SLOT_LEGS = 7,
	CONST_SLOT_FEET = 8,
	CONST_SLOT_RING = 9,
	CONST_SLOT_AMMO = 10,
	CONST_SLOT_LAST = CONST_SLOT_AMMO
};

struct FindPathParams {
	bool fullPathSearch;
	bool clearSight;
	bool allowDiagonal;
	bool keepDistance;
	int32_t maxSearchDist;
	int32_t minTargetDist;
	int32_t maxTargetDist;

	FindPathParams() {
		fullPathSearch = true;
		clearSight = true;
		allowDiagonal = true;
		keepDistance = false;
		maxSearchDist = 0;
		minTargetDist = -1;
		maxTargetDist = -1;
	}
};

class Map;
class Thing;
class Container;
class Player;
class Monster;
class Npc;
class Item;
class Tile;

#define EVENT_CREATURECOUNT 10
#define EVENT_CREATURE_THINK_INTERVAL 1000
#define EVENT_CHECK_CREATURE_INTERVAL (EVENT_CREATURE_THINK_INTERVAL / EVENT_CREATURECOUNT)

class FrozenPathingConditionCall
{
	public:
		FrozenPathingConditionCall(const Position& targetPos) : targetPos(targetPos) {}
		~FrozenPathingConditionCall() {}

		bool operator()(const Position& startPos, const Position& testPos,
		                const FindPathParams& fpp, int32_t& bestMatchDist) const;

		bool isInRange(const Position& startPos, const Position& testPos,
		               const FindPathParams& fpp) const;

	protected:
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

		virtual Creature* getCreature() {
			return this;
		}
		virtual const Creature* getCreature()const {
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

		virtual int32_t getThrowRange() const {
			return 1;
		}
		virtual bool isPushable() const {
			return (getWalkDelay() <= 0);
		}
		virtual bool isRemoved() const {
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
		int32_t getBaseSpeed() const {
			return baseSpeed;
		}

		int32_t getHealth() const {
			return health;
		}
		virtual int32_t getMaxHealth() const {
			return healthMax;
		}
		uint32_t getMana() const {
			return mana;
		}
		virtual uint32_t getMaxMana() const {
			return 0;
		}

		const Outfit_t getCurrentOutfit() const {
			return currentOutfit;
		}
		void setCurrentOutfit(const Outfit_t& outfit) {
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
		void startAutoWalk(const std::list<Direction>& listDir);
		void addEventWalk(bool firstStep = false);
		void stopEventWalk();
		virtual void goToFollowCreature();

		//walk events
		virtual void onWalk(Direction& dir);
		virtual void onWalkAborted() {}
		virtual void onWalkComplete() {}

		//follow functions
		virtual Creature* getFollowCreature() const {
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

		void setMaster(Creature* creature) {
			master = creature;
		}
		bool isSummon() const {
			return master != nullptr;
		}
		Creature* getMaster() const {
			return master;
		}

		virtual void addSummon(Creature* creature);
		virtual void removeSummon(Creature* creature);
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

		virtual uint8_t getSpeechBubble() const {
			return SPEECHBUBBLE_NONE;
		}

		bool addCondition(Condition* condition, bool force = false);
		bool addCombatCondition(Condition* condition);
		void removeCondition(ConditionType_t type, ConditionId_t id, bool force = false);
		void removeCondition(ConditionType_t type, bool force = false);
		void removeCondition(Condition* condition, bool force = false);
		void removeCombatCondition(ConditionType_t type);
		Condition* getCondition(ConditionType_t type) const;
		Condition* getCondition(ConditionType_t type, ConditionId_t id, uint32_t subId = 0) const;
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
		virtual void changeMana(int32_t manaChange);

		virtual void gainHealth(Creature* attacker, int32_t healthGain);
		virtual void drainHealth(Creature* attacker, int32_t damage);
		virtual void drainMana(Creature* attacker, int32_t manaLoss);

		virtual bool challengeCreature(Creature*) {
			return false;
		}
		virtual bool convinceCreature(Creature*) {
			return false;
		}

		virtual void onDeath();
		virtual uint64_t getGainedExperience(Creature* attacker) const;
		void addDamagePoints(Creature* attacker, int32_t damagePoints);
		bool hasBeenAttacked(uint32_t attackerId);

		//combat event functions
		virtual void onAddCondition(ConditionType_t type);
		virtual void onAddCombatCondition(ConditionType_t type);
		virtual void onEndCondition(ConditionType_t type);
		virtual void onTickCondition(ConditionType_t type, bool& bRemove);
		virtual void onCombatRemoveCondition(Condition* condition);
		virtual void onAttackedCreature(Creature*) {}
		virtual void onAttacked();
		virtual void onAttackedCreatureDrainHealth(Creature* target, int32_t points);
		virtual void onTargetCreatureGainHealth(Creature*, int32_t) {}
		virtual void onAttackedCreatureKilled(Creature* target);
		virtual bool onKilledCreature(Creature* target, bool lastHit = true);
		virtual void onGainExperience(uint64_t gainExp, Creature* target);
		virtual void onAttackedCreatureBlockHit(BlockType_t) {}
		virtual void onBlockHit() {}
		virtual void onChangeZone(ZoneType_t zone);
		virtual void onAttackedCreatureChangeZone(ZoneType_t zone);
		virtual void onIdleStatus();

		virtual void getCreatureLight(LightInfo& light) const;
		virtual void setNormalCreatureLight();
		void setCreatureLight(const LightInfo& light) {
			internalLight = light;
		}

		virtual void onThink(uint32_t interval);
		virtual void onAttacking(uint32_t interval);
		virtual void onWalk();
		virtual bool getNextStep(Direction& dir, uint32_t& flags);

		virtual void onAddTileItem(const Tile* tile, const Position& pos);
		virtual void onUpdateTileItem(const Tile* tile, const Position& pos, const Item* oldItem,
		                              const ItemType& oldType, const Item* newItem, const ItemType& newType);
		virtual void onRemoveTileItem(const Tile* tile, const Position& pos, const ItemType& iType,
		                              const Item* item);

		virtual void onCreatureAppear(const Creature* creature, bool isLogin);
		virtual void onCreatureDisappear(const Creature* creature, uint32_t stackpos, bool isLogout);
		virtual void onCreatureMove(const Creature* creature, const Tile* newTile, const Position& newPos,
		                            const Tile* oldTile, const Position& oldPos, bool teleport);

		virtual void onAttackedCreatureDisappear(bool) {}
		virtual void onFollowCreatureDisappear(bool) {}

		virtual void onCreatureSay(const Creature*, SpeakClasses, const std::string&) {}

		virtual void onCreatureConvinced(const Creature*, const Creature*) {}
		virtual void onPlacedCreature() {}
		virtual void onRemovedCreature() {}

		virtual WeaponType_t getWeaponType() {
			return WEAPON_NONE;
		}
		virtual bool getCombatValues(int32_t&, int32_t&) {
			return false;
		}

		size_t getSummonCount() const {
			return summons.size();
		}
		void setDropLoot(bool _lootDrop) {
			lootDrop = _lootDrop;
		}
		void setLossSkill(bool _skillLoss) {
			skillLoss = _skillLoss;
		}

		//creature script events
		bool registerCreatureEvent(const std::string& name);
		bool unregisterCreatureEvent(const std::string& name);

		Cylinder* getParent() {
			return _tile;
		}
		const Cylinder* getParent() const {
			return _tile;
		}
		virtual void setParent(Cylinder* cylinder) {
			_tile = static_cast<Tile*>(cylinder);
			_position = _tile->getPosition();
		}

		inline const Position& getPosition() const {
			return _position;
		}

		Tile* getTile() {
			return _tile;
		}
		const Tile* getTile() const {
			return _tile;
		}

		int32_t getWalkCache(const Position& pos) const;

		const Position& getLastPosition() const {
			return lastPosition;
		}
		void setLastPosition(const Position& newLastPos) {
			lastPosition = newLastPos;
		}

		static bool canSee(const Position& myPos, const Position& pos, int32_t viewRangeX, int32_t viewRangeY);

		virtual double getDamageRatio(Creature* attacker) const;

		bool getPathTo(const Position& targetPos, std::list<Direction>& dirList, const FindPathParams& fpp) const;
		bool getPathTo(const Position& targetPos, std::list<Direction>& dirList, int32_t minTargetDist, int32_t maxTargetDist, bool fullPathSearch = true, bool clearSight = true, int32_t maxSearchDist = 0) const;

		void useThing2() {
			++useCount;
		}
		void releaseThing2() {
			if (--useCount == 0) {
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

		Position _position;

		typedef std::map<uint32_t, CountBlock_t> CountMap;
		CountMap damageMap;

		std::list<Direction> listWalkDir;
		std::list<Creature*> summons;
		CreatureEventList eventsList;
		ConditionList conditions;

		Tile* _tile;
		Creature* attackedCreature;
		Creature* master;
		Creature* followCreature;

		uint64_t lastStep;
		uint32_t useCount;
		uint32_t id;
		uint32_t scriptEventsBitField;
		uint32_t eventWalk;
		uint32_t walkUpdateTicks;
		uint32_t lastHitCreature;
		uint32_t blockCount;
		uint32_t blockTicks;
		uint32_t lastStepCost;
		uint32_t baseSpeed;
		uint32_t mana;
		int32_t varSpeed;
		int32_t health;
		int32_t healthMax;

		Outfit_t currentOutfit;
		Outfit_t defaultOutfit;

		Position lastPosition;
		Direction direction;
		LightInfo internalLight;

		static const int32_t mapWalkWidth = Map::maxViewportX * 2 + 1;
		static const int32_t mapWalkHeight = Map::maxViewportY * 2 + 1;
		bool localMapCache[mapWalkHeight][mapWalkWidth];

		static const int32_t maxWalkCacheWidth = (mapWalkWidth - 1) / 2;
		static const int32_t maxWalkCacheHeight = (mapWalkHeight - 1) / 2;

		bool isInternalRemoved;
		bool isMapLoaded;
		bool isUpdatingPath;
		bool creatureCheck;
		bool inCheckCreaturesVector;
		bool skillLoss;
		bool lootDrop;
		bool cancelNextWalk;
		bool hasFollowPath;
		bool forceUpdateFollowPath;
		bool hiddenHealth;

		//creature script events
		bool hasEventRegistered(CreatureEventType_t event) const {
			return (0 != (scriptEventsBitField & ((uint32_t)1 << event)));
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
		virtual bool dropCorpse(Creature* _lastHitCreature, Creature* mostDamageCreature, bool lastHitUnjustified, bool mostDamageUnjustified);
		virtual Item* getCorpse(Creature* _lastHitCreature, Creature* mostDamageCreature);

		friend class Game;
		friend class Map;
		friend class Commands;
		friend class LuaScriptInterface;
};

#endif
