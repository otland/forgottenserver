local combat = Combat()
combat:setParameter(COMBAT_PARAM_TYPE, COMBAT_PHYSICALDAMAGE)
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_GROUNDSHAKER)
combat:setParameter(COMBAT_PARAM_BLOCKARMOR, 1)
combat:setParameter(COMBAT_PARAM_USECHARGES, 1)

local area = createCombatArea(AREA_CIRCLE3X3)
combat:setArea(area)

function onGetFormulaValues(player, level, skill, attack, factor)
	local physPower = skill * attack
	local levelPower = level / 5
	local min = levelPower + (physPower * 0.02) + 4
	local max = levelPower + (physPower * 0.03) + 6
	return -min, -max
end

combat:setCallback(CALLBACK_PARAM_SKILLVALUE, "onGetFormulaValues")

function onCastSpell(creature, var)
	return combat:execute(creature, var)
end
