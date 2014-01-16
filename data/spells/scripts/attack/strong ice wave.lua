local combat = createCombatObject()
setCombatParam(combat, COMBAT_PARAM_TYPE, COMBAT_ICEDAMAGE)
setCombatParam(combat, COMBAT_PARAM_EFFECT, CONST_ME_ICEATTACK)

function onGetFormulaValues(cid, level, maglevel)
min = -((level*0.2) + (maglevel*4.623) + 20)
max = -((level*0.2) + (maglevel*7.703) + 48)
return min, max
end

setCombatCallback(combat, CALLBACK_PARAM_LEVELMAGICVALUE, "onGetFormulaValues")

local area = createCombatArea(AREA_WAVESICE, AREADIAGONAL_WAVE4)
setCombatArea(combat, area)

function onCastSpell(cid, var)
	return doCombat(cid, combat, var)
end
