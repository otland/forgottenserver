local area = {{1, 1, 1}, {1, 1, 1}, {0, 1, 0}, {0, 3, 0}}

local combat = Combat()
combat:setParameter(COMBAT_PARAM_TYPE, COMBAT_PHYSICALDAMAGE)
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_EXPLOSIONHIT)
combat:setArea(createCombatArea(area))

local spell = Spell(SPELL_INSTANT)

function spell.onCastSpell(creature, variant)
	return combat:execute(creature, variant)
end

spell:name("explosion wave")
spell:words("###54")
spell:needDirection(true)
spell:blockWalls(true)
spell:needLearn(true)
spell:register()
