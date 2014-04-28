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

#include "otpch.h"

#include "monster.h"
#include "monsters.h"
#include "game.h"
#include "spells.h"
#include "combat.h"
#include "spawn.h"
#include "tasks.h"

extern Game g_game;
extern Monsters g_monsters;

int32_t Monster::despawnRange;
int32_t Monster::despawnRadius;

uint32_t Monster::monsterAutoID = 0x40000000;

Monster* Monster::createMonster(MonsterType* mType)
{
	return new Monster(mType);
}

Monster* Monster::createMonster(const std::string& name)
{
	MonsterType* mType = g_monsters.getMonsterType(name);
	if (!mType) {
		return nullptr;
	}

	return createMonster(mType);
}

Monster::Monster(MonsterType* _mtype) :
	Creature()
{
	isIdle = true;
	isMasterInRange = false;
	mType = _mtype;
	spawn = nullptr;
	defaultOutfit = mType->outfit;
	currentOutfit = mType->outfit;

	health = mType->health;
	healthMax = mType->healthMax;
	baseSpeed = mType->baseSpeed;
	internalLight.level = mType->lightLevel;
	internalLight.color = mType->lightColor;

	hiddenHealth = mType->hiddenHealth;

	minCombatValue = 0;
	maxCombatValue = 0;

	masterRadius = -1;

	targetTicks = 0;
	targetChangeTicks = 0;
	targetChangeCooldown = 0;
	attackTicks = 0;
	defenseTicks = 0;
	yellTicks = 0;
	extraMeleeAttack = false;

	strDescription = mType->nameDescription;
	toLowerCaseString(strDescription);

	stepDuration = 0;

	lastMeleeAttack = 0;

	// register creature events
	for (const std::string& scriptName : mType->scriptList) {
		if (!registerCreatureEvent(scriptName)) {
			std::cout << "[Warning - Monster::Monster] Unknown event name: " << scriptName << std::endl;
		}
	}
}

Monster::~Monster()
{
	clearTargetList();
	clearFriendList();
}

void Monster::addList()
{
	g_game.addMonster(this);
}

void Monster::removeList()
{
	g_game.removeMonster(this);
}

bool Monster::canSee(const Position& pos) const
{
	return Creature::canSee(getPosition(), pos, 9, 9);
}

void Monster::onAttackedCreatureDisappear(bool)
{
	attackTicks = 0;
	extraMeleeAttack = true;
}

void Monster::onFollowCreatureDisappear(bool)
{
	//
}

void Monster::onCreatureAppear(const Creature* creature, bool isLogin)
{
	Creature::onCreatureAppear(creature, isLogin);

	if (mType->creatureAppearEvent != -1) {
		// onCreatureAppear(self, creature)
		LuaScriptInterface* scriptInterface = mType->scriptInterface;
		if (!scriptInterface->reserveScriptEnv()) {
			std::cout << "[Error - Monster::onCreatureAppear] Call stack overflow" << std::endl;
			return;
		}

		ScriptEnvironment* env = scriptInterface->getScriptEnv();
		env->setScriptId(mType->creatureAppearEvent, scriptInterface);

		lua_State* L = scriptInterface->getLuaState();
		scriptInterface->pushFunction(mType->creatureAppearEvent);

		LuaScriptInterface::pushUserdata<Monster>(L, this);
		LuaScriptInterface::setMetatable(L, -1, "Monster");

		LuaScriptInterface::pushUserdata<Creature>(L, const_cast<Creature*>(creature));
		LuaScriptInterface::setCreatureMetatable(L, -1, creature);

		if (scriptInterface->callFunction(2)) {
			return;
		}
	}

	if (creature == this) {
		//We just spawned lets look around to see who is there.
		if (isSummon()) {
			isMasterInRange = canSee(getMaster()->getPosition());
		}

		updateTargetList();
		updateIdleStatus();
	} else {
		onCreatureEnter(const_cast<Creature*>(creature));
	}
}

void Monster::onCreatureDisappear(const Creature* creature, uint32_t stackpos, bool isLogout)
{
	Creature::onCreatureDisappear(creature, stackpos, isLogout);

	if (mType->creatureDisappearEvent != -1) {
		// onCreatureDisappear(self, creature)
		LuaScriptInterface* scriptInterface = mType->scriptInterface;
		if (!scriptInterface->reserveScriptEnv()) {
			std::cout << "[Error - Monster::onCreatureDisappear] Call stack overflow" << std::endl;
			return;
		}

		ScriptEnvironment* env = scriptInterface->getScriptEnv();
		env->setScriptId(mType->creatureDisappearEvent, scriptInterface);

		lua_State* L = scriptInterface->getLuaState();
		scriptInterface->pushFunction(mType->creatureDisappearEvent);

		LuaScriptInterface::pushUserdata<Monster>(L, this);
		LuaScriptInterface::setMetatable(L, -1, "Monster");

		LuaScriptInterface::pushUserdata<Creature>(L, const_cast<Creature*>(creature));
		LuaScriptInterface::setCreatureMetatable(L, -1, creature);

		if (scriptInterface->callFunction(2)) {
			return;
		}
	}

	if (creature == this) {
		if (spawn) {
			spawn->startSpawnCheck();
		}

		setIdle(true);
	} else {
		onCreatureLeave(const_cast<Creature*>(creature));
	}
}

void Monster::onCreatureMove(const Creature* creature, const Tile* newTile, const Position& newPos,
                             const Tile* oldTile, const Position& oldPos, bool teleport)
{
	Creature::onCreatureMove(creature, newTile, newPos, oldTile, oldPos, teleport);

	if (mType->creatureMoveEvent != -1) {
		// onCreatureMove(self, creature, oldPosition, newPosition)
		LuaScriptInterface* scriptInterface = mType->scriptInterface;
		if (!scriptInterface->reserveScriptEnv()) {
			std::cout << "[Error - Monster::onCreatureMove] Call stack overflow" << std::endl;
			return;
		}

		ScriptEnvironment* env = scriptInterface->getScriptEnv();
		env->setScriptId(mType->creatureMoveEvent, scriptInterface);

		lua_State* L = scriptInterface->getLuaState();
		scriptInterface->pushFunction(mType->creatureMoveEvent);

		LuaScriptInterface::pushUserdata<Monster>(L, this);
		LuaScriptInterface::setMetatable(L, -1, "Monster");

		LuaScriptInterface::pushUserdata<Creature>(L, const_cast<Creature*>(creature));
		LuaScriptInterface::setCreatureMetatable(L, -1, creature);

		LuaScriptInterface::pushPosition(L, oldPos);
		LuaScriptInterface::pushPosition(L, newPos);

		if (scriptInterface->callFunction(4)) {
			return;
		}
	}

	if (creature == this) {
		if (isSummon()) {
			isMasterInRange = canSee(getMaster()->getPosition());
		}

		updateTargetList();
		updateIdleStatus();
	} else {
		bool canSeeNewPos = canSee(newPos);
		bool canSeeOldPos = canSee(oldPos);

		if (canSeeNewPos && !canSeeOldPos) {
			onCreatureEnter(const_cast<Creature*>(creature));
		} else if (!canSeeNewPos && canSeeOldPos) {
			onCreatureLeave(const_cast<Creature*>(creature));
		}

		if (canSeeNewPos && isSummon() && getMaster() == creature) {
			isMasterInRange = true;    //Turn the summon on again
		}

		updateIdleStatus();

		if (!isSummon()) {
			if (followCreature) {
				const Position& followPosition = followCreature->getPosition();
				const Position& position = getPosition();

				int32_t offset_x = Position::getDistanceX(followPosition, position);
				int32_t offset_y = Position::getDistanceY(followPosition, position);
				if ((offset_x > 1 || offset_y > 1) && mType->changeTargetChance > 0) {
					Direction dir = getDirectionTo(position, followPosition);
					const Position& checkPosition = getNextPosition(dir, position);

					Tile* tile = g_game.getTile(checkPosition);
					if (tile) {
						Creature* topCreature = tile->getTopCreature();
						if (topCreature && followCreature != topCreature && isOpponent(topCreature)) {
							selectTarget(topCreature);
						}
					}
				}
			} else if (isOpponent(creature)) {
				//we have no target lets try pick this one
				selectTarget(const_cast<Creature*>(creature));
			}
		}
	}
}

