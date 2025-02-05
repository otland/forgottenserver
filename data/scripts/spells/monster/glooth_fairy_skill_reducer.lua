local area = {
	{0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0}
}

local combat = {}

for i = 40, 60 do
	local condition = Condition(CONDITION_ATTRIBUTES)
	condition:setParameter(CONDITION_PARAM_TICKS, 6000)
	condition:setParameter(CONDITION_PARAM_STAT_MAGICPOINTSPERCENT, i)

	combat[i] = Combat()
	combat[i]:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_ENERGYAREA)
	combat[i]:setArea(createCombatArea(area))
	combat[i]:addCondition(condition)
end

local spell = Spell(SPELL_INSTANT)

function spell.onCastSpell(creature, variant)
	return combat[math.random(40, 60)]:execute(creature, variant)
end

spell:name("glooth fairy skill reducer")
spell:words("###67")
spell:needDirection(true)
spell:blockWalls(true)
spell:needLearn(true)
spell:register()
