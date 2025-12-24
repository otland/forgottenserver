#include "../../otpch.h"

#include "../../condition.h"

#include "../api.h"
#include "../meta.h"
#include "../register.h"
#include "../script.h"

namespace {

int luaConditionCreate(lua_State* L)
{
	// Condition(conditionType[, conditionId = CONDITIONID_COMBAT])
	ConditionType_t conditionType = tfs::lua::getNumber<ConditionType_t>(L, 2);
	ConditionId_t conditionId = tfs::lua::getNumber<ConditionId_t>(L, 3, CONDITIONID_COMBAT);

	Condition* condition = Condition::createCondition(conditionId, conditionType, 0, 0);
	if (condition) {
		tfs::lua::pushUserdata(L, condition);
		tfs::lua::setMetatable(L, -1, "Condition");
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaConditionDelete(lua_State* L)
{
	// condition:delete()
	Condition** conditionPtr = tfs::lua::getRawUserdata<Condition>(L, 1);
	if (conditionPtr && *conditionPtr) {
		delete *conditionPtr;
		*conditionPtr = nullptr;
	}
	return 0;
}

int luaConditionGetId(lua_State* L)
{
	// condition:getId()
	Condition* condition = tfs::lua::getUserdata<Condition>(L, 1);
	if (condition) {
		tfs::lua::pushNumber(L, condition->getId());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaConditionGetSubId(lua_State* L)
{
	// condition:getSubId()
	Condition* condition = tfs::lua::getUserdata<Condition>(L, 1);
	if (condition) {
		tfs::lua::pushNumber(L, condition->getSubId());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaConditionGetType(lua_State* L)
{
	// condition:getType()
	Condition* condition = tfs::lua::getUserdata<Condition>(L, 1);
	if (condition) {
		tfs::lua::pushNumber(L, condition->getType());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaConditionGetIcons(lua_State* L)
{
	// condition:getIcons()
	Condition* condition = tfs::lua::getUserdata<Condition>(L, 1);
	if (condition) {
		tfs::lua::pushNumber(L, condition->getIcons());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaConditionGetEndTime(lua_State* L)
{
	// condition:getEndTime()
	Condition* condition = tfs::lua::getUserdata<Condition>(L, 1);
	if (condition) {
		tfs::lua::pushNumber(L, condition->getEndTime());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaConditionClone(lua_State* L)
{
	// condition:clone()
	Condition* condition = tfs::lua::getUserdata<Condition>(L, 1);
	if (condition) {
		tfs::lua::pushUserdata(L, condition->clone());
		tfs::lua::setMetatable(L, -1, "Condition");
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaConditionGetTicks(lua_State* L)
{
	// condition:getTicks()
	Condition* condition = tfs::lua::getUserdata<Condition>(L, 1);
	if (condition) {
		tfs::lua::pushNumber(L, condition->getTicks());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaConditionSetTicks(lua_State* L)
{
	// condition:setTicks(ticks)
	int32_t ticks = tfs::lua::getNumber<int32_t>(L, 2);
	Condition* condition = tfs::lua::getUserdata<Condition>(L, 1);
	if (condition) {
		condition->setTicks(ticks);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaConditionSetParameter(lua_State* L)
{
	// condition:setParameter(key, value)
	Condition* condition = tfs::lua::getUserdata<Condition>(L, 1);
	if (!condition) {
		lua_pushnil(L);
		return 1;
	}

	ConditionParam_t key = tfs::lua::getNumber<ConditionParam_t>(L, 2);
	int32_t value;
	if (lua_isboolean(L, 3)) {
		value = tfs::lua::getBoolean(L, 3) ? 1 : 0;
	} else {
		value = tfs::lua::getNumber<int32_t>(L, 3);
	}
	condition->setParam(key, value);
	tfs::lua::pushBoolean(L, true);
	return 1;
}

int luaConditionGetParameter(lua_State* L)
{
	// condition:getParameter(key)
	Condition* condition = tfs::lua::getUserdata<Condition>(L, 1);
	if (!condition) {
		lua_pushnil(L);
		return 1;
	}

	int32_t value = condition->getParam(tfs::lua::getNumber<ConditionParam_t>(L, 2));
	if (value == std::numeric_limits<int32_t>().max()) {
		lua_pushnil(L);
		return 1;
	}

	tfs::lua::pushNumber(L, value);
	return 1;
}

int luaConditionSetFormula(lua_State* L)
{
	// condition:setFormula(mina, minb, maxa, maxb)
	double maxb = tfs::lua::getNumber<double>(L, 5);
	double maxa = tfs::lua::getNumber<double>(L, 4);
	double minb = tfs::lua::getNumber<double>(L, 3);
	double mina = tfs::lua::getNumber<double>(L, 2);
	ConditionSpeed* condition = dynamic_cast<ConditionSpeed*>(tfs::lua::getUserdata<Condition>(L, 1));
	if (condition) {
		condition->setFormulaVars(mina, minb, maxa, maxb);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaConditionSetOutfit(lua_State* L)
{
	// condition:setOutfit(outfit)
	// condition:setOutfit(lookTypeEx, lookType, lookHead, lookBody, lookLegs, lookFeet[, lookAddons[, lookMount]])
	Outfit_t outfit;
	if (lua_istable(L, 2)) {
		outfit = tfs::lua::getOutfit(L, 2);
	} else {
		outfit.lookMount = tfs::lua::getNumber<uint16_t>(L, 9, outfit.lookMount);
		outfit.lookAddons = tfs::lua::getNumber<uint8_t>(L, 8, outfit.lookAddons);
		outfit.lookFeet = tfs::lua::getNumber<uint8_t>(L, 7);
		outfit.lookLegs = tfs::lua::getNumber<uint8_t>(L, 6);
		outfit.lookBody = tfs::lua::getNumber<uint8_t>(L, 5);
		outfit.lookHead = tfs::lua::getNumber<uint8_t>(L, 4);
		outfit.lookType = tfs::lua::getNumber<uint16_t>(L, 3);
		outfit.lookTypeEx = tfs::lua::getNumber<uint16_t>(L, 2);
	}

	ConditionOutfit* condition = dynamic_cast<ConditionOutfit*>(tfs::lua::getUserdata<Condition>(L, 1));
	if (condition) {
		condition->setOutfit(outfit);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaConditionAddDamage(lua_State* L)
{
	// condition:addDamage(rounds, time, value)
	int32_t value = tfs::lua::getNumber<int32_t>(L, 4);
	int32_t time = tfs::lua::getNumber<int32_t>(L, 3);
	int32_t rounds = tfs::lua::getNumber<int32_t>(L, 2);
	ConditionDamage* condition = dynamic_cast<ConditionDamage*>(tfs::lua::getUserdata<Condition>(L, 1));
	if (condition) {
		tfs::lua::pushBoolean(L, condition->addDamage(rounds, time, value));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

} // namespace

void tfs::lua::registerCondition(LuaScriptInterface& lsi)
{
	lsi.registerClass("Condition", "", luaConditionCreate);
	lsi.registerMetaMethod("Condition", "__eq", tfs::lua::luaUserdataCompare);
	lsi.registerMetaMethod("Condition", "__gc", luaConditionDelete);

	lsi.registerMethod("Condition", "getId", luaConditionGetId);
	lsi.registerMethod("Condition", "getSubId", luaConditionGetSubId);
	lsi.registerMethod("Condition", "getType", luaConditionGetType);
	lsi.registerMethod("Condition", "getIcons", luaConditionGetIcons);
	lsi.registerMethod("Condition", "getEndTime", luaConditionGetEndTime);

	lsi.registerMethod("Condition", "clone", luaConditionClone);

	lsi.registerMethod("Condition", "getTicks", luaConditionGetTicks);
	lsi.registerMethod("Condition", "setTicks", luaConditionSetTicks);

	lsi.registerMethod("Condition", "setParameter", luaConditionSetParameter);
	lsi.registerMethod("Condition", "getParameter", luaConditionGetParameter);

	lsi.registerMethod("Condition", "setFormula", luaConditionSetFormula);
	lsi.registerMethod("Condition", "setOutfit", luaConditionSetOutfit);

	lsi.registerMethod("Condition", "addDamage", luaConditionAddDamage);
}
