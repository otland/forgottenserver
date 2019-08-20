local combat = Combat()
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_MAGIC_RED)

local condition = Condition(CONDITION_PARALYZE)
condition:setParameter(CONDITION_PARAM_TICKS, 20 * 1000)
condition:setFormula(-1, 80, -1, 80)
combat:addCondition(condition)

local spell = Spell("rune")

function spell.onCastSpell(creature, variant, isHotkey)
	if not combat:execute(creature, variant) then
		return false
	end

	creature:getPosition():sendMagicEffect(CONST_ME_MAGIC_GREEN)
	return true
end

spell:name("Paralyze Rune")
spell:group("attack")
spell:vocation("druid;true", "elder druid")
spell:id(54)
spell:cooldown(2 * 1000)
spell:groupCooldown(2 * 1000)
spell:level(54)
spell:mana(1400)
spell:magicLevel(18)
spell:runeId(2278)
spell:charges(1)
spell:needTarget(true)
spell:isBlocking(true)
spell:allowFarUse(true)
spell:register()
