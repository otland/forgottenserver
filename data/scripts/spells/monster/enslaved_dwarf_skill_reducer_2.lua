local combat = Combat()
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_HITAREA)
combat:setArea(createCombatArea(AREA_CROSS1X1))

local parameters = {{key = CONDITION_PARAM_TICKS, value = 6 * 1000}, {key = CONDITION_PARAM_SKILL_MELEEPERCENT, value = 45}}

local spell = Spell(SPELL_INSTANT)

function spell.onCastSpell(creature, variant)
	for _, target in ipairs(combat:getTargets(creature, variant)) do
		target:addAttributeCondition(parameters)
	end
	return true
end

spell:name("enslaved dwarf skill reducer 2")
spell:words("###46")
spell:blockWalls(true)
spell:needLearn(true)
spell:register()
