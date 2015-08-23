local combat = Combat()
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_MAGIC_GREEN)
combat:setParameter(COMBAT_PARAM_AGGRESSIVE, 0)

local skill = Condition(CONDITION_ATTRIBUTES)
skill:setParameter(CONDITION_PARAM_TICKS, 10000)
skill:setParameter(CONDITION_PARAM_SKILL_MELEEPERCENT, 135)
skill:setParameter(CONDITION_PARAM_SKILL_SHIELDPERCENT, -100)
skill:setParameter(CONDITION_PARAM_BUFF_SPELL, 1)
combat:setCondition(skill)

function onCastSpell(creature, var)
	return combat:execute(creature, var)
end
