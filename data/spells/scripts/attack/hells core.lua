local combat = Combat()
combat:setParameter(COMBAT_PARAM_TYPE, COMBAT_FIREDAMAGE)
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_FIREAREA)

local area = createCombatArea(AREA_CROSS5X5)
combat:setArea(area)

function onGetFormulaValues(player, level, maglevel)
	min = (level / 5) + (maglevel * 8) + 50
	max = (level / 5) + (maglevel * 12) + 75
	return -min, -max
end

combat:setCallback(CALLBACK_PARAM_LEVELMAGICVALUE, "onGetFormulaValues")

function onCastSpell(creature, var)
	return combat:execute(creature, var)
end
