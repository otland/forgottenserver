local combat = Combat()
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_ICEAREA)
combat:setParameter(COMBAT_PARAM_DISTANCEEFFECT, CONST_ANI_ICE)
combat:setArea(createCombatArea(AREA_BEAM1))

local parameters = {
	{key = CONDITION_PARAM_TICKS, value = 5 * 1000},
	{key = CONDITION_PARAM_SKILL_SHIELDPERCENT, value = 50},
	{key = CONDITION_PARAM_SKILL_MELEEPERCENT, value = 50}
}

local spell = Spell(SPELL_INSTANT)

function spell.onCastSpell(creature, variant)
	for _, target in ipairs(combat:getTargets(creature, variant)) do
		target:addAttributeCondition(parameters)
	end
	return true
end

spell:name("warlock skill reducer")
spell:words("###30")
spell:blockWalls(true)
spell:needTarget(true)
spell:needLearn(true)
spell:register()
