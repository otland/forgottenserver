local combat = Combat()
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_MAGIC_RED)

local condition = Condition(CONDITION_PARALYZE)
condition:setParameter(CONDITION_PARAM_TICKS, 20000)
condition:setFormula(-1, 80, -1, 80)
combat:addCondition(condition)

local paralyzeRune = Spell("rune")

function paralyzeRune.onCastSpell(creature, variant, isHotkey)
	if not combat:execute(creature, variant) then
		return false
	end

	creature:getPosition():sendMagicEffect(CONST_ME_MAGIC_GREEN)
	return true
end

paralyzeRune:group("attack")
paralyzeRune:id(54)
paralyzeRune:name("Paralyze Rune")
paralyzeRune:runeId(2278)
paralyzeRune:allowFarUse(true)
paralyzeRune:charges(1)
paralyzeRune:level(54)
paralyzeRune:magicLevel(18)
paralyzeRune:cooldown(2000)
paralyzeRune:groupCooldown(2000)
paralyzeRune:needTarget(true)
paralyzeRune:isBlocking(true)
paralyzeRune:mana(1400)
paralyzeRune:isPzLock(true)
paralyzeRune:vocation("Druid;true", "Elder Druid")
paralyzeRune:register()
