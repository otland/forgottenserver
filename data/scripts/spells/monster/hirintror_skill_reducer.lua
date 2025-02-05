local combat = {}

for i = 20, 40 do
	combat[i] = Combat()
	combat[i]:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_HITAREA)

	local condition = Condition(CONDITION_ATTRIBUTES)
	condition:setParameter(CONDITION_PARAM_TICKS, 15000)
	condition:setParameter(CONDITION_PARAM_SKILL_MELEEPERCENT, i)
	condition:setParameter(CONDITION_PARAM_SKILL_FISTPERCENT, i)
	condition:setParameter(CONDITION_PARAM_SKILL_DISTANCEPERCENT, i)
	condition:setParameter(CONDITION_PARAM_SKILL_SHIELDPERCENT, i)

	local area = createCombatArea(AREA_SQUARE1X1)
	combat[i]:setArea(area)
	combat[i]:addCondition(condition)
end

local spell = Spell(SPELL_INSTANT)

function spell.onCastSpell(creature, variant)
	return combat[math.random(20, 40)]:execute(creature, variant)
end

spell:name("hirintror skill reducer")
spell:words("###63")
spell:blockWalls(true)
spell:needLearn(true)
spell:register()
