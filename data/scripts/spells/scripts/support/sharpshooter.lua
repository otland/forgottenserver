local combat = Combat()
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_MAGIC_GREEN)
combat:setParameter(COMBAT_PARAM_AGGRESSIVE, false)

local skill = Condition(CONDITION_ATTRIBUTES)
skill:setParameter(CONDITION_PARAM_TICKS, 10 * 1000)
skill:setParameter(CONDITION_PARAM_SKILL_DISTANCEPERCENT, 150)
skill:setParameter(CONDITION_PARAM_DISABLE_DEFENSE, true)
skill:setParameter(CONDITION_PARAM_BUFF_SPELL, true)
combat:addCondition(skill)

local speed = Condition(CONDITION_PARALYZE)
speed:setParameter(CONDITION_PARAM_TICKS, 10 * 1000)
speed:setFormula(-0.7, 56, -0.7, 56)
combat:addCondition(speed)

local spell = Spell("instant")

function spell.onCastSpell(creature, variant)
	return combat:execute(creature, variant)
end

spell:name("Sharpshooter")
spell:words("utito tempo san")
spell:group("support", "healing")
spell:vocation("paladin", "royal paladin")
spell:id(135)
spell:cooldown(2 * 1000)
spell:groupCooldown(10 * 1000, 10 * 1000)
spell:level(60)
spell:mana(450)
spell:isSelfTarget(true)
spell:isAggressive(false)
spell:isPremium(true)
spell:register()
