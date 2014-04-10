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

class Monster : public Creature
{
	private:
		Monster(MonsterType* mtype);

	public:
#ifdef ENABLE_SERVER_DIAGNOSTIC
		static uint32_t monsterCount;
#endif
		static Monster* createMonster(MonsterType* mType);
		static Monster* createMonster(const std::string& name);
		static int32_t despawnRange;
		static int32_t despawnRadius;

		virtual ~Monster();

		virtual Monster* getMonster() {
			return this;
		}
		virtual const Monster* getMonster() const {
			return this;
		}

		void setID() {
			if (this->id == 0) {
				this->id = monsterAutoID++;
			}
		}

		void removeList();
		void addList();

		virtual const std::string& getName() const {
			return mType->name;
		}
		virtual const std::string& getNameDescription() const {
			return mType->nameDescription;
		}
		virtual std::string getDescription(int32_t) const {
			return strDescription + '.';
		}

		virtual CreatureType_t getType() const {
			return CREATURETYPE_MONSTER;
		}

		virtual const Position& getMasterPos() const {
			return masterPos;
		}
		void setMasterPos(const Position& pos, uint32_t radius = 1) {
			masterPos = pos;
			masterRadius = radius;
		}

		virtual RaceType_t getRace() const {
			return mType->race;
		}
		virtual int32_t getArmor() const {
			return mType->armor;
		}
		virtual int32_t getDefense() const {
			return mType->defense;
		}
		virtual bool isPushable() const {
			return mType->pushable && (baseSpeed > 0);
		}
		virtual bool isAttackable() const {
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
		virtual bool canSee(const Position& pos) const;
		virtual bool canSeeInvisibility() const {
			return isImmune(CONDITION_INVISIBLE);
		}
		uint32_t getManaCost() const {
			return mType->manaCost;
		}
		void setSpawn(Spawn* _spawn) {
			spawn = _spawn;
		}

		virtual void onAttackedCreatureDisappear(bool isLogout);
		virtual void onFollowCreatureDisappear(bool isLogout);

		virtual void onCreatureAppear(const Creature* creature, bool isLogin);
		virtual void onCreatureDisappear(const Creature* creature, uint32_t stackpos, bool isLogout);
		virtual void onCreatureMove(const Creature* creature, const Tile* newTile, const Position& newPos, const Tile* oldTile, const Position& oldPos, bool teleport);
		virtual void onCreatureSay(const Creature* creature, SpeakClasses type, const std::string& text);

		virtual void drainHealth(Creature* attacker, int32_t damage);
		virtual void changeHealth(int32_t healthChange, bool sendHealthChange = true);
		virtual void onWalk();
		virtual bool getNextStep(Direction& dir, uint32_t& flags);
		virtual void onFollowCreatureComplete(const Creature* creature);

		virtual void onThink(uint32_t interval);

		virtual bool challengeCreature(Creature* creature);
		virtual bool convinceCreature(Creature* creature);

		virtual void setNormalCreatureLight();
		virtual bool getCombatValues(int32_t& min, int32_t& max);

		virtual void doAttacking(uint32_t interval);
		virtual bool hasExtraSwing() {
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

		bool resetTicks;
		bool isIdle;
		bool extraMeleeAttack;
		bool isMasterInRange;

		virtual void onCreatureEnter(Creature* creature);
		virtual void onCreatureLeave(Creature* creature);
		void onCreatureFound(Creature* creature, bool pushFront = false);

		void updateLookDirection();

		void addFriend(Creature* creature);
		void removeFriend(Creature* creature);
		void addTarget(Creature* creature, bool pushFront = false);
		void removeTarget(Creature* creature);

		void updateTargetList();
		void clearTargetList();
		void clearFriendList();

		void death(Creature* _lastHitCreature);
		Item* getCorpse(Creature* _lastHitCreature, Creature* mostDamageCreature);
		bool despawn();
		bool inDespawnRange(const Position& pos);

		void setIdle(bool _idle);
		void updateIdleStatus();
		bool getIdleStatus() const {
			return isIdle;
		}

		virtual void onAddCondition(ConditionType_t type);
		virtual void onEndCondition(ConditionType_t type);
		virtual void onCreatureConvinced(const Creature* convincer, const Creature* creature);

		bool canUseAttack(const Position& pos, const Creature* target) const;
		bool canUseSpell(const Position& pos, const Position& targetPos,
		                 const spellBlock_t& sb, uint32_t interval, bool& inRange);
		bool getRandomStep(const Position& creaturePos, Direction& dir);
		bool getDanceStep(const Position& creaturePos, Direction& dir,
		                  bool keepAttack = true, bool keepDistance = true);
		bool isInSpawnRange(const Position& toPos);
		bool canWalkTo(Position pos, Direction dir);

		static bool pushItem(Item* item);
		void pushItems(Tile* tile);
		static bool pushCreature(Creature* creature);
		void pushCreatures(Tile* tile);

		void onThinkTarget(uint32_t interval);
		void onThinkYell(uint32_t interval);
		void onThinkDefense(uint32_t interval);

		bool isFriend(const Creature* creature) const;
		bool isOpponent(const Creature* creature) const;

		virtual uint64_t getLostExperience() const {
			return ((skillLoss ? mType->experience : 0));
		}
		virtual uint16_t getLookCorpse() {
			return mType->lookcorpse;
		}
		virtual void dropLoot(Container* corpse, Creature* _lastHitCreature);
		virtual uint32_t getDamageImmunities() const {
			return mType->damageImmunities;
		}
		virtual uint32_t getConditionImmunities() const {
			return mType->conditionImmunities;
		}
		virtual uint16_t getLookCorpse() const {
			return mType->lookcorpse;
		}
		virtual void getPathSearchParams(const Creature* creature, FindPathParams& fpp) const;
		virtual bool useCacheMap() const {
			return true;
		}

		friend class LuaScriptInterface;
};

#endif
