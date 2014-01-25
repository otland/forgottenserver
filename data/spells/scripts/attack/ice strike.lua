local combat = createCombatObject()
setCombatParam(combat, COMBAT_PARAM_TYPE, COMBAT_ICEDAMAGE)
setCombatParam(combat, COMBAT_PARAM_EFFECT, CONST_ME_ICEATTACK)
setCombatParam(combat, COMBAT_PARAM_DISTANCEEFFECT, CONST_ANI_SMALLICE)

function onGetFormulaValues(cid, level, maglevel)
	min = -((level / 5) + (maglevel * 1.4) + 8) -- * 1.4) + 8)
	max = -((level / 5) + (maglevel * 2.2) + 14) -- * 2.2) + 14)
	return min, max
end

setCombatCallback(combat, CALLBACK_PARAM_LEVELMAGICVALUE, "onGetFormulaValues")

function onCastSpell(cid, var)
	return doCombat(cid, combat, var)
end
