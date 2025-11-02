// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "otpch.h"

#include "monster.h"

#include "condition.h"
#include "configmanager.h"
#include "events.h"
#include "game.h"
#include "spectators.h"
#include "spells.h"
#include "tasks.h"

extern Game g_game;
extern Monsters g_monsters;

int32_t Monster::despawnRange;
int32_t Monster::despawnRadius;

uint32_t Monster::monsterAutoID = 0x21000000;

std::shared_ptr<Monster> Monster::createMonster(const std::string& name)
{
	MonsterType* mType = g_monsters.getMonsterType(name);
	if (!mType) {
		return nullptr;
	}
	return std::make_shared<Monster>(mType);
}

Monster::Monster(MonsterType* mType) : Creature(), nameDescription(mType->nameDescription), mType(mType)
{
	defaultOutfit = mType->info.outfit;
	currentOutfit = mType->info.outfit;
	skull = mType->info.skull;
	health = mType->info.health;
	healthMax = mType->info.healthMax;
	baseSpeed = mType->info.baseSpeed;
	internalLight = mType->info.light;
	hiddenHealth = mType->info.hiddenHealth;

	// register creature events
	for (const std::string& scriptName : mType->info.scripts) {
		if (!registerCreatureEvent(scriptName)) {
			std::cout << "[Warning - Monster::Monster] Unknown event name: " << scriptName << std::endl;
		}
	}
}

void Monster::addList() { g_game.addMonster(getMonster()); }

void Monster::removeList() { g_game.removeMonster(getMonster()); }

const std::string& Monster::getName() const
{
	if (name.empty()) {
		return mType->name;
	}
	return name;
}

void Monster::setName(const std::string& name)
{
	if (getName() == name) {
		return;
	}

	this->name = name;

	// NOTE: Due to how client caches known creatures, it is not feasible to send creature update to everyone that has
	// ever met it
	g_game.updateKnownCreature(getMonster());
}

const std::string& Monster::getNameDescription() const
{
	if (nameDescription.empty()) {
		return mType->nameDescription;
	}
	return nameDescription;
}

bool Monster::canSee(const Position& pos) const
{
	return Creature::canSee(getPosition(), pos, Map::maxClientViewportX + 1, Map::maxClientViewportX + 1);
}

bool Monster::canWalkOnFieldType(CombatType_t combatType) const
{
	switch (combatType) {
		case COMBAT_ENERGYDAMAGE:
			return mType->info.canWalkOnEnergy;
		case COMBAT_FIREDAMAGE:
			return mType->info.canWalkOnFire;
		case COMBAT_EARTHDAMAGE:
			return mType->info.canWalkOnPoison;
		default:
			return true;
	}
}

void Monster::onAttackedCreatureDisappear(bool) { attackTicks = 0; }

void Monster::onCreatureAppear(const std::shared_ptr<Creature>& creature, bool isLogin)
{
	Creature::onCreatureAppear(creature, isLogin);

	if (mType->info.creatureAppearEvent != -1) {
		// onCreatureAppear(self, creature)
		LuaScriptInterface* scriptInterface = mType->info.scriptInterface;
		if (!tfs::lua::reserveScriptEnv()) {
			std::cout << "[Error - Monster::onCreatureAppear] Call stack overflow" << std::endl;
			return;
		}

		ScriptEnvironment* env = tfs::lua::getScriptEnv();
		env->setScriptId(mType->info.creatureAppearEvent, scriptInterface);

		lua_State* L = scriptInterface->getLuaState();
		scriptInterface->pushFunction(mType->info.creatureAppearEvent);

		tfs::lua::pushSharedPtr(L, getMonster());
		tfs::lua::setMetatable(L, -1, "Monster");

		tfs::lua::pushSharedPtr(L, creature);
		tfs::lua::setCreatureMetatable(L, -1, creature);

		if (scriptInterface->callFunction(2)) {
			return;
		}
	}

	if (creature.get() == this) {
		// We just spawned lets look around to see who is there.
		if (isSummon()) {
			isMasterInRange = canSee(getMaster()->getPosition());
		}

		updateTargetList();
		updateIdleStatus();
	} else {
		onCreatureEnter(creature);
	}
}

void Monster::onRemoveCreature(const std::shared_ptr<Creature>& creature, bool isLogout)
{
	Creature::onRemoveCreature(creature, isLogout);

	if (mType->info.creatureDisappearEvent != -1) {
		// onCreatureDisappear(self, creature)
		LuaScriptInterface* scriptInterface = mType->info.scriptInterface;
		if (!tfs::lua::reserveScriptEnv()) {
			std::cout << "[Error - Monster::onCreatureDisappear] Call stack overflow" << std::endl;
			return;
		}

		ScriptEnvironment* env = tfs::lua::getScriptEnv();
		env->setScriptId(mType->info.creatureDisappearEvent, scriptInterface);

		lua_State* L = scriptInterface->getLuaState();
		scriptInterface->pushFunction(mType->info.creatureDisappearEvent);

		tfs::lua::pushSharedPtr(L, getMonster());
		tfs::lua::setMetatable(L, -1, "Monster");

		tfs::lua::pushSharedPtr(L, creature);
		tfs::lua::setCreatureMetatable(L, -1, creature);

		if (scriptInterface->callFunction(2)) {
			return;
		}
	}

	if (creature.get() == this) {
		if (spawn) {
			spawn->startSpawnCheck();
		}

		setIdle(true);
	} else {
		onCreatureLeave(creature);
	}
}

void Monster::onCreatureMove(const std::shared_ptr<Creature>& creature, const std::shared_ptr<const Tile>& newTile,
                             const Position& newPos, const std::shared_ptr<const Tile>& oldTile, const Position& oldPos,
                             bool teleport)
{
	Creature::onCreatureMove(creature, newTile, newPos, oldTile, oldPos, teleport);

	if (mType->info.creatureMoveEvent != -1) {
		// onCreatureMove(self, creature, oldPosition, newPosition)
		LuaScriptInterface* scriptInterface = mType->info.scriptInterface;
		if (!tfs::lua::reserveScriptEnv()) {
			std::cout << "[Error - Monster::onCreatureMove] Call stack overflow" << std::endl;
			return;
		}

		ScriptEnvironment* env = tfs::lua::getScriptEnv();
		env->setScriptId(mType->info.creatureMoveEvent, scriptInterface);

		lua_State* L = scriptInterface->getLuaState();
		scriptInterface->pushFunction(mType->info.creatureMoveEvent);

		tfs::lua::pushSharedPtr(L, getMonster());
		tfs::lua::setMetatable(L, -1, "Monster");

		tfs::lua::pushSharedPtr(L, creature);
		tfs::lua::setCreatureMetatable(L, -1, creature);

		tfs::lua::pushPosition(L, oldPos);
		tfs::lua::pushPosition(L, newPos);

		if (scriptInterface->callFunction(4)) {
			return;
		}
	}

	if (creature.get() == this) {
		if (isSummon()) {
			isMasterInRange = canSee(getMaster()->getPosition());
		}

		updateTargetList();
		updateIdleStatus();
	} else {
		bool canSeeNewPos = canSee(newPos);
		bool canSeeOldPos = canSee(oldPos);

		if (canSeeNewPos && !canSeeOldPos) {
			onCreatureEnter(creature);
		} else if (!canSeeNewPos && canSeeOldPos) {
			onCreatureLeave(creature);
		}

		if (canSeeNewPos && isSummon() && getMaster() == creature) {
			isMasterInRange = true; // Follow master again
		}

		updateIdleStatus();

		if (!isSummon()) {
			if (followCreature) {
				const Position& followPosition = followCreature->getPosition();
				const Position& position = getPosition();

				int32_t offset_x = followPosition.getDistanceX(position);
				int32_t offset_y = followPosition.getDistanceY(position);
				if ((offset_x > 1 || offset_y > 1) && mType->info.changeTargetChance > 0) {
					Direction dir = getDirectionTo(position, followPosition);
					const Position& checkPosition = getNextPosition(dir, position);

					if (const auto& tile = g_game.map.getTile(checkPosition)) {
						if (const auto& topCreature = tile->getTopCreature()) {
							if (followCreature != topCreature && isOpponent(topCreature)) {
								selectTarget(topCreature);
							}
						}
					}
				}
			} else if (isOpponent(creature)) {
				// we have no target lets try pick this one
				selectTarget(creature);
			}
		}
	}
}

