local combat = createCombatObject()
setCombatParam(combat, COMBAT_PARAM_DISTANCEEFFECT, CONST_ANI_EARTH)
setCombatParam(combat, COMBAT_PARAM_CREATEITEM, ITEM_WILDGROWTH)

function onCastSpell(cid, var)
	return doCombat(cid, combat, var)
end