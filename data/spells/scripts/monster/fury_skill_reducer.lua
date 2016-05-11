local condition = Condition(CONDITION_ATTRIBUTES)
condition:setParameter(CONDITION_PARAM_TICKS, 5000)
condition:setParameter(CONDITION_PARAM_SKILL_SHIELDPERCENT, 60)
condition:setParameter(CONDITION_PARAM_SKILL_MELEEPERCENT, 70)

local combat = Combat()
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_SOUND_YELLOW)
combat:setArea(createCombatArea(AREA_CIRCLE3X3))
combat:setCondition(condition)

function onCastSpell(creature, variant)
	return combat:execute(creature, variant)
end
