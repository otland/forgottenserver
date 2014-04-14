local combat = createCombatObject()
setCombatParam(combat, COMBAT_PARAM_EFFECT, CONST_ME_MAGIC_GREEN)
setCombatParam(combat, COMBAT_PARAM_AGGRESSIVE, 0)

local condition = createConditionObject(CONDITION_ATTRIBUTES)
setConditionParam(condition, CONDITION_PARAM_TICKS, 10000)
setConditionParam(condition, CONDITION_PARAM_SKILL_DISTANCEPERCENT, 150)
setConditionParam(condition, CONDITION_PARAM_SKILL_SHIELDPERCENT, -100)
setConditionParam(condition, CONDITION_PARAM_BUFF_SPELL, 1)
setCombatCondition(combat, condition)

local speed = createConditionObject(CONDITION_PARALYZE)
setConditionParam(speed, CONDITION_PARAM_TICKS, 10000)
setConditionFormula(speed, -0.7, 56, -0.7, 56)
setCombatCondition(combat, speed)

local exhaustHealGroup = createConditionObject(CONDITION_SPELLGROUPCOOLDOWN)
setConditionParam(exhaustHealGroup, CONDITION_PARAM_SUBID, 2)
setConditionParam(exhaustHealGroup, CONDITION_PARAM_TICKS, 10000)
setCombatCondition(combat, exhaustHealGroup)

local exhaustSupportGroup = createConditionObject(CONDITION_SPELLGROUPCOOLDOWN)
setConditionParam(exhaustSupportGroup, CONDITION_PARAM_SUBID, 3)
setConditionParam(exhaustSupportGroup, CONDITION_PARAM_TICKS, 10000)
setCombatCondition(combat, exhaustSupportGroup)

function onCastSpell(cid, var)
	return doCombat(cid, combat, var)
end
