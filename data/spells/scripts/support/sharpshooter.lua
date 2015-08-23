local combat = Combat()
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_MAGIC_GREEN)
combat:setParameter(COMBAT_PARAM_AGGRESSIVE, 0)

local skill = Condition(CONDITION_ATTRIBUTES)
skill:setParameter(CONDITION_PARAM_TICKS, 10000)
skill:setParameter(CONDITION_PARAM_SKILL_DISTANCEPERCENT, 150)
skill:setParameter(CONDITION_PARAM_SKILL_SHIELDPERCENT, -100)
skill:setParameter(CONDITION_PARAM_BUFF_SPELL, 1)
combat:setCondition(skill)

local speed = Condition(CONDITION_PARALYZE)
speed:setParameter(CONDITION_PARAM_TICKS, 10000)
speed:setFormula(-0.7, 56, -0.7, 56)
combat:setCondition(speed)

local healingSpellsCooldown = Condition(CONDITION_SPELLGROUPCOOLDOWN)
healingSpellsCooldown:setParameter(CONDITION_PARAM_TICKS, 10000)
healingSpellsCooldown:setParameter(CONDITION_PARAM_SUBID, 2)
combat:setCondition(healingSpellsCooldown)

function onCastSpell(creature, var)
	return combat:execute(creature, var)
end
