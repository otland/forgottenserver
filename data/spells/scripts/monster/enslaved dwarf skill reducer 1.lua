local combat = {}

for i = 13, 50 do
	local condition = Condition(CONDITION_ATTRIBUTES)
	condition:setParameter(CONDITION_PARAM_TICKS, 6000)
	condition:setParameter(CONDITION_PARAM_SKILL_SHIELDPERCENT, i)

	combat[i] = Combat()
	combat[i]:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_MAGIC_RED)
	combat[i]:setArea(createCombatArea(AREA_CIRCLE2X2))
	combat[i]:setCondition(condition)
end

function onCastSpell(creature, variant)
	return combat[math.random(13, 50)]:execute(creature, variant)
end
