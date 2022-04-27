area = {
	{1, 0, 1},
	{0, 2, 0},
	{1, 0, 1}
}

local combat = Combat()
combat:setParameter(COMBAT_PARAM_TYPE, COMBAT_EARTHDAMAGE)
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_GROUNDSHAKER)
combat:setArea(createCombatArea(area))

function onCastSpell(creature, var)
	return combat:execute(creature, var)
end
