//////////////////////////////////////////////////////////////////////
// The Forgotten Server - a server application for the MMORPG Tibia
//////////////////////////////////////////////////////////////////////
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//////////////////////////////////////////////////////////////////////

#include "otpch.h"

#include <vector>
#include <string>
#include <algorithm>

#include "monster.h"
#include "monsters.h"
#include "game.h"
#include "spells.h"
#include "combat.h"
#include "spawn.h"
#include "configmanager.h"

extern Game g_game;
extern ConfigManager g_config;
extern Monsters g_monsters;

AutoList<Monster>Monster::listMonster;

int32_t Monster::despawnRange;
int32_t Monster::despawnRadius;

uint32_t Monster::monsterAutoID = 0x40000000;

#ifdef __ENABLE_SERVER_DIAGNOSTIC__
uint32_t Monster::monsterCount = 0;
#endif

Monster* Monster::createMonster(MonsterType* mType)
{
	return new Monster(mType);
}

Monster* Monster::createMonster(const std::string& name)
{
	MonsterType* mType = g_monsters.getMonsterType(name);

	if (!mType) {
		return NULL;
	}

	return createMonster(mType);
}

Monster::Monster(MonsterType* _mtype) :
	Creature()
{
	isIdle = true;
	isMasterInRange = false;
	mType = _mtype;
	spawn = NULL;
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
	MonsterScriptList::iterator it;

	for (it = mType->scriptList.begin(); it != mType->scriptList.end(); ++it) {
		if (!registerCreatureEvent(*it)) {
			std::cout << "Warning: [Monster::Monster]. Unknown event name - " << *it << std::endl;
		}
	}

#ifdef __ENABLE_SERVER_DIAGNOSTIC__
	monsterCount++;
#endif
}

Monster::~Monster()
{
	clearTargetList();
	clearFriendList();
#ifdef __ENABLE_SERVER_DIAGNOSTIC__
	monsterCount--;
#endif
}

bool Monster::canSee(const Position& pos) const
{
	return Creature::canSee(getPosition(), pos, 9, 9);
}

void Monster::onAttackedCreatureDisappear(bool isLogout)
{
	attackTicks = 0;
	extraMeleeAttack = true;
}

void Monster::onFollowCreatureDisappear(bool isLogout)
{
	//
}

