local combat = Combat()
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_MAGIC_GREEN)
combat:setParameter(COMBAT_PARAM_AGGRESSIVE, 0)

local condition = Condition(CONDITION_ATTRIBUTES)
condition:setParameter(CONDITION_PARAM_TICKS, 10000)
condition:setParameter(CONDITION_PARAM_SKILL_DISTANCEPERCENT, 150)
condition:setParameter(CONDITION_PARAM_SKILL_SHIELDPERCENT, -100)
condition:setParameter(CONDITION_PARAM_BUFF_SPELL, 1)
combat:setCondition(condition)

local speed = Condition(CONDITION_HASTE)
speed:setParameter(CONDITION_PARAM_TICKS, 10000)
speed:setFormula(-0.7, 56, -0.7, 56)
combat:setCondition(speed)

local exhaustHealGroup = Condition(CONDITION_EXHAUST_HEAL)
exhaustHealGroup:setParameter(CONDITION_PARAM_SUBID, 2)
exhaustHealGroup:setParameter(CONDITION_PARAM_TICKS, 10000)
combat:setCondition(exhaustHealGroup)

local exhaustSupportGroup = Condition(CONDITION_SPELLGROUPCOOLDOWN)
exhaustSupportGroup:setParameter(CONDITION_PARAM_SUBID, 3)
exhaustSupportGroup:setParameter(CONDITION_PARAM_TICKS, 10000)
combat:setCondition(exhaustSupportGroup)

function onCastSpell(creature, var)
	return combat:execute(creature, var)
end