void Monster::onCreatureSay(const Creature* creature, SpeakClasses type, const std::string& text)
{
	Creature::onCreatureSay(creature, type, text);

	if (mType->creatureSayEvent != -1) {
		// onCreatureSay(self, creature, type, message)
		LuaScriptInterface* scriptInterface = mType->scriptInterface;
		if (!scriptInterface->reserveScriptEnv()) {
			std::cout << "[Error - Monster::onCreatureSay] Call stack overflow" << std::endl;
			return;
		}

		ScriptEnvironment* env = scriptInterface->getScriptEnv();
		env->setScriptId(mType->creatureSayEvent, scriptInterface);

		lua_State* L = scriptInterface->getLuaState();
		scriptInterface->pushFunction(mType->creatureSayEvent);

		LuaScriptInterface::pushUserdata<Monster>(L, this);
		LuaScriptInterface::setMetatable(L, -1, "Monster");

		LuaScriptInterface::pushUserdata<Creature>(L, const_cast<Creature*>(creature));
		LuaScriptInterface::setCreatureMetatable(L, -1, creature);

		lua_pushnumber(L, type);
		LuaScriptInterface::pushString(L, text);

		scriptInterface->callVoidFunction(4);
	}
}

void Monster::addFriend(Creature* creature)
{
	assert(creature != this);
	auto result = friendList.insert(creature);
	if (result.second) {
		creature->useThing2();
	}
}

void Monster::removeFriend(Creature* creature)
{
	auto it = friendList.find(creature);
	if (it != friendList.end()) {
		creature->releaseThing2();
		friendList.erase(it);
	}
}

void Monster::addTarget(Creature* creature, bool pushFront/* = false*/)
{
	assert(creature != this);
	if (std::find(targetList.begin(), targetList.end(), creature) == targetList.end()) {
		creature->useThing2();
		if (pushFront) {
			targetList.push_front(creature);
		} else {
			targetList.push_back(creature);
		}
	}
}

void Monster::removeTarget(Creature* creature)
{
	auto it = std::find(targetList.begin(), targetList.end(), creature);
	if (it != targetList.end()) {
		creature->releaseThing2();
		targetList.erase(it);
	}
}

void Monster::updateTargetList()
{
	auto friendIterator = friendList.begin();
	while (friendIterator != friendList.end()) {
		Creature* creature = *friendIterator;
		if (creature->getHealth() <= 0 || !canSee(creature->getPosition())) {
			creature->releaseThing2();
			friendIterator = friendList.erase(friendIterator);
		} else {
			++friendIterator;
		}
	}

	auto targetIterator = targetList.begin();
	while (targetIterator != targetList.end()) {
		Creature* creature = *targetIterator;
		if (creature->getHealth() <= 0 || !canSee(creature->getPosition())) {
			creature->releaseThing2();
			targetIterator = targetList.erase(targetIterator);
		} else {
			++targetIterator;
		}
	}

	for (Creature* spectator : g_game.getSpectators(getPosition())) {
		if (spectator != this && canSee(spectator->getPosition())) {
			onCreatureFound(spectator);
		}
	}
}

void Monster::clearTargetList()
{
	for (Creature* creature : targetList) {
		creature->releaseThing2();
	}
	targetList.clear();
}

void Monster::clearFriendList()
{
	for (Creature* creature : friendList) {
		creature->releaseThing2();
	}
	friendList.clear();
}

void Monster::onCreatureFound(Creature* creature, bool pushFront/* = false*/)
{
	if (isFriend(creature)) {
		addFriend(creature);
	}

	if (isOpponent(creature)) {
		addTarget(creature, pushFront);
	}

	updateIdleStatus();
}

void Monster::onCreatureEnter(Creature* creature)
{
	// std::cout << "onCreatureEnter - " << creature->getName() << std::endl;

	if (getMaster() == creature) {
		//Turn the summon on again
		isMasterInRange = true;
		updateIdleStatus();
	}

	onCreatureFound(creature, true);
}

bool Monster::isFriend(const Creature* creature) const
{
	if (isSummon() && getMaster()->getPlayer()) {
		const Player* masterPlayer = getMaster()->getPlayer();
		const Player* tmpPlayer = nullptr;

		if (creature->getPlayer()) {
			tmpPlayer = creature->getPlayer();
		} else {
			const Creature* creatureMaster = creature->getMaster();

			if (creatureMaster && creatureMaster->getPlayer()) {
				tmpPlayer = creatureMaster->getPlayer();
			}
		}

		if (tmpPlayer && (tmpPlayer == getMaster() || masterPlayer->isPartner(tmpPlayer))) {
			return true;
		}
	} else if (creature->getMonster() && !creature->isSummon()) {
		return true;
	}

	return false;
}

bool Monster::isOpponent(const Creature* creature) const
{
	if (isSummon() && getMaster()->getPlayer()) {
		if (creature != getMaster()) {
			return true;
		}
	} else {
		if ((creature->getPlayer() && !creature->getPlayer()->hasFlag(PlayerFlag_IgnoredByMonsters)) ||
		        (creature->getMaster() && creature->getMaster()->getPlayer())) {
			return true;
		}
	}

	return false;
}

void Monster::onCreatureLeave(Creature* creature)
{
	// std::cout << "onCreatureLeave - " << creature->getName() << std::endl;

	if (getMaster() == creature) {
		//Turn the monster off until its master comes back
		isMasterInRange = false;
		updateIdleStatus();
	}

	//update friendList
	if (isFriend(creature)) {
		removeFriend(creature);
	}

	//update targetList
	if (isOpponent(creature)) {
		removeTarget(creature);
		if (targetList.empty()) {
			updateIdleStatus();
		}
	}
}

