local combat = Combat()
combat:setParameter(COMBAT_PARAM_TYPE, COMBAT_HEALING)
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_MAGIC_BLUE)
combat:setParameter(COMBAT_PARAM_DISPEL, CONDITION_PARALYZE)
combat:setParameter(COMBAT_PARAM_TARGETCASTERORTOPMOST, true)
combat:setParameter(COMBAT_PARAM_AGGRESSIVE, false)

function onGetFormulaValues(player, level, magicLevel)
	local min = (level / 5) + (magicLevel * 3.2) + 20
	local max = (level / 5) + (magicLevel * 5.4) + 40
	return min, max
end

combat:setCallback(CALLBACK_PARAM_LEVELMAGICVALUE, "onGetFormulaValues")

local spell = Spell(SPELL_RUNE)

function spell.onCastSpell(creature, variant, isHotkey)
	return combat:execute(creature, variant)
end

spell:name("test rune")
spell:runeId(2275)
spell:id(220)
spell:level(20)
spell:magicLevel(5)
spell:needTarget(true)
spell:isAggressive(false)
spell:allowFarUse(true)
spell:charges(25)
spell:vocation("sorcerer;true", "master sorcerer")
spell:register()

local conjureRune = Spell(SPELL_INSTANT)

function conjureRune.onCastSpell(creature, variant)
	return creature:conjureItem(2260, 2275, 25)
end

conjureRune:name("Test")
conjureRune:id(221)
conjureRune:words("adori mas test")
conjureRune:level(30)
conjureRune:mana(530)
conjureRune:group("support")
conjureRune:soul(3)
conjureRune:isAggressive(false)
conjureRune:cooldown(2000)
conjureRune:groupCooldown(2000)
conjureRune:needLearn(false)
conjureRune:vocation("sorcerer", "master sorcerer")
conjureRune:register()
