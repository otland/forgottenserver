local combat = Combat()
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_MAGIC_GREEN)
combat:setParameter(COMBAT_PARAM_AGGRESSIVE, false)

local condition = Condition(CONDITION_ATTRIBUTES)
condition:setParameter(CONDITION_PARAM_TICKS, 10 * 1000)
condition:setParameter(CONDITION_PARAM_SKILL_MELEEPERCENT, 135)
condition:setParameter(CONDITION_PARAM_DISABLE_DEFENSE, true)
condition:setParameter(CONDITION_PARAM_BUFF_SPELL, true)
combat:addCondition(condition)

local spell = Spell("instant")

function spell.onCastSpell(creature, variant)
	return combat:execute(creature, variant)
end

spell:name("Blood Rage")
spell:words("utito tempo")
spell:group("support")
spell:vocation("knight", "elite knight")
spell:id(133)
spell:cooldown(2 * 1000)
spell:groupCooldown(2 * 1000)
spell:level(60)
spell:mana(290)
spell:isSelfTarget(true)
spell:isAggressive(false)
spell:isPremium(true)
spell:register()

