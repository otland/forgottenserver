// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "otpch.h"

#include "creature.h"

#include "combat.h"
#include "configmanager.h"
#include "events.h"
#include "game.h"
#include "monster.h"
#include "party.h"
#include "scheduler.h"
#include "spectators.h"

double Creature::speedA = 857.36;
double Creature::speedB = 261.29;
double Creature::speedC = -4795.01;

extern Game g_game;
extern CreatureEvents* g_creatureEvents;

Creature::Creature() { onIdleStatus(); }

Creature::~Creature()
{
	for (const auto& summon : summons) {
		if (summon) {
			summon->removeAttackedCreature();
			summon->removeMaster();
		}
	}

	for (Condition* condition : conditions) {
		condition->endCondition(shared_from_this());
	}

	for (auto condition : conditions) {
		delete condition;
	}

	releaseFollowers();
}

bool Creature::canSee(const Position& myPos, const Position& pos, int32_t viewRangeX, int32_t viewRangeY)
{
	if (myPos.z <= 7) {
		// we are on ground level or above (7 -> 0)
		// view is from 7 -> 0
		if (pos.z > 7) {
			return false;
		}
	} else if (myPos.z >= 8) {
		// we are underground (8 -> 15)
		// we can't see floors above 8
		if (pos.z < 8) {
			return false;
		}

		// view is +/- 2 from the floor we stand on
		if (myPos.getDistanceZ(pos) > 2) {
			return false;
		}
	}

	int32_t offsetz = myPos.getOffsetZ(pos);
	return (pos.getX() >= myPos.getX() - viewRangeX + offsetz) && (pos.getX() <= myPos.getX() + viewRangeX + offsetz) &&
	       (pos.getY() >= myPos.getY() - viewRangeY + offsetz) && (pos.getY() <= myPos.getY() + viewRangeY + offsetz);
}

bool Creature::canSee(const Position& pos) const
{
	return canSee(getPosition(), pos, Map::maxViewportX, Map::maxViewportY);
}

bool Creature::canSeeCreature(std::shared_ptr<const Creature> creature) const
{
	if (!canSeeGhostMode(creature) && creature->isInGhostMode()) {
		return false;
	}

	if (!canSeeInvisibility() && creature->isInvisible()) {
		return false;
	}
	return true;
}

void Creature::setSkull(Skulls_t newSkull)
{
	skull = newSkull;
	g_game.updateCreatureSkull(shared_from_this());
}

int64_t Creature::getTimeSinceLastMove() const
{
	if (lastStep) {
		return OTSYS_TIME() - lastStep;
	}
	return std::numeric_limits<int64_t>::max();
}

int32_t Creature::getWalkDelay(Direction dir) const
{
	if (lastStep == 0) {
		return 0;
	}

	int64_t ct = OTSYS_TIME();
	int64_t stepDuration = getStepDuration(dir);
	return stepDuration - (ct - lastStep);
}

int32_t Creature::getWalkDelay() const
{
	// Used for auto-walking
	if (lastStep == 0) {
		return 0;
	}

	int64_t ct = OTSYS_TIME();
	int64_t stepDuration = getStepDuration() * lastStepCost;
	return stepDuration - (ct - lastStep);
}

void Creature::onThink(uint32_t interval)
{
	if (followCreature && master != followCreature && !canSeeCreature(followCreature)) {
		onCreatureDisappear(followCreature, false);
	}

	if (attackedCreature && master != attackedCreature && !canSeeCreature(attackedCreature)) {
		onCreatureDisappear(attackedCreature, false);
	}

	blockTicks += interval;
	if (blockTicks >= 1000) {
		blockCount = std::min<uint32_t>(blockCount + 1, 2);
		blockTicks = 0;
	}

	// scripting event - onThink
	const CreatureEventList& thinkEvents = getCreatureEvents(CREATURE_EVENT_THINK);
	for (CreatureEvent* thinkEvent : thinkEvents) {
		thinkEvent->executeOnThink(shared_from_this(), interval);
	}
}

void Creature::forceUpdatePath()
{
	if (!attackedCreature && !followCreature) {
		return;
	}

	lastPathUpdate = OTSYS_TIME() + getNumber(ConfigManager::PATHFINDING_DELAY);
	g_dispatcher.addTask(createTask([id = getID()]() { g_game.updateCreatureWalk(id); }));
}

void Creature::onAttacking(uint32_t interval)
{
	if (!attackedCreature) {
		return;
	}

	onAttacked();
	attackedCreature->onAttacked();

	if (g_game.isSightClear(getPosition(), attackedCreature->getPosition(), true)) {
		doAttacking(interval);
	}
}

void Creature::onIdleStatus()
{
	if (!isDead()) {
		damageMap.clear();
		lastHitCreatureId = 0;
	}
}

void Creature::onWalk()
{
	if (getWalkDelay() <= 0) {
		Direction dir;
		uint32_t flags = FLAG_IGNOREFIELDDAMAGE;
		if (getNextStep(dir, flags)) {
			ReturnValue ret = g_game.internalMoveCreature(shared_from_this(), dir, flags);
			if (ret != RETURNVALUE_NOERROR) {
				if (auto player = getPlayer()) {
					player->sendCancelMessage(ret);
					player->sendCancelWalk();
				}
			}
		} else {
			stopEventWalk();

			if (listWalkDir.empty()) {
				onWalkComplete();
			}
		}
	}

	removeFollowers();
	updateFollowersPaths();

	if (cancelNextWalk) {
		listWalkDir.clear();
		onWalkAborted();
		cancelNextWalk = false;
	}

	if (eventWalk != 0) {
		eventWalk = 0;
		addEventWalk();
	}

	if (attackedCreature || followCreature) {
		if (lastPathUpdate < OTSYS_TIME()) {
			g_dispatcher.addTask(createTask([id = getID()]() { g_game.updateCreatureWalk(id); }));
			lastPathUpdate = OTSYS_TIME() + getNumber(ConfigManager::PATHFINDING_DELAY);
		}
	}
}

