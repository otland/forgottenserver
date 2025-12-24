#include "../../otpch.h"

#include "../../creature.h"

#include "../../condition.h"
#include "../../events.h"
#include "../../game.h"
#include "../api.h"
#include "../env.h"
#include "../meta.h"
#include "../register.h"
#include "../script.h"

extern Game g_game;

namespace {

int luaCreatureCreate(lua_State* L)
{
	// Creature(id or name or userdata)
	std::shared_ptr<Creature> creature = nullptr;
	if (tfs::lua::isNumber(L, 2)) {
		creature = g_game.getCreatureByID(tfs::lua::getNumber<uint32_t>(L, 2));
	} else if (lua_isstring(L, 2)) {
		creature = g_game.getCreatureByName(tfs::lua::getString(L, 2));
	} else if (lua_isuserdata(L, 2)) {
		const auto type = tfs::lua::getUserdataType(L, 2);
		if (type != tfs::lua::LuaData_Player && type != tfs::lua::LuaData_Monster && type != tfs::lua::LuaData_Npc) {
			lua_pushnil(L);
			return 1;
		}
		creature = tfs::lua::getSharedPtr<Creature>(L, 2);
	}

	if (creature) {
		tfs::lua::pushSharedPtr(L, creature);
		tfs::lua::setCreatureMetatable(L, -1, creature);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaCreatureGetEvents(lua_State* L)
{
	// creature:getEvents(type)
	const auto& creature = tfs::lua::getSharedPtr<Creature>(L, 1);
	if (!creature) {
		lua_pushnil(L);
		return 1;
	}

	CreatureEventType_t eventType = tfs::lua::getNumber<CreatureEventType_t>(L, 2);
	const auto& eventList = creature->getCreatureEvents(eventType);
	lua_createtable(L, eventList.size(), 0);

	int index = 0;
	for (CreatureEvent* event : eventList) {
		tfs::lua::pushString(L, event->getName());
		lua_rawseti(L, -2, ++index);
	}
	return 1;
}

int luaCreatureRegisterEvent(lua_State* L)
{
	// creature:registerEvent(name)
	if (const auto& creature = tfs::lua::getSharedPtr<Creature>(L, 1)) {
		const std::string& name = tfs::lua::getString(L, 2);
		tfs::lua::pushBoolean(L, creature->registerCreatureEvent(name));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaCreatureUnregisterEvent(lua_State* L)
{
	// creature:unregisterEvent(name)
	const std::string& name = tfs::lua::getString(L, 2);
	if (const auto& creature = tfs::lua::getSharedPtr<Creature>(L, 1)) {
		tfs::lua::pushBoolean(L, creature->unregisterCreatureEvent(name));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaCreatureIsRemoved(lua_State* L)
{
	// creature:isRemoved()
	if (const auto& creature = tfs::lua::getSharedPtr<const Creature>(L, 1)) {
		tfs::lua::pushBoolean(L, creature->isRemoved());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaCreatureIsCreature(lua_State* L)
{
	// creature:isCreature()
	tfs::lua::pushBoolean(L, tfs::lua::getSharedPtr<const Creature>(L, 1) != nullptr);
	return 1;
}

int luaCreatureIsInGhostMode(lua_State* L)
{
	// creature:isInGhostMode()
	if (const auto& creature = tfs::lua::getSharedPtr<const Creature>(L, 1)) {
		tfs::lua::pushBoolean(L, creature->isInGhostMode());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaCreatureIsHealthHidden(lua_State* L)
{
	// creature:isHealthHidden()
	if (const auto& creature = tfs::lua::getSharedPtr<const Creature>(L, 1)) {
		tfs::lua::pushBoolean(L, creature->isHealthHidden());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaCreatureIsMovementBlocked(lua_State* L)
{
	// creature:isMovementBlocked()
	if (const auto& creature = tfs::lua::getSharedPtr<const Creature>(L, 1)) {
		tfs::lua::pushBoolean(L, creature->isMovementBlocked());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaCreatureCanSee(lua_State* L)
{
	// creature:canSee(position)
	if (const auto& creature = tfs::lua::getSharedPtr<const Creature>(L, 1)) {
		const Position& position = tfs::lua::getPosition(L, 2);
		tfs::lua::pushBoolean(L, creature->canSee(position));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaCreatureCanSeeCreature(lua_State* L)
{
	// creature:canSeeCreature(creature)
	if (const auto& creature = tfs::lua::getSharedPtr<const Creature>(L, 1)) {
		const auto& otherCreature = tfs::lua::getCreature(L, 2);
		if (!otherCreature) {
			tfs::lua::reportError(L, tfs::lua::getErrorDesc(tfs::lua::LUA_ERROR_CREATURE_NOT_FOUND));
			tfs::lua::pushBoolean(L, false);
			return 1;
		}

		tfs::lua::pushBoolean(L, creature->canSeeCreature(otherCreature));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaCreatureCanSeeGhostMode(lua_State* L)
{
	// creature:canSeeGhostMode(creature)
	if (const auto& creature = tfs::lua::getSharedPtr<const Creature>(L, 1)) {
		const auto& otherCreature = tfs::lua::getCreature(L, 2);
		if (!otherCreature) {
			tfs::lua::reportError(L, tfs::lua::getErrorDesc(tfs::lua::LUA_ERROR_CREATURE_NOT_FOUND));
			tfs::lua::pushBoolean(L, false);
			return 1;
		}

		tfs::lua::pushBoolean(L, creature->canSeeGhostMode(otherCreature));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaCreatureCanSeeInvisibility(lua_State* L)
{
	// creature:canSeeInvisibility()
	if (const auto& creature = tfs::lua::getSharedPtr<const Creature>(L, 1)) {
		tfs::lua::pushBoolean(L, creature->canSeeInvisibility());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaCreatureHasParent(lua_State* L)
{
	// creature:hasParent()
	const auto& creature = tfs::lua::getSharedPtr<Creature>(L, 1);
	if (!creature) {
		lua_pushnil(L);
		return 1;
	}

	tfs::lua::pushBoolean(L, creature->hasParent());
	return 1;
}

int luaCreatureGetParent(lua_State* L)
{
	// creature:getParent()
	const auto& creature = tfs::lua::getSharedPtr<Creature>(L, 1);
	if (!creature) {
		lua_pushnil(L);
		return 1;
	}

	const auto& parent = creature->getParent();
	if (!parent) {
		lua_pushnil(L);
		return 1;
	}

	tfs::lua::pushThing(L, parent);
	return 1;
}

int luaCreatureGetId(lua_State* L)
{
	// creature:getId()
	if (const auto& creature = tfs::lua::getSharedPtr<const Creature>(L, 1)) {
		tfs::lua::pushNumber(L, creature->getID());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaCreatureGetName(lua_State* L)
{
	// creature:getName()
	if (const auto& creature = tfs::lua::getSharedPtr<const Creature>(L, 1)) {
		tfs::lua::pushString(L, creature->getName());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaCreatureGetTarget(lua_State* L)
{
	// creature:getTarget()
	const auto& creature = tfs::lua::getSharedPtr<Creature>(L, 1);
	if (!creature) {
		lua_pushnil(L);
		return 1;
	}

	if (const auto& target = creature->getAttackedCreature()) {
		tfs::lua::pushSharedPtr(L, target);
		tfs::lua::setCreatureMetatable(L, -1, target);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaCreatureSetTarget(lua_State* L)
{
	// creature:setTarget(target)
	const auto& creature = tfs::lua::getSharedPtr<Creature>(L, 1);
	if (!creature) {
		lua_pushnil(L);
		return 1;
	}

	if (const auto& target = tfs::lua::getCreature(L, 2)) {
		creature->setAttackedCreature(target);
		tfs::lua::pushBoolean(L, creature->canAttackCreature(target));
	} else {
		creature->removeAttackedCreature();
		tfs::lua::pushBoolean(L, true);
	}
	return 1;
}

int luaCreatureGetFollowCreature(lua_State* L)
{
	// creature:getFollowCreature()
	const auto& creature = tfs::lua::getSharedPtr<Creature>(L, 1);
	if (!creature) {
		lua_pushnil(L);
		return 1;
	}

	if (const auto& followCreature = creature->getFollowCreature()) {
		tfs::lua::pushSharedPtr(L, followCreature);
		tfs::lua::setCreatureMetatable(L, -1, followCreature);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaCreatureSetFollowCreature(lua_State* L)
{
	// creature:setFollowCreature(followedCreature)
	const auto& creature = tfs::lua::getSharedPtr<Creature>(L, 1);
	if (!creature) {
		lua_pushnil(L);
		return 1;
	}

	if (const auto& followedCreature = tfs::lua::getCreature(L, 2)) {
		creature->setFollowCreature(followedCreature);
		tfs::lua::pushBoolean(L, creature->canFollowCreature(followedCreature));
	} else {
		creature->removeFollowCreature();
		tfs::lua::pushBoolean(L, true);
	}
	return 1;
}

int luaCreatureGetMaster(lua_State* L)
{
	// creature:getMaster()
	const auto& creature = tfs::lua::getSharedPtr<Creature>(L, 1);
	if (!creature) {
		lua_pushnil(L);
		return 1;
	}

	const auto& master = creature->getMaster();
	if (!master) {
		lua_pushnil(L);
		return 1;
	}

	tfs::lua::pushSharedPtr(L, master);
	tfs::lua::setCreatureMetatable(L, -1, master);
	return 1;
}

int luaCreatureSetMaster(lua_State* L)
{
	// creature:setMaster(master)
	const auto& creature = tfs::lua::getSharedPtr<Creature>(L, 1);
	if (!creature) {
		lua_pushnil(L);
		return 1;
	}

	tfs::lua::pushBoolean(L, creature->setMaster(tfs::lua::getCreature(L, 2)));

	// update summon icon
	g_game.updateKnownCreature(creature);
	return 1;
}

int luaCreatureGetLight(lua_State* L)
{
	// creature:getLight()
	const auto& creature = tfs::lua::getSharedPtr<const Creature>(L, 1);
	if (!creature) {
		lua_pushnil(L);
		return 1;
	}

	LightInfo lightInfo = creature->getCreatureLight();
	tfs::lua::pushNumber(L, lightInfo.level);
	tfs::lua::pushNumber(L, lightInfo.color);
	return 2;
}

int luaCreatureSetLight(lua_State* L)
{
	// creature:setLight(color, level)
	const auto& creature = tfs::lua::getSharedPtr<Creature>(L, 1);
	if (!creature) {
		lua_pushnil(L);
		return 1;
	}

	LightInfo light;
	light.color = tfs::lua::getNumber<uint8_t>(L, 2);
	light.level = tfs::lua::getNumber<uint8_t>(L, 3);
	creature->setCreatureLight(light);
	g_game.changeLight(creature);
	tfs::lua::pushBoolean(L, true);
	return 1;
}

int luaCreatureGetSpeed(lua_State* L)
{
	// creature:getSpeed()
	if (const auto& creature = tfs::lua::getSharedPtr<const Creature>(L, 1)) {
		tfs::lua::pushNumber(L, creature->getSpeed());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaCreatureGetBaseSpeed(lua_State* L)
{
	// creature:getBaseSpeed()
	if (const auto& creature = tfs::lua::getSharedPtr<const Creature>(L, 1)) {
		tfs::lua::pushNumber(L, creature->getBaseSpeed());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaCreatureChangeSpeed(lua_State* L)
{
	// creature:changeSpeed(delta)
	const auto& creature = tfs::lua::getCreature(L, 1);
	if (!creature) {
		tfs::lua::reportError(L, tfs::lua::getErrorDesc(tfs::lua::LUA_ERROR_CREATURE_NOT_FOUND));
		tfs::lua::pushBoolean(L, false);
		return 1;
	}

	int32_t delta = tfs::lua::getNumber<int32_t>(L, 2);
	g_game.changeSpeed(creature, delta);
	tfs::lua::pushBoolean(L, true);
	return 1;
}

int luaCreatureSetDropLoot(lua_State* L)
{
	// creature:setDropLoot(doDrop)
	if (const auto& creature = tfs::lua::getSharedPtr<Creature>(L, 1)) {
		creature->setDropLoot(tfs::lua::getBoolean(L, 2));
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaCreatureSetSkillLoss(lua_State* L)
{
	// creature:setSkillLoss(skillLoss)
	if (const auto& creature = tfs::lua::getSharedPtr<Creature>(L, 1)) {
		creature->setSkillLoss(tfs::lua::getBoolean(L, 2));
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaCreatureGetPosition(lua_State* L)
{
	// creature:getPosition()
	if (const auto& creature = tfs::lua::getSharedPtr<const Creature>(L, 1)) {
		tfs::lua::pushPosition(L, creature->getPosition());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaCreatureGetTile(lua_State* L)
{
	// creature:getTile()
	const auto& creature = tfs::lua::getSharedPtr<Creature>(L, 1);
	if (!creature) {
		lua_pushnil(L);
		return 1;
	}

	if (const auto& tile = creature->getTile()) {
		tfs::lua::pushSharedPtr(L, tile);
		tfs::lua::setMetatable(L, -1, "Tile");
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaCreatureGetDirection(lua_State* L)
{
	// creature:getDirection()
	if (const auto& creature = tfs::lua::getSharedPtr<const Creature>(L, 1)) {
		tfs::lua::pushNumber(L, creature->getDirection());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaCreatureSetDirection(lua_State* L)
{
	// creature:setDirection(direction)
	if (const auto& creature = tfs::lua::getSharedPtr<Creature>(L, 1)) {
		tfs::lua::pushBoolean(L, g_game.internalCreatureTurn(creature, tfs::lua::getNumber<Direction>(L, 2)));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaCreatureGetHealth(lua_State* L)
{
	// creature:getHealth()
	if (const auto& creature = tfs::lua::getSharedPtr<const Creature>(L, 1)) {
		tfs::lua::pushNumber(L, creature->getHealth());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaCreatureSetHealth(lua_State* L)
{
	// creature:setHealth(health)
	const auto& creature = tfs::lua::getSharedPtr<Creature>(L, 1);
	if (!creature) {
		lua_pushnil(L);
		return 1;
	}

	creature->setHealth(std::min<int32_t>(tfs::lua::getNumber<uint32_t>(L, 2), creature->getMaxHealth()));
	g_game.addCreatureHealth(creature);

	if (const auto& player = creature->asPlayer()) {
		player->sendStats();
	}
	tfs::lua::pushBoolean(L, true);
	return 1;
}

int luaCreatureAddHealth(lua_State* L)
{
	// creature:addHealth(healthChange)
	const auto& creature = tfs::lua::getSharedPtr<Creature>(L, 1);
	if (!creature) {
		lua_pushnil(L);
		return 1;
	}

	CombatDamage damage;
	damage.primary.value = tfs::lua::getNumber<int32_t>(L, 2);
	if (damage.primary.value >= 0) {
		damage.primary.type = COMBAT_HEALING;
	} else {
		damage.primary.type = COMBAT_UNDEFINEDDAMAGE;
	}
	tfs::lua::pushBoolean(L, g_game.combatChangeHealth(nullptr, creature, damage));
	return 1;
}

int luaCreatureGetMaxHealth(lua_State* L)
{
	// creature:getMaxHealth()
	if (const auto& creature = tfs::lua::getSharedPtr<const Creature>(L, 1)) {
		tfs::lua::pushNumber(L, creature->getMaxHealth());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaCreatureSetMaxHealth(lua_State* L)
{
	// creature:setMaxHealth(maxHealth)
	const auto& creature = tfs::lua::getSharedPtr<Creature>(L, 1);
	if (!creature) {
		lua_pushnil(L);
		return 1;
	}

	creature->setMaxHealth(tfs::lua::getNumber<uint32_t>(L, 2));
	creature->setHealth(std::min<int32_t>(creature->getHealth(), creature->getMaxHealth()));
	g_game.addCreatureHealth(creature);

	if (const auto& player = creature->asPlayer()) {
		player->sendStats();
	}
	tfs::lua::pushBoolean(L, true);
	return 1;
}

int luaCreatureSetHiddenHealth(lua_State* L)
{
	// creature:setHiddenHealth(hide)
	if (const auto& creature = tfs::lua::getSharedPtr<Creature>(L, 1)) {
		creature->setHiddenHealth(tfs::lua::getBoolean(L, 2));
		g_game.addCreatureHealth(creature);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaCreatureSetMovementBlocked(lua_State* L)
{
	// creature:setMovementBlocked(state)
	if (const auto& creature = tfs::lua::getSharedPtr<Creature>(L, 1)) {
		creature->setMovementBlocked(tfs::lua::getBoolean(L, 2));
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaCreatureGetSkull(lua_State* L)
{
	// creature:getSkull()
	if (const auto& creature = tfs::lua::getSharedPtr<Creature>(L, 1)) {
		tfs::lua::pushNumber(L, creature->getSkull());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaCreatureSetSkull(lua_State* L)
{
	// creature:setSkull(skull)
	if (const auto& creature = tfs::lua::getSharedPtr<Creature>(L, 1)) {
		creature->setSkull(tfs::lua::getNumber<Skulls_t>(L, 2));
		g_game.updateCreatureSkull(creature);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaCreatureGetOutfit(lua_State* L)
{
	// creature:getOutfit()
	if (const auto& creature = tfs::lua::getSharedPtr<const Creature>(L, 1)) {
		tfs::lua::pushOutfit(L, creature->getCurrentOutfit());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaCreatureSetOutfit(lua_State* L)
{
	// creature:setOutfit(outfit)
	if (const auto& creature = tfs::lua::getSharedPtr<Creature>(L, 1)) {
		creature->setDefaultOutfit(tfs::lua::getOutfit(L, 2));
		g_game.internalCreatureChangeOutfit(creature, creature->getDefaultOutfit());
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaCreatureGetCondition(lua_State* L)
{
	// creature:getCondition(conditionType[, conditionId = CONDITIONID_COMBAT[, subId = 0]])
	const auto& creature = tfs::lua::getSharedPtr<Creature>(L, 1);
	if (!creature) {
		lua_pushnil(L);
		return 1;
	}

	ConditionType_t conditionType = tfs::lua::getNumber<ConditionType_t>(L, 2);
	ConditionId_t conditionId = tfs::lua::getNumber<ConditionId_t>(L, 3, CONDITIONID_COMBAT);
	uint32_t subId = tfs::lua::getNumber<uint32_t>(L, 4, 0);

	Condition* condition = creature->getCondition(conditionType, conditionId, subId);
	if (condition) {
		tfs::lua::pushUserdata(L, condition);
		tfs::lua::setWeakMetatable(L, -1, "Condition");
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaCreatureAddCondition(lua_State* L)
{
	// creature:addCondition(condition[, force = false])
	const auto& creature = tfs::lua::getSharedPtr<Creature>(L, 1);
	Condition* condition = tfs::lua::getUserdata<Condition>(L, 2);
	if (creature && condition) {
		bool force = tfs::lua::getBoolean(L, 3, false);
		tfs::lua::pushBoolean(L, creature->addCondition(condition->clone(), force));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaCreatureRemoveCondition(lua_State* L)
{
	// creature:removeCondition(conditionType[, conditionId = CONDITIONID_COMBAT[, subId = 0[, force = false]]])
	// creature:removeCondition(condition[, force = false])
	const auto& creature = tfs::lua::getSharedPtr<Creature>(L, 1);
	if (!creature) {
		lua_pushnil(L);
		return 1;
	}

	Condition* creatureCondition = nullptr;
	bool force = false;

	if (lua_isuserdata(L, 2)) {
		const Condition* const condition = tfs::lua::getUserdata<Condition>(L, 2);
		const ConditionType_t conditionType = condition->getType();
		const ConditionId_t conditionId = condition->getId();
		const uint32_t subId = condition->getSubId();
		creatureCondition = creature->getCondition(conditionType, conditionId, subId);
		force = tfs::lua::getBoolean(L, 3, false);
	} else {
		const ConditionType_t conditionType = tfs::lua::getNumber<ConditionType_t>(L, 2);
		const ConditionId_t conditionId = tfs::lua::getNumber<ConditionId_t>(L, 3, CONDITIONID_COMBAT);
		const uint32_t subId = tfs::lua::getNumber<uint32_t>(L, 4, 0);
		creatureCondition = creature->getCondition(conditionType, conditionId, subId);
		force = tfs::lua::getBoolean(L, 5, false);
	}

	if (creatureCondition) {
		creature->removeCondition(creatureCondition, force);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaCreatureHasCondition(lua_State* L)
{
	// creature:hasCondition(conditionType[, subId = 0])
	const auto& creature = tfs::lua::getSharedPtr<Creature>(L, 1);
	if (!creature) {
		lua_pushnil(L);
		return 1;
	}

	ConditionType_t conditionType = tfs::lua::getNumber<ConditionType_t>(L, 2);
	uint32_t subId = tfs::lua::getNumber<uint32_t>(L, 3, 0);
	tfs::lua::pushBoolean(L, creature->hasCondition(conditionType, subId));
	return 1;
}

int luaCreatureIsImmune(lua_State* L)
{
	// creature:isImmune(condition or conditionType)
	const auto& creature = tfs::lua::getSharedPtr<Creature>(L, 1);
	if (!creature) {
		lua_pushnil(L);
		return 1;
	}

	if (tfs::lua::isNumber(L, 2)) {
		tfs::lua::pushBoolean(L, creature->isImmune(tfs::lua::getNumber<ConditionType_t>(L, 2)));
	} else if (Condition* condition = tfs::lua::getUserdata<Condition>(L, 2)) {
		tfs::lua::pushBoolean(L, creature->isImmune(condition->getType()));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaCreatureRemove(lua_State* L)
{
	// creature:remove()
	auto* creaturePtr = tfs::lua::getRawSharedPtr<Creature>(L, 1);
	if (!creaturePtr) {
		lua_pushnil(L);
		return 1;
	}

	const auto& creature = *creaturePtr;
	if (!creature) {
		lua_pushnil(L);
		return 1;
	}

	if (const auto& player = creature->asPlayer()) {
		player->kickPlayer(true);
	} else {
		g_game.removeCreature(creature);
	}

	*creaturePtr = nullptr;
	tfs::lua::pushBoolean(L, true);
	return 1;
}

int luaCreatureTeleportTo(lua_State* L)
{
	// creature:teleportTo(position[, pushMovement = false])
	bool pushMovement = tfs::lua::getBoolean(L, 3, false);

	const Position& position = tfs::lua::getPosition(L, 2);
	const auto& creature = tfs::lua::getSharedPtr<Creature>(L, 1);
	if (!creature) {
		lua_pushnil(L);
		return 1;
	}

	const Position oldPosition = creature->getPosition();
	if (g_game.internalTeleport(creature, position, pushMovement) != RETURNVALUE_NOERROR) {
		tfs::lua::pushBoolean(L, false);
		return 1;
	}

	if (pushMovement) {
		if (oldPosition.x == position.x) {
			if (oldPosition.y < position.y) {
				g_game.internalCreatureTurn(creature, DIRECTION_SOUTH);
			} else {
				g_game.internalCreatureTurn(creature, DIRECTION_NORTH);
			}
		} else if (oldPosition.x > position.x) {
			g_game.internalCreatureTurn(creature, DIRECTION_WEST);
		} else if (oldPosition.x < position.x) {
			g_game.internalCreatureTurn(creature, DIRECTION_EAST);
		}
	}
	tfs::lua::pushBoolean(L, true);
	return 1;
}

int luaCreatureSay(lua_State* L)
{
	// creature:say(text[, type = TALKTYPE_MONSTER_SAY[, ghost = false[, target = nullptr[, position]]]])
	int parameters = lua_gettop(L);

	Position position;
	if (parameters >= 6) {
		position = tfs::lua::getPosition(L, 6);
		if (!position.x || !position.y) {
			tfs::lua::reportError(L, "Invalid position specified.");
			tfs::lua::pushBoolean(L, false);
			return 1;
		}
	}

	std::shared_ptr<Creature> target = nullptr;
	if (parameters >= 5) {
		target = tfs::lua::getCreature(L, 5);
	}

	bool ghost = tfs::lua::getBoolean(L, 4, false);

	SpeakClasses type = tfs::lua::getNumber<SpeakClasses>(L, 3, TALKTYPE_MONSTER_SAY);
	const std::string& text = tfs::lua::getString(L, 2);
	const auto& creature = tfs::lua::getSharedPtr<Creature>(L, 1);
	if (!creature) {
		lua_pushnil(L);
		return 1;
	}

	SpectatorVec spectators;
	if (target) {
		spectators.emplace(target);
	}

	// Prevent infinity echo on event onHear
	bool echo = tfs::lua::getScriptEnv()->getScriptId() == tfs::events::getScriptId(EventInfoId::CREATURE_ONHEAR);

	if (position.x != 0) {
		tfs::lua::pushBoolean(L, g_game.internalCreatureSay(creature, type, text, ghost, &spectators, &position, echo));
	} else {
		tfs::lua::pushBoolean(L, g_game.internalCreatureSay(creature, type, text, ghost, &spectators, nullptr, echo));
	}
	return 1;
}

int luaCreatureGetDamageMap(lua_State* L)
{
	// creature:getDamageMap()
	const auto& creature = tfs::lua::getSharedPtr<Creature>(L, 1);
	if (!creature) {
		lua_pushnil(L);
		return 1;
	}

	const auto& damageMap = creature->getDamageMap();
	lua_createtable(L, damageMap.size(), 0);
	for (auto&& [id, cb] : damageMap) {
		lua_createtable(L, 0, 2);
		tfs::lua::setField(L, "total", cb.total);
		tfs::lua::setField(L, "ticks", cb.ticks);
		lua_rawseti(L, -2, id);
	}
	return 1;
}

int luaCreatureGetSummons(lua_State* L)
{
	// creature:getSummons()
	const auto& creature = tfs::lua::getSharedPtr<Creature>(L, 1);
	if (!creature) {
		lua_pushnil(L);
		return 1;
	}

	const auto& summons = creature->getSummons() | tfs::views::lock_weak_ptrs | std::ranges::to<std::vector>();
	lua_createtable(L, summons.size(), 0);

	int index = 0;
	for (const auto& summon : summons) {
		tfs::lua::pushSharedPtr(L, summon);
		tfs::lua::setCreatureMetatable(L, -1, summon);
		lua_rawseti(L, -2, ++index);
	}
	return 1;
}

int luaCreatureGetDescription(lua_State* L)
{
	// creature:getDescription(distance)
	int32_t distance = tfs::lua::getNumber<int32_t>(L, 2);
	if (const auto& creature = tfs::lua::getSharedPtr<Creature>(L, 1)) {
		tfs::lua::pushString(L, creature->getDescription(distance));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaCreatureGetPathTo(lua_State* L)
{
	// creature:getPathTo(pos[, minTargetDist = 0[, maxTargetDist = 1[, fullPathSearch = true[, clearSight = true[,
	// maxSearchDist = 0]]]]])
	const auto& creature = tfs::lua::getSharedPtr<Creature>(L, 1);
	if (!creature) {
		lua_pushnil(L);
		return 1;
	}

	const Position& position = tfs::lua::getPosition(L, 2);

	FindPathParams fpp;
	fpp.minTargetDist = tfs::lua::getNumber<int32_t>(L, 3, 0);
	fpp.maxTargetDist = tfs::lua::getNumber<int32_t>(L, 4, 1);
	fpp.fullPathSearch = tfs::lua::getBoolean(L, 5, fpp.fullPathSearch);
	fpp.clearSight = tfs::lua::getBoolean(L, 6, fpp.clearSight);
	fpp.maxSearchDist = tfs::lua::getNumber<int32_t>(L, 7, fpp.maxSearchDist);

	std::vector<Direction> dirList;
	if (creature->getPathTo(position, dirList, fpp)) {
		lua_newtable(L);

		int index = 0;
		for (auto it = dirList.rbegin(); it != dirList.rend(); ++it) {
			tfs::lua::pushNumber(L, *it);
			lua_rawseti(L, -2, ++index);
		}
	} else {
		tfs::lua::pushBoolean(L, false);
	}
	return 1;
}

int luaCreatureMove(lua_State* L)
{
	// creature:move(direction)
	// creature:move(tile[, flags = 0])
	const auto& creature = tfs::lua::getSharedPtr<Creature>(L, 1);
	if (!creature) {
		lua_pushnil(L);
		return 1;
	}

	if (tfs::lua::isNumber(L, 2)) {
		Direction direction = tfs::lua::getNumber<Direction>(L, 2);
		if (direction > DIRECTION_LAST) {
			lua_pushnil(L);
			return 1;
		}
		tfs::lua::pushNumber(L, g_game.internalMoveCreature(creature, direction, FLAG_NOLIMIT));
	} else {
		const auto& tile = tfs::lua::getSharedPtr<Tile>(L, 2);
		if (!tile) {
			lua_pushnil(L);
			return 1;
		}
		tfs::lua::pushNumber(L, g_game.internalMoveCreature(creature, tile, tfs::lua::getNumber<uint32_t>(L, 3)));
	}
	return 1;
}

int luaCreatureGetZone(lua_State* L)
{
	// creature:getZone()
	if (const auto& creature = tfs::lua::getSharedPtr<Creature>(L, 1)) {
		tfs::lua::pushNumber(L, creature->getZone());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaCreatureHasIcon(lua_State* L)
{
	// creature:hasIcon(iconId)
	if (const auto& creature = tfs::lua::getSharedPtr<const Creature>(L, 1)) {
		auto iconId = tfs::lua::getNumber<CreatureIcon_t>(L, 2);
		tfs::lua::pushBoolean(L, creature->getIcons().contains(iconId));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaCreatureSetIcon(lua_State* L)
{
	// creature:setIcon(iconId, value)
	const auto& creature = tfs::lua::getSharedPtr<Creature>(L, 1);
	if (!creature) {
		lua_pushnil(L);
		return 1;
	}

	auto iconId = tfs::lua::getNumber<CreatureIcon_t>(L, 2);
	if (iconId > CREATURE_ICON_LAST) {
		tfs::lua::reportError(L, "Invalid Creature Icon Id");
		tfs::lua::pushBoolean(L, false);
		return 1;
	}

	creature->getIcons()[iconId] = tfs::lua::getNumber<uint16_t>(L, 3);
	creature->updateIcons();
	tfs::lua::pushBoolean(L, true);
	return 1;
}

int luaCreatureGetIcon(lua_State* L)
{
	// creature:getIcon(iconId)
	const auto& creature = tfs::lua::getSharedPtr<const Creature>(L, 1);
	if (!creature) {
		lua_pushnil(L);
		return 1;
	}

	auto iconId = tfs::lua::getNumber<CreatureIcon_t>(L, 2);
	const auto& icons = creature->getIcons();
	auto it = icons.find(iconId);
	if (it != icons.end()) {
		tfs::lua::pushNumber(L, it->second);
	} else {
		tfs::lua::pushNumber(L, 0);
	}
	return 1;
}

int luaCreatureRemoveIcon(lua_State* L)
{
	// creature:removeIcon(iconId)
	const auto& creature = tfs::lua::getSharedPtr<Creature>(L, 1);
	if (!creature) {
		lua_pushnil(L);
		return 1;
	}

	auto iconId = tfs::lua::getNumber<CreatureIcon_t>(L, 2);
	auto& icons = creature->getIcons();
	auto it = icons.find(iconId);
	if (it != icons.end()) {
		icons.erase(it);
		creature->updateIcons();
		tfs::lua::pushBoolean(L, true);
	} else {
		tfs::lua::pushBoolean(L, false);
	}

	return 1;
}

int luaCreatureGetStorageValue(lua_State* L)
{
	// creature:getStorageValue(key)
	const auto& creature = tfs::lua::getSharedPtr<Creature>(L, 1);
	if (!creature) {
		lua_pushnil(L);
		return 1;
	}

	uint32_t key = tfs::lua::getNumber<uint32_t>(L, 2);
	if (auto storage = creature->getStorageValue(key)) {
		tfs::lua::pushNumber(L, storage.value());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaCreatureSetStorageValue(lua_State* L)
{
	// creature:setStorageValue(key, value)
	const auto& creature = tfs::lua::getSharedPtr<Creature>(L, 1);
	if (!creature) {
		lua_pushnil(L);
		return 1;
	}

	uint32_t key = tfs::lua::getNumber<uint32_t>(L, 2);
	if (IS_IN_KEYRANGE(key, RESERVED_RANGE)) {
		tfs::lua::reportError(L, std::format("Accessing reserved range: {:d}", key));
		tfs::lua::pushBoolean(L, false);
		return 1;
	}

	if (lua_isnoneornil(L, 3)) {
		creature->setStorageValue(key, std::nullopt);
	} else {
		int32_t value = tfs::lua::getNumber<int32_t>(L, 3);
		creature->setStorageValue(key, value);
	}

	tfs::lua::pushBoolean(L, true);
	return 1;
}

} // namespace

void tfs::lua::registerCreature(LuaScriptInterface& lsi)
{
	registerEnum(lsi, SKULL_NONE);
	registerEnum(lsi, SKULL_YELLOW);
	registerEnum(lsi, SKULL_GREEN);
	registerEnum(lsi, SKULL_WHITE);
	registerEnum(lsi, SKULL_RED);
	registerEnum(lsi, SKULL_BLACK);
	registerEnum(lsi, SKULL_ORANGE);

	registerEnum(lsi, CONST_SLOT_HEAD);
	registerEnum(lsi, CONST_SLOT_NECKLACE);
	registerEnum(lsi, CONST_SLOT_BACKPACK);
	registerEnum(lsi, CONST_SLOT_ARMOR);
	registerEnum(lsi, CONST_SLOT_RIGHT);
	registerEnum(lsi, CONST_SLOT_LEFT);
	registerEnum(lsi, CONST_SLOT_LEGS);
	registerEnum(lsi, CONST_SLOT_FEET);
	registerEnum(lsi, CONST_SLOT_RING);
	registerEnum(lsi, CONST_SLOT_AMMO);

	registerEnum(lsi, CREATURE_ID_MIN);
	registerEnum(lsi, CREATURE_ID_MAX);

	registerEnum(lsi, CREATURE_ICON_CROSS_WHITE);
	registerEnum(lsi, CREATURE_ICON_CROSS_WHITE_RED);
	registerEnum(lsi, CREATURE_ICON_ORB_RED);
	registerEnum(lsi, CREATURE_ICON_ORB_GREEN);
	registerEnum(lsi, CREATURE_ICON_ORB_RED_GREEN);
	registerEnum(lsi, CREATURE_ICON_GEM_GREEN);
	registerEnum(lsi, CREATURE_ICON_GEM_YELLOW);
	registerEnum(lsi, CREATURE_ICON_GEM_BLUE);
	registerEnum(lsi, CREATURE_ICON_GEM_PURPLE);
	registerEnum(lsi, CREATURE_ICON_GEM_RED);
	registerEnum(lsi, CREATURE_ICON_PIGEON);
	registerEnum(lsi, CREATURE_ICON_ENERGY);
	registerEnum(lsi, CREATURE_ICON_POISON);
	registerEnum(lsi, CREATURE_ICON_WATER);
	registerEnum(lsi, CREATURE_ICON_FIRE);
	registerEnum(lsi, CREATURE_ICON_ICE);
	registerEnum(lsi, CREATURE_ICON_ARROW_UP);
	registerEnum(lsi, CREATURE_ICON_ARROW_DOWN);
	registerEnum(lsi, CREATURE_ICON_WARNING);
	registerEnum(lsi, CREATURE_ICON_QUESTION);
	registerEnum(lsi, CREATURE_ICON_CROSS_RED);
	registerEnum(lsi, CREATURE_ICON_FIRST);
	registerEnum(lsi, CREATURE_ICON_LAST);

	lsi.registerClass("Creature", "", luaCreatureCreate);
	lsi.registerMetaMethod("Creature", "__eq", tfs::lua::luaUserdataCompare);

	lsi.registerMethod("Creature", "getEvents", luaCreatureGetEvents);
	lsi.registerMethod("Creature", "registerEvent", luaCreatureRegisterEvent);
	lsi.registerMethod("Creature", "unregisterEvent", luaCreatureUnregisterEvent);

	lsi.registerMethod("Creature", "isRemoved", luaCreatureIsRemoved);
	lsi.registerMethod("Creature", "isCreature", luaCreatureIsCreature);
	lsi.registerMethod("Creature", "isInGhostMode", luaCreatureIsInGhostMode);
	lsi.registerMethod("Creature", "isHealthHidden", luaCreatureIsHealthHidden);
	lsi.registerMethod("Creature", "isMovementBlocked", luaCreatureIsMovementBlocked);
	lsi.registerMethod("Creature", "isImmune", luaCreatureIsImmune);

	lsi.registerMethod("Creature", "canSee", luaCreatureCanSee);
	lsi.registerMethod("Creature", "canSeeCreature", luaCreatureCanSeeCreature);
	lsi.registerMethod("Creature", "canSeeGhostMode", luaCreatureCanSeeGhostMode);
	lsi.registerMethod("Creature", "canSeeInvisibility", luaCreatureCanSeeInvisibility);

	lsi.registerMethod("Creature", "hasParent", luaCreatureHasParent);
	lsi.registerMethod("Creature", "getParent", luaCreatureGetParent);

	lsi.registerMethod("Creature", "getId", luaCreatureGetId);
	lsi.registerMethod("Creature", "getName", luaCreatureGetName);

	lsi.registerMethod("Creature", "getTarget", luaCreatureGetTarget);
	lsi.registerMethod("Creature", "setTarget", luaCreatureSetTarget);

	lsi.registerMethod("Creature", "getFollowCreature", luaCreatureGetFollowCreature);
	lsi.registerMethod("Creature", "setFollowCreature", luaCreatureSetFollowCreature);

	lsi.registerMethod("Creature", "getMaster", luaCreatureGetMaster);
	lsi.registerMethod("Creature", "setMaster", luaCreatureSetMaster);

	lsi.registerMethod("Creature", "getLight", luaCreatureGetLight);
	lsi.registerMethod("Creature", "setLight", luaCreatureSetLight);

	lsi.registerMethod("Creature", "getSpeed", luaCreatureGetSpeed);
	lsi.registerMethod("Creature", "getBaseSpeed", luaCreatureGetBaseSpeed);
	lsi.registerMethod("Creature", "changeSpeed", luaCreatureChangeSpeed);

	lsi.registerMethod("Creature", "setDropLoot", luaCreatureSetDropLoot);
	lsi.registerMethod("Creature", "setSkillLoss", luaCreatureSetSkillLoss);

	lsi.registerMethod("Creature", "getPosition", luaCreatureGetPosition);
	lsi.registerMethod("Creature", "getTile", luaCreatureGetTile);
	lsi.registerMethod("Creature", "getDirection", luaCreatureGetDirection);
	lsi.registerMethod("Creature", "setDirection", luaCreatureSetDirection);

	lsi.registerMethod("Creature", "getHealth", luaCreatureGetHealth);
	lsi.registerMethod("Creature", "setHealth", luaCreatureSetHealth);
	lsi.registerMethod("Creature", "addHealth", luaCreatureAddHealth);
	lsi.registerMethod("Creature", "getMaxHealth", luaCreatureGetMaxHealth);
	lsi.registerMethod("Creature", "setMaxHealth", luaCreatureSetMaxHealth);
	lsi.registerMethod("Creature", "setHiddenHealth", luaCreatureSetHiddenHealth);
	lsi.registerMethod("Creature", "setMovementBlocked", luaCreatureSetMovementBlocked);

	lsi.registerMethod("Creature", "getSkull", luaCreatureGetSkull);
	lsi.registerMethod("Creature", "setSkull", luaCreatureSetSkull);

	lsi.registerMethod("Creature", "getOutfit", luaCreatureGetOutfit);
	lsi.registerMethod("Creature", "setOutfit", luaCreatureSetOutfit);

	lsi.registerMethod("Creature", "getCondition", luaCreatureGetCondition);
	lsi.registerMethod("Creature", "addCondition", luaCreatureAddCondition);
	lsi.registerMethod("Creature", "removeCondition", luaCreatureRemoveCondition);
	lsi.registerMethod("Creature", "hasCondition", luaCreatureHasCondition);

	lsi.registerMethod("Creature", "remove", luaCreatureRemove);
	lsi.registerMethod("Creature", "teleportTo", luaCreatureTeleportTo);
	lsi.registerMethod("Creature", "say", luaCreatureSay);

	lsi.registerMethod("Creature", "getDamageMap", luaCreatureGetDamageMap);

	lsi.registerMethod("Creature", "getSummons", luaCreatureGetSummons);

	lsi.registerMethod("Creature", "getDescription", luaCreatureGetDescription);

	lsi.registerMethod("Creature", "getPathTo", luaCreatureGetPathTo);
	lsi.registerMethod("Creature", "move", luaCreatureMove);

	lsi.registerMethod("Creature", "getZone", luaCreatureGetZone);

	lsi.registerMethod("Creature", "hasIcon", luaCreatureHasIcon);
	lsi.registerMethod("Creature", "setIcon", luaCreatureSetIcon);
	lsi.registerMethod("Creature", "getIcon", luaCreatureGetIcon);
	lsi.registerMethod("Creature", "removeIcon", luaCreatureRemoveIcon);

	lsi.registerMethod("Creature", "getStorageValue", luaCreatureGetStorageValue);
	lsi.registerMethod("Creature", "setStorageValue", luaCreatureSetStorageValue);
}
