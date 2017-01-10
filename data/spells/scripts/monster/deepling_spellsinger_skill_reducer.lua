local combat = {}

for i = 45, 65 do
	local condition = Condition(CONDITION_ATTRIBUTES)
	condition:setParameter(CONDITION_PARAM_TICKS, 8000)
	condition:setParameter(CONDITION_PARAM_SKILL_MELEEPERCENT, i)
	condition:setParameter(CONDITION_PARAM_SKILL_DISTANCEPERCENT, i)

	combat[i] = Combat()
	combat[i]:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_STUN)
	combat[i]:setParameter(COMBAT_PARAM_DISTANCEEFFECT, CONST_ANI_EXPLOSION)
	combat[i]:setArea(createCombatArea(AREA_BEAM1))
	combat[i]:setCondition(condition)
end

function onCastSpell(creature, variant)
	return combat[math.random(45, 65)]:execute(creature, variant)
end
