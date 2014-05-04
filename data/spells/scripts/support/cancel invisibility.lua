local combat = Combat()
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_MAGIC_BLUE)
combat:setParameter(COMBAT_PARAM_DISPEL, CONDITION_INVISIBLE)

local area = createCombatArea(AREA_CIRCLE3X3)
combat:setArea(area)

function onCastSpell(creature, var)
	return combat:execute(creature, var)
end
