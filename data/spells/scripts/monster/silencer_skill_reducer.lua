local combat = {}

for i = 20, 70 do
	local condition = Condition(CONDITION_ATTRIBUTES)
	condition:setParameter(CONDITION_PARAM_TICKS, 6000)
	condition:setParameter(CONDITION_PARAM_STAT_MAGICPOINTSPERCENT, i)

	combat[i] = Combat()
	combat[i]:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_ENERGYHIT)
	combat[i]:setParameter(COMBAT_PARAM_DISTANCEEFFECT, CONST_ANI_ENERGY)
	combat[i]:setArea(createCombatArea(AREA_CIRCLE2X2))
	combat[i]:setCondition(condition)
end

function onCastSpell(creature, variant)
	return combat[math.random(20, 70)]:execute(creature, variant)
end
