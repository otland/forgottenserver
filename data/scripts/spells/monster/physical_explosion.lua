local condition = Condition(COMBAT_PHYSICALDAMAGE)
condition:setParameter(CONDITION_PARAM_DELAYED, 1)
condition:addDamage(3, 10000, -25)

local combat = Combat()
combat:setParameter(COMBAT_PARAM_TYPE, COMBAT_PHYSICALDAMAGE)
combat:setParameter(COMBAT_PARAM_EFFECT, 6)
combat:setArea(createCombatArea(AREA_SQUARE1X1))
combat:addCondition(condition)

local spell = Spell(SPELL_INSTANT)

function spell.onCastSpell(creature, variant)
	return combat:execute(creature, variant)
end

spell:name("physical explosion")
spell:words("###68")
spell:isAggressive(false)
spell:blockWalls(true)
spell:needLearn(true)
spell:register()
