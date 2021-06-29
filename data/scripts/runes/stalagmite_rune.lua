local combat = Combat()
combat:setParameter(COMBAT_PARAM_TYPE, COMBAT_EARTHDAMAGE)
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_STONES)
combat:setParameter(COMBAT_PARAM_DISTANCEEFFECT, CONST_ANI_EARTH)

function onGetFormulaValues(player, level, magicLevel)
	local min = (level / 5) + (magicLevel * 0.8) + 5
	local max = (level / 5) + (magicLevel * 1.6) + 10
	return -min, -max
end

combat:setCallback(CALLBACK_PARAM_LEVELMAGICVALUE, "onGetFormulaValues")

local stalagmiteRune = Spell("rune")

function stalagmiteRune.onCastSpell(creature, variant, isHotkey)
	return combat:execute(creature, variant)
end

stalagmiteRune:group("attack")
stalagmiteRune:id(77)
stalagmiteRune:name("Stalagmite Rune")
stalagmiteRune:runeId(2292)
stalagmiteRune:allowFarUse(true)
stalagmiteRune:charges(10)
stalagmiteRune:level(24)
stalagmiteRune:magicLevel(3)
stalagmiteRune:cooldown(2000)
stalagmiteRune:groupCooldown(2000)
stalagmiteRune:needTarget(true)
stalagmiteRune:register()
