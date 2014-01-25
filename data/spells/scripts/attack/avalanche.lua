local combat = createCombatObject()
setCombatParam(combat, COMBAT_PARAM_TYPE, COMBAT_ICEDAMAGE)
setCombatParam(combat, COMBAT_PARAM_EFFECT, CONST_ME_ICEAREA)
setCombatParam(combat, COMBAT_PARAM_DISTANCEEFFECT, CONST_ANI_ICE)

local area = createCombatArea(AREA_CIRCLE3X3)
setCombatArea(combat, area)

function onGetFormulaValues(cid, level, maglevel)
	min = -((level / 5) + (maglevel * 1.2) + 7) -- * 1.2) + 7)
	max = -((level / 5) + (maglevel * 2.85) + 16) -- * 2.85) + 16)
	return min, max
end

setCombatCallback(combat, CALLBACK_PARAM_LEVELMAGICVALUE, "onGetFormulaValues")

function onCastSpell(cid, var)
	return doCombat(cid, combat, var)
end
