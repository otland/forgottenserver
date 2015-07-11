local combat = Combat()
combat:setParameter(COMBAT_PARAM_TYPE, COMBAT_ENERGYDAMAGE)
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_ENERGYHIT)

local area = createCombatArea(AREA_BEAM5, AREADIAGONAL_BEAM5)
combat:setArea(area)

function onGetFormulaValues(player, level, maglevel)
	min = (level / 5) + (maglevel * 1.8) + 11
	max = (level / 5) + (maglevel * 3) + 19
	return -min, -max
end

combat:setCallback(CALLBACK_PARAM_LEVELMAGICVALUE, "onGetFormulaValues")

function onCastSpell(creature, var)
	return combat:execute(creature, var)
end
