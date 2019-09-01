local combat = Combat()
combat:setParameter(COMBAT_PARAM_AGGRESSIVE, false)
combat:setArea(createCombatArea(AREA_CIRCLE3X3))

local condition = Condition(CONDITION_ATTRIBUTES)
condition:setParameter(CONDITION_PARAM_TICKS, 2 * 60 * 1000)
condition:setParameter(CONDITION_PARAM_SKILL_SHIELD, 3)
condition:setParameter(CONDITION_PARAM_BUFF_SPELL, true)

local spell = Spell("instant")

function spell.onCastSpell(creature, variant)
	return creature:addPartyCondition(combat, variant, condition, 90)
end

spell:name("Protect Party")
spell:words("utamo mas sio")
spell:group("support")
spell:vocation("paladin", "royal paladin")
spell:id(127)
spell:cooldown(2 * 1000)
spell:groupCooldown(2 * 1000)
spell:level(32)
spell:isSelfTarget(true)
spell:isAggressive(false)
spell:isPremium(true)
spell:register()
