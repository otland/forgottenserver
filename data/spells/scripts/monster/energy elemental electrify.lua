local combat = Combat()
combat:setParameter(COMBAT_PARAM_TYPE, COMBAT_ENERGYDAMAGE)
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_BLOCKHIT)

local condition = Condition(CONDITION_ENERGY)
condition:setParameter(CONDITION_PARAM_DELAYED, 1)
condition:addDamage(7, 10000, -25)

local area = createCombatArea(AREA_CIRCLE3X3)
combat:setArea(area)
combat:setCondition(condition)

function onCastSpell(creature, var)
	return combat:execute(creature, var)
end