void Monster::onCreatureAppear(const Creature* creature, bool isLogin)
{
	Creature::onCreatureAppear(creature, isLogin);

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

				int32_t offset_x = std::abs(followPosition.x - position.x);
				int32_t offset_y = std::abs(followPosition.y - position.y);

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

void Monster::updateTargetList()
{
	CreatureHashSet::iterator friendIterator = friendList.begin();

	while (friendIterator != friendList.end()) {
		if ((*friendIterator)->getHealth() <= 0 || !canSee((*friendIterator)->getPosition())) {
			(*friendIterator)->releaseThing2();
			friendIterator = friendList.erase(friendIterator);
		} else {
			++friendIterator;
		}
	}

	CreatureList::iterator targetIterator = targetList.begin();

	while (targetIterator != targetList.end()) {
		if ((*targetIterator)->getHealth() <= 0 || !canSee((*targetIterator)->getPosition())) {
			(*targetIterator)->releaseThing2();
			targetIterator = targetList.erase(targetIterator);
		} else {
			++targetIterator;
		}
	}

	const SpectatorVec& list = g_game.getSpectators(getPosition());

	for (SpectatorVec::const_iterator list_it = list.begin(), list_end = list.end(); list_it != list_end; ++list_it) {
		Creature* spectator = *list_it;

		if (spectator != this && canSee(spectator->getPosition())) {
			onCreatureFound(spectator);
		}
	}
}

void Monster::clearTargetList()
{
	for (CreatureList::iterator it = targetList.begin(); it != targetList.end(); ++it) {
		(*it)->releaseThing2();
	}

	targetList.clear();
}

void Monster::clearFriendList()
{
	for (CreatureHashSet::iterator it = friendList.begin(), end = friendList.end(); it != end; ++it) {
		(*it)->releaseThing2();
	}

	friendList.clear();
}

void Monster::onCreatureFound(Creature* creature, bool pushFront /*= false*/)
{
	if (isFriend(creature)) {
		assert(creature != this);
		std::pair<CreatureHashSet::iterator, bool> res = friendList.insert(creature);

		if (res.second) {
			creature->useThing2();
		}
	}

	if (isOpponent(creature)) {
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

bool Monster::isFriend(const Creature* creature)
{
	if (isSummon() && getMaster()->getPlayer()) {
		const Player* masterPlayer = getMaster()->getPlayer();
		const Player* tmpPlayer = NULL;

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

bool Monster::isOpponent(const Creature* creature)
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
		if (friendList.erase(creature) != 0) {
			creature->releaseThing2();
		}
	}

	//update targetList
	if (isOpponent(creature)) {
		CreatureList::iterator it = std::find(targetList.begin(), targetList.end(), creature);

		if (it != targetList.end()) {
			(*it)->releaseThing2();
			targetList.erase(it);

			if (targetList.empty()) {
				updateIdleStatus();
			}
		}
	}
}

bool Monster::searchTarget(TargetSearchType_t searchType /*= TARGETSEARCH_DEFAULT*/)
{
	std::list<Creature*> resultList;
	const Position& myPos = getPosition();

	for (CreatureList::iterator it = targetList.begin(); it != targetList.end(); ++it) {
		if (followCreature != (*it) && isTarget(*it)) {
			if (searchType == TARGETSEARCH_RANDOM || canUseAttack(myPos, *it)) {
				resultList.push_back(*it);
			}
		}
	}

	switch (searchType) {
		case TARGETSEARCH_NEAREAST: {
			Creature* target = NULL;
			int32_t minRange = -1;

			for (std::list<Creature*>::iterator it = resultList.begin(); it != resultList.end(); ++it) {
				const Position& pos = (*it)->getPosition();

				if (minRange == -1 || std::max<int32_t>(std::abs(myPos.x - pos.x), std::abs(myPos.y - pos.y)) < minRange) {
					target = *it;
					minRange = std::max<int32_t>(std::abs(myPos.x - pos.x), std::abs(myPos.y - pos.y));
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
				uint32_t index = random_range(0, resultList.size() - 1);
				CreatureList::iterator it = resultList.begin();
				std::advance(it, index);
				return selectTarget(*it);
			}

			if (searchType == TARGETSEARCH_ATTACKRANGE) {
				return false;
			}

			break;
		}
	}

	//lets just pick the first target in the list
	for (CreatureList::iterator it = targetList.begin(); it != targetList.end(); ++it) {
		if (followCreature != (*it) && selectTarget(*it)) {
			return true;
		}
	}

	return false;
}

void Monster::onFollowCreatureComplete(const Creature* creature)
{
	if (creature) {
		CreatureList::iterator it = std::find(targetList.begin(), targetList.end(), creature);
		Creature* target;

		if (it != targetList.end()) {
			target = (*it);
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
                              bool checkDefense /* = false*/, bool checkArmor /* = false*/)
{
	BlockType_t blockType = Creature::blockHit(attacker, combatType, damage, checkDefense, checkArmor);

	if (damage != 0) {
		int32_t elementMod = 0;
		ElementMap::iterator it = mType->elementMap.find(combatType);

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


bool Monster::isTarget(Creature* creature)
{
	if (creature->isRemoved() || !creature->isAttackable() ||
	        creature->getZone() == ZONE_PROTECTION || !canSeeCreature(creature)) {
		return false;
	}

	if (creature->getPosition().z != getPosition().z) {
		return false;
	}

	if (creature->getPlayer() && creature->getPlayer()->isLagging()) {
		return false;
	}

	return true;
}

bool Monster::selectTarget(Creature* creature)
{
	if (!isTarget(creature)) {
		return false;
	}

	CreatureList::iterator it = std::find(targetList.begin(), targetList.end(), creature);

	if (it == targetList.end()) {
		//Target not found in our target list.
		return false;
	}

	if (isHostile() || isSummon()) {
		if (setAttackedCreature(creature) && !isSummon()) {
			g_dispatcher.addTask(createTask(
			                         boost::bind(&Game::checkCreatureAttack, &g_game, getID())));
		}
	}

	return setFollowCreature(creature, true);
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
		g_game.removeCreatureCheck(this);
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
						///This happens if the monster is summoned during combat
						selectTarget(getMaster()->getAttackedCreature());
					} else if (getMaster() != followCreature) {
						//Our master has not ordered us to attack anything, lets follow him around instead.
						setFollowCreature(getMaster());
					}
				} else if (attackedCreature == this) {
					setFollowCreature(NULL);
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

	if (attackedCreature->getPlayer() && attackedCreature->getPlayer()->isLagging()) {
		onCreatureLeave(attackedCreature);
		setAttackedCreature(NULL);
		setFollowCreature(NULL);
		onAttackedCreatureDisappear(false);
		return;
	}

	bool updateLook = true;

	resetTicks = interval != 0;
	attackTicks += interval;

	const Position& myPos = getPosition();
	const Position& targetPos = attackedCreature->getPosition();

	for (SpellList::iterator it = mType->spellAttackList.begin(); it != mType->spellAttackList.end(); ++it) {
		bool inRange = false;

		if (canUseSpell(myPos, targetPos, *it, interval, inRange)) {
			if (it->chance >= (uint32_t)random_range(1, 100)) {
				if (updateLook) {
					updateLookDirection();
					updateLook = false;
				}

				minCombatValue = it->minCombatValue;
				maxCombatValue = it->maxCombatValue;
				it->spell->castSpell(this, attackedCreature);

				if (it->isMelee) {
					extraMeleeAttack = false;
				}
			}
		}

		if (!inRange && it->isMelee) {
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
		uint32_t distance = std::max<uint32_t>(std::abs(pos.x - targetPos.x), std::abs(pos.y - targetPos.y));
		for (SpellList::iterator it = mType->spellAttackList.begin(); it != mType->spellAttackList.end(); ++it) {
			if (it->range != 0 && distance <= it->range) {
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

	if (sb.range != 0 && std::max<uint32_t>(std::abs(pos.x - targetPos.x), std::abs(pos.y - targetPos.y)) > sb.range) {
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

					if (mType->changeTargetChance >= random_range(1, 100)) {
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

	for (SpellList::iterator it = mType->spellDefenseList.begin(); it != mType->spellDefenseList.end(); ++it) {
		if (it->speed > defenseTicks) {
			resetTicks = false;
			continue;
		}

		if (defenseTicks % it->speed >= interval) {
			//already used this spell for this round
			continue;
		}

		if ((it->chance >= (uint32_t)random_range(1, 100))) {
			minCombatValue = it->minCombatValue;
			maxCombatValue = it->maxCombatValue;
			it->spell->castSpell(this, this);
		}
	}

	if (!isSummon() && (int32_t)summons.size() < mType->maxSummons) {
		for (SummonList::iterator it = mType->summonList.begin(); it != mType->summonList.end(); ++it) {
			if (it->speed > defenseTicks) {
				resetTicks = false;
				continue;
			}

			if ((int32_t)summons.size() >= mType->maxSummons) {
				continue;
			}

			if (defenseTicks % it->speed >= interval) {
				//already used this spell for this round
				continue;
			}

			if ((it->chance >= (uint32_t)random_range(1, 100))) {
				Monster* summon = Monster::createMonster(it->name);

				if (summon) {
					const Position& summonPos = getPosition();

					addSummon(summon);

					if (!g_game.placeCreature(summon, summonPos)) {
						removeSummon(summon);
					} else {
						g_game.addMagicEffect(getPosition(), NM_ME_MAGIC_ENERGY);
						g_game.addMagicEffect(summon->getPosition(), NM_ME_TELEPORT);
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
	if (mType->yellSpeedTicks > 0) {
		yellTicks += interval;

		if (yellTicks >= mType->yellSpeedTicks) {
			yellTicks = 0;

			if (!mType->voiceVector.empty() && (mType->yellChance >= (uint32_t)random_range(1, 100))) {
				uint32_t index = random_range(0, mType->voiceVector.size() - 1);
				const voiceBlock_t& vb = mType->voiceVector[index];

				if (vb.yellText) {
					g_game.internalCreatureSay(this, SPEAK_MONSTER_YELL, vb.text, false);
				} else {
					g_game.internalCreatureSay(this, SPEAK_MONSTER_SAY, vb.text, false);
				}
			}
		}
	}
}

void Monster::onWalk()
{
	Creature::onWalk();
}

bool Monster::pushItem(Item* item, int32_t radius)
{
	const Position& centerPos = item->getPosition();

	typedef std::pair<int32_t, int32_t> relPair;
	std::vector<relPair> relList;
	relList.push_back(relPair(-1, -1));
	relList.push_back(relPair(-1, 0));
	relList.push_back(relPair(-1, 1));
	relList.push_back(relPair(0, -1));
	relList.push_back(relPair(0, 1));
	relList.push_back(relPair(1, -1));
	relList.push_back(relPair(1, 0));
	relList.push_back(relPair(1, 1));

	std::random_shuffle(relList.begin(), relList.end());

	Position tryPos;

	for (int32_t n = 1; n <= radius; ++n) {
		for (std::vector<relPair>::iterator it = relList.begin(); it != relList.end(); ++it) {
			int32_t dx = it->first * n;
			int32_t dy = it->second * n;

			tryPos = centerPos;
			tryPos.x = tryPos.x + dx;
			tryPos.y = tryPos.y + dy;

			Tile* tile = g_game.getTile(tryPos.x, tryPos.y, tryPos.z);

			if (tile && g_game.canThrowObjectTo(centerPos, tryPos)) {
				if (g_game.internalMoveItem(item->getParent(), tile,
				                            INDEX_WHEREEVER, item, item->getItemCount(), NULL) == RET_NOERROR) {
					return true;
				}
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

		for (int32_t i = downItemSize - 1; i >= 0; --i) {
			assert(i >= 0 && i < downItemSize);
			Item* item = items->at(i);

			if (item && item->hasProperty(MOVEABLE) && (item->hasProperty(BLOCKPATH)
			        || item->hasProperty(BLOCKSOLID))) {
				if (moveCount < 20 && pushItem(item, 1)) {
					moveCount++;
				} else if (g_game.internalRemoveItem(item) == RET_NOERROR) {
					++removeCount;
				}
			}
		}

		if (removeCount > 0) {
			g_game.addMagicEffect(tile->getPosition(), NM_ME_POFF);
		}
	}
}

bool Monster::pushCreature(Creature* creature)
{
	Position monsterPos = creature->getPosition();

	std::vector<Direction> dirList;
	dirList.push_back(NORTH);
	dirList.push_back(SOUTH);
	dirList.push_back(WEST);
	dirList.push_back(EAST);

	std::random_shuffle(dirList.begin(), dirList.end());

	for (std::vector<Direction>::iterator it = dirList.begin(); it != dirList.end(); ++it) {
		const Position& tryPos = Spells::getCasterPosition(creature, *it);
		Tile* toTile = g_game.getTile(tryPos.x, tryPos.y, tryPos.z);

		if (toTile && !toTile->hasProperty(BLOCKPATH)) {
			if (g_game.internalMoveCreature(creature, *it) == RET_NOERROR) {
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
		Monster* lastPushedMonster = NULL;

		for (uint32_t i = 0; i < creatures->size();) {
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
			g_game.addMagicEffect(tile->getPosition(), NM_ME_BLOCKHIT);
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
				} else if (mType->staticAttackChance < (uint32_t)random_range(1, 100)) {
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
	std::vector<Direction> dirList;

	dirList.push_back(NORTH);
	dirList.push_back(SOUTH);
	dirList.push_back(WEST);
	dirList.push_back(EAST);

	std::random_shuffle(dirList.begin(), dirList.end());

	for (std::vector<Direction>::iterator it = dirList.begin(); it != dirList.end(); ++it) {
		if (canWalkTo(creaturePos, *it)) {
			dir = *it;
			return true;
		}
	}

	return false;
}

bool Monster::getDanceStep(const Position& creaturePos, Direction& dir,
                           bool keepAttack /*= true*/, bool keepDistance /*= true*/)
{
	bool canDoAttackNow = canUseAttack(creaturePos, attackedCreature);

	assert(attackedCreature != NULL);
	const Position& centerPos = attackedCreature->getPosition();
	uint32_t centerToDist = std::max<uint32_t>(std::abs(creaturePos.x - centerPos.x), std::abs(creaturePos.y - centerPos.y));
	uint32_t tmpDist;

	std::vector<Direction> dirList;

	if (!keepDistance || creaturePos.y - centerPos.y >= 0) {
		tmpDist = std::max<uint32_t>(std::abs((creaturePos.x) - centerPos.x), std::abs((creaturePos.y - 1) - centerPos.y));
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

	if (!keepDistance || creaturePos.y - centerPos.y <= 0) {
		tmpDist = std::max<uint32_t>(std::abs((creaturePos.x) - centerPos.x), std::abs((creaturePos.y + 1) - centerPos.y));
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

	if (!keepDistance || creaturePos.x - centerPos.x <= 0) {
		tmpDist = std::max<uint32_t>(std::abs((creaturePos.x + 1) - centerPos.x), std::abs(creaturePos.y - centerPos.y));
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

	if (!keepDistance || creaturePos.x - centerPos.x >= 0) {
		tmpDist = std::max<uint32_t>(std::abs((creaturePos.x - 1) - centerPos.x), std::abs(creaturePos.y - centerPos.y));
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
		std::random_shuffle(dirList.begin(), dirList.end());
		dir = dirList[random_range(0, dirList.size() - 1)];
		return true;
	}

	return false;
}

bool Monster::getDistanceStep(const Position& targetPos, Direction& dir, bool flee /* = false */)
{
	/*
	OK, understand this - I didn't intend to write as low code as possible, I wanted to make it easily readable so everyone can understand it, it basically works
	almost like a real Tibia for fleeing monsters (like Dragon on low health) and distance monsters (like orc spearman), they will now choose the path almost exactly like the real ones.

	I'm very well aware that the whole code can be shorter and better programmed, in fact - I'm hoping for someone to do that for me, I basically just described how should it work and how does it work on real Tibia.
	*/

	const Position& creaturePos = getPosition();
	int32_t distance = std::abs(creaturePos.x - targetPos.x) > std::abs(creaturePos.y - targetPos.y) ? std::abs(creaturePos.x - targetPos.x) : std::abs(creaturePos.y - targetPos.y);

	if (!flee && (distance > mType->targetDistance || !g_game.isSightClear(creaturePos, targetPos, true))) {
		return false;    // let the A* calculate it
	} else if (!flee && distance == mType->targetDistance) {
		return true;    // we don't really care here, since it's what we wanted to reach (a dancestep will take of dancing in that position)
	}

	int offsetx = creaturePos.x - targetPos.x;
	int offsety = creaturePos.y - targetPos.y;

	if (std::abs(offsetx) <= 1 && std::abs(offsety) <= 1) {
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

	if (std::abs(offsetx) == std::abs(offsety)) {
		//player is diagonal to the monster
		if (offsetx >= 1 && offsety >= 1) {
			// player is NW
			//escape to SE, S or E [and some extra]
			bool s = canWalkTo(creaturePos, SOUTH);
			bool e = canWalkTo(creaturePos, EAST);

			if (s && e) {
				dir = random_range(1, 2) == 1 ? SOUTH : EAST;
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
					dir = random_range(1, 2) == 1 ? NORTH : WEST;
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
				dir = random_range(1, 2) == 1 ? WEST : NORTH;
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
					dir = random_range(1, 2) == 1 ? SOUTH : EAST;
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
				dir = random_range(1, 2) == 1 ? NORTH : EAST;
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
					dir = random_range(1, 2) == 1 ? SOUTH : WEST;
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
				dir = random_range(1, 2) == 1 ? WEST : SOUTH;
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
					dir = random_range(1, 2) == 1 ? NORTH : EAST;
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
	if (std::abs(offsety) > std::abs(offsetx)) {
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
					dir = random_range(1, 2) == 1 ? WEST : EAST;
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
						dir = random_range(1, 2) == 1 ? WEST : EAST;
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
						dir = random_range(1, 2) == 1 ? SOUTHWEST : SOUTHEAST;
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
					dir = random_range(1, 2) == 1 ? WEST : EAST;
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
						dir = random_range(1, 2) == 1 ? WEST : EAST;
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
						dir = random_range(1, 2) == 1 ? NORTHWEST : NORTHEAST;
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
					dir = random_range(1, 2) == 1 ? NORTH : SOUTH;
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
						dir = random_range(1, 2) == 1 ? NORTH : SOUTH;
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
						dir = random_range(1, 2) == 1 ? SOUTHEAST : NORTHEAST;
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
					dir = random_range(1, 2) == 1 ? NORTH : SOUTH;
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
						dir = random_range(1, 2) == 1 ? NORTH : SOUTH;
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
						dir = random_range(1, 2) == 1 ? NORTHWEST : SOUTHWEST;
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
	switch (dir) {
		case NORTH:
			pos.y += -1;
			break;
		case WEST:
			pos.x += -1;
			break;
		case EAST:
			pos.x += 1;
			break;
		case SOUTH:
			pos.y += 1;
			break;
		default:
			break;
	}

	if (isInSpawnRange(pos)) {
		if (getWalkCache(pos) == 0) {
			return false;
		}

		Tile* tile = g_game.getTile(pos.x, pos.y, pos.z);

		if (tile && tile->getTopVisibleCreature(this) == NULL && tile->__queryAdd(0, this, 1, FLAG_PATHFINDING) == RET_NOERROR) {
			return true;
		}
	}

	return false;
}

void Monster::death()
{
	setAttackedCreature(NULL);

	for (std::list<Creature*>::iterator cit = summons.begin(); cit != summons.end(); ++cit) {
		(*cit)->changeHealth(-(*cit)->getHealth());
		(*cit)->setMaster(NULL);
		(*cit)->releaseThing2();
	}

	summons.clear();

	clearTargetList();
	clearFriendList();
	onIdleStatus();
}

Item* Monster::getCorpse()
{
	Item* corpse = Creature::getCorpse();

	if (corpse) {
		Creature* lastHitCreature_ = NULL;
		Creature* mostDamageCreature = NULL;

		if (getKillers(&lastHitCreature_, &mostDamageCreature) && mostDamageCreature) {
			uint32_t corpseOwner = 0;

			if (mostDamageCreature->getPlayer()) {
				corpseOwner = mostDamageCreature->getID();
			} else {
				const Creature* mostDamageCreatureMaster = mostDamageCreature->getMaster();

				if (mostDamageCreatureMaster && mostDamageCreatureMaster->getPlayer()) {
					corpseOwner = mostDamageCreatureMaster->getID();
				}
			}

			if (corpseOwner != 0) {
				corpse->setCorpseOwner(corpseOwner);
			}
		}
	}

	return corpse;
}

bool Monster::inDespawnRange(const Position& pos)
{
	if (spawn) {
		if (Monster::despawnRadius == 0) {
			return false;
		}

		if (!Spawns::getInstance()->isInZone(masterPos, Monster::despawnRadius, pos)) {
			return true;
		}

		if (Monster::despawnRange == 0) {
			return false;
		}

		if (std::abs(pos.z - masterPos.z) > Monster::despawnRange) {
			return true;
		}

		return false;
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
		int32_t dx = attackedCreaturePos.x - pos.x;
		int32_t dy = attackedCreaturePos.y - pos.y;

		if (std::abs(dx) > std::abs(dy)) {
			//look EAST/WEST
			if (dx < 0) {
				newDir = WEST;
			} else {
				newDir = EAST;
			}
		} else if (std::abs(dx) < std::abs(dy)) {
			//look NORTH/SOUTH
			if (dy < 0) {
				newDir = NORTH;
			} else {
				newDir = SOUTH;
			}
		} else {
			if (dx < 0 && dy < 0) {
				if (getDirection() == SOUTH) {
					newDir = WEST;
				} else if (getDirection() == EAST) {
					newDir = NORTH;
				}
			} else if (dx < 0 && dy > 0) {
				if (getDirection() == NORTH) {
					newDir = WEST;
				} else if (getDirection() == EAST) {
					newDir = SOUTH;
				}
			} else if (dx > 0 && dy < 0) {
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

void Monster::dropLoot(Container* corpse)
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

void Monster::drainHealth(Creature* attacker, CombatType_t combatType, int32_t damage)
{
	Creature::drainHealth(attacker, combatType, damage);

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
	} else {
		bool result = selectTarget(creature);

		if (result) {
			targetChangeCooldown = 8000;
			targetChangeTicks = 0;
		}

		return result;
	}

	return false;
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

			setFollowCreature(NULL);
			setAttackedCreature(NULL);

			//destroy summons
			for (std::list<Creature*>::iterator cit = summons.begin(); cit != summons.end(); ++cit) {
				(*cit)->changeHealth(-(*cit)->getHealth());
				(*cit)->setMaster(NULL);
				(*cit)->releaseThing2();
			}

			summons.clear();

			isMasterInRange = true;
			updateTargetList();
			updateIdleStatus();

			//Notify surrounding about the change
			SpectatorVec list;
			g_game.getSpectators(list, getPosition(), true);
			g_game.getSpectators(list, creature->getPosition(), true);

			for (SpectatorVec::iterator it = list.begin(); it != list.end(); ++it) {
				(*it)->onCreatureConvinced(creature, this);
			}

			if (spawn) {
				spawn->removeMonster(this);
				spawn = NULL;
				masterRadius = -1;
			}

			return true;
		}
	} else {
		creature->addSummon(this);
		setFollowCreature(NULL);
		setAttackedCreature(NULL);

		for (std::list<Creature*>::iterator cit = summons.begin(); cit != summons.end(); ++cit) {
			(*cit)->changeHealth(-(*cit)->getHealth());
			(*cit)->setMaster(NULL);
			(*cit)->releaseThing2();
		}

		summons.clear();

		isMasterInRange = true;
		updateTargetList();
		updateIdleStatus();

		//Notify surrounding about the change
		SpectatorVec list;
		g_game.getSpectators(list, getPosition(), true);
		g_game.getSpectators(list, creature->getPosition(), true);

		for (SpectatorVec::iterator it = list.begin(); it != list.end(); ++it) {
			(*it)->onCreatureConvinced(creature, this);
		}

		if (spawn) {
			spawn->removeMonster(this);
			spawn = NULL;
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
