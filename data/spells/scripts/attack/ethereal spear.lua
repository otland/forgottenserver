local combat = Combat()
combat:setParameter(COMBAT_PARAM_TYPE, COMBAT_PHYSICALDAMAGE)
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_HITAREA)
combat:setParameter(COMBAT_PARAM_DISTANCEEFFECT, CONST_ANI_ETHEREALSPEAR)
combat:setParameter(COMBAT_PARAM_BLOCKARMOR, 1)

function onGetFormulaValues(player, level, skill, attack, factor)
	local skillDouble = 2 * player:getEffectiveSkillLevel(SKILL_DISTANCE)
	local levelPower = level / 5
	local min = levelPower + ((skillDouble + (attack / 3500)) * 0.35)
	local max = levelPower + ((skillDouble + (attack / 3125)) * 0.5) + 5
	return -min, -max
end

combat:setCallback(CALLBACK_PARAM_SKILLVALUE, "onGetFormulaValues")

function onCastSpell(creature, var)
	return combat:execute(creature, var)
end
