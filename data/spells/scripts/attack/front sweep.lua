local combat = createCombatObject()
setCombatParam(combat, COMBAT_PARAM_TYPE, COMBAT_PHYSICALDAMAGE)
setCombatParam(combat, COMBAT_PARAM_EFFECT, CONST_ME_HITAREA)
setCombatFormula(combat, COMBAT_FORMULA_SKILL, 0.6, 0, 1.4, 0)

local area = createCombatArea(AREA_WAVE6, AREADIAGONAL_WAVE6)
setCombatArea(combat, area)

function onCastSpell(cid, var)
	return doCombat(cid, combat, var)
end
