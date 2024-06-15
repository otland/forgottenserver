local combat = Combat()
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_MAGIC_GREEN)
combat:setParameter(COMBAT_PARAM_AGGRESSIVE, false)

local condition = Condition(CONDITION_REGENERATION)
condition:setParameter(CONDITION_PARAM_TICKS, 1 * 60 * 1000)
condition:setParameter(CONDITION_PARAM_HEALTHGAIN, 40)
condition:setParameter(CONDITION_PARAM_HEALTHTICKS, 3000)
condition:setParameter(CONDITION_PARAM_BUFF_SPELL, true)
combat:addCondition(condition)

local spell = Spell(SPELL_INSTANT)

function spell.onCastSpell(creature, variant)
	return combat:execute(creature, variant)
end

spell:group("healing")
spell:id(160)
spell:name("Intense Recovery")
spell:words("utura gran")
spell:level(100)
spell:mana(165)
spell:isAggressive(false)
spell:isSelfTarget(true)
spell:cooldown(60000)
spell:groupCooldown(1000)
spell:vocation("paladin;true", "knight;true", "royal paladin;true", "elite knight;true")
spell:register()
