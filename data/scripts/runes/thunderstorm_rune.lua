local combat = Combat()
combat:setParameter(COMBAT_PARAM_TYPE, COMBAT_ENERGYDAMAGE)
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_ENERGYHIT)
combat:setParameter(COMBAT_PARAM_DISTANCEEFFECT, CONST_ANI_ENERGYBALL)
combat:setArea(createCombatArea(AREA_CIRCLE3X3))

function onGetFormulaValues(player, level, magicLevel)
	local min = (level / 5) + magicLevel + 6
	local max = (level / 5) + (magicLevel * 2.6) + 16
	return -min, -max
end

combat:setCallback(CALLBACK_PARAM_LEVELMAGICVALUE, "onGetFormulaValues")

local thunderstormRune = Spell("rune")

function thunderstormRune.onCastSpell(creature, variant, isHotkey)
	return combat:execute(creature, variant)
end

thunderstormRune:group("attack")
thunderstormRune:id(117)
thunderstormRune:name("Thunderstorm Rune")
thunderstormRune:runeId(2315)
thunderstormRune:allowFarUse(true)
thunderstormRune:charges(4)
thunderstormRune:level(28)
thunderstormRune:magicLevel(4)
thunderstormRune:cooldown(2000)
thunderstormRune:groupCooldown(2000)
thunderstormRune:register()
