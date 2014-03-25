local combat = createCombatObject()
setCombatParam(combat, COMBAT_PARAM_EFFECT, CONST_ME_MAGIC_BLUE)
setCombatParam(combat, COMBAT_PARAM_DISTANCEEFFECT, CONST_ANI_LARGEROCK)

local condition = createConditionObject(CONDITION_ATTRIBUTES)
setConditionParam(condition, CONDITION_PARAM_TICKS, 4000)
setConditionParam(condition, CONDITION_PARAM_SKILL_SHIELDPERCENT, 50)
setConditionParam(condition, CONDITION_PARAM_SKILL_DISTANCEPERCENT, 50)

local area = createCombatArea(AREA_CIRCLE2X2)
setCombatArea(combat, area)
setCombatCondition(combat, condition)

function onCastSpell(cid, var)
       return doCombat(cid, combat, var)
end
