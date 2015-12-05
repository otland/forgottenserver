local combat = Combat()
combat:setParameter(COMBAT_PARAM_TYPE, COMBAT_PHYSICALDAMAGE)
combat:setParameter(COMBAT_PARAM_DISTANCEEFFECT, CONST_ANI_POISONARROW)
combat:setParameter(COMBAT_PARAM_BLOCKARMOR, 1)
combat:setParameter(COMBAT_PARAM_BLOCKSHIELD, 1)
combat:setFormula(COMBAT_FORMULA_SKILL, 0, 0, 1, 0)

local condition = Condition(CONDITION_POISON)
condition:setParameter(CONDITION_PARAM_DELAYED, 1)
condition:addDamage(4, 4000, -3)
condition:addDamage(9, 4000, -2)
condition:addDamage(20, 4000, -1)
combat:setCondition(condition)

function onUseWeapon(player, var)
	return combat:execute(player, var)
end