void Creature::onWalk(Direction& dir)
{
	if (!hasCondition(CONDITION_DRUNK)) {
		return;
	}

	uint16_t rand = uniform_random(0, 399);
	if (rand / 4 > getDrunkenness()) {
		return;
	}

	dir = static_cast<Direction>(rand % 4);
	g_game.internalCreatureSay(shared_from_this(), TALKTYPE_MONSTER_SAY, "Hicks!", false);
}

bool Creature::getNextStep(Direction& dir, uint32_t&)
{
	if (listWalkDir.empty()) {
		return false;
	}

	dir = listWalkDir.back();
	listWalkDir.pop_back();
	onWalk(dir);
	return true;
}

void Creature::startAutoWalk()
{
	auto player = getPlayer();
	if (player && player->isMovementBlocked()) {
		player->sendCancelWalk();
		return;
	}

	addEventWalk(listWalkDir.size() == 1);
}

void Creature::startAutoWalk(Direction direction)
{
	auto player = getPlayer();
	if (player && player->isMovementBlocked()) {
		player->sendCancelWalk();
		return;
	}

	listWalkDir.clear();
	listWalkDir.push_back(direction);
	addEventWalk(true);
}

void Creature::startAutoWalk(const std::vector<Direction>& listDir)
{
	if (hasCondition(CONDITION_ROOT)) {
		return;
	}

	auto player = getPlayer();
	if (player && player->isMovementBlocked()) {
		player->sendCancelWalk();
		return;
	}

	listWalkDir = listDir;
	addEventWalk(listWalkDir.size() == 1);
}

void Creature::addEventWalk(bool firstStep)
{
	cancelNextWalk = false;

	if (getStepSpeed() <= 0) {
		return;
	}

	if (eventWalk != 0) {
		return;
	}

	int64_t ticks = getEventStepTicks(firstStep);
	if (ticks <= 0) {
		return;
	}

	// Take first step right away, but still queue the next
	if (ticks == 1) {
		g_game.checkCreatureWalk(getID());
	}

	eventWalk = g_scheduler.addEvent(createSchedulerTask(ticks, [id = getID()]() { g_game.checkCreatureWalk(id); }));
}

void Creature::stopEventWalk()
{
	if (eventWalk != 0) {
		g_scheduler.stopEvent(eventWalk);
		eventWalk = 0;
	}
}

void Creature::updateIcons() const
{
	SpectatorVec spectators;
	g_game.map.getSpectators(spectators, position, true, true);
	for (auto spectator : spectators) {
		assert(spectator->getPlayer() != nullptr);
		std::static_pointer_cast<Player>(spectator)->sendUpdateCreatureIcons(this->getCreature());
	}
}

void Creature::onCreatureAppear(std::shared_ptr<Creature> creature, bool isLogin)
{
	if (creature.get() == this) {
		if (isLogin) {
			setLastPosition(getPosition());
		}
	}
}

void Creature::onRemoveCreature(std::shared_ptr<Creature> creature, bool) { onCreatureDisappear(creature, true); }

void Creature::onCreatureDisappear(std::shared_ptr<const Creature> creature, bool isLogout)
{
	if (attackedCreature == creature) {
		removeAttackedCreature();
		onAttackedCreatureDisappear(isLogout);
	}

	if (followCreature == creature) {
		removeFollowCreature();
		onFollowCreatureDisappear(isLogout);
	}
}

void Creature::updateFollowCreaturePath(FindPathParams& fpp)
{
	listWalkDir.clear();

	if (getPathTo(followCreature->getPosition(), listWalkDir, fpp)) {
		hasFollowPath = true;
		startAutoWalk();
	} else {
		hasFollowPath = false;
	}
}

void Creature::onChangeZone(ZoneType_t zone)
{
	if (attackedCreature && zone == ZONE_PROTECTION) {
		onCreatureDisappear(attackedCreature, false);
	}
}

void Creature::onAttackedCreatureChangeZone(ZoneType_t zone)
{
	if (zone == ZONE_PROTECTION) {
		onCreatureDisappear(attackedCreature, false);
	}
}

void Creature::onCreatureMove(std::shared_ptr<Creature> creature, std::shared_ptr<const Tile> newTile,
                              const Position& newPos, std::shared_ptr<const Tile> oldTile, const Position& oldPos,
                              bool teleport)
{
	if (creature.get() == this) {
		lastStep = OTSYS_TIME();
		lastStepCost = 1;

		if (!teleport) {
			if (oldPos.z != newPos.z) {
				// floor change extra cost
				lastStepCost = 2;
			} else if (newPos.getDistanceX(oldPos) >= 1 && newPos.getDistanceY(oldPos) >= 1) {
				// diagonal extra cost
				lastStepCost = 3;
			}
		} else {
			stopEventWalk();
		}

		if (!summons.empty()) {
			// check if any of our summons is out of range (+/- 2 floors or 30 tiles away)
			std::forward_list<std::shared_ptr<Creature>> despawnList;
			for (const auto& summon : summons) {
				if (!summon) {
					continue;
				}
				const Position& pos = summon->getPosition();
				if (newPos.getDistanceZ(pos) > 2 || std::max(newPos.getDistanceX(pos), newPos.getDistanceY(pos)) > 30) {
					despawnList.push_front(summon);
				}
			}

			for (const auto& despawnCreature : despawnList) {
				if (despawnCreature) {
					g_game.removeCreature(despawnCreature, true);
				}
			}
		}

		if (newTile->getZone() != oldTile->getZone()) {
			tfs::events::creature::onChangeZone(shared_from_this(), oldTile->getZone(), newTile->getZone());
			onChangeZone(getZone());
		}
	}

	if (creature == followCreature || (creature.get() == this && followCreature)) {
		if (newPos.z != oldPos.z || !canSee(followCreature->getPosition())) {
			onCreatureDisappear(followCreature, false);
		}
	}

	if (creature == attackedCreature || (creature.get() == this && attackedCreature)) {
		if (newPos.z != oldPos.z || !canSee(attackedCreature->getPosition())) {
			onCreatureDisappear(attackedCreature, false);
		} else {
			if (hasExtraSwing()) {
				// our target is moving lets see if we can get in hit
				g_dispatcher.addTask([id = getID()]() { g_game.checkCreatureAttack(id); });
			}

			if (newTile->getZone() != oldTile->getZone()) {
				onAttackedCreatureChangeZone(attackedCreature->getZone());
			}
		}
	}
}

