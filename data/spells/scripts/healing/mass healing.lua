local combat = Combat()
combat:setParameter(COMBAT_PARAM_TYPE, COMBAT_HEALING)
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_MAGIC_BLUE)
combat:setParameter(COMBAT_PARAM_AGGRESSIVE, 0)
combat:setParameter(COMBAT_PARAM_DISPEL, CONDITION_PARALYZE)

local area = createCombatArea(AREA_CIRCLE3X3)
combat:setArea(area)

function onGetFormulaValues(player, level, maglevel)
	local min = (level / 5) + (maglevel * 4.6) + 100
	local max = (level / 5) + (maglevel * 9.6) + 125
	return min, max
end

combat:setCallback(CALLBACK_PARAM_LEVELMAGICVALUE, "onGetFormulaValues")

function onCastSpell(creature, var)
	return combat:execute(creature, var)
end
