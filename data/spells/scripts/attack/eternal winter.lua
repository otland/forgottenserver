local combat = createCombatObject()
setCombatParam(combat, COMBAT_PARAM_TYPE, COMBAT_ICEDAMAGE)
setCombatParam(combat, COMBAT_PARAM_EFFECT, CONST_ME_ICETORNADO)


local area = createCombatArea(AREA_CROSS5X5)
setCombatArea(combat, area)

function onGetFormulaValues(cid, level, maglevel)
min = -((level*0.2) + (maglevel*6))
max = -((level*0.2) + (maglevel*12))
return min, max
end

setCombatCallback(combat, CALLBACK_PARAM_LEVELMAGICVALUE, "onGetFormulaValues")
function onCastSpell(cid, var)
        return doCombat(cid, combat, var)
end