CreatureVector Creature::getKillers()
{
	CreatureVector killers;
	const int64_t timeNow = OTSYS_TIME();
	const uint32_t inFightTicks = getNumber(ConfigManager::PZ_LOCKED);
	for (const auto& it : damageMap) {
		std::shared_ptr<Creature> attacker = g_game.getCreatureByID(it.first);
		if (attacker && attacker.get() != this && timeNow - it.second.ticks <= inFightTicks) {
			killers.push_back(attacker);
		}
	}
	return killers;
}

void Creature::onDeath()
{
	bool lastHitUnjustified = false;
	bool mostDamageUnjustified = false;
	std::shared_ptr<Creature> lastHitCreature = g_game.getCreatureByID(lastHitCreatureId);
	std::shared_ptr<Creature> lastHitCreatureMaster;
	if (lastHitCreature) {
		lastHitUnjustified = lastHitCreature->onKilledCreature(shared_from_this());
		lastHitCreatureMaster = lastHitCreature->getMaster();
	} else {
		lastHitCreatureMaster = nullptr;
	}

	std::shared_ptr<Creature> mostDamageCreature = nullptr;

	const int64_t timeNow = OTSYS_TIME();
	const uint32_t inFightTicks = getNumber(ConfigManager::PZ_LOCKED);
	int32_t mostDamage = 0;
	std::map<std::shared_ptr<Creature>, uint64_t> experienceMap;
	for (const auto& it : damageMap) {
		if (std::shared_ptr<Creature> attacker = g_game.getCreatureByID(it.first)) {
			CountBlock_t cb = it.second;
			if ((cb.total > mostDamage && (timeNow - cb.ticks <= inFightTicks))) {
				mostDamage = cb.total;
				mostDamageCreature = attacker;
			}

			if (attacker.get() != this) {
				uint64_t gainExp = getGainedExperience(attacker);
				if (auto attackerPlayer = attacker->getPlayer()) {
					attackerPlayer->removeAttacked(getPlayer());

					Party* party = attackerPlayer->getParty();
					if (party && party->getLeader() && party->isSharedExperienceActive() &&
					    party->isSharedExperienceEnabled()) {
						attacker = party->getLeader();
					}
				}

				auto tmpIt = experienceMap.find(attacker);
				if (tmpIt == experienceMap.end()) {
					experienceMap[attacker] = gainExp;
				} else {
					tmpIt->second += gainExp;
				}
			}
		}
	}

	for (const auto& it : experienceMap) {
		it.first->onGainExperience(it.second, shared_from_this());
	}

	if (mostDamageCreature) {
		if (mostDamageCreature != lastHitCreature && mostDamageCreature != lastHitCreatureMaster) {
			std::shared_ptr<Creature> mostDamageCreatureMaster = mostDamageCreature->getMaster();
			if (lastHitCreature != mostDamageCreatureMaster &&
			    (!lastHitCreatureMaster || mostDamageCreatureMaster != lastHitCreatureMaster)) {
				mostDamageUnjustified = mostDamageCreature->onKilledCreature(shared_from_this(), false);
			}
		}
	}

	bool droppedCorpse = dropCorpse(lastHitCreature, mostDamageCreature, lastHitUnjustified, mostDamageUnjustified);
	death(lastHitCreature);

	if (master) {
		setMaster(nullptr);
	}

	if (droppedCorpse) {
		g_game.removeCreature(shared_from_this(), false);
	}
}

bool Creature::dropCorpse(std::shared_ptr<Creature> lastHitCreature, std::shared_ptr<Creature> mostDamageCreature,
                          bool lastHitUnjustified, bool mostDamageUnjustified)
{
	if (!lootDrop && getMonster()) {
		if (master) {
			// scripting event - onDeath
			const CreatureEventList& deathEvents = getCreatureEvents(CREATURE_EVENT_DEATH);
			for (CreatureEvent* deathEvent : deathEvents) {
				deathEvent->executeOnDeath(shared_from_this(), nullptr, lastHitCreature, mostDamageCreature,
				                           lastHitUnjustified, mostDamageUnjustified);
			}
		}

		g_game.addMagicEffect(getPosition(), CONST_ME_POFF);
	} else {
		std::shared_ptr<Item> splash;
		switch (getRace()) {
			case RACE_VENOM:
				splash = Item::CreateItem(ITEM_FULLSPLASH, FLUID_SLIME);
				break;

			case RACE_BLOOD:
				splash = Item::CreateItem(ITEM_FULLSPLASH, FLUID_BLOOD);
				break;

			case RACE_INK:
				splash = Item::CreateItem(ITEM_FULLSPLASH, FLUID_INK);
				break;

			default:
				splash = nullptr;
				break;
		}

		auto tile = getTile();

		if (splash) {
			g_game.internalAddItem(tile, splash, INDEX_WHEREEVER, FLAG_NOLIMIT);
			g_game.startDecay(splash);
		}

		std::shared_ptr<Item> corpse = getCorpse(lastHitCreature, mostDamageCreature);
		if (corpse) {
			g_game.internalAddItem(tile, corpse, INDEX_WHEREEVER, FLAG_NOLIMIT);
			g_game.startDecay(corpse);
		}

		// scripting event - onDeath
		for (CreatureEvent* deathEvent : getCreatureEvents(CREATURE_EVENT_DEATH)) {
			deathEvent->executeOnDeath(shared_from_this(), corpse, lastHitCreature, mostDamageCreature,
			                           lastHitUnjustified, mostDamageUnjustified);
		}

		if (corpse) {
			dropLoot(corpse->getContainer(), lastHitCreature);
		}
	}

	return true;
}

