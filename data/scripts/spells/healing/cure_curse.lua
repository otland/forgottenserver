local combat = Combat()
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_MAGIC_BLUE)
combat:setParameter(COMBAT_PARAM_DISPEL, CONDITION_CURSED)
combat:setParameter(COMBAT_PARAM_AGGRESSIVE, false)

local spell = Spell("instant")

function spell.onCastSpell(creature, variant)
	return combat:execute(creature, variant)
end

spell:name("Cure Curse")
spell:words("exana mort")
spell:group("healing")
spell:vocation("paladin", "royal paladin")
spell:id(147)
spell:cooldown(6 * 1000)
spell:groupCooldown(1 * 1000)
spell:level(80)
spell:mana(40)
spell:isSelfTarget(true)
spell:isAggressive(false)
spell:isPremium(true)
spell:register()