bool Monster::searchTarget(TargetSearchType_t searchType /*= TARGETSEARCH_DEFAULT*/)
{
	std::list<Creature*> resultList;
	const Position& myPos = getPosition();

	for (Creature* creature : targetList) {
		if (followCreature != creature && isTarget(creature)) {
			if (searchType == TARGETSEARCH_RANDOM || canUseAttack(myPos, creature)) {
				resultList.push_back(creature);
			}
		}
	}

	switch (searchType) {
		case TARGETSEARCH_NEAREAST: {
			if (!resultList.empty()) {
				auto it = resultList.begin();
				Creature* target = *it;

				if (++it != resultList.end()) {
					const Position& targetPosition = target->getPosition();
					int32_t minRange = std::max<int32_t>(Position::getDistanceX(myPos, targetPosition), Position::getDistanceY(myPos, targetPosition));
					do {
						const Position& pos = (*it)->getPosition();

						int32_t distance = std::max<int32_t>(Position::getDistanceX(myPos, pos), Position::getDistanceY(myPos, pos));
						if (distance < minRange) {
							target = *it;
							minRange = distance;
						}
					} while (++it != resultList.end());
				}

				if (selectTarget(target)) {
					return true;
				}
			}
			break;
		}

		case TARGETSEARCH_DEFAULT:
		case TARGETSEARCH_ATTACKRANGE:
		case TARGETSEARCH_RANDOM:
		default: {
			if (!resultList.empty()) {
				auto it = resultList.begin();
				std::advance(it, uniform_random(0, resultList.size() - 1));
				return selectTarget(*it);
			}

			if (searchType == TARGETSEARCH_ATTACKRANGE) {
				return false;
			}

			break;
		}
	}

	//lets just pick the first target in the list
	for (Creature* target : targetList) {
		if (followCreature != target && selectTarget(target)) {
			return true;
		}
	}
	return false;
}

void Monster::onFollowCreatureComplete(const Creature* creature)
{
	if (creature) {
		auto it = std::find(targetList.begin(), targetList.end(), creature);
		if (it != targetList.end()) {
			Creature* target = (*it);
			targetList.erase(it);

			if (hasFollowPath) {
				targetList.push_front(target);
			} else if (!isSummon()) {
				targetList.push_back(target);
			} else {
				target->releaseThing2();
			}
		}
	}
}

BlockType_t Monster::blockHit(Creature* attacker, CombatType_t combatType, int32_t& damage,
                              bool checkDefense /* = false*/, bool checkArmor /* = false*/, bool /* field = false */)
{
	BlockType_t blockType = Creature::blockHit(attacker, combatType, damage, checkDefense, checkArmor);

	if (damage != 0) {
		int32_t elementMod = 0;
		auto it = mType->elementMap.find(combatType);
		if (it != mType->elementMap.end()) {
			elementMod = it->second;
		}

		if (elementMod != 0) {
			damage = (int32_t)std::ceil(damage * ((float)(100 - elementMod) / 100));
			if (damage <= 0) {
				damage = 0;
				blockType = BLOCK_DEFENSE;
			}
		}
	}

	return blockType;
}


bool Monster::isTarget(const Creature* creature) const
{
	if (creature->isRemoved() || !creature->isAttackable() ||
	        creature->getZone() == ZONE_PROTECTION || !canSeeCreature(creature)) {
		return false;
	}

	if (creature->getPosition().z != getPosition().z) {
		return false;
	}
	return true;
}

bool Monster::selectTarget(Creature* creature)
{
	if (!isTarget(creature)) {
		return false;
	}

	auto it = std::find(targetList.begin(), targetList.end(), creature);
	if (it == targetList.end()) {
		//Target not found in our target list.
		return false;
	}

	if (isHostile() || isSummon()) {
		if (setAttackedCreature(creature) && !isSummon()) {
			g_dispatcher.addTask(createTask(std::bind(&Game::checkCreatureAttack, &g_game, getID())));
		}
	}
	return setFollowCreature(creature);
}

void Monster::setIdle(bool _idle)
{
	if (isRemoved() || getHealth() <= 0) {
		return;
	}

	isIdle = _idle;

	if (!isIdle) {
		g_game.addCreatureCheck(this);
	} else {
		onIdleStatus();
		clearTargetList();
		clearFriendList();
		Game::removeCreatureCheck(this);
	}
}

void Monster::updateIdleStatus()
{
	bool idle = false;

	if (conditions.empty()) {
		if (isSummon()) {
			if (!isMasterInRange || (getMaster()->getMonster() && getMaster()->getMonster()->getIdleStatus())) {
				idle = true;
			}
		} else if (targetList.empty()) {
			idle = true;
		}
	}

	setIdle(idle);
}

void Monster::onAddCondition(ConditionType_t type)
{
	if (type == CONDITION_FIRE || type == CONDITION_ENERGY || type == CONDITION_POISON) {
		updateMapCache();
	}

	updateIdleStatus();
}

void Monster::onEndCondition(ConditionType_t type)
{
	if (type == CONDITION_FIRE || type == CONDITION_ENERGY || type == CONDITION_POISON) {
		updateMapCache();
	}

	updateIdleStatus();
}

void Monster::onThink(uint32_t interval)
{
	Creature::onThink(interval);

	if (mType->thinkEvent != -1) {
		// onThink(self, interval)
		LuaScriptInterface* scriptInterface = mType->scriptInterface;
		if (!scriptInterface->reserveScriptEnv()) {
			std::cout << "[Error - Monster::onThink] Call stack overflow" << std::endl;
			return;
		}

		ScriptEnvironment* env = scriptInterface->getScriptEnv();
		env->setScriptId(mType->thinkEvent, scriptInterface);

		lua_State* L = scriptInterface->getLuaState();
		scriptInterface->pushFunction(mType->thinkEvent);

		LuaScriptInterface::pushUserdata<Monster>(L, this);
		LuaScriptInterface::setMetatable(L, -1, "Monster");

		lua_pushnumber(L, interval);

		if (scriptInterface->callFunction(2)) {
			return;
		}
	}

	if (despawn()) {
		g_game.internalTeleport(this, masterPos);
		setIdle(true);
	} else {
		updateIdleStatus();

		if (!isIdle) {
			addEventWalk();

			if (isSummon()) {
				if (!attackedCreature) {
					if (getMaster() && getMaster()->getAttackedCreature()) {
						//This happens if the monster is summoned during combat
						selectTarget(getMaster()->getAttackedCreature());
					} else if (getMaster() != followCreature) {
						//Our master has not ordered us to attack anything, lets follow him around instead.
						setFollowCreature(getMaster());
					}
				} else if (attackedCreature == this) {
					setFollowCreature(nullptr);
				} else if (followCreature != attackedCreature) {
					//This happens just after a master orders an attack, so lets follow it aswell.
					setFollowCreature(attackedCreature);
				}
			} else if (!targetList.empty()) {
				if (!followCreature || !hasFollowPath) {
					searchTarget();
				} else if (isFleeing()) {
					if (attackedCreature && !canUseAttack(getPosition(), attackedCreature)) {
						searchTarget(TARGETSEARCH_ATTACKRANGE);
					}
				}
			}

			onThinkTarget(interval);
			onThinkYell(interval);
			onThinkDefense(interval);
		}
	}
}