bool Creature::hasBeenAttacked(uint32_t attackerId)
{
	auto it = damageMap.find(attackerId);
	if (it == damageMap.end()) {
		return false;
	}
	return (OTSYS_TIME() - it->second.ticks) <= getNumber(ConfigManager::PZ_LOCKED);
}

std::shared_ptr<Item> Creature::getCorpse(std::shared_ptr<Creature>, std::shared_ptr<Creature>)
{
	return Item::CreateItem(getLookCorpse());
}

void Creature::changeHealth(int32_t healthChange, bool sendHealthChange /* = true*/)
{
	int32_t oldHealth = health;

	if (healthChange > 0) {
		health += std::min<int32_t>(healthChange, getMaxHealth() - health);
	} else {
		health = std::max<int32_t>(0, health + healthChange);
	}

	if (sendHealthChange && oldHealth != health) {
		g_game.addCreatureHealth(shared_from_this());
	}

	if (isDead()) {
		g_dispatcher.addTask([id = getID()]() { g_game.executeDeath(id); });
	}
}

void Creature::gainHealth(std::shared_ptr<Creature> healer, int32_t healthGain)
{
	changeHealth(healthGain);
	if (healer) {
		healer->onTargetCreatureGainHealth(shared_from_this(), healthGain);
	}
}

void Creature::drainHealth(std::shared_ptr<Creature> attacker, int32_t damage)
{
	changeHealth(-damage, false);

	if (attacker) {
		attacker->onAttackedCreatureDrainHealth(shared_from_this(), damage);
	} else {
		lastHitCreatureId = 0;
	}
}

BlockType_t Creature::blockHit(std::shared_ptr<Creature> attacker, CombatType_t combatType, int32_t& damage,
                               bool checkDefense /* = false */, bool checkArmor /* = false */, bool /* field = false */,
                               bool /* ignoreResistances = false */)
{
	BlockType_t blockType = BLOCK_NONE;

	if (isImmune(combatType)) {
		damage = 0;
		blockType = BLOCK_IMMUNITY;
	} else if (combatType != COMBAT_HEALING && (checkDefense || checkArmor)) {
		bool hasDefense = false;

		if (blockCount > 0) {
			--blockCount;
			hasDefense = true;
		}

		if (checkDefense && hasDefense && canUseDefense) {
			int32_t defense = getDefense();
			damage -= uniform_random(defense / 2, defense);
			if (damage <= 0) {
				damage = 0;
				blockType = BLOCK_DEFENSE;
				checkArmor = false;
			}
		}

		if (checkArmor) {
			int32_t armor = getArmor();
			if (armor > 3) {
				damage -= uniform_random(armor / 2, armor - (armor % 2 + 1));
			} else if (armor > 0) {
				--damage;
			}

			if (damage <= 0) {
				damage = 0;
				blockType = BLOCK_ARMOR;
			}
		}

		if (hasDefense && blockType != BLOCK_NONE) {
			onBlockHit();
		}
	}

	if (attacker) {
		if (auto attackerPlayer = attacker->getPlayer()) {
			for (int32_t slot = CONST_SLOT_FIRST; slot <= CONST_SLOT_LAST; ++slot) {
				if (!attackerPlayer->isItemAbilityEnabled(static_cast<slots_t>(slot))) {
					continue;
				}

				auto item = attackerPlayer->getInventoryItem(static_cast<slots_t>(slot));
				if (!item) {
					continue;
				}

				const uint16_t boostPercent = item->getBoostPercent(combatType);
				if (boostPercent != 0) {
					damage += std::round(damage * (boostPercent / 100.));
				}
			}
		}

		if (damage <= 0) {
			damage = 0;
			blockType = BLOCK_ARMOR;
		}

		if (combatType != COMBAT_HEALING) {
			attacker->onAttackedCreature(shared_from_this());
			attacker->onAttackedCreatureBlockHit(blockType);
			if (attacker->getMaster() && attacker->getMaster()->getPlayer()) {
				auto masterPlayer = attacker->getMaster()->getPlayer();
				masterPlayer->onAttackedCreature(shared_from_this());
			}
		}
	}

	if (combatType != COMBAT_HEALING) {
		onAttacked();
	}
	return blockType;
}

void Creature::setAttackedCreature(std::shared_ptr<Creature> creature)
{
	if (isAttackingCreature(creature)) {
		return;
	}

	if (!canAttackCreature(creature)) {
		removeAttackedCreature();
		return;
	}

	attackedCreature = creature;
	creature->addFollower(shared_from_this());
	onAttackedCreature(attackedCreature);
	attackedCreature->onAttacked();

	for (const auto& summon : summons) {
		if (summon) {
			summon->setAttackedCreature(creature);
		}
	}
}

