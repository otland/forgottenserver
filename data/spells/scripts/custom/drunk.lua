local condition = Condition(CONDITION_DRUNK)
condition:setParameter(CONDITION_PARAM_TICKS, 20000)

local combat = Combat()
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_MAGIC_RED)
combat:addCondition(condition)
combat:setArea(createCombatArea(AREA_SQUARE1X1))

function onCastSpell(creature, variant, isHotkey)
	return combat:execute(creature, variant)
end