void Monster::doAttacking(uint32_t interval)
{
	if (!attackedCreature || (isSummon() && attackedCreature == this)) {
		return;
	}

	bool updateLook = true;

	resetTicks = interval != 0;
	attackTicks += interval;

	const Position& myPos = getPosition();
	const Position& targetPos = attackedCreature->getPosition();

	for (const spellBlock_t& spellBlock : mType->spellAttackList) {
		bool inRange = false;

		if (canUseSpell(myPos, targetPos, spellBlock, interval, inRange)) {
			if (spellBlock.chance >= (uint32_t)uniform_random(1, 100)) {
				if (updateLook) {
					updateLookDirection();
					updateLook = false;
				}

				minCombatValue = spellBlock.minCombatValue;
				maxCombatValue = spellBlock.maxCombatValue;
				spellBlock.spell->castSpell(this, attackedCreature);
				if (!attackedCreature) {
					break;
				}

				if (spellBlock.isMelee) {
					extraMeleeAttack = false;
				}
			}
		}

		if (!inRange && spellBlock.isMelee) {
			//melee swing out of reach
			extraMeleeAttack = true;
		}
	}

	if (updateLook) {
		updateLookDirection();
	}

	if (resetTicks) {
		attackTicks = 0;
	}
}

bool Monster::canUseAttack(const Position& pos, const Creature* target) const
{
	if (isHostile()) {
		const Position& targetPos = target->getPosition();
		uint32_t distance = std::max<uint32_t>(Position::getDistanceX(pos, targetPos), Position::getDistanceY(pos, targetPos));
		for (const spellBlock_t& spellBlock : mType->spellAttackList) {
			if (spellBlock.range != 0 && distance <= spellBlock.range) {
				return g_game.isSightClear(pos, targetPos, true);
			}
		}
		return false;
	}
	return true;
}

bool Monster::canUseSpell(const Position& pos, const Position& targetPos,
                          const spellBlock_t& sb, uint32_t interval, bool& inRange)
{
	inRange = true;

	if (sb.isMelee && isFleeing()) {
		return false;
	}

	if (extraMeleeAttack) {
		lastMeleeAttack = OTSYS_TIME();
	} else if (sb.isMelee && (OTSYS_TIME() - lastMeleeAttack) < 1500) {
		return false;
	}

	if (!sb.isMelee || !extraMeleeAttack) {
		if (sb.speed > attackTicks) {
			resetTicks = false;
			return false;
		}

		if (attackTicks % sb.speed >= interval) {
			//already used this spell for this round
			return false;
		}
	}

	if (sb.range != 0 && std::max<uint32_t>(Position::getDistanceX(pos, targetPos), Position::getDistanceY(pos, targetPos)) > sb.range) {
		inRange = false;
		return false;
	}
	return true;
}

void Monster::onThinkTarget(uint32_t interval)
{
	if (!isSummon()) {
		if (mType->changeTargetSpeed > 0) {
			bool canChangeTarget = true;

			if (targetChangeCooldown > 0) {
				targetChangeCooldown -= interval;

				if (targetChangeCooldown <= 0) {
					targetChangeCooldown = 0;
					targetChangeTicks = (uint32_t)mType->changeTargetSpeed;
				} else {
					canChangeTarget = false;
				}
			}

			if (canChangeTarget) {
				targetChangeTicks += interval;

				if (targetChangeTicks >= (uint32_t)mType->changeTargetSpeed) {
					targetChangeTicks = 0;
					targetChangeCooldown = (uint32_t)mType->changeTargetSpeed;

					if (mType->changeTargetChance >= uniform_random(1, 100)) {
						if (mType->targetDistance <= 1) {
							searchTarget(TARGETSEARCH_RANDOM);
						} else {
							searchTarget(TARGETSEARCH_NEAREAST);
						}
					}
				}
			}
		}
	}
}

void Monster::onThinkDefense(uint32_t interval)
{
	resetTicks = true;
	defenseTicks += interval;

	for (const spellBlock_t& spellBlock : mType->spellDefenseList) {
		if (spellBlock.speed > defenseTicks) {
			resetTicks = false;
			continue;
		}

		if (defenseTicks % spellBlock.speed >= interval) {
			//already used this spell for this round
			continue;
		}

		if ((spellBlock.chance >= (uint32_t)uniform_random(1, 100))) {
			minCombatValue = spellBlock.minCombatValue;
			maxCombatValue = spellBlock.maxCombatValue;
			spellBlock.spell->castSpell(this, this);
		}
	}

	if (!isSummon() && summons.size() < mType->maxSummons) {
		for (const summonBlock_t& summonBlock : mType->summonList) {
			if (summonBlock.speed > defenseTicks) {
				resetTicks = false;
				continue;
			}

			if (summons.size() >= mType->maxSummons) {
				continue;
			}

			if (defenseTicks % summonBlock.speed >= interval) {
				//already used this spell for this round
				continue;
			}

			if (summonBlock.chance < (uint32_t)uniform_random(1, 100)) {
				continue;
			}

			Monster* summon = Monster::createMonster(summonBlock.name);
			if (summon) {
				const Position& summonPos = getPosition();

				addSummon(summon);

				if (!g_game.placeCreature(summon, summonPos)) {
					removeSummon(summon);
				} else {
					g_game.addMagicEffect(getPosition(), CONST_ME_MAGIC_BLUE);
					g_game.addMagicEffect(summon->getPosition(), CONST_ME_TELEPORT);
				}
			}
		}
	}

	if (resetTicks) {
		defenseTicks = 0;
	}
}

void Monster::onThinkYell(uint32_t interval)
{
	if (mType->yellSpeedTicks == 0) {
		return;
	}

	yellTicks += interval;
	if (yellTicks >= mType->yellSpeedTicks) {
		yellTicks = 0;

		if (!mType->voiceVector.empty() && (mType->yellChance >= (uint32_t)uniform_random(1, 100))) {
			uint32_t index = uniform_random(0, mType->voiceVector.size() - 1);
			const voiceBlock_t& vb = mType->voiceVector[index];

			if (vb.yellText) {
				g_game.internalCreatureSay(this, TALKTYPE_MONSTER_YELL, vb.text, false);
			} else {
				g_game.internalCreatureSay(this, TALKTYPE_MONSTER_SAY, vb.text, false);
			}
		}
	}
}

void Monster::onWalk()
{
	Creature::onWalk();
}

bool Monster::pushItem(Item* item)
{
	const Position& centerPos = item->getPosition();

	static std::vector<std::pair<int32_t, int32_t>> relList {
		{-1, -1}, {0, -1}, {1, -1},
		{-1,  0},          {1,  0},
		{-1,  1}, {0,  1}, {1,  1}
	};

	std::shuffle(relList.begin(), relList.end(), getRandomGenerator());

	for (const auto& it : relList) {
		Position tryPos(centerPos.x + it.first, centerPos.y + it.second, centerPos.z);
		Tile* tile = g_game.getTile(tryPos);
		if (tile && g_game.canThrowObjectTo(centerPos, tryPos)) {
			if (g_game.internalMoveItem(item->getParent(), tile, INDEX_WHEREEVER, item, item->getItemCount(), nullptr) == RET_NOERROR) {
				return true;
			}
		}
	}
	return false;
}