void Creature::removeAttackedCreature()
{
	attackedCreature = nullptr;

	for (const auto& summon : summons) {
		if (summon) {
			summon->removeAttackedCreature();
		}
	}
}

bool Creature::canAttackCreature(std::shared_ptr<Creature> creature)
{
	const auto& creaturePos = creature->getPosition();
	if (creaturePos.z != getPosition().z) {
		return false;
	}
	return canSee(creaturePos);
}

void Creature::getPathSearchParams(std::shared_ptr<const Creature>, FindPathParams& fpp) const
{
	fpp.fullPathSearch = !hasFollowPath;
	fpp.clearSight = true;
	fpp.maxSearchDist = Map::maxViewportX + Map::maxViewportY;
	fpp.minTargetDist = 1;
	fpp.maxTargetDist = 1;
}

void Creature::setFollowCreature(std::shared_ptr<Creature> creature)
{
	if (isFollowingCreature(creature)) {
		return;
	}

	if (!canFollowCreature(creature)) {
		removeFollowCreature();
		return;
	}

	followCreature = creature;
	creature->addFollower(shared_from_this());
	hasFollowPath = false;
	onFollowCreature(creature);
}

void Creature::removeFollowCreature()
{
	followCreature = nullptr;
	onUnfollowCreature();
}

bool Creature::canFollowCreature(std::shared_ptr<Creature> creature)
{
	const auto& creaturePos = creature->getPosition();
	if (creaturePos.z != getPosition().z) {
		return false;
	}
	return canSee(creaturePos);
}

void Creature::onFollowCreature(std::shared_ptr<const Creature>)
{
	if (!listWalkDir.empty()) {
		listWalkDir.clear();
		onWalkAborted();
	}
}

void Creature::onUnfollowCreature() { hasFollowPath = false; }

// Pathfinding Events
bool Creature::isFollower(std::shared_ptr<const Creature> creature)
{
	auto it = std::find(followers.begin(), followers.end(), creature);
	return it != followers.end();
}

void Creature::addFollower(std::shared_ptr<Creature> creature)
{
	// store a weak reference to avoid ownership cycles
	if (!isFollower(creature)) {
		followers.push_back(creature->shared_from_this());
	}
}

void Creature::removeFollower(std::shared_ptr<Creature> creature)
{
	auto it = std::find(followers.begin(), followers.end(), creature);
	if (it != followers.end()) {
		followers.erase(it);
	}
}

void Creature::removeFollowers()
{
	const Position& position = getPosition();

	followers.erase(std::remove_if(followers.begin(), followers.end(),
	                               [&position](auto creature) {
		                               const Position& followerPosition = creature->getPosition();
		                               uint16_t distance = position.getDistanceX(followerPosition) +
		                                                   position.getDistanceY(followerPosition);
		                               bool isInRemoveRange = distance >= Map::maxViewportX + Map::maxViewportY ||
		                                                      position.z != followerPosition.z;
		                               return isInRemoveRange;
	                               }),
	                followers.end());
}

void Creature::releaseFollowers() { followers.clear(); }

void Creature::updateFollowersPaths()
{
	if (followers.empty()) {
		return;
	}

	const Position& thisPosition = getPosition();
	for (const auto& follower : followers) {
		if (follower) {
			const Position& followerPosition = follower->getPosition();

			if (follower->lastPathUpdate < OTSYS_TIME()) {
				continue;
			}

			if (thisPosition.getDistanceX(followerPosition) >= Map::maxViewportX ||
			    thisPosition.getDistanceY(followerPosition) >= Map::maxViewportY) {
				continue;
			}

			g_dispatcher.addTask(createTask([id = follower->getID()]() { g_game.updateCreatureWalk(id); }));
			follower->lastPathUpdate = OTSYS_TIME() + getNumber(ConfigManager::PATHFINDING_DELAY);
		}
	}
}

double Creature::getDamageRatio(std::shared_ptr<Creature> attacker) const
{
	uint32_t totalDamage = 0;
	uint32_t attackerDamage = 0;

	for (const auto& it : damageMap) {
		const CountBlock_t& cb = it.second;
		totalDamage += cb.total;
		if (it.first == attacker->getID()) {
			attackerDamage += cb.total;
		}
	}

	if (totalDamage == 0) {
		return 0;
	}

	return (static_cast<double>(attackerDamage) / totalDamage);
}

uint64_t Creature::getGainedExperience(std::shared_ptr<Creature> attacker) const
{
	return std::floor(getDamageRatio(attacker) * getLostExperience());
}

void Creature::addDamagePoints(std::shared_ptr<Creature> attacker, int32_t damagePoints)
{
	if (damagePoints <= 0) {
		return;
	}

	uint32_t attackerId = attacker->id;

	auto& cb = damageMap[attackerId];
	cb.ticks = OTSYS_TIME();
	cb.total += damagePoints;

	lastHitCreatureId = attackerId;
}

void Creature::onAddCondition(ConditionType_t type)
{
	if (type == CONDITION_PARALYZE && hasCondition(CONDITION_HASTE)) {
		removeCondition(CONDITION_HASTE);
	} else if (type == CONDITION_HASTE && hasCondition(CONDITION_PARALYZE)) {
		removeCondition(CONDITION_PARALYZE);
	}
}

void Creature::onAddCombatCondition(ConditionType_t)
{
	//
}

void Creature::onEndCondition(ConditionType_t)
{
	//
}

