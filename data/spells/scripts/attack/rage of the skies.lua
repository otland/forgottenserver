local combat = createCombatObject()
setCombatParam(combat, COMBAT_PARAM_TYPE, COMBAT_ENERGYDAMAGE)
setCombatParam(combat, COMBAT_PARAM_EFFECT, CONST_ME_BIGCLOUDS)

local area = createCombatArea(AREA_CROSS6X6)
setCombatArea(combat, area)

function onGetFormulaValues(cid, level, maglevel)
min = -((level*0.2) + (maglevel*5))
max = -((level*0.2) + (maglevel*12))
return min, max
end

setCombatCallback(combat, CALLBACK_PARAM_LEVELMAGICVALUE, "onGetFormulaValues")

function onCastSpell(cid, var)
        return doCombat(cid, combat, var)
end