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

#ifndef FS_MONSTER_H_9F5EEFE64314418CA7DA41D1B9409DD0
#define FS_MONSTER_H_9F5EEFE64314418CA7DA41D1B9409DD0

#include "tile.h"
#include "monsters.h"

class Creature;
class Game;
class Spawn;

typedef std::unordered_set<Creature*> CreatureHashSet;
typedef std::list<Creature*> CreatureList;

enum TargetSearchType_t {
	TARGETSEARCH_DEFAULT,
	TARGETSEARCH_RANDOM,
	TARGETSEARCH_ATTACKRANGE,
	TARGETSEARCH_NEAREAST
};

class Monster final : public Creature
{
	private:
		Monster(MonsterType* mtype);

	public:
		static Monster* createMonster(MonsterType* mType);
		static Monster* createMonster(const std::string& name);
		static int32_t despawnRange;
		static int32_t despawnRadius;

		~Monster();

		Monster* getMonster() final {
			return this;
		}
		const Monster* getMonster() const final {
			return this;
		}

		void setID() final {
			if (this->id == 0) {
				this->id = monsterAutoID++;
			}
		}

		void removeList() final;
		void addList() final;

		const std::string& getName() const final {
			return mType->name;
		}
		const std::string& getNameDescription() const final {
			return mType->nameDescription;
		}
		std::string getDescription(int32_t) const final {
			return strDescription + '.';
		}

		CreatureType_t getType() const final {
			return CREATURETYPE_MONSTER;
		}

		const Position& getMasterPos() const {
			return masterPos;
		}
		void setMasterPos(const Position& pos, int32_t radius = 1) {
			masterPos = pos;
			masterRadius = radius;
		}

		RaceType_t getRace() const final {
			return mType->race;
		}
		int32_t getArmor() const final {
			return mType->armor;
		}
		int32_t getDefense() const final {
			return mType->defense;
		}
		bool isPushable() const final {
			return mType->pushable && baseSpeed != 0;
		}
		bool isAttackable() const final {
			return mType->isAttackable;
		}

		bool canPushItems() const {
			return mType->canPushItems;
		}
		bool canPushCreatures() const {
			return mType->canPushCreatures;
		}
		bool isHostile() const {
			return mType->isHostile;
		}
		bool canSee(const Position& pos) const final;
		bool canSeeInvisibility() const final {
			return isImmune(CONDITION_INVISIBLE);
		}
		uint32_t getManaCost() const {
			return mType->manaCost;
		}
		void setSpawn(Spawn* _spawn) {
			spawn = _spawn;
		}

		void onAttackedCreatureDisappear(bool isLogout) final;

		void onCreatureAppear(Creature* creature, bool isLogin) final;
		void onCreatureDisappear(Creature* creature, uint32_t stackpos, bool isLogout) final;
		void onCreatureMove(Creature* creature, const Tile* newTile, const Position& newPos, const Tile* oldTile, const Position& oldPos, bool teleport) final;
		void onCreatureSay(Creature* creature, SpeakClasses type, const std::string& text) final;

		void drainHealth(Creature* attacker, int32_t damage) final;
		void changeHealth(int32_t healthChange, bool sendHealthChange = true) final;
		void onWalk() final;
		bool getNextStep(Direction& dir, uint32_t& flags) final;
		void onFollowCreatureComplete(const Creature* creature) final;

		void onThink(uint32_t interval) final;

		bool challengeCreature(Creature* creature) final;
		bool convinceCreature(Creature* creature) final;

		void setNormalCreatureLight() final;
		bool getCombatValues(int32_t& min, int32_t& max) final;

		void doAttacking(uint32_t interval) final;
		bool hasExtraSwing() final {
			return extraMeleeAttack;
		}

		bool searchTarget(TargetSearchType_t searchType = TARGETSEARCH_DEFAULT);
		bool selectTarget(Creature* creature);

