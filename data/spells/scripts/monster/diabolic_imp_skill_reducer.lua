local combat = {}

for i = 70, 80 do
	local condition = Condition(CONDITION_ATTRIBUTES)
	condition:setParameter(CONDITION_PARAM_TICKS, 6000)
	condition:setParameter(CONDITION_PARAM_SKILL_MELEEPERCENT, i)

	combat[i] = Combat()
	combat[i]:setParameter(COMBAT_PARAM_DISTANCEEFFECT, CONST_ANI_ENERGY)
	combat[i]:setArea(createCombatArea(AREA_BEAM1))
	combat[i]:setCondition(condition)
end

function onCastSpell(creature, variant)
	return combat[math.random(70, 80)]:execute(creature, variant)
end