void Monster::pushItems(Tile* tile)
{
	//We can not use iterators here since we can push the item to another tile
	//which will invalidate the iterator.
	//start from the end to minimize the amount of traffic
	if (TileItemVector* items = tile->getItemList()) {
		uint32_t moveCount = 0;
		uint32_t removeCount = 0;

		int32_t downItemSize = tile->getDownItemCount();
		for (int32_t i = downItemSize; --i >= 0;) {
			Item* item = items->at(i);
			if (item && item->hasProperty(CONST_PROP_MOVEABLE) && (item->hasProperty(CONST_PROP_BLOCKPATH)
			        || item->hasProperty(CONST_PROP_BLOCKSOLID))) {
				if (moveCount < 20 && pushItem(item)) {
					++moveCount;
				} else if (g_game.internalRemoveItem(item) == RET_NOERROR) {
					++removeCount;
				}
			}
		}

		if (removeCount > 0) {
			g_game.addMagicEffect(tile->getPosition(), CONST_ME_POFF);
		}
	}
}

bool Monster::pushCreature(Creature* creature)
{
	static std::vector<Direction> dirList {
		     NORTH,
		WEST,      EAST,
		     SOUTH
	};
	std::shuffle(dirList.begin(), dirList.end(), getRandomGenerator());

	for (Direction dir : dirList) {
		const Position& tryPos = Spells::getCasterPosition(creature, dir);
		Tile* toTile = g_game.getTile(tryPos.x, tryPos.y, tryPos.z);
		if (toTile && !toTile->hasProperty(CONST_PROP_BLOCKPATH)) {
			if (g_game.internalMoveCreature(creature, dir) == RET_NOERROR) {
				return true;
			}
		}
	}
	return false;
}

void Monster::pushCreatures(Tile* tile)
{
	//We can not use iterators here since we can push a creature to another tile
	//which will invalidate the iterator.
	if (CreatureVector* creatures = tile->getCreatures()) {
		uint32_t removeCount = 0;
		Monster* lastPushedMonster = nullptr;

		for (size_t i = 0; i < creatures->size();) {
			Monster* monster = creatures->at(i)->getMonster();
			if (monster && monster->isPushable()) {
				if (monster != lastPushedMonster && pushCreature(monster)) {
					lastPushedMonster = monster;
					continue;
				}

				monster->changeHealth(-monster->getHealth());
				monster->setDropLoot(false);
				removeCount++;
			}

			++i;
		}

		if (removeCount > 0) {
			g_game.addMagicEffect(tile->getPosition(), CONST_ME_BLOCKHIT);
		}
	}
}

bool Monster::getNextStep(Direction& dir, uint32_t& flags)
{
	if (isIdle || getHealth() <= 0) {
		//we dont have anyone watching might aswell stop walking
		eventWalk = 0;
		return false;
	}

	bool result = false;

	if ((!followCreature || !hasFollowPath) && !isSummon()) {
		if (followCreature || getTimeSinceLastMove() > 1000) {
			//choose a random direction
			result = getRandomStep(getPosition(), dir);
		}
	} else if (isSummon() || followCreature) {
		result = Creature::getNextStep(dir, flags);

		if (result) {
			flags |= FLAG_PATHFINDING;
		} else {
			//target dancing
			if (attackedCreature && attackedCreature == followCreature) {
				if (isFleeing()) {
					result = getDanceStep(getPosition(), dir, false, false);
				} else if (mType->staticAttackChance < (uint32_t)uniform_random(1, 100)) {
					result = getDanceStep(getPosition(), dir);
				}
			}
		}
	}

	if (result && (canPushItems() || canPushCreatures())) {
		const Position& pos = Spells::getCasterPosition(this, dir);
		Tile* tile = g_game.getTile(pos.x, pos.y, pos.z);

		if (tile) {
			if (canPushItems()) {
				pushItems(tile);
			}

			if (canPushCreatures()) {
				pushCreatures(tile);
			}
		}
	}

	return result;
}

bool Monster::getRandomStep(const Position& creaturePos, Direction& dir)
{
	static std::vector<Direction> dirList {
		     NORTH,
		WEST,      EAST,
		     SOUTH
	};
	std::shuffle(dirList.begin(), dirList.end(), getRandomGenerator());

	for (Direction _dir : dirList) {
		if (canWalkTo(creaturePos, _dir)) {
			dir = _dir;
			return true;
		}
	}
	return false;
}

bool Monster::getDanceStep(const Position& creaturePos, Direction& dir,
                           bool keepAttack /*= true*/, bool keepDistance /*= true*/)
{
	bool canDoAttackNow = canUseAttack(creaturePos, attackedCreature);

	assert(attackedCreature != nullptr);
	const Position& centerPos = attackedCreature->getPosition();

	int_fast32_t offset_x = Position::getOffsetX(creaturePos, centerPos);
	int_fast32_t offset_y = Position::getOffsetY(creaturePos, centerPos);

	int_fast32_t distance_x = std::abs(offset_x);
	int_fast32_t distance_y = std::abs(offset_y);

	uint32_t centerToDist = std::max<uint32_t>(distance_x, distance_y);

	std::vector<Direction> dirList;

	if (!keepDistance || offset_y >= 0) {
		uint32_t tmpDist = std::max<uint32_t>(distance_x, std::abs((creaturePos.getY() - 1) - centerPos.getY()));
		if (tmpDist == centerToDist && canWalkTo(creaturePos, NORTH)) {
			bool result = true;

			if (keepAttack) {
				result = (!canDoAttackNow || canUseAttack(Position(creaturePos.x, creaturePos.y - 1, creaturePos.z), attackedCreature));
			}

			if (result) {
				dirList.push_back(NORTH);
			}
		}
	}

	if (!keepDistance || offset_y <= 0) {
		uint32_t tmpDist = std::max<uint32_t>(distance_x, std::abs((creaturePos.getY() + 1) - centerPos.getY()));
		if (tmpDist == centerToDist && canWalkTo(creaturePos, SOUTH)) {
			bool result = true;

			if (keepAttack) {
				result = (!canDoAttackNow || canUseAttack(Position(creaturePos.x, creaturePos.y + 1, creaturePos.z), attackedCreature));
			}

			if (result) {
				dirList.push_back(SOUTH);
			}
		}
	}

	if (!keepDistance || offset_x <= 0) {
		uint32_t tmpDist = std::max<uint32_t>(std::abs((creaturePos.getX() + 1) - centerPos.getX()), distance_y);
		if (tmpDist == centerToDist && canWalkTo(creaturePos, EAST)) {
			bool result = true;

			if (keepAttack) {
				result = (!canDoAttackNow || canUseAttack(Position(creaturePos.x + 1, creaturePos.y, creaturePos.z), attackedCreature));
			}

			if (result) {
				dirList.push_back(EAST);
			}
		}
	}

	if (!keepDistance || offset_x >= 0) {
		uint32_t tmpDist = std::max<uint32_t>(std::abs((creaturePos.getX() - 1) - centerPos.getX()), distance_y);
		if (tmpDist == centerToDist && canWalkTo(creaturePos, WEST)) {
			bool result = true;

			if (keepAttack) {
				result = (!canDoAttackNow || canUseAttack(Position(creaturePos.x - 1, creaturePos.y, creaturePos.z), attackedCreature));
			}

			if (result) {
				dirList.push_back(WEST);
			}
		}
	}

	if (!dirList.empty()) {
		std::shuffle(dirList.begin(), dirList.end(), getRandomGenerator());
		dir = dirList[uniform_random(0, dirList.size() - 1)];
		return true;
	}
	return false;
}

