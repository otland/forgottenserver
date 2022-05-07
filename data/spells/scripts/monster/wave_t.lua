local area = {
	{1, 1, 1},
	{0, 1, 0},
	{0, 3, 0}
}

local combat = Combat()
combat:setParameter(COMBAT_PARAM_TYPE, COMBAT_EARTHDAMAGE)
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_GREEN_RINGS)
combat:setArea(createCombatArea(area))

function onCastSpell(creature, variant)
	return combat:execute(creature, variant)
end