		const CreatureList& getTargetList() const {
			return targetList;
		}
		const CreatureHashSet& getFriendList() const {
			return friendList;
		}

		bool isTarget(const Creature* creature) const;
		bool isFleeing() const {
			return getHealth() <= mType->runAwayHealth;
		}

		bool getDistanceStep(const Position& targetPos, Direction& dir, bool flee = false);
		bool isTargetNearby() const {
			return stepDuration >= 1;
		}

		BlockType_t blockHit(Creature* attacker, CombatType_t combatType, int32_t& damage,
		                     bool checkDefense = false, bool checkArmor = false, bool field = false);

		static uint32_t monsterAutoID;

	private:
		CreatureHashSet friendList;
		CreatureList targetList;

		std::string strDescription;

		MonsterType* mType;
		Spawn* spawn;

		int64_t lastMeleeAttack;

		uint32_t attackTicks;
		uint32_t targetTicks;
		uint32_t targetChangeTicks;
		uint32_t defenseTicks;
		uint32_t yellTicks;
		int32_t minCombatValue;
		int32_t maxCombatValue;
		int32_t targetChangeCooldown;
		int32_t stepDuration;
		int32_t masterRadius;

		Position masterPos;

		bool isIdle;
		bool extraMeleeAttack;
		bool isMasterInRange;

		void onCreatureEnter(Creature* creature);
		void onCreatureLeave(Creature* creature);
		void onCreatureFound(Creature* creature, bool pushFront = false);

		void updateLookDirection();

		void addFriend(Creature* creature);
		void removeFriend(Creature* creature);
		void addTarget(Creature* creature, bool pushFront = false);
		void removeTarget(Creature* creature);

		void updateTargetList();
		void clearTargetList();
		void clearFriendList();

		void death(Creature* _lastHitCreature) final;
		Item* getCorpse(Creature* _lastHitCreature, Creature* mostDamageCreature) final;
		bool despawn();
		bool inDespawnRange(const Position& pos) const;

		void setIdle(bool _idle);
		void updateIdleStatus();
		bool getIdleStatus() const {
			return isIdle;
		}

		void onAddCondition(ConditionType_t type) final;
		void onEndCondition(ConditionType_t type) final;
		void onCreatureConvinced(const Creature* convincer, const Creature* creature) final;

		bool canUseAttack(const Position& pos, const Creature* target) const;
		bool canUseSpell(const Position& pos, const Position& targetPos,
		                 const spellBlock_t& sb, uint32_t interval, bool& inRange, bool& resetTicks);
		bool getRandomStep(const Position& creaturePos, Direction& dir) const;
		bool getDanceStep(const Position& creaturePos, Direction& dir,
		                  bool keepAttack = true, bool keepDistance = true);
		bool isInSpawnRange(const Position& toPos) const;
		bool canWalkTo(Position pos, Direction dir) const;

		static bool pushItem(Item* item);
		void pushItems(Tile* tile);
		static bool pushCreature(Creature* creature);
		void pushCreatures(Tile* tile);

		void onThinkTarget(uint32_t interval);
		void onThinkYell(uint32_t interval);
		void onThinkDefense(uint32_t interval);

		bool isFriend(const Creature* creature) const;
		bool isOpponent(const Creature* creature) const;

		uint64_t getLostExperience() const final {
			return skillLoss ? mType->experience : 0;
		}
		uint16_t getLookCorpse() const final {
			return mType->lookcorpse;
		}
		void dropLoot(Container* corpse, Creature* _lastHitCreature) final;
		uint32_t getDamageImmunities() const final {
			return mType->damageImmunities;
		}
		uint32_t getConditionImmunities() const final {
			return mType->conditionImmunities;
		}
		void getPathSearchParams(const Creature* creature, FindPathParams& fpp) const final;
		bool useCacheMap() const final {
			return true;
		}

		friend class LuaScriptInterface;
};

#endif
