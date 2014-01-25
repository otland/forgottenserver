local combat = createCombatObject()
setCombatParam(combat, COMBAT_PARAM_TYPE, COMBAT_EARTHDAMAGE)
setCombatParam(combat, COMBAT_PARAM_EFFECT, CONST_ME_PLANTATTACK)

local area = createCombatArea(AREA_CROSS6X6)
setCombatArea(combat, area)

function onGetFormulaValues(cid, level, maglevel)
        min = -((level / 5) + (maglevel * 3) + 32) -- * 3) + 32)
        max = -((level / 5) + (maglevel * 9) + 40) -- * 9) + 40)
        return min, max
end

setCombatCallback(combat, CALLBACK_PARAM_LEVELMAGICVALUE, "onGetFormulaValues")

function onCastSpell(cid, var)
	return doCombat(cid, combat, var)
end