bool Monster::getDistanceStep(const Position& targetPos, Direction& dir, bool flee /* = false */)
{
	const Position& creaturePos = getPosition();

	int_fast32_t dx = Position::getDistanceX(creaturePos, targetPos);
	int_fast32_t dy = Position::getDistanceY(creaturePos, targetPos);

	int32_t distance = std::max<int32_t>(dx, dy);

	if (!flee && (distance > mType->targetDistance || !g_game.isSightClear(creaturePos, targetPos, true))) {
		return false;    // let the A* calculate it
	} else if (!flee && distance == mType->targetDistance) {
		return true;    // we don't really care here, since it's what we wanted to reach (a dancestep will take of dancing in that position)
	}

	int_fast32_t offsetx = Position::getOffsetX(creaturePos, targetPos);
	int_fast32_t offsety = Position::getOffsetY(creaturePos, targetPos);

	if (dx <= 1 && dy <= 1) {
		//seems like a target is near, it this case we need to slow down our movements (as a monster)
		if (stepDuration < 2) {
			stepDuration++;
		}
	} else if (stepDuration > 0) {
		stepDuration--;
	}

	if (offsetx == 0 && offsety == 0) {
		return getRandomStep(creaturePos, dir);    // player is "on" the monster so let's get some random step and rest will be taken care later.
	}

	if (dx == dy) {
		//player is diagonal to the monster
		if (offsetx >= 1 && offsety >= 1) {
			// player is NW
			//escape to SE, S or E [and some extra]
			bool s = canWalkTo(creaturePos, SOUTH);
			bool e = canWalkTo(creaturePos, EAST);

			if (s && e) {
				dir = boolean_random() ? SOUTH : EAST;
				return true;
			} else if (s) {
				dir = SOUTH;
				return true;
			} else if (e) {
				dir = EAST;
				return true;
			} else if (canWalkTo(creaturePos, SOUTHEAST)) {
				dir = SOUTHEAST;
				return true;
			}

			/* fleeing */
			bool n = canWalkTo(creaturePos, NORTH);
			bool w = canWalkTo(creaturePos, WEST);

			if (flee) {
				if (n && w) {
					dir = boolean_random() ? NORTH : WEST;
					return true;
				} else if (n) {
					dir = NORTH;
					return true;
				} else if (w) {
					dir = WEST;
					return true;
				}
			}

			/* end of fleeing */

			if (w && canWalkTo(creaturePos, SOUTHWEST)) {
				dir = WEST;
			} else if (n && canWalkTo(creaturePos, NORTHEAST)) {
				dir = NORTH;
			}

			return true;
		} else if (offsetx <= -1 && offsety <= -1) {
			//player is SE
			//escape to NW , W or N [and some extra]
			bool w = canWalkTo(creaturePos, WEST);
			bool n = canWalkTo(creaturePos, NORTH);

			if (w && n) {
				dir = boolean_random() ? WEST : NORTH;
				return true;
			} else if (w) {
				dir = WEST;
				return true;
			} else if (n) {
				dir = NORTH;
				return true;
			}

			if (canWalkTo(creaturePos, NORTHWEST)) {
				dir = NORTHWEST;
				return true;
			}

			/* fleeing */
			bool s = canWalkTo(creaturePos, SOUTH);
			bool e = canWalkTo(creaturePos, EAST);

			if (flee) {
				if (s && e) {
					dir = boolean_random() ? SOUTH : EAST;
					return true;
				} else if (s) {
					dir = SOUTH;
					return true;
				} else if (e) {
					dir = EAST;
					return true;
				}
			}

			/* end of fleeing */

			if (s && canWalkTo(creaturePos, SOUTHWEST)) {
				dir = SOUTH;
			} else if (e && canWalkTo(creaturePos, NORTHEAST)) {
				dir = EAST;
			}

			return true;
		} else if (offsetx >= 1 && offsety <= -1) {
			//player is SW
			//escape to NE, N, E [and some extra]
			bool n = canWalkTo(creaturePos, NORTH);
			bool e = canWalkTo(creaturePos, EAST);

			if (n && e) {
				dir = boolean_random() ? NORTH : EAST;
				return true;
			} else if (n) {
				dir = NORTH;
				return true;
			} else if (e) {
				dir = EAST;
				return true;
			}

			if (canWalkTo(creaturePos, NORTHEAST)) {
				dir = NORTHEAST;
				return true;
			}

			/* fleeing */
			bool s = canWalkTo(creaturePos, SOUTH);
			bool w = canWalkTo(creaturePos, WEST);

			if (flee) {
				if (s && w) {
					dir = boolean_random() ? SOUTH : WEST;
					return true;
				} else if (s) {
					dir = SOUTH;
					return true;
				} else if (w) {
					dir = WEST;
					return true;
				}
			}

			/* end of fleeing */

			if (w && canWalkTo(creaturePos, NORTHWEST)) {
				dir = WEST;
			} else if (s && canWalkTo(creaturePos, SOUTHEAST)) {
				dir = SOUTH;
			}

			return true;
		} else if (offsetx <= -1 && offsety >= 1) {
			// player is NE
			//escape to SW, S, W [and some extra]
			bool w = canWalkTo(creaturePos, WEST);
			bool s = canWalkTo(creaturePos, SOUTH);

			if (w && s) {
				dir = boolean_random() ? WEST : SOUTH;
				return true;
			} else if (w) {
				dir = WEST;
				return true;
			} else if (s) {
				dir = SOUTH;
				return true;
			} else if (canWalkTo(creaturePos, SOUTHWEST)) {
				dir = SOUTHWEST;
				return true;
			}

			/* fleeing */
			bool n = canWalkTo(creaturePos, NORTH);
			bool e = canWalkTo(creaturePos, EAST);

			if (flee) {
				if (n && e) {
					dir = boolean_random() ? NORTH : EAST;
					return true;
				} else if (n) {
					dir = NORTH;
					return true;
				} else if (e) {
					dir = EAST;
					return true;
				}
			}

			/* end of fleeing */

			if (e && canWalkTo(creaturePos, SOUTHEAST)) {
				dir = EAST;
			} else if (n && canWalkTo(creaturePos, NORTHWEST)) {
				dir = NORTH;
			}

			return true;
		}
	}

	//Now let's decide where the player is located to the monster (what direction) so we can decide where to escape.
	if (dy > dx) {
		Direction playerDir = offsety < 0 ? SOUTH : NORTH;
		switch (playerDir) {
			case NORTH: {
				// Player is to the NORTH, so obviously we need to check if we can go SOUTH, if not then let's choose WEST or EAST and again if we can't we need to decide about some diagonal movements.
				if (canWalkTo(creaturePos, SOUTH)) {
					dir = SOUTH;
					return true;
				}

				bool w = canWalkTo(creaturePos, WEST);
				bool e = canWalkTo(creaturePos, EAST);
				if (w && e && offsetx == 0) {
					dir = boolean_random() ? WEST : EAST;
					return true;
				} else if (w && offsetx <= 0) {
					dir = WEST;
					return true;
				} else if (e && offsetx >= 0) {
					dir = EAST;
					return true;
				}

				/* fleeing */
				if (flee) {
					if (w && e) {
						dir = boolean_random() ? WEST : EAST;
						return true;
					} else if (w) {
						dir = WEST;
						return true;
					} else if (e) {
						dir = EAST;
						return true;
					}
				}

				/* end of fleeing */

				bool sw = canWalkTo(creaturePos, SOUTHWEST);
				bool se = canWalkTo(creaturePos, SOUTHEAST);
				if (sw || se) {
					// we can move both dirs
					if (sw && se) {
						dir = boolean_random() ? SOUTHWEST : SOUTHEAST;
						return true;
					} else if (sw && !w) {
						dir = SOUTHWEST;
						return true;
					} else if (w) {
						dir = WEST;
						return true;
					} else if (se && !e) {
						dir = SOUTHEAST;
						return true;
					} else if (e) {
						dir = EAST;
						return true;
					}
				}

				/* fleeing */
				if (flee && canWalkTo(creaturePos, NORTH)) {
					// towards player, yea
					dir = NORTH;
					return true;
				}

				/* end of fleeing */
				break;
			}

			case SOUTH: {
				if (canWalkTo(creaturePos, NORTH)) {
					dir = NORTH;
					return true;
				}

				bool w = canWalkTo(creaturePos, WEST);
				bool e = canWalkTo(creaturePos, EAST);
				if (w && e && offsetx == 0) {
					dir = boolean_random() ? WEST : EAST;
					return true;
				} else if (w && offsetx <= 0) {
					dir = WEST;
					return true;
				} else if (e && offsetx >= 0) {
					dir = EAST;
					return true;
				}

				/* fleeing */
				if (flee) {
					if (w && e) {
						dir = boolean_random() ? WEST : EAST;
						return true;
					} else if (w) {
						dir = WEST;
						return true;
					} else if (e) {
						dir = EAST;
						return true;
					}
				}

				/* end of fleeing */

				bool nw = canWalkTo(creaturePos, NORTHWEST);
				bool ne = canWalkTo(creaturePos, NORTHEAST);
				if (nw || ne) {
					// we can move both dirs
					if (nw && ne) {
						dir = boolean_random() ? NORTHWEST : NORTHEAST;
						return true;
					} else if (nw && !w) {
						dir = NORTHWEST;
						return true;
					} else if (w) {
						dir = WEST;
						return true;
					} else if (ne && !e) {
						dir = NORTHEAST;
						return true;
					} else if (e) {
						dir = EAST;
						return true;
					}
				}

				/* fleeing */
				if (flee && canWalkTo(creaturePos, SOUTH)) {
					// towards player, yea
					dir = SOUTH;
					return true;
				}

				/* end of fleeing */
				break;
			}

			default:
				break;
		}
	} else {
		Direction playerDir = offsetx < 0 ? EAST : WEST;
		switch (playerDir) {
			case WEST: {
				if (canWalkTo(creaturePos, EAST)) {
					dir = EAST;
					return true;
				}

				bool n = canWalkTo(creaturePos, NORTH);
				bool s = canWalkTo(creaturePos, SOUTH);
				if (n && s && offsety == 0) {
					dir = boolean_random() ? NORTH : SOUTH;
					return true;
				} else if (n && offsety <= 0) {
					dir = NORTH;
					return true;
				} else if (s && offsety >= 0) {
					dir = SOUTH;
					return true;
				}

				/* fleeing */
				if (flee) {
					if (n && s) {
						dir = boolean_random() ? NORTH : SOUTH;
						return true;
					} else if (n) {
						dir = NORTH;
						return true;
					} else if (s) {
						dir = SOUTH;
						return true;
					}
				}

				/* end of fleeing */

				bool se = canWalkTo(creaturePos, SOUTHEAST);
				bool ne = canWalkTo(creaturePos, NORTHEAST);
				if (se || ne) {
					if (se && ne) {
						dir = boolean_random() ? SOUTHEAST : NORTHEAST;
						return true;
					} else if (se && !s) {
						dir = SOUTHEAST;
						return true;
					} else if (s) {
						dir = SOUTH;
						return true;
					} else if (ne && !n) {
						dir = NORTHEAST;
						return true;
					} else if (n) {
						dir = NORTH;
						return true;
					}
				}

				/* fleeing */
				if (flee && canWalkTo(creaturePos, WEST)) {
					// towards player, yea
					dir = WEST;
					return true;
				}

				/* end of fleeing */
				break;
			}

			case EAST: {
				if (canWalkTo(creaturePos, WEST)) {
					dir = WEST;
					return true;
				}

				bool n = canWalkTo(creaturePos, NORTH);
				bool s = canWalkTo(creaturePos, SOUTH);
				if (n && s && offsety == 0) {
					dir = boolean_random() ? NORTH : SOUTH;
					return true;
				} else if (n && offsety <= 0) {
					dir = NORTH;
					return true;
				} else if (s && offsety >= 0) {
					dir = SOUTH;
					return true;
				}

				/* fleeing */
				if (flee) {
					if (n && s) {
						dir = boolean_random() ? NORTH : SOUTH;
						return true;
					} else if (n) {
						dir = NORTH;
						return true;
					} else if (s) {
						dir = SOUTH;
						return true;
					}
				}

				/* end of fleeing */

				bool nw = canWalkTo(creaturePos, NORTHWEST);
				bool sw = canWalkTo(creaturePos, SOUTHWEST);
				if (nw || sw) {
					if (nw && sw) {
						dir = boolean_random() ? NORTHWEST : SOUTHWEST;
						return true;
					} else if (nw && !n) {
						dir = NORTHWEST;
						return true;
					} else if (n) {
						dir = NORTH;
						return true;
					} else if (sw && !s) {
						dir = SOUTHWEST;
						return true;
					} else if (s) {
						dir = SOUTH;
						return true;
					}
				}

				/* fleeing */
				if (flee && canWalkTo(creaturePos, EAST)) {
					// towards player, yea
					dir = EAST;
					return true;
				}

				/* end of fleeing */
				break;
			}

			default:
				break;
		}
	}

	return true;
}

