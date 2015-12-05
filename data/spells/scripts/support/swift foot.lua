local combat = Combat()
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_MAGIC_GREEN)
combat:setParameter(COMBAT_PARAM_AGGRESSIVE, 0)

local speed = Condition(CONDITION_HASTE)
speed:setParameter(CONDITION_PARAM_TICKS, 10000)
speed:setFormula(0.8, -72, 0.8, -72)
combat:setCondition(speed)

local attackSpellsCooldown = Condition(CONDITION_SPELLGROUPCOOLDOWN)
attackSpellsCooldown:setParameter(CONDITION_PARAM_TICKS, 10000)
attackSpellsCooldown:setParameter(CONDITION_PARAM_SUBID, 1)
combat:setCondition(attackSpellsCooldown)

local pacified = Condition(CONDITION_PACIFIED)
pacified:setParameter(CONDITION_PARAM_TICKS, 10000)
combat:setCondition(pacified)

function onCastSpell(creature, var)
	return combat:execute(creature, var)
end
