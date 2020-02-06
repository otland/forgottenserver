local combat = Combat()
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_MAGIC_BLUE)
combat:setParameter(COMBAT_PARAM_AGGRESSIVE, false)

local condition = Condition(CONDITION_LIGHT)
condition:setParameter(CONDITION_PARAM_LIGHT_LEVEL, 8)
condition:setParameter(CONDITION_PARAM_LIGHT_COLOR, 215)
condition:setParameter(CONDITION_PARAM_TICKS, (11 * 60 + 35) * 1000)
combat:addCondition(condition)

local spell = Spell("instant")

function spell.onCastSpell(creature, variant)
	return combat:execute(creature, variant)
end

spell:name("Great Light")
spell:words("utevo gran lux")
spell:group("support")
spell:vocation("druid", "elder druid", "knight", "elite knight", "paladin", "royal paladin", "sorcerer", "master sorcerer")
spell:id(11)
spell:cooldown(2 * 1000)
spell:groupCooldown(2 * 1000)
spell:level(13)
spell:mana(60)
spell:isSelfTarget(true)
spell:isAggressive(false)
spell:register()