void Creature::onTickCondition(ConditionType_t type, bool& bRemove)
{
	std::shared_ptr<const Tile> tile = getTile();
	if (!tile) {
		return;
	}

	auto field = tile->getFieldItem();
	if (!field) {
		return;
	}

	switch (type) {
		case CONDITION_FIRE:
			bRemove = (field->getCombatType() != COMBAT_FIREDAMAGE);
			break;
		case CONDITION_ENERGY:
			bRemove = (field->getCombatType() != COMBAT_ENERGYDAMAGE);
			break;
		case CONDITION_POISON:
			bRemove = (field->getCombatType() != COMBAT_EARTHDAMAGE);
			break;
		case CONDITION_FREEZING:
			bRemove = (field->getCombatType() != COMBAT_ICEDAMAGE);
			break;
		case CONDITION_DAZZLED:
			bRemove = (field->getCombatType() != COMBAT_HOLYDAMAGE);
			break;
		case CONDITION_CURSED:
			bRemove = (field->getCombatType() != COMBAT_DEATHDAMAGE);
			break;
		case CONDITION_DROWN:
			bRemove = (field->getCombatType() != COMBAT_DROWNDAMAGE);
			break;
		case CONDITION_BLEEDING:
			bRemove = (field->getCombatType() != COMBAT_PHYSICALDAMAGE);
			break;
		default:
			break;
	}
}

void Creature::onCombatRemoveCondition(Condition* condition) { removeCondition(condition); }

void Creature::onAttacked()
{
	//
}

void Creature::onAttackedCreatureDrainHealth(std::shared_ptr<Creature> target, int32_t points)
{
	target->addDamagePoints(shared_from_this(), points);
}

bool Creature::onKilledCreature(std::shared_ptr<Creature> target, bool)
{
	if (master) {
		master->onKilledCreature(target);
	}

	// scripting event - onKill
	const CreatureEventList& killEvents = getCreatureEvents(CREATURE_EVENT_KILL);
	for (CreatureEvent* killEvent : killEvents) {
		killEvent->executeOnKill(shared_from_this(), target);
	}
	return false;
}

void Creature::onGainExperience(uint64_t gainExp, std::shared_ptr<Creature> target)
{
	if (gainExp == 0 || !master) {
		return;
	}

	gainExp /= 2;
	master->onGainExperience(gainExp, target);

	SpectatorVec spectators;
	g_game.map.getSpectators(spectators, position, false, true);
	if (spectators.empty()) {
		return;
	}

	TextMessage message(MESSAGE_EXPERIENCE_OTHERS, ucfirst(getNameDescription()) + " gained " +
	                                                   std::to_string(gainExp) +
	                                                   (gainExp != 1 ? " experience points." : " experience point."));
	message.position = position;
	message.primary.color = TEXTCOLOR_WHITE_EXP;
	message.primary.value = gainExp;

	for (auto spectator : spectators) {
		assert(spectator->getPlayer() != nullptr);
		std::static_pointer_cast<Player>(spectator)->sendTextMessage(message);
	}
}

bool Creature::setMaster(std::shared_ptr<Creature> newMaster)
{
	if (!newMaster && !master) {
		return false;
	}

	if (newMaster) {
		// store a shared_ptr reference in the master's summons list to keep the summon alive
		newMaster->summons.push_back(shared_from_this());
	}

	std::shared_ptr<Creature> oldMaster = master;
	master = newMaster;

	if (oldMaster) {
		auto summon = std::find_if(oldMaster->summons.begin(), oldMaster->summons.end(),
		                           [this](const auto& ptr) { return ptr.get() == this; });
		if (summon != oldMaster->summons.end()) {
			oldMaster->summons.erase(summon);
		}
	}
	return true;
}

bool Creature::addCondition(Condition* condition, bool force /* = false*/)
{
	if (!condition) {
		return false;
	}

	if (!force && condition->getType() == CONDITION_HASTE && hasCondition(CONDITION_PARALYZE)) {
		int64_t walkDelay = getWalkDelay();
		if (walkDelay > 0) {
			g_scheduler.addEvent(
			    createSchedulerTask(walkDelay, [=, id = getID()]() { g_game.forceAddCondition(id, condition); }));
			return false;
		}
	}

	Condition* prevCond = getCondition(condition->getType(), condition->getId(), condition->getSubId());
	if (prevCond) {
		prevCond->addCondition(shared_from_this(), condition);
		delete condition;
		return true;
	}

	if (condition->startCondition(shared_from_this())) {
		conditions.push_back(condition);
		onAddCondition(condition->getType());
		return true;
	}

	delete condition;
	return false;
}

bool Creature::addCombatCondition(Condition* condition)
{
	// Caution: condition variable could be deleted after the call to addCondition
	ConditionType_t type = condition->getType();

	if (!addCondition(condition)) {
		return false;
	}

	onAddCombatCondition(type);
	return true;
}

void Creature::removeCondition(ConditionType_t type, bool force /* = false*/)
{
	auto it = conditions.begin(), end = conditions.end();
	while (it != end) {
		Condition* condition = *it;
		if (condition->getType() != type) {
			++it;
			continue;
		}

		if (!force && type == CONDITION_PARALYZE) {
			int64_t walkDelay = getWalkDelay();
			if (walkDelay > 0) {
				g_scheduler.addEvent(
				    createSchedulerTask(walkDelay, [=, id = getID()]() { g_game.forceRemoveCondition(id, type); }));
				return;
			}
		}

		it = conditions.erase(it);

		condition->endCondition(shared_from_this());
		delete condition;

		onEndCondition(type);
	}
}

void Creature::removeCondition(ConditionType_t type, ConditionId_t conditionId, bool force /* = false*/)
{
	auto it = conditions.begin(), end = conditions.end();
	while (it != end) {
		Condition* condition = *it;
		if (condition->getType() != type || condition->getId() != conditionId) {
			++it;
			continue;
		}

		if (!force && type == CONDITION_PARALYZE) {
			int64_t walkDelay = getWalkDelay();
			if (walkDelay > 0) {
				g_scheduler.addEvent(
				    createSchedulerTask(walkDelay, [=, id = getID()]() { g_game.forceRemoveCondition(id, type); }));
				return;
			}
		}

		it = conditions.erase(it);

		condition->endCondition(shared_from_this());
		delete condition;

		onEndCondition(type);
	}
}

