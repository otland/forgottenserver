local combat = Combat()
combat:setParameter(COMBAT_PARAM_TYPE, COMBAT_HOLYDAMAGE)
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_HOLYAREA)
combat:setParameter(COMBAT_PARAM_DISTANCEEFFECT, CONST_ANI_HOLY)

function onGetFormulaValues(player, level, magicLevel)
	local min = (level / 5) + (magicLevel * 1.8) + 11
	local max = (level / 5) + (magicLevel * 3.8) + 23
	return -min, -max
end

combat:setCallback(CALLBACK_PARAM_LEVELMAGICVALUE, "onGetFormulaValues")

local spell = Spell(SPELL_RUNE)

function spell.onCastSpell(creature, variant, isHotkey)
	return combat:execute(creature, variant)
end

spell:group("attack")
spell:id(130)
spell:name("Holy Missile Rune")
spell:runeId(2295)
spell:allowFarUse(true)
spell:charges(5)
spell:runeLevel(27)
spell:runeMagicLevel(4)
spell:cooldown(2000)
spell:groupCooldown(2000)
spell:needTarget(true)
spell:isBlocking(true, false)
spell:vocation("paladin;true", "royal paladin")
spell:register()
