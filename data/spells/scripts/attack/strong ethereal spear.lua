local combat = createCombatObject()
setCombatParam(combat, COMBAT_PARAM_TYPE, COMBAT_PHYSICALDAMAGE)
setCombatParam(combat, COMBAT_PARAM_BLOCKARMOR, 1)
setCombatParam(combat, COMBAT_PARAM_DISTANCEEFFECT, CONST_ANI_ETHEREALSPEAR)

function onGetFormulaValues(cid, level, maglevel)
	local distanceSkill = Player(cid):getSkillLevel(SKILL_DISTANCE) -- TODO: use varSkills
	local min = (((distanceSkill + 25) / 3) + (level / 5)) * 1.47
	local max = (distanceSkill + 25 + (level / 5)) * 1.47
	return -min, -max
end
setCombatCallback(combat, CALLBACK_PARAM_LEVELMAGICVALUE, "onGetFormulaValues")

function onCastSpell(cid, var)
    return doCombat(cid, combat, var)
end
