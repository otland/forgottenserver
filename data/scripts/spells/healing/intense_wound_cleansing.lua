local combat = Combat()
combat:setParameter(COMBAT_PARAM_TYPE, COMBAT_HEALING)
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_MAGIC_BLUE)
combat:setParameter(COMBAT_PARAM_DISPEL, CONDITION_PARALYZE)
combat:setParameter(COMBAT_PARAM_AGGRESSIVE, false)

function onGetFormulaValues(player, level, magicLevel)
	local min = (level / 5) + (magicLevel * 70) + 438
	local max = (level / 5) + (magicLevel * 92) + 544
	return min, max
end

combat:setCallback(CALLBACK_PARAM_LEVELMAGICVALUE, "onGetFormulaValues")

local spell = Spell(SPELL_INSTANT)

function spell.onCastSpell(creature, variant)
	return combat:execute(creature, variant)
end

spell:group("healing")
spell:id(158)
spell:name("Intense Wound Cleansing")
spell:words("exura gran ico")
spell:level(80)
spell:mana(200)
spell:isSelfTarget(true)
spell:isAggressive(false)
spell:cooldown(600000)
spell:groupCooldown(1000)
spell:vocation("knight;true", "elite knight;true")
spell:register()
