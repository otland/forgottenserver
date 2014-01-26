local combat = createCombatObject()
setCombatParam(combat, COMBAT_PARAM_TYPE, COMBAT_PHYSICALDAMAGE)
setCombatParam(combat, COMBAT_PARAM_BLOCKARMOR, 1)
setCombatParam(combat, COMBAT_PARAM_EFFECT, CONST_ME_HITAREA)
setCombatParam(combat, COMBAT_PARAM_DISTANCEEFFECT, CONST_ANI_ETHEREALSPEAR)

function onGetPlayerMinMaxValues(cid, level, attackValue)
	local distanceSkill = Player(cid):getEffectiveSkillLevel(SKILL_DISTANCE)
	local skillTotal, levelTotal = distanceSkill * attackValue, level / 5
	return -(((skillTotal * 0.015) + 17) + (levelTotal)) / 1.5, -(((skillTotal * 0.025) + 13) + (levelTotal))
end
-- If you prefer to use min values from TS instead of raw values, e.g. '/ 1.5'
-- return -(((skillTotal * 0.0155) + 7) + (levelTotal)), -(((skillTotal * 0.025) + 13) + (levelTotal))
setCombatCallback(combat, CALLBACK_PARAM_SKILLVALUE, "onGetPlayerMinMaxValues")

function onCastSpell(cid, var)
	return doCombat(cid, combat, var)
end
