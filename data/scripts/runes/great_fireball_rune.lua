local combat = Combat()
combat:setParameter(COMBAT_PARAM_TYPE, COMBAT_FIREDAMAGE)
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_FIREAREA)
combat:setParameter(COMBAT_PARAM_DISTANCEEFFECT, CONST_ANI_FIRE)
combat:setArea(createCombatArea(AREA_CIRCLE3X3))

function onGetFormulaValues(player, level, magicLevel)
	local min = (level / 5) + (magicLevel * 1.2) + 7
	local max = (level / 5) + (magicLevel * 2.85) + 16
	return -min, -max
end

combat:setCallback(CALLBACK_PARAM_LEVELMAGICVALUE, "onGetFormulaValues")

local greatFireballRune = Spell("rune")

function greatFireballRune.onCastSpell(creature, variant, isHotkey)
	return combat:execute(creature, variant)
end

greatFireballRune:group("attack")
greatFireballRune:id(16)
greatFireballRune:name("Great Fireball Rune")
greatFireballRune:runeId(2304)
greatFireballRune:allowFarUse(true)
greatFireballRune:charges(4)
greatFireballRune:level(30)
greatFireballRune:magicLevel(4)
greatFireballRune:cooldown(2000)
greatFireballRune:groupCooldown(2000)
greatFireballRune:isBlocking(true)
greatFireballRune:register()
