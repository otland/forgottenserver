local combat = Combat()
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_MAGIC_GREEN)
combat:setParameter(COMBAT_PARAM_AGGRESSIVE, false)

local condition = Condition(CONDITION_ATTRIBUTES)
condition:setParameter(CONDITION_PARAM_TICKS, 10000)
condition:setParameter(CONDITION_PARAM_SKILL_MELEEPERCENT, 135)
condition:setParameter(CONDITION_PARAM_SKILL_SHIELDPERCENT, -100)
condition:setParameter(CONDITION_PARAM_BUFF_SPELL, true)
combat:setCondition(condition)

function onCastSpell(creature, variant)
	return combat:execute(creature, variant)
end
