local combat = Combat()
combat:setParameter(COMBAT_PARAM_TYPE, COMBAT_PHYSICALDAMAGE)
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_HITAREA)
combat:setParameter(COMBAT_PARAM_DISTANCEEFFECT, CONST_ANI_ETHEREALSPEAR)
combat:setParameter(COMBAT_PARAM_BLOCKARMOR, 1)

function onGetFormulaValues(player, attack, factor)
	local skillTotal = 2 * player:getEffectiveSkillLevel(SKILL_DISTANCE)
	local levelTotal = player:getLevel() / 5
	return -(((skillTotal + attack / 3500) * 0.35) + (levelTotal) + 0), -(((skillTotal + attack / 3125) * 0.5) + (levelTotal) + 5)
end

combat:setCallback(CALLBACK_PARAM_SKILLVALUE, "onGetFormulaValues")

function onCastSpell(creature, var)
	return combat:execute(creature, var)
end