void Monster::onCreatureSay(const std::shared_ptr<Creature>& creature, SpeakClasses type, const std::string& text)
{
	Creature::onCreatureSay(creature, type, text);

	if (mType->info.creatureSayEvent != -1) {
		// onCreatureSay(self, creature, type, message)
		LuaScriptInterface* scriptInterface = mType->info.scriptInterface;
		if (!tfs::lua::reserveScriptEnv()) {
			std::cout << "[Error - Monster::onCreatureSay] Call stack overflow" << std::endl;
			return;
		}

		ScriptEnvironment* env = tfs::lua::getScriptEnv();
		env->setScriptId(mType->info.creatureSayEvent, scriptInterface);

		lua_State* L = scriptInterface->getLuaState();
		scriptInterface->pushFunction(mType->info.creatureSayEvent);

		tfs::lua::pushSharedPtr(L, getMonster());
		tfs::lua::setMetatable(L, -1, "Monster");

		tfs::lua::pushSharedPtr(L, creature);
		tfs::lua::setCreatureMetatable(L, -1, creature);

		lua_pushnumber(L, type);
		tfs::lua::pushString(L, text);

		scriptInterface->callVoidFunction(4);
	}
}

void Monster::addFriend(const std::shared_ptr<Creature>& creature)
{
	assert(creature.get() != this);
	friendList.insert(creature);
}

void Monster::removeFriend(const std::shared_ptr<Creature>& creature)
{
	auto it = friendList.find(creature);
	if (it != friendList.end()) {
		friendList.erase(it);
	}
}

void Monster::addTarget(const std::shared_ptr<Creature>& creature, bool pushFront /* = false*/)
{
	assert(creature.get() != this);
	if (std::find(targetList.begin(), targetList.end(), creature) == targetList.end()) {
		if (pushFront) {
			targetList.push_front(creature);
		} else {
			targetList.push_back(creature);
		}
	}
}

void Monster::removeTarget(const std::shared_ptr<Creature>& creature)
{
	auto it = std::find(targetList.begin(), targetList.end(), creature);
	if (it != targetList.end()) {
		targetList.erase(it);
	}
}

void Monster::updateTargetList()
{
	for (auto friendIterator = friendList.begin(); friendIterator != friendList.end();) {
		const auto& creature = *friendIterator;
		if (creature->isDead() || !canSee(creature->getPosition())) {
			friendIterator = friendList.erase(friendIterator);
		} else {
			++friendIterator;
		}
	}

	for (auto targetIterator = targetList.begin(); targetIterator != targetList.end();) {
		const auto& creature = *targetIterator;
		if (creature->isDead() || !canSee(creature->getPosition())) {
			targetIterator = targetList.erase(targetIterator);
		} else {
			++targetIterator;
		}
	}

	SpectatorVec spectators;
	g_game.map.getSpectators(spectators, position, true);
	spectators.erase(getMonster());
	for (const auto& spectator : spectators) {
		onCreatureFound(spectator);
	}
}

void Monster::clearTargetList() { targetList.clear(); }

void Monster::clearFriendList() { friendList.clear(); }

void Monster::onCreatureFound(const std::shared_ptr<Creature>& creature, bool pushFront /* = false*/)
{
	if (!creature) {
		return;
	}

	if (!canSee(creature->getPosition())) {
		return;
	}

	if (isFriend(creature)) {
		addFriend(creature);
	}

	if (isOpponent(creature)) {
		addTarget(creature, pushFront);
	}

	updateIdleStatus();
}

void Monster::onCreatureEnter(const std::shared_ptr<Creature>& creature)
{
	// std::cout << "onCreatureEnter - " << creature->getName() << std::endl;

	if (getMaster() == creature) {
		// Follow master again
		isMasterInRange = true;
	}

	onCreatureFound(creature, true);
}

