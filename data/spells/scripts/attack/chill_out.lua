local combat = Combat()
combat:setParameter(COMBAT_PARAM_TYPE, COMBAT_ICEDAMAGE)
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_ICEAREA)
combat:setArea(createCombatArea(AREA_WAVE4))

function onGetFormulaValues(player, level, maglevel)
	level = math.min(level, 20)
	maglevel = math.min(maglevel, 20)
	local min = (level / 5) + (maglevel * 0.3) + 2
	local max = (level / 5) + (maglevel * 0.45) + 3
	return -min, -max
end

combat:setCallback(CALLBACK_PARAM_LEVELMAGICVALUE, "onGetFormulaValues")

function onCastSpell(creature, variant)
	return combat:execute(creature, variant)
end
