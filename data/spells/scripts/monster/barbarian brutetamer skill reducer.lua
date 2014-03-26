local combat = {}

for i = CONDITION_PARAM_SKILL_FIST, CONDITION_PARAM_SKILL_SHIELD do
	for j = 1, 6 do
		local index = ((i - 20) * 6) + j
		combat[index] = createCombatObject()
		setCombatParam(combat[index], COMBAT_PARAM_DISTANCEEFFECT, CONST_ANI_SNOWBALL)
		setCombatParam(combat[index], COMBAT_PARAM_EFFECT, CONST_ME_POFF)

		local condition = createConditionObject(CONDITION_ATTRIBUTES)
		setConditionParam(condition, CONDITION_PARAM_TICKS, 5000)
		setConditionParam(condition, i, -j)
		setCombatCondition(combat[index], condition)
	end
end

function onCastSpell(cid, var)
	return doCombat(cid, combat[math.random(#combat)], var)
end
