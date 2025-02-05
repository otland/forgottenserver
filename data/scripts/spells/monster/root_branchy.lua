local combat = Combat()
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_ROOTING)
combat:setParameter(COMBAT_PARAM_DISTANCEEFFECT, CONST_ANI_LEAFSTAR)

local condition = Condition(CONDITION_ROOT)
condition:setParameter(CONDITION_PARAM_TICKS, 3000)
combat:addCondition(condition)

local spell = Spell(SPELL_INSTANT)

function spell.onCastSpell(creature, variant)
	return combat:execute(creature, variant)
end

spell:name("root branchy")
spell:words("###53")
spell:blockWalls(true)
spell:needTarget(true)
spell:needLearn(true)
spell:register()
