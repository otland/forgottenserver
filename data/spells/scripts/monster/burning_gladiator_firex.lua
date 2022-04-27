area = {
	{1, 0, 1},
	{0, 2, 0},
	{1, 0, 1}
}

local combat = Combat()
combat:setParameter(COMBAT_PARAM_TYPE, COMBAT_FIREDAMAGE)
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_FIREATTACK)
combat:setArea(createCombatArea(area))

function onCastSpell(creature, var)
	return combat:execute(creature, var)
end
