#include "../../otpch.h"

#include "../../vocation.h"

#include "../api.h"
#include "../meta.h"
#include "../register.h"
#include "../script.h"

extern Vocations g_vocations;

namespace {

int luaVocationCreate(lua_State* L)
{
	// Vocation(id or name)
	uint32_t id;
	if (tfs::lua::isNumber(L, 2)) {
		id = tfs::lua::getNumber<uint32_t>(L, 2);
	} else {
		id = g_vocations.getVocationId(tfs::lua::getString(L, 2));
	}

	Vocation* vocation = g_vocations.getVocation(id);
	if (vocation) {
		tfs::lua::pushUserdata(L, vocation);
		tfs::lua::setMetatable(L, -1, "Vocation");
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaVocationGetId(lua_State* L)
{
	// vocation:getId()
	Vocation* vocation = tfs::lua::getUserdata<Vocation>(L, 1);
	if (vocation) {
		tfs::lua::pushNumber(L, vocation->getId());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaVocationGetClientId(lua_State* L)
{
	// vocation:getClientId()
	Vocation* vocation = tfs::lua::getUserdata<Vocation>(L, 1);
	if (vocation) {
		tfs::lua::pushNumber(L, vocation->getClientId());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaVocationGetName(lua_State* L)
{
	// vocation:getName()
	Vocation* vocation = tfs::lua::getUserdata<Vocation>(L, 1);
	if (vocation) {
		tfs::lua::pushString(L, vocation->getVocName());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaVocationGetDescription(lua_State* L)
{
	// vocation:getDescription()
	Vocation* vocation = tfs::lua::getUserdata<Vocation>(L, 1);
	if (vocation) {
		tfs::lua::pushString(L, vocation->getVocDescription());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaVocationGetRequiredSkillTries(lua_State* L)
{
	// vocation:getRequiredSkillTries(skillType, skillLevel)
	Vocation* vocation = tfs::lua::getUserdata<Vocation>(L, 1);
	if (vocation) {
		skills_t skillType = tfs::lua::getNumber<skills_t>(L, 2);
		uint16_t skillLevel = tfs::lua::getNumber<uint16_t>(L, 3);
		tfs::lua::pushNumber(L, vocation->getReqSkillTries(skillType, skillLevel));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaVocationGetRequiredManaSpent(lua_State* L)
{
	// vocation:getRequiredManaSpent(magicLevel)
	Vocation* vocation = tfs::lua::getUserdata<Vocation>(L, 1);
	if (vocation) {
		uint32_t magicLevel = tfs::lua::getNumber<uint32_t>(L, 2);
		tfs::lua::pushNumber(L, vocation->getReqMana(magicLevel));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaVocationGetCapacityGain(lua_State* L)
{
	// vocation:getCapacityGain()
	Vocation* vocation = tfs::lua::getUserdata<Vocation>(L, 1);
	if (vocation) {
		tfs::lua::pushNumber(L, vocation->getCapGain());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaVocationGetHealthGain(lua_State* L)
{
	// vocation:getHealthGain()
	Vocation* vocation = tfs::lua::getUserdata<Vocation>(L, 1);
	if (vocation) {
		tfs::lua::pushNumber(L, vocation->getHPGain());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaVocationGetHealthGainTicks(lua_State* L)
{
	// vocation:getHealthGainTicks()
	Vocation* vocation = tfs::lua::getUserdata<Vocation>(L, 1);
	if (vocation) {
		tfs::lua::pushNumber(L, vocation->getHealthGainTicks());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaVocationGetHealthGainAmount(lua_State* L)
{
	// vocation:getHealthGainAmount()
	Vocation* vocation = tfs::lua::getUserdata<Vocation>(L, 1);
	if (vocation) {
		tfs::lua::pushNumber(L, vocation->getHealthGainAmount());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaVocationGetManaGain(lua_State* L)
{
	// vocation:getManaGain()
	Vocation* vocation = tfs::lua::getUserdata<Vocation>(L, 1);
	if (vocation) {
		tfs::lua::pushNumber(L, vocation->getManaGain());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaVocationGetManaGainTicks(lua_State* L)
{
	// vocation:getManaGainTicks()
	Vocation* vocation = tfs::lua::getUserdata<Vocation>(L, 1);
	if (vocation) {
		tfs::lua::pushNumber(L, vocation->getManaGainTicks());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaVocationGetManaGainAmount(lua_State* L)
{
	// vocation:getManaGainAmount()
	Vocation* vocation = tfs::lua::getUserdata<Vocation>(L, 1);
	if (vocation) {
		tfs::lua::pushNumber(L, vocation->getManaGainAmount());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaVocationGetMaxSoul(lua_State* L)
{
	// vocation:getMaxSoul()
	Vocation* vocation = tfs::lua::getUserdata<Vocation>(L, 1);
	if (vocation) {
		tfs::lua::pushNumber(L, vocation->getSoulMax());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaVocationGetSoulGainTicks(lua_State* L)
{
	// vocation:getSoulGainTicks()
	Vocation* vocation = tfs::lua::getUserdata<Vocation>(L, 1);
	if (vocation) {
		tfs::lua::pushNumber(L, vocation->getSoulGainTicks());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaVocationGetAttackSpeed(lua_State* L)
{
	// vocation:getAttackSpeed()
	Vocation* vocation = tfs::lua::getUserdata<Vocation>(L, 1);
	if (vocation) {
		tfs::lua::pushNumber(L, vocation->getAttackSpeed());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaVocationGetBaseSpeed(lua_State* L)
{
	// vocation:getBaseSpeed()
	Vocation* vocation = tfs::lua::getUserdata<Vocation>(L, 1);
	if (vocation) {
		tfs::lua::pushNumber(L, vocation->getBaseSpeed());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaVocationGetDemotion(lua_State* L)
{
	// vocation:getDemotion()
	Vocation* vocation = tfs::lua::getUserdata<Vocation>(L, 1);
	if (!vocation) {
		lua_pushnil(L);
		return 1;
	}

	uint16_t fromId = vocation->getFromVocation();
	if (fromId == VOCATION_NONE) {
		lua_pushnil(L);
		return 1;
	}

	Vocation* demotedVocation = g_vocations.getVocation(fromId);
	if (demotedVocation && demotedVocation != vocation) {
		tfs::lua::pushUserdata(L, demotedVocation);
		tfs::lua::setMetatable(L, -1, "Vocation");
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaVocationGetPromotion(lua_State* L)
{
	// vocation:getPromotion()
	Vocation* vocation = tfs::lua::getUserdata<Vocation>(L, 1);
	if (!vocation) {
		lua_pushnil(L);
		return 1;
	}

	uint16_t promotedId = g_vocations.getPromotedVocation(vocation->getId());
	if (promotedId == VOCATION_NONE) {
		lua_pushnil(L);
		return 1;
	}

	Vocation* promotedVocation = g_vocations.getVocation(promotedId);
	if (promotedVocation && promotedVocation != vocation) {
		tfs::lua::pushUserdata(L, promotedVocation);
		tfs::lua::setMetatable(L, -1, "Vocation");
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaVocationAllowsPvp(lua_State* L)
{
	// vocation:allowsPvp()
	Vocation* vocation = tfs::lua::getUserdata<Vocation>(L, 1);
	if (vocation) {
		tfs::lua::pushBoolean(L, vocation->allowsPvp());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaVocationGetNoPongKickTime(lua_State* L)
{
	// vocation:getNoPongKickTime()
	Vocation* vocation = tfs::lua::getUserdata<Vocation>(L, 1);
	if (vocation) {
		tfs::lua::pushNumber(L, vocation->getNoPongKickTime());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

} // namespace

void tfs::lua::registerVocation(LuaScriptInterface& lsi)
{
	lsi.registerClass("Vocation", "", luaVocationCreate);
	lsi.registerMetaMethod("Vocation", "__eq", tfs::lua::luaUserdataCompare);

	lsi.registerMethod("Vocation", "getId", luaVocationGetId);
	lsi.registerMethod("Vocation", "getClientId", luaVocationGetClientId);
	lsi.registerMethod("Vocation", "getName", luaVocationGetName);
	lsi.registerMethod("Vocation", "getDescription", luaVocationGetDescription);

	lsi.registerMethod("Vocation", "getRequiredSkillTries", luaVocationGetRequiredSkillTries);
	lsi.registerMethod("Vocation", "getRequiredManaSpent", luaVocationGetRequiredManaSpent);

	lsi.registerMethod("Vocation", "getCapacityGain", luaVocationGetCapacityGain);

	lsi.registerMethod("Vocation", "getHealthGain", luaVocationGetHealthGain);
	lsi.registerMethod("Vocation", "getHealthGainTicks", luaVocationGetHealthGainTicks);
	lsi.registerMethod("Vocation", "getHealthGainAmount", luaVocationGetHealthGainAmount);

	lsi.registerMethod("Vocation", "getManaGain", luaVocationGetManaGain);
	lsi.registerMethod("Vocation", "getManaGainTicks", luaVocationGetManaGainTicks);
	lsi.registerMethod("Vocation", "getManaGainAmount", luaVocationGetManaGainAmount);

	lsi.registerMethod("Vocation", "getMaxSoul", luaVocationGetMaxSoul);
	lsi.registerMethod("Vocation", "getSoulGainTicks", luaVocationGetSoulGainTicks);

	lsi.registerMethod("Vocation", "getAttackSpeed", luaVocationGetAttackSpeed);
	lsi.registerMethod("Vocation", "getBaseSpeed", luaVocationGetBaseSpeed);

	lsi.registerMethod("Vocation", "getDemotion", luaVocationGetDemotion);
	lsi.registerMethod("Vocation", "getPromotion", luaVocationGetPromotion);

	lsi.registerMethod("Vocation", "allowsPvp", luaVocationAllowsPvp);
	lsi.registerMethod("Vocation", "getNoPongKickTime", luaVocationGetNoPongKickTime);
}
