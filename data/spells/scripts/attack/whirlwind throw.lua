local combat = createCombatObject()
setCombatParam(combat, COMBAT_PARAM_TYPE, COMBAT_PHYSICALDAMAGE)
setCombatParam(combat, COMBAT_PARAM_BLOCKARMOR, TRUE)
setCombatParam(combat, COMBAT_PARAM_DISTANCEEFFECT, CONST_ANI_WEAPONTYPE)
setCombatFormula(combat, COMBAT_FORMULA_SKILL, 0, -20, 0.5, 0)

function onCastSpell(cid, var)
	return doCombat(cid, combat, var)
end
