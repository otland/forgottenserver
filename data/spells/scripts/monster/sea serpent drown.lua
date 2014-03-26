local combat = createCombatObject()
setCombatParam(combat, COMBAT_PARAM_TYPE, COMBAT_DROWNDAMAGE)
setCombatParam(combat, COMBAT_PARAM_EFFECT, CONST_ME_WATERSPLASH)

local condition = createConditionObject(CONDITION_DROWN)
setConditionParam(condition, CONDITION_PARAM_DELAYED, 1)
addDamageCondition(condition, 12, 5000, -20)

local area = createCombatArea(AREA_SQUARE1X1)
setCombatArea(combat, area)
setCombatCondition(combat, condition)

function onCastSpell(cid, var)
	return doCombat(cid, combat, var)
end