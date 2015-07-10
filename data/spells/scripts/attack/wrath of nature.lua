local combat = Combat()
combat:setParameter(COMBAT_PARAM_TYPE, COMBAT_EARTHDAMAGE)
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_PLANTATTACK)

local area = createCombatArea(AREA_CROSS6X6)
combat:setArea(area)

function onGetFormulaValues(player, level, maglevel)
	min = (level / 5) + (maglevel * 3) + 32
	max = (level / 5) + (maglevel * 9) + 40
	return -min, -max
end

combat:setCallback(CALLBACK_PARAM_LEVELMAGICVALUE, "onGetFormulaValues")

function onCastSpell(creature, var)
	return combat:execute(creature, var)
end