void Creature::removeCombatCondition(ConditionType_t type)
{
	std::vector<Condition*> removeConditions;
	for (Condition* condition : conditions) {
		if (condition->getType() == type) {
			removeConditions.push_back(condition);
		}
	}

	for (Condition* condition : removeConditions) {
		onCombatRemoveCondition(condition);
	}
}

void Creature::removeCondition(Condition* condition, bool force /* = false*/)
{
	auto it = std::find(conditions.begin(), conditions.end(), condition);
	if (it == conditions.end()) {
		return;
	}

	if (!force && condition->getType() == CONDITION_PARALYZE) {
		int64_t walkDelay = getWalkDelay();
		if (walkDelay > 0) {
			g_scheduler.addEvent(createSchedulerTask(
			    walkDelay, [id = getID(), type = condition->getType()]() { g_game.forceRemoveCondition(id, type); }));
			return;
		}
	}

	conditions.erase(it);

	condition->endCondition(shared_from_this());
	onEndCondition(condition->getType());
	delete condition;
}

Condition* Creature::getCondition(ConditionType_t type) const
{
	for (Condition* condition : conditions) {
		if (condition->getType() == type) {
			return condition;
		}
	}
	return nullptr;
}

Condition* Creature::getCondition(ConditionType_t type, ConditionId_t conditionId, uint32_t subId /* = 0*/) const
{
	for (Condition* condition : conditions) {
		if (condition->getType() == type && condition->getId() == conditionId && condition->getSubId() == subId) {
			return condition;
		}
	}
	return nullptr;
}

void Creature::executeConditions(uint32_t interval)
{
	ConditionList tempConditions{conditions};
	for (Condition* condition : tempConditions) {
		auto it = std::find(conditions.begin(), conditions.end(), condition);
		if (it == conditions.end()) {
			continue;
		}

		if (!condition->executeCondition(shared_from_this(), interval)) {
			it = std::find(conditions.begin(), conditions.end(), condition);
			if (it != conditions.end()) {
				conditions.erase(it);
				condition->endCondition(shared_from_this());
				onEndCondition(condition->getType());
				delete condition;
			}
		}
	}
}

bool Creature::hasCondition(ConditionType_t type, uint32_t subId /* = 0*/) const
{
	if (isSuppress(type)) {
		return false;
	}

	int64_t timeNow = OTSYS_TIME();
	for (Condition* condition : conditions) {
		if (condition->getType() != type || condition->getSubId() != subId) {
			continue;
		}

		if (condition->getEndTime() >= timeNow || condition->getTicks() == -1) {
			return true;
		}
	}
	return false;
}

bool Creature::isImmune(CombatType_t type) const
{
	return hasBitSet(static_cast<uint32_t>(type), getDamageImmunities());
}

bool Creature::isImmune(ConditionType_t type) const
{
	return hasBitSet(static_cast<uint32_t>(type), getConditionImmunities());
}

bool Creature::isSuppress(ConditionType_t type) const
{
	return hasBitSet(static_cast<uint32_t>(type), getConditionSuppressions());
}

int64_t Creature::getStepDuration(Direction dir) const
{
	int64_t stepDuration = getStepDuration();
	if ((dir & DIRECTION_DIAGONAL_MASK) != 0) {
		stepDuration *= 3;
	}
	return stepDuration;
}

int64_t Creature::getStepDuration() const
{
	if (isRemoved()) {
		return 0;
	}

	uint32_t calculatedStepSpeed;
	uint32_t groundSpeed;

	int32_t stepSpeed = getStepSpeed();
	if (stepSpeed > -Creature::speedB) {
		calculatedStepSpeed =
		    floor((Creature::speedA * log((stepSpeed / 2) + Creature::speedB) + Creature::speedC) + 0.5);
		if (calculatedStepSpeed == 0) {
			calculatedStepSpeed = 1;
		}
	} else {
		calculatedStepSpeed = 1;
	}

	auto ground = tile->getGround();
	if (ground) {
		groundSpeed = Item::items[ground->getID()].speed;
		if (groundSpeed == 0) {
			groundSpeed = 150;
		}
	} else {
		groundSpeed = 150;
	}

	double duration = std::floor(1000 * groundSpeed / calculatedStepSpeed);
	int64_t stepDuration = std::ceil(duration / 50) * 50;

	auto monster = this->getMonster();
	if (monster && monster->isTargetNearby() && !monster->isFleeing() && !monster->getMaster()) {
		stepDuration *= 2;
	}

	return stepDuration;
}

int64_t Creature::getEventStepTicks(bool onlyDelay) const
{
	int64_t ret = getWalkDelay();
	if (ret <= 0) {
		int64_t stepDuration = getStepDuration();
		if (onlyDelay && stepDuration > 0) {
			ret = 1;
		} else {
			ret = stepDuration * lastStepCost;
		}
	}
	return ret;
}

LightInfo Creature::getCreatureLight() const { return internalLight; }

void Creature::setCreatureLight(LightInfo lightInfo) { internalLight = std::move(lightInfo); }

void Creature::setNormalCreatureLight() { internalLight = {}; }

bool Creature::registerCreatureEvent(const std::string& name)
{
	CreatureEvent* event = g_creatureEvents->getEventByName(name);
	if (!event) {
		return false;
	}

	CreatureEventType_t type = event->getEventType();
	if (hasEventRegistered(type)) {
		for (CreatureEvent* creatureEvent : eventsList) {
			if (creatureEvent == event) {
				return false;
			}
		}
	} else {
		scriptEventsBitField |= static_cast<uint32_t>(1) << type;
	}

	eventsList.push_back(event);
	return true;
}

