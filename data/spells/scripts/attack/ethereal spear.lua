local combat = Combat()
combat:setParameter(COMBAT_PARAM_TYPE, COMBAT_PHYSICALDAMAGE)
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_HITAREA)
combat:setParameter(COMBAT_PARAM_DISTANCEEFFECT, CONST_ANI_ETHEREALSPEAR)
combat:setParameter(COMBAT_PARAM_BLOCKSHIELD, 1)

function onGetFormulaValues(player, skill, attack, factor)
	local skillTotal = 2 * player:getEffectiveSkillLevel(SKILL_DISTANCE)
	local levelTotal = player:getLevel() / 5
	local min = levelTotal + (skillTotal * 0.35)
	local max = levelTotal + (skillTotal * 0.5) + 5
	return -min, -max
end

combat:setCallback(CALLBACK_PARAM_SKILLVALUE, "onGetFormulaValues")

function onCastSpell(creature, var)
	return combat:execute(creature, var)
end
