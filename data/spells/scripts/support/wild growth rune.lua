local combat = Combat()
combat:setParameter(COMBAT_PARAM_DISTANCEEFFECT, CONST_ANI_EARTH)
combat:setParameter(COMBAT_PARAM_CREATEITEM, ITEM_WILDGROWTH)

function onCastSpell(creature, var)
	return combat:execute(creature, var)
end
