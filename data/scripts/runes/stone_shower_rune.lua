local combat = Combat()
combat:setParameter(COMBAT_PARAM_TYPE, COMBAT_EARTHDAMAGE)
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_STONES)
combat:setParameter(COMBAT_PARAM_DISTANCEEFFECT, CONST_ANI_EARTH)
combat:setArea(createCombatArea(AREA_CIRCLE3X3))

function onGetFormulaValues(player, level, magicLevel)
	local min = (level / 5) + magicLevel + 6
	local max = (level / 5) + (magicLevel * 2.6) + 16
	return -min, -max
end

combat:setCallback(CALLBACK_PARAM_LEVELMAGICVALUE, "onGetFormulaValues")

local stoneShowerRune = Spell("rune")

function stoneShowerRune.onCastSpell(creature, variant, isHotkey)
	return combat:execute(creature, variant)
end

stoneShowerRune:group("attack")
stoneShowerRune:id(116)
stoneShowerRune:name("Stone Shower Rune")
stoneShowerRune:runeId(2288)
stoneShowerRune:allowFarUse(true)
stoneShowerRune:charges(4)
stoneShowerRune:level(28)
stoneShowerRune:magicLevel(4)
stoneShowerRune:cooldown(2000)
stoneShowerRune:groupCooldown(2000)
stoneShowerRune:register()
