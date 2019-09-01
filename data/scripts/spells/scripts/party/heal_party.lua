local combat = Combat()
combat:setParameter(COMBAT_PARAM_AGGRESSIVE, false)
combat:setArea(createCombatArea(AREA_CIRCLE3X3))

local condition = Condition(CONDITION_REGENERATION)
condition:setParameter(CONDITION_PARAM_TICKS, 2 * 60 * 1000)
condition:setParameter(CONDITION_PARAM_HEALTHGAIN, 20)
condition:setParameter(CONDITION_PARAM_HEALTHTICKS, 2 * 1000)
condition:setParameter(CONDITION_PARAM_BUFF_SPELL, true)

local spell = Spell("instant")

function spell.onCastSpell(creature, variant)
	return creature:addPartyCondition(combat, variant, condition, 120)
end

spell:name("Heal Party")
spell:words("utura mas sio")
spell:group("support")
spell:vocation("druid", "elder druid")
spell:id(128)
spell:cooldown(2 * 1000)
spell:groupCooldown(2 * 1000)
spell:level(32)
spell:isSelfTarget(true)
spell:isAggressive(false)
spell:isPremium(true)
spell:register()
