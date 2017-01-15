local combat = Combat()
combat:setParameter(COMBAT_PARAM_TYPE, COMBAT_PHYSICALDAMAGE)
combat:setParameter(COMBAT_PARAM_DISTANCEEFFECT, CONST_ANI_GREENSTAR)
combat:setParameter(COMBAT_PARAM_BLOCKARMOR, true)
combat:setFormula(COMBAT_FORMULA_SKILL, 0, 0, 1, 0)

local condition = Condition(CONDITION_POISON)
condition:setParameter(CONDITION_PARAM_DELAYED, true)
condition:addDamage(10, 4000, -2)
condition:addDamage(20, 4000, -1)

local secondCombat = Combat()
secondCombat:setParameter(COMBAT_PARAM_TYPE, COMBAT_EARTHDAMAGE)
secondCombat:setCondition(condition)

function onUseWeapon(player, variant)
	local boolean = combat:execute(player, variant)
	if not boolean then
		return false
	end

	local target = variant:getNumber()
	if target ~= 0 then
		if math.random(1, 100) > 90 then
			boolean = secondCombat:execute(player, variant)
		end
	end
	return boolean
end
