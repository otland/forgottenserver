local combat = Combat()
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_MORTAREA)
combat:setParameter(COMBAT_PARAM_DISTANCEEFFECT, CONST_ANI_SUDDENDEATH)

local condition = Condition(CONDITION_ATTRIBUTES)
condition:setParameter(CONDITION_PARAM_TICKS, 6000)
condition:setParameter(CONDITION_PARAM_SKILL_MELEEPERCENT, 50)
condition:setParameter(CONDITION_PARAM_SKILL_SHIELDPERCENT, 50)
combat:addCondition(condition)

function onCastSpell(creature, var)
	return combat:execute(creature, var)
end
