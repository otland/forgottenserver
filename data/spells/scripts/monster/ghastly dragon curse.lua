local combat = {}

for i = 40, 170 do
	for j = 1, 3 do
		local index = ((i - 40) * 3) + j
		combat[index] = Combat()
		combat[index]:setParameter(COMBAT_PARAM_TYPE, COMBAT_DEATHDAMAGE)
		combat[index]:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_SMALLCLOUDS)
		combat[index]:setParameter(COMBAT_PARAM_DISTANCEEFFECT, CONST_ANI_DEATH)

		local condition = Condition(CONDITION_CURSED)
		condition:setParameter(CONDITION_PARAM_DELAYED, 1)
		
		local damage = i
		condition:addDamage(1, 4000, -damage)
		for k = 1, 4 do
			damage = damage * 1.2
			condition:addDamage(1, 4000, -damage)
		end
		if j > 1 then
			damage = damage * 1.2
			condition:addDamage(1, 4000, -damage)
			if j > 2 then
				condition:addDamage(1, 4000, -(damage * 1.2))
			end
		end
		combat[index]:setCondition(condition)
	end
end

function onCastSpell(creature, var)
	return combat[math.random(#combat)]:execute(creature, var)
end
