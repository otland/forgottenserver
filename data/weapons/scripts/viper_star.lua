local combat = Combat()
combat:setParameter(COMBAT_PARAM_BLOCKARMOR, 1)
combat:setParameter(COMBAT_PARAM_TYPE, COMBAT_PHYSICALDAMAGE)
combat:setParameter(COMBAT_PARAM_DISTANCEEFFECT, CONST_ANI_GREENSTAR)
combat:setFormula(COMBAT_FORMULA_SKILL, 0, 0, 1.0, 0)

local xCombat = Combat()
xCombat:setParameter(COMBAT_PARAM_TYPE, COMBAT_EARTHDAMAGE)

local condition = Condition(CONDITION_POISON)
condition:setParameter(CONDITION_PARAM_DELAYED, 1)
condition:addDamage(4, 2000, -2)
condition:addDamage(6, 2000, -1)
xCombat:setCondition(condition)

function onUseWeapon(creature, var)
	local ret = combat:execute(creature, var)
	if ret == LUA_ERROR then
		return LUA_ERROR
	end

	local target = var:getNumber()
	if target ~= 0 then
		if math.random(0, 100) > 90 then
			ret = xCombat:execute(creature, var)
		end
	end
	return ret
end
