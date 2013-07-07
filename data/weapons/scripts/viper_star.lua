local combat = createCombatObject()
setCombatParam(combat, COMBAT_PARAM_BLOCKARMOR, 1)
setCombatParam(combat, COMBAT_PARAM_TYPE, COMBAT_PHYSICALDAMAGE)
setCombatParam(combat, COMBAT_PARAM_DISTANCEEFFECT, CONST_ANI_GREENSTAR)
setCombatFormula(combat, COMBAT_FORMULA_SKILL, 0, 0, 1.0, 0)

local xCombat = createCombatObject()
setCombatParam(xCombat, COMBAT_PARAM_TYPE, COMBAT_EARTHDAMAGE)

local condition = createConditionObject(CONDITION_POISON)
setConditionParam(condition, CONDITION_PARAM_DELAYED, 1)
addDamageCondition(condition, 4, 2000, -2)
addDamageCondition(condition, 6, 2000, -1)
setCombatCondition(xCombat, condition)

function onUseWeapon(cid, var)
	local ret = doCombat(cid, combat, var)
	if(ret == LUA_ERROR) then
		return LUA_ERROR
	end

	local target = variantToNumber(var)
	if(target ~= 0) then
		-- chance to poison the enemy
		local chance = math.random(0, 100)
		if(chance > 90) then
			ret = doCombat(cid, xCombat, var)
		end
	end
	return ret
end