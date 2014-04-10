local combat = createCombatObject()
setCombatParam(combat, COMBAT_PARAM_TYPE, COMBAT_FIREDAMAGE)
setCombatParam(combat, COMBAT_PARAM_EFFECT, CONST_ME_HITBYFIRE)

local area = createCombatArea(AREA_WAVE4, AREADIAGONAL_WAVE4)
setCombatArea(combat, area)

function onGetFormulaValues(cid, level, maglevel)
	min = -8
	max = -11
	return min, max
end

setCombatCallback(combat, CALLBACK_PARAM_LEVELMAGICVALUE, "onGetFormulaValues")

function onCastSpell(cid, var)
	return doCombat(cid, combat, var)
end
