local combat = Combat()
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_GROUNDSHAKER)
combat:setArea(createCombatArea(AREA_CROSS1X1))

local condition = Condition(CONDITION_ATTRIBUTES)
condition:setParameter(CONDITION_PARAM_BUFF_SPELL, 1)
condition:setParameter(CONDITION_PARAM_TICKS, 10000)
condition:setParameter(CONDITION_PARAM_SKILL_DISTANCEPERCENT, 40)
combat:addCondition(condition)

function onCastSpell(creature, var)
	return combat:execute(creature, var)
end
