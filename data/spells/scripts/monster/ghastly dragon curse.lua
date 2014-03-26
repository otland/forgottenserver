local combat = {}

for i = 40, 170 do
	for j = 1, 3 do
		local index = ((i - 40) * 3) + j
		combat[index] = createCombatObject()
		setCombatParam(combat[index], COMBAT_PARAM_TYPE, COMBAT_DEATHDAMAGE)
		setCombatParam(combat[index], COMBAT_PARAM_EFFECT, CONST_ME_SMALLCLOUDS)
		setCombatParam(combat[index], COMBAT_PARAM_DISTANCEEFFECT, CONST_ANI_DEATH)

		local condition = createConditionObject(CONDITION_CURSED)
		setConditionParam(condition, CONDITION_PARAM_DELAYED, 1)
		
		local damage = i
		addDamageCondition(condition, 1, 4000, -damage)
		for k = 1, 4 do
			damage = damage * 1.2
			addDamageCondition(condition, 1, 4000, -damage)
		end
		if j > 1 then
			damage = damage * 1.2
			addDamageCondition(condition, 1, 4000, -damage)
			if j > 2 then
				addDamageCondition(condition, 1, 4000, -(damage * 1.2))
			end
		end
		setCombatCondition(combat[index], condition)
	end
end

function onCastSpell(cid, var)
	return doCombat(cid, combat[math.random(#combat)], var)
end
