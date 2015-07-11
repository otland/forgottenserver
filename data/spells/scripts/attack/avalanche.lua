local combat = Combat()
combat:setParameter(COMBAT_PARAM_TYPE, COMBAT_ICEDAMAGE)
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_ICEAREA)
combat:setParameter(COMBAT_PARAM_DISTANCEEFFECT, CONST_ANI_ICE)

local area = createCombatArea(AREA_CIRCLE3X3)
combat:setArea(area)

function onGetFormulaValues(player, level, maglevel)
	min = (level / 5) + (maglevel * 1.2) + 7
	max = (level / 5) + (maglevel * 2.85) + 16
	return -min, -max
end

combat:setCallback(CALLBACK_PARAM_LEVELMAGICVALUE, "onGetFormulaValues")

function onCastSpell(creature, var, isHotkey)
	return combat:execute(creature, var)
end