bool Creature::unregisterCreatureEvent(const std::string& name)
{
	CreatureEvent* event = g_creatureEvents->getEventByName(name);
	if (!event) {
		return false;
	}

	CreatureEventType_t type = event->getEventType();
	if (!hasEventRegistered(type)) {
		return false;
	}

	bool resetTypeBit = true;

	auto it = eventsList.begin(), end = eventsList.end();
	while (it != end) {
		CreatureEvent* curEvent = *it;
		if (curEvent == event) {
			it = eventsList.erase(it);
			continue;
		}

		if (curEvent->getEventType() == type) {
			resetTypeBit = false;
		}
		++it;
	}

	if (resetTypeBit) {
		scriptEventsBitField &= ~(static_cast<uint32_t>(1) << type);
	}
	return true;
}

CreatureEventList Creature::getCreatureEvents(CreatureEventType_t type)
{
	CreatureEventList tmpEventList;

	if (!hasEventRegistered(type)) {
		return tmpEventList;
	}

	for (CreatureEvent* creatureEvent : eventsList) {
		if (!creatureEvent->isLoaded()) {
			continue;
		}

		if (creatureEvent->getEventType() == type) {
			tmpEventList.push_back(creatureEvent);
		}
	}

	return tmpEventList;
}

bool FrozenPathingConditionCall::isInRange(const Position& startPos, const Position& testPos,
                                           const FindPathParams& fpp) const
{
	if (fpp.fullPathSearch) {
		if (testPos.x > targetPos.x + fpp.maxTargetDist) {
			return false;
		}

		if (testPos.x < targetPos.x - fpp.maxTargetDist) {
			return false;
		}

		if (testPos.y > targetPos.y + fpp.maxTargetDist) {
			return false;
		}

		if (testPos.y < targetPos.y - fpp.maxTargetDist) {
			return false;
		}
	} else {
		int32_t dx = startPos.getOffsetX(targetPos);

		int32_t dxMax = (dx >= 0 ? fpp.maxTargetDist : 0);
		if (testPos.x > targetPos.x + dxMax) {
			return false;
		}

		int32_t dxMin = (dx <= 0 ? fpp.maxTargetDist : 0);
		if (testPos.x < targetPos.x - dxMin) {
			return false;
		}

		int32_t dy = startPos.getOffsetY(targetPos);

		int32_t dyMax = (dy >= 0 ? fpp.maxTargetDist : 0);
		if (testPos.y > targetPos.y + dyMax) {
			return false;
		}

		int32_t dyMin = (dy <= 0 ? fpp.maxTargetDist : 0);
		if (testPos.y < targetPos.y - dyMin) {
			return false;
		}
	}
	return true;
}

bool FrozenPathingConditionCall::operator()(const Position& startPos, const Position& testPos,
                                            const FindPathParams& fpp, int32_t& bestMatchDist) const
{
	if (!isInRange(startPos, testPos, fpp)) {
		return false;
	}

	if (fpp.clearSight && !g_game.isSightClear(testPos, targetPos, true)) {
		return false;
	}

	int32_t testDist = std::max(targetPos.getDistanceX(testPos), targetPos.getDistanceY(testPos));
	if (fpp.maxTargetDist == 1) {
		if (testDist < fpp.minTargetDist || testDist > fpp.maxTargetDist) {
			return false;
		}

		return true;
	} else if (testDist <= fpp.maxTargetDist) {
		if (testDist < fpp.minTargetDist) {
			return false;
		}

		if (testDist == fpp.maxTargetDist) {
			bestMatchDist = 0;
			return true;
		} else if (testDist > bestMatchDist) {
			// not quite what we want, but the best so far
			bestMatchDist = testDist;
			return true;
		}
	}
	return false;
}

bool Creature::isInvisible() const
{
	return std::find_if(conditions.begin(), conditions.end(), [](const Condition* condition) {
		       return condition->getType() == CONDITION_INVISIBLE;
	       }) != conditions.end();
}

bool Creature::getPathTo(const Position& targetPos, std::vector<Direction>& dirList, const FindPathParams& fpp) const
{
	return g_game.map.getPathMatching(this->getCreature(), targetPos, dirList, FrozenPathingConditionCall(targetPos),
	                                  fpp);
}

bool Creature::getPathTo(const Position& targetPos, std::vector<Direction>& dirList, int32_t minTargetDist,
                         int32_t maxTargetDist, bool fullPathSearch /*= true*/, bool clearSight /*= true*/,
                         int32_t maxSearchDist /*= 0*/) const
{
	FindPathParams fpp;
	fpp.fullPathSearch = fullPathSearch;
	fpp.maxSearchDist = maxSearchDist;
	fpp.clearSight = clearSight;
	fpp.minTargetDist = minTargetDist;
	fpp.maxTargetDist = maxTargetDist;
	return getPathTo(targetPos, dirList, fpp);
}

void Creature::setStorageValue(uint32_t key, std::optional<int32_t> value, bool isSpawn)
{
	auto oldValue = getStorageValue(key);
	if (value) {
		storageMap.insert_or_assign(key, value.value());
	} else {
		storageMap.erase(key);
	}
	tfs::events::creature::onUpdateStorage(shared_from_this(), key, oldValue, value, isSpawn);
}

std::optional<int32_t> Creature::getStorageValue(uint32_t key) const
{
	auto it = storageMap.find(key);
	if (it == storageMap.end()) {
		return std::nullopt;
	}
	return std::make_optional(it->second);
}
