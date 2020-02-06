local combat = Combat()
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_MAGIC_BLUE)
combat:setParameter(COMBAT_PARAM_DISPEL, CONDITION_POISON)
combat:setParameter(COMBAT_PARAM_AGGRESSIVE, false)

local spell = Spell("instant")

function spell.onCastSpell(creature, variant)
	return combat:execute(creature, variant)
end

spell:name("Cure Poison")
spell:words("exana pox")
spell:group("healing")
spell:vocation("druid", "elder druid", "knight", "elite knight", "paladin", "royal paladin", "sorcerer", "master sorcerer")
spell:id(29)
spell:cooldown(6 * 1000)
spell:groupCooldown(1 * 1000)
spell:level(10)
spell:mana(30)
spell:isSelfTarget(true)
spell:isAggressive(false)
spell:register()
