local combat = Combat()
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_MAGIC_GREEN)
combat:setParameter(COMBAT_PARAM_AGGRESSIVE, false)

local speed = Condition(CONDITION_HASTE)
speed:setParameter(CONDITION_PARAM_TICKS, 10000)
speed:setFormula(0.8, -72, 0.8, -72)
combat:setCondition(speed)

local cooldownAttackGroup = Condition(CONDITION_SPELLGROUPCOOLDOWN)
cooldownAttackGroup:setParameter(CONDITION_PARAM_SUBID, 1)
cooldownAttackGroup:setParameter(CONDITION_PARAM_TICKS, 10000)
combat:setCondition(cooldownAttackGroup)

local pacified = Condition(CONDITION_PACIFIED)
pacified:setParameter(CONDITION_PARAM_TICKS, 10000)
combat:setCondition(pacified)

function onCastSpell(creature, variant)
	return combat:execute(creature, variant)
end
