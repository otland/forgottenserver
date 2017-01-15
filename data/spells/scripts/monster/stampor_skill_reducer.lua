local combat = {}

for i = 60, 85 do
	local condition = Condition(CONDITION_ATTRIBUTES)
	condition:setParameter(CONDITION_PARAM_TICKS, 4000)
	condition:setParameter(CONDITION_PARAM_SKILL_SHIELDPERCENT, i)
	condition:setParameter(CONDITION_PARAM_SKILL_MELEEPERCENT, i)

	combat[i] = Combat()
	combat[i]:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_SMALLPLANTS)
	combat[i]:setParameter(COMBAT_PARAM_DISTANCEEFFECT, CONST_ANI_SMALLEARTH)
	combat[i]:setArea(createCombatArea(AREA_BEAM1))
	combat[i]:setCondition(condition)
end

function onCastSpell(creature, variant)
	return combat[math.random(60, 85)]:execute(creature, variant)
end
