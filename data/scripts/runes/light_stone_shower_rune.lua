local combat = Combat()
combat:setParameter(COMBAT_PARAM_TYPE, COMBAT_EARTHDAMAGE)
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_STONES)
combat:setParameter(COMBAT_PARAM_DISTANCEEFFECT, CONST_ANI_EARTH)
combat:setArea(createCombatArea(AREA_CROSS1X1))

function onGetFormulaValues(player, level, magicLevel)
	level = math.min(level, 20)
	magicLevel = math.min(magicLevel, 20)
	local min = (level / 5) + (magicLevel * 0.3) + 2
	local max = (level / 5) + (magicLevel * 0.45) + 3
	return -min, -max
end

combat:setCallback(CALLBACK_PARAM_LEVELMAGICVALUE, "onGetFormulaValues")

local lightStoneShowerRune = Spell("rune")

function lightStoneShowerRune.onCastSpell(creature, variant, isHotkey)
	return combat:execute(creature, variant)
end

lightStoneShowerRune:group("attack")
lightStoneShowerRune:id(116)
lightStoneShowerRune:name("Light Stone Shower Rune")
lightStoneShowerRune:runeId(23722)
lightStoneShowerRune:allowFarUse(true)
lightStoneShowerRune:charges(4)
lightStoneShowerRune:level(1)
lightStoneShowerRune:magicLevel(0)
lightStoneShowerRune:cooldown(2000)
lightStoneShowerRune:groupCooldown(2000)
lightStoneShowerRune:register()