bool Monster::isFriend(const std::shared_ptr<const Creature>& creature) const
{
	if (isSummon() && getMaster()->getPlayer()) {
		const auto& masterPlayer = getMaster()->getPlayer();
		std::shared_ptr<const Player> tmpPlayer = nullptr;

		if (creature->getPlayer()) {
			tmpPlayer = creature->getPlayer();
		} else {
			const auto& creatureMaster = creature->getMaster();

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

bool Monster::isOpponent(const std::shared_ptr<const Creature>& creature) const
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

void Monster::onCreatureLeave(const std::shared_ptr<Creature>& creature)
{
	// std::cout << "onCreatureLeave - " << creature->getName() << std::endl;

	if (getMaster() == creature) {
		// Take random steps and only use defense abilities (e.g. heal) until its master comes back
		isMasterInRange = false;
	}

	// update friendList
	if (isFriend(creature)) {
		removeFriend(creature);
	}

	// update targetList
	if (isOpponent(creature)) {
		removeTarget(creature);
		updateIdleStatus();

		if (!isSummon() && targetList.empty()) {
			int32_t walkToSpawnRadius = getNumber(ConfigManager::DEFAULT_WALKTOSPAWNRADIUS);
			if (walkToSpawnRadius > 0 && !position.isInRange(masterPos, walkToSpawnRadius, walkToSpawnRadius)) {
				walkToSpawn();
			}
		}
	}
}

bool Monster::searchTarget(TargetSearchType_t searchType /*= TARGETSEARCH_DEFAULT*/)
{
	std::list<std::shared_ptr<Creature>> resultList;
	const Position& myPos = getPosition();

	for (const auto& creature : targetList) {
		if (followCreature != creature && isTarget(creature)) {
			if (searchType == TARGETSEARCH_RANDOM || canUseAttack(myPos, creature)) {
				resultList.push_back(creature);
			}
		}
	}

	switch (searchType) {
		case TARGETSEARCH_NEAREST: {
			std::shared_ptr<Creature> target = nullptr;
			if (!resultList.empty()) {
				auto it = resultList.begin();
				target = *it;

				if (++it != resultList.end()) {
					const Position& targetPosition = target->getPosition();
					int32_t minRange = myPos.getDistanceX(targetPosition) + myPos.getDistanceY(targetPosition);
					do {
						const Position& pos = (*it)->getPosition();

						if (int32_t distance = myPos.getDistanceX(pos) + myPos.getDistanceY(pos); distance < minRange) {
							target = *it;
							minRange = distance;
						}
					} while (++it != resultList.end());
				}
			} else {
				int32_t minRange = std::numeric_limits<int32_t>::max();
				for (const auto& creature : targetList) {
					if (!isTarget(creature)) {
						continue;
					}

					const Position& pos = creature->getPosition();
					if (int32_t distance = myPos.getDistanceX(pos) + myPos.getDistanceY(pos); distance < minRange) {
						target = creature;
						minRange = distance;
					}
				}
			}

			if (target && selectTarget(target)) {
				return true;
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

	// lets just pick the first target in the list
	for (const auto& target : targetList) {
		if (followCreature != target && selectTarget(target)) {
			return true;
		}
	}
	return false;
}

void Monster::goToFollowCreature()
{
	if (!followCreature) {
		return;
	}

	FindPathParams fpp;
	getPathSearchParams(followCreature, fpp);

	if (!isSummon()) {
		Direction dir = DIRECTION_NONE;

		if (isFleeing()) {
			getDistanceStep(followCreature->getPosition(), dir, true);
		} else { // maxTargetDist > 1
			if (!getDistanceStep(followCreature->getPosition(), dir)) {
				// if we can't get anything then let the A* calculate
				updateFollowCreaturePath(fpp);
				return;
			}
		}

		if (dir != DIRECTION_NONE) {
			listWalkDir.clear();
			listWalkDir.push_back(dir);

			hasFollowPath = true;
			startAutoWalk();
		}
	} else {
		updateFollowCreaturePath(fpp);
	}

	onFollowCreatureComplete();
}

void Monster::onFollowCreatureComplete()
{
	auto it = std::find(targetList.begin(), targetList.end(), followCreature);
	if (it != targetList.end()) {
		if (hasFollowPath) {
			std::iter_swap(it, targetList.begin());
		} else if (!isSummon()) {
			std::iter_swap(it, targetList.end() - 1);
		} else {
			targetList.erase(it);
		}
	}
}

BlockType_t Monster::blockHit(const std::shared_ptr<Creature>& attacker, CombatType_t combatType, int32_t& damage,
                              bool checkDefense /* = false*/, bool checkArmor /* = false*/, bool /* field = false */,
                              bool /* ignoreResistances = false */)
{
	BlockType_t blockType = Creature::blockHit(attacker, combatType, damage, checkDefense, checkArmor);

	if (damage != 0) {
		int32_t elementMod = 0;
		auto it = mType->info.elementMap.find(combatType);
		if (it != mType->info.elementMap.end()) {
			elementMod = it->second;
		}

		if (elementMod != 0) {
			damage = static_cast<int32_t>(std::round(damage * ((100 - elementMod) / 100.)));
			if (damage <= 0) {
				damage = 0;
				blockType = BLOCK_ARMOR;
			}
		}
	}

	return blockType;
}

bool Monster::isTarget(const std::shared_ptr<const Creature>& creature) const
{
	if (creature->isRemoved() || !creature->isAttackable() || creature->getZone() == ZONE_PROTECTION ||
	    !canSeeCreature(creature)) {
		return false;
	}

	if (creature->getPosition().z != getPosition().z) {
		return false;
	}
	return true;
}

bool Monster::selectTarget(const std::shared_ptr<Creature>& creature)
{
	if (!isTarget(creature)) {
		return false;
	}

	auto it = std::find(targetList.begin(), targetList.end(), creature);
	if (it == targetList.end()) {
		// Target not found in our target list.
		return false;
	}

	if (isHostile() || isSummon()) {
		if (canAttackCreature(creature)) {
			setAttackedCreature(creature);

			if (isHostile()) {
				g_dispatcher.addTask([id = getID()]() { g_game.checkCreatureAttack(id); });
			}
		} else {
			removeAttackedCreature();
		}
	}

	if (isFollowingCreature(creature) || canFollowCreature(creature)) {
		setFollowCreature(creature);
		return true;
	}
	return false;
}

void Monster::setIdle(bool idle)
{
	if (isRemoved() || isDead()) {
		return;
	}

	isIdle = idle;

	if (!isIdle) {
		g_game.addCreatureCheck(getMonster());
	} else {
		onIdleStatus();
		clearTargetList();
		clearFriendList();
		Game::removeCreatureCheck(getMonster());
	}
}

void Monster::updateIdleStatus()
{
	bool idle = false;
	if (!isSummon() && targetList.empty()) {
		// check if there are aggressive conditions
		idle = std::find_if(conditions.begin(), conditions.end(),
		                    [](Condition* condition) { return condition->isAggressive(); }) == conditions.end();
	}

	setIdle(idle);
}

void Monster::onAddCondition(ConditionType_t) { updateIdleStatus(); }

void Monster::onEndCondition(ConditionType_t type)
{
	if (type == CONDITION_FIRE || type == CONDITION_ENERGY || type == CONDITION_POISON) {
		ignoreFieldDamage = false;
	}

	updateIdleStatus();
}

void Monster::onThink(uint32_t interval)
{
	Creature::onThink(interval);

	if (mType->info.thinkEvent != -1) {
		// onThink(self, interval)
		if (!tfs::lua::reserveScriptEnv()) {
			std::cout << "[Error - Monster::onThink] Call stack overflow" << std::endl;
			return;
		}

		LuaScriptInterface* scriptInterface = mType->info.scriptInterface;
		ScriptEnvironment* env = tfs::lua::getScriptEnv();
		env->setScriptId(mType->info.thinkEvent, scriptInterface);

		lua_State* L = scriptInterface->getLuaState();
		scriptInterface->pushFunction(mType->info.thinkEvent);

		tfs::lua::pushSharedPtr(L, getMonster());
		tfs::lua::setMetatable(L, -1, "Monster");

		lua_pushnumber(L, interval);

		if (scriptInterface->callFunction(2)) {
			return;
		}
	}

	if (!isInSpawnRange(position)) {
		if (getBoolean(ConfigManager::MONSTER_OVERSPAWN)) {
			if (spawn) {
				spawn->removeMonster(getMonster());
				spawn->startSpawnCheck();
				spawn = nullptr;
			}
		} else {
			g_game.addMagicEffect(this->getPosition(), CONST_ME_POFF);
			if (getBoolean(ConfigManager::REMOVE_ON_DESPAWN)) {
				g_game.removeCreature(getMonster(), false);
			} else {
				g_game.internalTeleport(getMonster(), masterPos);
				setIdle(true);
			}
		}
	} else {
		updateIdleStatus();

		if (!isIdle) {
			addEventWalk();

			if (isSummon()) {
				if (!attackedCreature) {
					if (getMaster() && getMaster()->getAttackedCreature()) {
						// This happens if the monster is summoned during combat
						selectTarget(getMaster()->getAttackedCreature());
					} else if (getMaster() != followCreature) {
						// Our master has not ordered us to attack anything, lets follow him around instead.
						setFollowCreature(getMaster());
					}
				} else if (attackedCreature.get() == this) {
					removeFollowCreature();
				} else if (followCreature != attackedCreature) {
					// This happens just after a master orders an attack, so lets follow it as well.
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
	if (!attackedCreature || (isSummon() && attackedCreature.get() == this)) {
		return;
	}

	bool lookUpdated = false;
	bool resetTicks = interval != 0;
	attackTicks += interval;

	const Position& myPos = getPosition();
	const Position& targetPos = attackedCreature->getPosition();

	for (const spellBlock_t& spellBlock : mType->info.attackSpells) {
		bool inRange = false;

		if (!attackedCreature) {
			break;
		}

		if (canUseSpell(myPos, targetPos, spellBlock, interval, inRange, resetTicks)) {
			if (spellBlock.chance >= static_cast<uint32_t>(uniform_random(1, 100))) {
				if (!lookUpdated) {
					updateLookDirection();
					lookUpdated = true;
				}

				minCombatValue = spellBlock.minCombatValue;
				maxCombatValue = spellBlock.maxCombatValue;
				spellBlock.spell->castSpell(getMonster(), attackedCreature);

				if (spellBlock.isMelee) {
					lastMeleeAttack = OTSYS_TIME();
				}
			}
		}

		if (!inRange && spellBlock.isMelee) {
			// melee swing out of reach
			lastMeleeAttack = 0;
		}
	}

	// ensure ranged creatures turn to player
	if (!lookUpdated && lastMeleeAttack == 0 && !isFleeing()) {
		updateLookDirection();
	}

	if (resetTicks) {
		attackTicks = 0;
	}
}

bool Monster::canUseAttack(const Position& pos, const std::shared_ptr<const Creature>& target) const
{
	if (isHostile()) {
		const Position& targetPos = target->getPosition();
		uint32_t distance = std::max<uint32_t>(pos.getDistanceX(targetPos), pos.getDistanceY(targetPos));
		for (const spellBlock_t& spellBlock : mType->info.attackSpells) {
			if (spellBlock.range != 0 && distance <= spellBlock.range) {
				return g_game.isSightClear(pos, targetPos, true);
			}
		}
		return false;
	}
	return true;
}

bool Monster::canUseSpell(const Position& pos, const Position& targetPos, const spellBlock_t& sb, uint32_t interval,
                          bool& inRange, bool& resetTicks)
{
	inRange = true;

	if (sb.isMelee) {
		if (isFleeing() || (OTSYS_TIME() - lastMeleeAttack) < sb.speed) {
			return false;
		}
	} else {
		if (sb.speed > attackTicks) {
			resetTicks = false;
			return false;
		}

		if (attackTicks % sb.speed >= interval) {
			// already used this spell for this round
			return false;
		}
	}

	if (sb.range != 0 && std::max<uint32_t>(pos.getDistanceX(targetPos), pos.getDistanceY(targetPos)) > sb.range) {
		inRange = false;
		return false;
	}
	return true;
}

void Monster::onThinkTarget(uint32_t interval)
{
	if (!isSummon()) {
		if (mType->info.changeTargetSpeed != 0) {
			bool canChangeTarget = true;

			if (challengeFocusDuration > 0) {
				challengeFocusDuration -= interval;

				if (challengeFocusDuration <= 0) {
					challengeFocusDuration = 0;
				}
			}

			if (targetChangeCooldown > 0) {
				targetChangeCooldown -= interval;

				if (targetChangeCooldown <= 0) {
					targetChangeCooldown = 0;
					targetChangeTicks = mType->info.changeTargetSpeed;
				} else {
					canChangeTarget = false;
				}
			}

			if (canChangeTarget) {
				targetChangeTicks += interval;

				if (targetChangeTicks >= mType->info.changeTargetSpeed) {
					targetChangeTicks = 0;
					targetChangeCooldown = mType->info.changeTargetSpeed;

					if (challengeFocusDuration > 0) {
						challengeFocusDuration = 0;
					}

					if (mType->info.changeTargetChance >= uniform_random(1, 100)) {
						if (mType->info.targetDistance <= 1) {
							searchTarget(TARGETSEARCH_RANDOM);
						} else {
							searchTarget(TARGETSEARCH_NEAREST);
						}
					}
				}
			}
		}
	}
}

void Monster::onThinkDefense(uint32_t interval)
{
	bool resetTicks = true;
	defenseTicks += interval;

	for (const spellBlock_t& spellBlock : mType->info.defenseSpells) {
		if (spellBlock.speed > defenseTicks) {
			resetTicks = false;
			continue;
		}

		if (defenseTicks % spellBlock.speed >= interval) {
			// already used this spell for this round
			continue;
		}

		if ((spellBlock.chance >= static_cast<uint32_t>(uniform_random(1, 100)))) {
			minCombatValue = spellBlock.minCombatValue;
			maxCombatValue = spellBlock.maxCombatValue;
			spellBlock.spell->castSpell(getMonster(), getMonster());
		}
	}

	if (!isSummon() && summons.size() < mType->info.maxSummons && hasFollowPath) {
		for (const summonBlock_t& summonBlock : mType->info.summons) {
			if (summonBlock.speed > defenseTicks) {
				resetTicks = false;
				continue;
			}

			if (summons.size() >= mType->info.maxSummons) {
				continue;
			}

			if (defenseTicks % summonBlock.speed >= interval) {
				// already used this spell for this round
				continue;
			}

			uint32_t summonCount = 0;
			for (const auto& summon : summons) {
				if (summon->getName() == summonBlock.name) {
					++summonCount;
				}
			}

			if (summonCount >= summonBlock.max) {
				continue;
			}

			if (summonBlock.chance < static_cast<uint32_t>(uniform_random(1, 100))) {
				continue;
			}

			if (const auto& summon = Monster::createMonster(summonBlock.name)) {
				if (g_game.placeCreature(summon, getPosition(), false, summonBlock.force, summonBlock.effect)) {
					summon->setDropLoot(false);
					summon->setSkillLoss(false);
					summon->setMaster(getMonster());
					if (summonBlock.masterEffect != CONST_ME_NONE) {
						g_game.addMagicEffect(getPosition(), summonBlock.masterEffect);
					}
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
	if (mType->info.yellSpeedTicks == 0) {
		return;
	}

	yellTicks += interval;
	if (yellTicks >= mType->info.yellSpeedTicks) {
		yellTicks = 0;

		if (!mType->info.voiceVector.empty() &&
		    (mType->info.yellChance >= static_cast<uint32_t>(uniform_random(1, 100)))) {
			uint32_t index = uniform_random(0, mType->info.voiceVector.size() - 1);
			const voiceBlock_t& vb = mType->info.voiceVector[index];

			if (vb.yellText) {
				g_game.internalCreatureSay(getMonster(), TALKTYPE_MONSTER_YELL, vb.text, false);
			} else {
				g_game.internalCreatureSay(getMonster(), TALKTYPE_MONSTER_SAY, vb.text, false);
			}
		}
	}
}

bool Monster::walkToSpawn()
{
	if (walkingToSpawn || !spawn || !targetList.empty()) {
		return false;
	}

	int32_t distance = std::max(position.getDistanceX(masterPos), position.getDistanceY(masterPos));
	if (distance == 0) {
		return false;
	}

	listWalkDir.clear();
	if (!getPathTo(masterPos, listWalkDir, 0, std::max(0, distance - 5), true, true, distance)) {
		return false;
	}

	walkingToSpawn = true;
	startAutoWalk();
	return true;
}

void Monster::onWalk()
{
	Creature::onWalk();

	if ((attackedCreature || followCreature) && isFleeing()) {
		if (lastPathUpdate < OTSYS_TIME()) {
			g_dispatcher.addTask(createTask([id = getID()]() { g_game.updateCreatureWalk(id); }));
			lastPathUpdate = OTSYS_TIME() + getNumber(ConfigManager::PATHFINDING_DELAY);
		}
	}
}

void Monster::onWalkComplete()
{
	// Continue walking to spawn
	if (walkingToSpawn) {
		walkingToSpawn = false;
		walkToSpawn();
	}
}

static bool pushItem(const std::shared_ptr<Item>& item)
{
	const Position& centerPos = item->getPosition();

	static auto relList = std::vector{std::pair{-1, -1}, {0, -1}, {1, -1}, {-1, 0}, {1, 0}, {-1, 1}, {0, 1}, {1, 1}};

	std::shuffle(relList.begin(), relList.end(), getRandomGenerator());

	for (const auto& [dx, dy] : relList) {
		Position tryPos(centerPos.x + dx, centerPos.y + dy, centerPos.z);
		if (const auto& tile = g_game.map.getTile(tryPos)) {
			if (g_game.canThrowObjectTo(centerPos, tryPos, true, true) &&
			    g_game.internalMoveItem(item->getParent(), tile, INDEX_WHEREEVER, item, item->getItemCount(),
			                            nullptr) == RETURNVALUE_NOERROR) {
				return true;
			}
		}
	}
	return false;
}

static void pushItems(const std::shared_ptr<Tile>& tile)
{
	// We can not use iterators here since we can push the item to another tile which will invalidate the iterator.
	// start from the end to minimize the amount of traffic
	if (TileItemVector* items = tile->getItemList()) {
		uint32_t moveCount = 0;
		uint32_t removeCount = 0;

		int32_t downItemSize = tile->getDownItemCount();
		for (int32_t i = downItemSize; --i >= 0;) {
			if (const auto& item = items->at(i)) {
				if (item->hasProperty(CONST_PROP_MOVEABLE) &&
				    (item->hasProperty(CONST_PROP_BLOCKPATH) || item->hasProperty(CONST_PROP_BLOCKSOLID))) {
					if (moveCount < 20 && pushItem(item)) {
						++moveCount;
					} else if (g_game.internalRemoveItem(item) == RETURNVALUE_NOERROR) {
						++removeCount;
					}
				}
			}
		}

		if (removeCount > 0) {
			g_game.addMagicEffect(tile->getPosition(), CONST_ME_POFF);
		}
	}
}

static bool pushCreature(const std::shared_ptr<Creature>& creature)
{
	for (Direction dir : getShuffleDirections()) {
		const Position& tryPos = Spells::getCasterPosition(creature, dir);
		if (const auto& toTile = g_game.map.getTile(tryPos)) {
			if (!toTile->hasFlag(TILESTATE_BLOCKPATH) &&
			    g_game.internalMoveCreature(creature, dir) == RETURNVALUE_NOERROR) {
				return true;
			}
		}
	}
	return false;
}

static void pushCreatures(const std::shared_ptr<Tile>& tile)
{
	// We can not use iterators here since we can push a creature to another tile which will invalidate the iterator.
	if (CreatureVector* creatures = tile->getCreatures()) {
		uint32_t removeCount = 0;
		std::shared_ptr<Monster> lastPushedMonster = nullptr;

		for (size_t i = 0; i < creatures->size();) {
			if (const auto& monster = creatures->at(i)->getMonster()) {
				if (monster->isPushable()) {
					if (monster != lastPushedMonster && pushCreature(monster)) {
						lastPushedMonster = monster;
						continue;
					}

					monster->changeHealth(-monster->getHealth());
					removeCount++;
				}

				++i;
			}
		}

		if (removeCount > 0) {
			g_game.addMagicEffect(tile->getPosition(), CONST_ME_BLOCKHIT);
		}
	}
}

bool Monster::getNextStep(Direction& direction, uint32_t& flags)
{
	if (!walkingToSpawn && (isIdle || isDead())) {
		// we don't have anyone watching, might as well stop walking
		eventWalk = 0;
		return false;
	}

	bool result = false;
	if (!walkingToSpawn && (!followCreature || !hasFollowPath) && (!isSummon() || !isMasterInRange)) {
		if (getTimeSinceLastMove() >= 1000) {
			randomStepping = true;
			// choose a random direction
			result = getRandomStep(getPosition(), direction);
		}
	} else if ((isSummon() && isMasterInRange) || followCreature || walkingToSpawn) {
		if (!hasFollowPath && getMaster() && !getMaster()->getPlayer()) {
			randomStepping = true;
			result = getRandomStep(getPosition(), direction);
		} else {
			randomStepping = false;
			result = Creature::getNextStep(direction, flags);
			if (result) {
				flags |= FLAG_PATHFINDING;
			} else {
				if (ignoreFieldDamage) {
					ignoreFieldDamage = false;
				}
				// target dancing
				if (attackedCreature && attackedCreature == followCreature) {
					if (isFleeing()) {
						result = getDanceStep(getPosition(), direction, false, false);
					} else if (mType->info.staticAttackChance < static_cast<uint32_t>(uniform_random(1, 100))) {
						result = getDanceStep(getPosition(), direction);
					}
				}
			}
		}
	}

	if (result && (canPushItems() || canPushCreatures())) {
		const Position& pos = Spells::getCasterPosition(getMonster(), direction);

		if (const auto& tile = g_game.map.getTile(pos)) {
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

bool Monster::getRandomStep(const Position& creaturePos, Direction& direction) const
{
	for (Direction dir : getShuffleDirections()) {
		if (canWalkTo(creaturePos, dir)) {
			direction = dir;
			return true;
		}
	}
	return false;
}

bool Monster::getDanceStep(const Position& creaturePos, Direction& direction, bool keepAttack /*= true*/,
                           bool keepDistance /*= true*/)
{
	bool canDoAttackNow = canUseAttack(creaturePos, attackedCreature);

	assert(attackedCreature);
	const Position& centerPos = attackedCreature->getPosition();

	int32_t offset_x = creaturePos.getOffsetX(centerPos);
	int32_t offset_y = creaturePos.getOffsetY(centerPos);

	int32_t distance_x = std::abs(offset_x);
	int32_t distance_y = std::abs(offset_y);

	int32_t centerToDist = std::max(distance_x, distance_y);

	std::vector<Direction> dirList;
	dirList.reserve(4);

	if (!keepDistance || offset_y >= 0) {
		int32_t tmpDist = std::max(distance_x, std::abs((creaturePos.getY() - 1) - centerPos.getY()));
		if (tmpDist == centerToDist && canWalkTo(creaturePos, DIRECTION_NORTH)) {
			bool result = true;

			if (keepAttack) {
				result = (!canDoAttackNow ||
				          canUseAttack(Position(creaturePos.x, creaturePos.y - 1, creaturePos.z), attackedCreature));
			}

			if (result) {
				dirList.push_back(DIRECTION_NORTH);
			}
		}
	}

	if (!keepDistance || offset_y <= 0) {
		int32_t tmpDist = std::max(distance_x, std::abs((creaturePos.getY() + 1) - centerPos.getY()));
		if (tmpDist == centerToDist && canWalkTo(creaturePos, DIRECTION_SOUTH)) {
			bool result = true;

			if (keepAttack) {
				result = (!canDoAttackNow ||
				          canUseAttack(Position(creaturePos.x, creaturePos.y + 1, creaturePos.z), attackedCreature));
			}

			if (result) {
				dirList.push_back(DIRECTION_SOUTH);
			}
		}
	}

	if (!keepDistance || offset_x <= 0) {
		int32_t tmpDist = std::max(std::abs((creaturePos.getX() + 1) - centerPos.getX()), distance_y);
		if (tmpDist == centerToDist && canWalkTo(creaturePos, DIRECTION_EAST)) {
			bool result = true;

			if (keepAttack) {
				result = (!canDoAttackNow ||
				          canUseAttack(Position(creaturePos.x + 1, creaturePos.y, creaturePos.z), attackedCreature));
			}

			if (result) {
				dirList.push_back(DIRECTION_EAST);
			}
		}
	}

	if (!keepDistance || offset_x >= 0) {
		int32_t tmpDist = std::max(std::abs((creaturePos.getX() - 1) - centerPos.getX()), distance_y);
		if (tmpDist == centerToDist && canWalkTo(creaturePos, DIRECTION_WEST)) {
			bool result = true;

			if (keepAttack) {
				result = (!canDoAttackNow ||
				          canUseAttack(Position(creaturePos.x - 1, creaturePos.y, creaturePos.z), attackedCreature));
			}

			if (result) {
				dirList.push_back(DIRECTION_WEST);
			}
		}
	}

	if (!dirList.empty()) {
		direction = dirList[uniform_random(0, dirList.size() - 1)];
		return true;
	}
	return false;
}

bool Monster::getDistanceStep(const Position& targetPos, Direction& direction, bool flee /* = false */)
{
	const Position& creaturePos = getPosition();

	int32_t dx = creaturePos.getDistanceX(targetPos);
	int32_t dy = creaturePos.getDistanceY(targetPos);

	int32_t distance = std::max(dx, dy);

	if (!flee && (distance > mType->info.targetDistance || !g_game.isSightClear(creaturePos, targetPos, true))) {
		return false; // let the A* calculate it
	} else if (!flee && distance == mType->info.targetDistance) {
		return true; // we don't really care here, since it's what we wanted to reach (a dance-step will take of dancing
		             // in that position)
	}

	int32_t offsetx = creaturePos.getOffsetX(targetPos);
	int32_t offsety = creaturePos.getOffsetY(targetPos);

	if (dx <= 1 && dy <= 1) {
		// seems like a target is near, it this case we need to slow down our movements (as a monster)
		if (stepDuration < 2) {
			stepDuration++;
		}
	} else if (stepDuration > 0) {
		stepDuration--;
	}

	if (offsetx == 0 && offsety == 0) {
		// player is "on" the monster so let's get some random step and rest will be taken care later.
		return getRandomStep(creaturePos, direction);
	}

	if (dx == dy) {
		// player is diagonal to the monster
		if (offsetx >= 1 && offsety >= 1) {
			// player is NW
			// escape to SE, S or E [and some extra]
			bool s = canWalkTo(creaturePos, DIRECTION_SOUTH);
			bool e = canWalkTo(creaturePos, DIRECTION_EAST);

			if (s && e) {
				direction = boolean_random() ? DIRECTION_SOUTH : DIRECTION_EAST;
				return true;
			} else if (s) {
				direction = DIRECTION_SOUTH;
				return true;
			} else if (e) {
				direction = DIRECTION_EAST;
				return true;
			} else if (canWalkTo(creaturePos, DIRECTION_SOUTHEAST)) {
				direction = DIRECTION_SOUTHEAST;
				return true;
			}

			/* fleeing */
			bool n = canWalkTo(creaturePos, DIRECTION_NORTH);
			bool w = canWalkTo(creaturePos, DIRECTION_WEST);

			if (flee) {
				if (n && w) {
					direction = boolean_random() ? DIRECTION_NORTH : DIRECTION_WEST;
					return true;
				} else if (n) {
					direction = DIRECTION_NORTH;
					return true;
				} else if (w) {
					direction = DIRECTION_WEST;
					return true;
				}
			}

			/* end of fleeing */

			if (w && canWalkTo(creaturePos, DIRECTION_SOUTHWEST)) {
				direction = DIRECTION_WEST;
			} else if (n && canWalkTo(creaturePos, DIRECTION_NORTHEAST)) {
				direction = DIRECTION_NORTH;
			}

			return true;
		} else if (offsetx <= -1 && offsety <= -1) {
			// player is SE
			// escape to NW , W or N [and some extra]
			bool w = canWalkTo(creaturePos, DIRECTION_WEST);
			bool n = canWalkTo(creaturePos, DIRECTION_NORTH);

			if (w && n) {
				direction = boolean_random() ? DIRECTION_WEST : DIRECTION_NORTH;
				return true;
			} else if (w) {
				direction = DIRECTION_WEST;
				return true;
			} else if (n) {
				direction = DIRECTION_NORTH;
				return true;
			}

			if (canWalkTo(creaturePos, DIRECTION_NORTHWEST)) {
				direction = DIRECTION_NORTHWEST;
				return true;
			}

			/* fleeing */
			bool s = canWalkTo(creaturePos, DIRECTION_SOUTH);
			bool e = canWalkTo(creaturePos, DIRECTION_EAST);

			if (flee) {
				if (s && e) {
					direction = boolean_random() ? DIRECTION_SOUTH : DIRECTION_EAST;
					return true;
				} else if (s) {
					direction = DIRECTION_SOUTH;
					return true;
				} else if (e) {
					direction = DIRECTION_EAST;
					return true;
				}
			}

			/* end of fleeing */

			if (s && canWalkTo(creaturePos, DIRECTION_SOUTHWEST)) {
				direction = DIRECTION_SOUTH;
			} else if (e && canWalkTo(creaturePos, DIRECTION_NORTHEAST)) {
				direction = DIRECTION_EAST;
			}

			return true;
		} else if (offsetx >= 1 && offsety <= -1) {
			// player is SW
			// escape to NE, N, E [and some extra]
			bool n = canWalkTo(creaturePos, DIRECTION_NORTH);
			bool e = canWalkTo(creaturePos, DIRECTION_EAST);
			if (n && e) {
				direction = boolean_random() ? DIRECTION_NORTH : DIRECTION_EAST;
				return true;
			} else if (n) {
				direction = DIRECTION_NORTH;
				return true;
			} else if (e) {
				direction = DIRECTION_EAST;
				return true;
			}

			if (canWalkTo(creaturePos, DIRECTION_NORTHEAST)) {
				direction = DIRECTION_NORTHEAST;
				return true;
			}

			/* fleeing */
			bool s = canWalkTo(creaturePos, DIRECTION_SOUTH);
			bool w = canWalkTo(creaturePos, DIRECTION_WEST);

			if (flee) {
				if (s && w) {
					direction = boolean_random() ? DIRECTION_SOUTH : DIRECTION_WEST;
					return true;
				} else if (s) {
					direction = DIRECTION_SOUTH;
					return true;
				} else if (w) {
					direction = DIRECTION_WEST;
					return true;
				}
			}

			/* end of fleeing */

			if (w && canWalkTo(creaturePos, DIRECTION_NORTHWEST)) {
				direction = DIRECTION_WEST;
			} else if (s && canWalkTo(creaturePos, DIRECTION_SOUTHEAST)) {
				direction = DIRECTION_SOUTH;
			}

			return true;
		} else if (offsetx <= -1 && offsety >= 1) {
			// player is NE
			// escape to SW, S, W [and some extra]
			bool w = canWalkTo(creaturePos, DIRECTION_WEST);
			bool s = canWalkTo(creaturePos, DIRECTION_SOUTH);
			if (w && s) {
				direction = boolean_random() ? DIRECTION_WEST : DIRECTION_SOUTH;
				return true;
			} else if (w) {
				direction = DIRECTION_WEST;
				return true;
			} else if (s) {
				direction = DIRECTION_SOUTH;
				return true;
			} else if (canWalkTo(creaturePos, DIRECTION_SOUTHWEST)) {
				direction = DIRECTION_SOUTHWEST;
				return true;
			}

			/* fleeing */
			bool n = canWalkTo(creaturePos, DIRECTION_NORTH);
			bool e = canWalkTo(creaturePos, DIRECTION_EAST);

			if (flee) {
				if (n && e) {
					direction = boolean_random() ? DIRECTION_NORTH : DIRECTION_EAST;
					return true;
				} else if (n) {
					direction = DIRECTION_NORTH;
					return true;
				} else if (e) {
					direction = DIRECTION_EAST;
					return true;
				}
			}

			/* end of fleeing */

			if (e && canWalkTo(creaturePos, DIRECTION_SOUTHEAST)) {
				direction = DIRECTION_EAST;
			} else if (n && canWalkTo(creaturePos, DIRECTION_NORTHWEST)) {
				direction = DIRECTION_NORTH;
			}

			return true;
		}
	}

	// Now let's decide where the player is located to the monster (what direction) so we can decide where to escape.
	if (dy > dx) {
		Direction playerDir = offsety < 0 ? DIRECTION_SOUTH : DIRECTION_NORTH;
		switch (playerDir) {
			case DIRECTION_NORTH: {
				// Player is to the NORTH, so obviously we need to check if we can go SOUTH, if not then let's choose
				// WEST or EAST and again if we can't we need to decide about some diagonal movements.
				if (canWalkTo(creaturePos, DIRECTION_SOUTH)) {
					direction = DIRECTION_SOUTH;
					return true;
				}

				bool w = canWalkTo(creaturePos, DIRECTION_WEST);
				bool e = canWalkTo(creaturePos, DIRECTION_EAST);
				if (w && e && offsetx == 0) {
					direction = boolean_random() ? DIRECTION_WEST : DIRECTION_EAST;
					return true;
				} else if (w && offsetx <= 0) {
					direction = DIRECTION_WEST;
					return true;
				} else if (e && offsetx >= 0) {
					direction = DIRECTION_EAST;
					return true;
				}

				/* fleeing */
				if (flee) {
					if (w && e) {
						direction = boolean_random() ? DIRECTION_WEST : DIRECTION_EAST;
						return true;
					} else if (w) {
						direction = DIRECTION_WEST;
						return true;
					} else if (e) {
						direction = DIRECTION_EAST;
						return true;
					}
				}

				/* end of fleeing */

				bool sw = canWalkTo(creaturePos, DIRECTION_SOUTHWEST);
				bool se = canWalkTo(creaturePos, DIRECTION_SOUTHEAST);
				if (sw || se) {
					// we can move both dirs
					if (sw && se) {
						direction = boolean_random() ? DIRECTION_SOUTHWEST : DIRECTION_SOUTHEAST;
					} else if (w) {
						direction = DIRECTION_WEST;
					} else if (sw) {
						direction = DIRECTION_SOUTHWEST;
					} else if (e) {
						direction = DIRECTION_EAST;
					} else if (se) {
						direction = DIRECTION_SOUTHEAST;
					}
					return true;
				}

				/* fleeing */
				if (flee && canWalkTo(creaturePos, DIRECTION_NORTH)) {
					// towards player, yea
					direction = DIRECTION_NORTH;
					return true;
				}

				/* end of fleeing */
				break;
			}

			case DIRECTION_SOUTH: {
				if (canWalkTo(creaturePos, DIRECTION_NORTH)) {
					direction = DIRECTION_NORTH;
					return true;
				}

				bool w = canWalkTo(creaturePos, DIRECTION_WEST);
				bool e = canWalkTo(creaturePos, DIRECTION_EAST);
				if (w && e && offsetx == 0) {
					direction = boolean_random() ? DIRECTION_WEST : DIRECTION_EAST;
					return true;
				} else if (w && offsetx <= 0) {
					direction = DIRECTION_WEST;
					return true;
				} else if (e && offsetx >= 0) {
					direction = DIRECTION_EAST;
					return true;
				}

				/* fleeing */
				if (flee) {
					if (w && e) {
						direction = boolean_random() ? DIRECTION_WEST : DIRECTION_EAST;
						return true;
					} else if (w) {
						direction = DIRECTION_WEST;
						return true;
					} else if (e) {
						direction = DIRECTION_EAST;
						return true;
					}
				}

				/* end of fleeing */

				bool nw = canWalkTo(creaturePos, DIRECTION_NORTHWEST);
				bool ne = canWalkTo(creaturePos, DIRECTION_NORTHEAST);
				if (nw || ne) {
					// we can move both dirs
					if (nw && ne) {
						direction = boolean_random() ? DIRECTION_NORTHWEST : DIRECTION_NORTHEAST;
					} else if (w) {
						direction = DIRECTION_WEST;
					} else if (nw) {
						direction = DIRECTION_NORTHWEST;
					} else if (e) {
						direction = DIRECTION_EAST;
					} else if (ne) {
						direction = DIRECTION_NORTHEAST;
					}
					return true;
				}

				/* fleeing */
				if (flee && canWalkTo(creaturePos, DIRECTION_SOUTH)) {
					// towards player, yea
					direction = DIRECTION_SOUTH;
					return true;
				}

				/* end of fleeing */
				break;
			}

			default:
				break;
		}
	} else {
		Direction playerDir = offsetx < 0 ? DIRECTION_EAST : DIRECTION_WEST;
		switch (playerDir) {
			case DIRECTION_WEST: {
				if (canWalkTo(creaturePos, DIRECTION_EAST)) {
					direction = DIRECTION_EAST;
					return true;
				}

				bool n = canWalkTo(creaturePos, DIRECTION_NORTH);
				bool s = canWalkTo(creaturePos, DIRECTION_SOUTH);
				if (n && s && offsety == 0) {
					direction = boolean_random() ? DIRECTION_NORTH : DIRECTION_SOUTH;
					return true;
				} else if (n && offsety <= 0) {
					direction = DIRECTION_NORTH;
					return true;
				} else if (s && offsety >= 0) {
					direction = DIRECTION_SOUTH;
					return true;
				}

				/* fleeing */
				if (flee) {
					if (n && s) {
						direction = boolean_random() ? DIRECTION_NORTH : DIRECTION_SOUTH;
						return true;
					} else if (n) {
						direction = DIRECTION_NORTH;
						return true;
					} else if (s) {
						direction = DIRECTION_SOUTH;
						return true;
					}
				}

				/* end of fleeing */

				bool se = canWalkTo(creaturePos, DIRECTION_SOUTHEAST);
				bool ne = canWalkTo(creaturePos, DIRECTION_NORTHEAST);
				if (se || ne) {
					if (se && ne) {
						direction = boolean_random() ? DIRECTION_SOUTHEAST : DIRECTION_NORTHEAST;
					} else if (s) {
						direction = DIRECTION_SOUTH;
					} else if (se) {
						direction = DIRECTION_SOUTHEAST;
					} else if (n) {
						direction = DIRECTION_NORTH;
					} else if (ne) {
						direction = DIRECTION_NORTHEAST;
					}
					return true;
				}

				/* fleeing */
				if (flee && canWalkTo(creaturePos, DIRECTION_WEST)) {
					// towards player, yea
					direction = DIRECTION_WEST;
					return true;
				}

				/* end of fleeing */
				break;
			}

			case DIRECTION_EAST: {
				if (canWalkTo(creaturePos, DIRECTION_WEST)) {
					direction = DIRECTION_WEST;
					return true;
				}

				bool n = canWalkTo(creaturePos, DIRECTION_NORTH);
				bool s = canWalkTo(creaturePos, DIRECTION_SOUTH);
				if (n && s && offsety == 0) {
					direction = boolean_random() ? DIRECTION_NORTH : DIRECTION_SOUTH;
					return true;
				} else if (n && offsety <= 0) {
					direction = DIRECTION_NORTH;
					return true;
				} else if (s && offsety >= 0) {
					direction = DIRECTION_SOUTH;
					return true;
				}

				/* fleeing */
				if (flee) {
					if (n && s) {
						direction = boolean_random() ? DIRECTION_NORTH : DIRECTION_SOUTH;
						return true;
					} else if (n) {
						direction = DIRECTION_NORTH;
						return true;
					} else if (s) {
						direction = DIRECTION_SOUTH;
						return true;
					}
				}

				/* end of fleeing */

				bool nw = canWalkTo(creaturePos, DIRECTION_NORTHWEST);
				bool sw = canWalkTo(creaturePos, DIRECTION_SOUTHWEST);
				if (nw || sw) {
					if (nw && sw) {
						direction = boolean_random() ? DIRECTION_NORTHWEST : DIRECTION_SOUTHWEST;
					} else if (n) {
						direction = DIRECTION_NORTH;
					} else if (nw) {
						direction = DIRECTION_NORTHWEST;
					} else if (s) {
						direction = DIRECTION_SOUTH;
					} else if (sw) {
						direction = DIRECTION_SOUTHWEST;
					}
					return true;
				}

				/* fleeing */
				if (flee && canWalkTo(creaturePos, DIRECTION_EAST)) {
					// towards player, yea
					direction = DIRECTION_EAST;
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

bool Monster::canWalkTo(Position pos, Direction direction) const
{
	pos = getNextPosition(direction, pos);
	if (isInSpawnRange(pos)) {
		if (const auto& tile = g_game.map.getTile(pos)) {
			if (!tile->getTopVisibleCreature(getMonster()) &&
			    tile->queryAdd(0, getMonster(), 1, FLAG_PATHFINDING) == RETURNVALUE_NOERROR) {
				return true;
			}
		}
	}
	return false;
}

void Monster::death(const std::shared_ptr<Creature>&)
{
	removeAttackedCreature();

	for (const auto& summon : summons) {
		summon->changeHealth(-summon->getHealth());
		summon->removeMaster();
	}
	summons.clear();

	clearTargetList();
	clearFriendList();
	onIdleStatus();
}

std::shared_ptr<Item> Monster::getCorpse(const std::shared_ptr<Creature>& lastHitCreature,
                                         const std::shared_ptr<Creature>& mostDamageCreature)
{
	const auto& corpse = Creature::getCorpse(lastHitCreature, mostDamageCreature);
	if (!corpse) {
		return nullptr;
	}

	if (mostDamageCreature) {
		if (mostDamageCreature->getPlayer()) {
			corpse->setCorpseOwner(mostDamageCreature->getID());
		} else {
			const auto& mostDamageCreatureMaster = mostDamageCreature->getMaster();
			if (mostDamageCreatureMaster && mostDamageCreatureMaster->getPlayer()) {
				corpse->setCorpseOwner(mostDamageCreatureMaster->getID());
			}
		}
	}
	return corpse;
}

bool Monster::isInSpawnRange(const Position& pos) const
{
	if (!spawn) {
		return true;
	}

	if (Monster::despawnRadius == 0) {
		return true;
	}

	if (!Spawns::isInZone(masterPos, Monster::despawnRadius, pos)) {
		return false;
	}

	if (Monster::despawnRange == 0) {
		return true;
	}

	if (pos.getDistanceZ(masterPos) > Monster::despawnRange) {
		return false;
	}

	return true;
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
	if (!attackedCreature) {
		return;
	}

	auto lookDirection = DIRECTION_NONE;

	const auto& currentPosition = getPosition();
	const auto& targetPosition = attackedCreature->getPosition();

	auto offsetX = targetPosition.getOffsetX(currentPosition);
	auto absOffsetX = std::abs(offsetX);

	auto offsetY = targetPosition.getOffsetY(currentPosition);
	auto absOffsetY = std::abs(offsetY);

	if (absOffsetX > absOffsetY) {
		// Target is farther on the horizontal axis
		lookDirection = (offsetX < 0) ? DIRECTION_WEST : DIRECTION_EAST;
	} else if (absOffsetY > absOffsetX) {
		// Target is farther on the vertical axis
		lookDirection = (offsetY < 0) ? DIRECTION_NORTH : DIRECTION_SOUTH;
	} else {
		// Target is equally far on both axes, prioritize horizontal direction
		lookDirection = (offsetX < 0) ? DIRECTION_WEST : DIRECTION_EAST;
	}

	g_game.internalCreatureTurn(getMonster(), lookDirection);
}

void Monster::dropLoot(const std::shared_ptr<Container>& corpse, const std::shared_ptr<Creature>&)
{
	if (corpse && lootDrop) {
		tfs::events::monster::onDropLoot(getMonster(), corpse);
	}
}

void Monster::setNormalCreatureLight() { internalLight = mType->info.light; }

void Monster::drainHealth(const std::shared_ptr<Creature>& attacker, int32_t damage)
{
	Creature::drainHealth(attacker, damage);

	if (damage > 0 && randomStepping) {
		ignoreFieldDamage = true;
	}

	if (isInvisible()) {
		removeCondition(CONDITION_INVISIBLE);
	}
}

void Monster::changeHealth(int32_t healthChange, bool sendHealthChange /* = true*/)
{
	// In case a player with ignore flag set attacks the monster
	setIdle(false);
	Creature::changeHealth(healthChange, sendHealthChange);
}

bool Monster::challengeCreature(const std::shared_ptr<Creature>& creature, bool force /* = false*/)
{
	if (isSummon()) {
		return false;
	}

	if (!mType->info.isChallengeable && !force) {
		return false;
	}

	bool result = selectTarget(creature);
	if (result) {
		targetChangeCooldown = 8000;
		challengeFocusDuration = targetChangeCooldown;
		targetChangeTicks = 0;
	}
	return result;
}

void Monster::getPathSearchParams(const std::shared_ptr<const Creature>& creature, FindPathParams& fpp) const
{
	Creature::getPathSearchParams(creature, fpp);

	fpp.minTargetDist = 1;
	fpp.maxTargetDist = mType->info.targetDistance;

	if (isSummon()) {
		if (followCreature && followCreature == getMaster()) {
			fpp.summonTargetMaster = true;
		}
		if (getMaster() == creature) {
			fpp.maxTargetDist = 2;
			fpp.fullPathSearch = true;
		} else if (mType->info.targetDistance <= 1) {
			fpp.fullPathSearch = true;
		} else {
			fpp.fullPathSearch = !canUseAttack(getPosition(), creature);
		}
	} else if (isFleeing()) {
		// Distance should be higher than the client view range (Map::maxClientViewportX/Map::maxClientViewportY)
		fpp.maxTargetDist = Map::maxViewportX;
		fpp.clearSight = false;
		fpp.keepDistance = true;
		fpp.fullPathSearch = false;
	} else if (mType->info.targetDistance <= 1) {
		fpp.fullPathSearch = true;
	} else {
		fpp.fullPathSearch = !canUseAttack(getPosition(), creature);
	}
}

bool Monster::canPushItems() const
{
	const auto& master = this->master ? this->master->getMonster() : nullptr;
	if (master) {
		return master->mType->info.canPushItems;
	}

	return mType->info.canPushItems;
}
