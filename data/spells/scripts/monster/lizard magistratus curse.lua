local combat = {}

for i = 2, 3 do
	combat[i] = createCombatObject()
	setCombatParam(combat[i], COMBAT_PARAM_TYPE, COMBAT_DEATHDAMAGE)
	setCombatParam(combat[i], COMBAT_PARAM_EFFECT, CONST_ME_SMALLCLOUDS)
	setCombatParam(combat[i], COMBAT_PARAM_DISTANCEEFFECT, CONST_ANI_DEATH)

	local condition = createConditionObject(CONDITION_CURSED)
	setConditionParam(condition, CONDITION_PARAM_DELAYED, 1)

	local damage = i
	addDamageCondition(condition, 1, 4000, -damage)
	for j = 1, 20 do
		damage = damage * 1.2
		addDamageCondition(condition, 1, 4000, -damage)
	end

	setCombatCondition(combat[i], condition)
end

function onCastSpell(cid, var)
	return doCombat(cid, combat[math.random(2, 3)], var)
end
