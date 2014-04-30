local combat = Combat()
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_STUN)

local condition = Condition(CONDITION_ATTRIBUTES)
condition:setParameter(CONDITION_PARAM_TICKS, 4000)
condition:setParameter(CONDITION_PARAM_STAT_MAGICPOINTSPERCENT, 70)

local area = createCombatArea(AREA_CROSS6X6)
combat:setArea(area)
combat:setCondition(condition)

function onCastSpell(creature, var)
	return combat:execute(creature, var)
end
