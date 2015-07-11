local combat = Combat()
combat:setParameter(COMBAT_PARAM_TYPE, COMBAT_ICEDAMAGE)
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_ICETORNADO)

local area = createCombatArea(AREA_CROSS5X5)
combat:setArea(area)

function onGetFormulaValues(player, level, maglevel)
	local min = (level / 5) + (maglevel * 5.5) + 25
	local max = (level / 5) + (maglevel * 11) + 50
	return -min, -max
end

combat:setCallback(CALLBACK_PARAM_LEVELMAGICVALUE, "onGetFormulaValues")

function onCastSpell(creature, var)
	return combat:execute(creature, var)
end