bool Monster::isInSpawnRange(const Position& toPos)
{
	if (masterRadius == -1) {
		return true;
	}

	return !inDespawnRange(toPos);
}

bool Monster::canWalkTo(Position pos, Direction dir)
{
	pos = getNextPosition(dir, pos);
	if (isInSpawnRange(pos)) {
		if (getWalkCache(pos) == 0) {
			return false;
		}

		Tile* tile = g_game.getTile(pos.x, pos.y, pos.z);
		if (tile && tile->getTopVisibleCreature(this) == nullptr && tile->__queryAdd(0, this, 1, FLAG_PATHFINDING) == RET_NOERROR) {
			return true;
		}
	}
	return false;
}

void Monster::death(Creature*)
{
	setAttackedCreature(nullptr);

	for (Creature* summon : summons) {
		summon->changeHealth(-summon->getHealth());
		summon->setMaster(nullptr);
		summon->releaseThing2();
	}
	summons.clear();

	clearTargetList();
	clearFriendList();
	onIdleStatus();
}

Item* Monster::getCorpse(Creature* _lastHitCreature, Creature* mostDamageCreature)
{
	Item* corpse = Creature::getCorpse(_lastHitCreature, mostDamageCreature);
	if (corpse) {
		if (mostDamageCreature) {
			if (mostDamageCreature->getPlayer()) {
				corpse->setCorpseOwner(mostDamageCreature->getID());
			} else {
				const Creature* mostDamageCreatureMaster = mostDamageCreature->getMaster();
				if (mostDamageCreatureMaster && mostDamageCreatureMaster->getPlayer()) {
					corpse->setCorpseOwner(mostDamageCreatureMaster->getID());
				}
			}
		}
	}

	return corpse;
}

