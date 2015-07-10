local combat = Combat()
combat:setParameter(COMBAT_PARAM_TYPE, COMBAT_EARTHDAMAGE)
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_GREEN_RINGS)
combat:setParameter(COMBAT_PARAM_DISTANCEEFFECT, CONST_ANI_POISON)
combat:setParameter(COMBAT_PARAM_CREATEITEM, ITEM_POISONFIELD_PVP)

local area = createCombatArea(AREA_WALLFIELD, AREADIAGONAL_WALLFIELD)
combat:setArea(area)

function onCastSpell(creature, var, isHotkey)
	return combat:execute(creature, var)
end