bool Monster::inDespawnRange(const Position& pos)
{
	if (!spawn) {
		return false;
	}

	if (Monster::despawnRadius == 0) {
		return false;
	}

	if (!Spawns::getInstance()->isInZone(masterPos, Monster::despawnRadius, pos)) {
		return true;
	}

	if (Monster::despawnRange == 0) {
		return false;
	}

	if (Position::getDistanceZ(pos, masterPos) > Monster::despawnRange) {
		return true;
	}

	return false;
}

bool Monster::despawn()
{
	return inDespawnRange(getPosition());
}

bool Monster::getCombatValues(int32_t& min, int32_t& max)
{
	if (minCombatValue == 0 && maxCombatValue == 0) {
		return false;
	}

	min = minCombatValue;
	max = maxCombatValue;
	return true;
}

void Monster::updateLookDirection()
{
	Direction newDir = getDirection();

	if (attackedCreature) {
		const Position& pos = getPosition();
		const Position& attackedCreaturePos = attackedCreature->getPosition();
		int_fast32_t offsetx = Position::getOffsetX(attackedCreaturePos, pos);
		int_fast32_t offsety = Position::getOffsetY(attackedCreaturePos, pos);
		int32_t dx = std::abs(offsetx);
		int32_t dy = std::abs(offsety);

		if (dx > dy) {
			//look EAST/WEST
			if (offsetx < 0) {
				newDir = WEST;
			} else {
				newDir = EAST;
			}
		} else if (dx < dy) {
			//look NORTH/SOUTH
			if (offsety < 0) {
				newDir = NORTH;
			} else {
				newDir = SOUTH;
			}
		} else {
			if (offsetx < 0 && offsety < 0) {
				if (getDirection() == SOUTH) {
					newDir = WEST;
				} else if (getDirection() == EAST) {
					newDir = NORTH;
				}
			} else if (offsetx < 0 && offsety > 0) {
				if (getDirection() == NORTH) {
					newDir = WEST;
				} else if (getDirection() == EAST) {
					newDir = SOUTH;
				}
			} else if (offsetx > 0 && offsety < 0) {
				if (getDirection() == SOUTH) {
					newDir = EAST;
				} else if (getDirection() == WEST) {
					newDir = NORTH;
				}
			} else {
				if (getDirection() == NORTH) {
					newDir = EAST;
				} else if (getDirection() == WEST) {
					newDir = SOUTH;
				}
			}
		}
	}

	g_game.internalCreatureTurn(this, newDir);
}

void Monster::dropLoot(Container* corpse, Creature*)
{
	if (corpse && lootDrop) {
		mType->createLoot(corpse);
	}
}

void Monster::setNormalCreatureLight()
{
	internalLight.level = mType->lightLevel;
	internalLight.color = mType->lightColor;
}

void Monster::drainHealth(Creature* attacker, int32_t damage)
{
	Creature::drainHealth(attacker, damage);
	if (isInvisible()) {
		removeCondition(CONDITION_INVISIBLE);
	}
}

void Monster::changeHealth(int32_t healthChange, bool sendHealthChange/* = true*/)
{
	//In case a player with ignore flag set attacks the monster
	setIdle(false);
	Creature::changeHealth(healthChange, sendHealthChange);
}

bool Monster::challengeCreature(Creature* creature)
{
	if (isSummon()) {
		return false;
	}

	bool result = selectTarget(creature);
	if (result) {
		targetChangeCooldown = 8000;
		targetChangeTicks = 0;
	}
	return result;
}

bool Monster::convinceCreature(Creature* creature)
{
	Player* player = creature->getPlayer();
	if (player && !player->hasFlag(PlayerFlag_CanConvinceAll)) {
		if (!mType->isConvinceable) {
			return false;
		}
	}

	if (isSummon()) {
		if (getMaster()->getPlayer()) {
			return false;
		} else if (getMaster() != creature) {
			Creature* oldMaster = getMaster();
			oldMaster->removeSummon(this);
			creature->addSummon(this);

			setFollowCreature(nullptr);
			setAttackedCreature(nullptr);

			//destroy summons
			for (Creature* summon : summons) {
				summon->changeHealth(-summon->getHealth());
				summon->setMaster(nullptr);
				summon->releaseThing2();
			}
			summons.clear();

			isMasterInRange = true;
			updateTargetList();
			updateIdleStatus();

			//Notify surrounding about the change
			SpectatorVec list;
			g_game.getSpectators(list, getPosition(), true);
			g_game.getSpectators(list, creature->getPosition(), true);
			for (Creature* spectator : list) {
				spectator->onCreatureConvinced(creature, this);
			}

			if (spawn) {
				spawn->removeMonster(this);
				spawn = nullptr;
				masterRadius = -1;
			}

			return true;
		}
	} else {
		creature->addSummon(this);
		setFollowCreature(nullptr);
		setAttackedCreature(nullptr);

		for (Creature* summon : summons) {
			summon->changeHealth(-summon->getHealth());
			summon->setMaster(nullptr);
			summon->releaseThing2();
		}
		summons.clear();

		isMasterInRange = true;
		updateTargetList();
		updateIdleStatus();

		//Notify surrounding about the change
		SpectatorVec list;
		g_game.getSpectators(list, getPosition(), true);
		g_game.getSpectators(list, creature->getPosition(), true);
		for (Creature* spectator : list) {
			spectator->onCreatureConvinced(creature, this);
		}

		if (spawn) {
			spawn->removeMonster(this);
			spawn = nullptr;
			masterRadius = -1;
		}

		return true;
	}

	return false;
}

void Monster::onCreatureConvinced(const Creature* convincer, const Creature* creature)
{
	if (convincer != this && (isFriend(creature) || isOpponent(creature))) {
		updateTargetList();
		updateIdleStatus();
	}
}

void Monster::getPathSearchParams(const Creature* creature, FindPathParams& fpp) const
{
	Creature::getPathSearchParams(creature, fpp);

	fpp.minTargetDist = 1;
	fpp.maxTargetDist = mType->targetDistance;

	if (isSummon()) {
		if (getMaster() == creature) {
			fpp.maxTargetDist = 2;
			fpp.fullPathSearch = true;
		} else if (mType->targetDistance <= 1) {
			fpp.fullPathSearch = true;
		} else {
			fpp.fullPathSearch = !canUseAttack(getPosition(), creature);
		}
	} else if (isFleeing()) {
		//Distance should be higher than the client view range (Map::maxClientViewportX/Map::maxClientViewportY)
		fpp.maxTargetDist = Map::maxViewportX;
		fpp.clearSight = false;
		fpp.keepDistance = true;
		fpp.fullPathSearch = false;
	} else if (mType->targetDistance <= 1) {
		fpp.fullPathSearch = true;
	} else {
		fpp.fullPathSearch = !canUseAttack(getPosition(), creature);
	}
}
