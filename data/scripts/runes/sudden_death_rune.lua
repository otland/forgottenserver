local combat = Combat()
combat:setParameter(COMBAT_PARAM_TYPE, COMBAT_DEATHDAMAGE)
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_MORTAREA)
combat:setParameter(COMBAT_PARAM_DISTANCEEFFECT, CONST_ANI_SUDDENDEATH)

function onGetFormulaValues(player, level, magicLevel)
	local min = (level / 5) + (magicLevel * 4.3) + 32
	local max = (level / 5) + (magicLevel * 7.4) + 48
	return -min, -max
end

combat:setCallback(CALLBACK_PARAM_LEVELMAGICVALUE, "onGetFormulaValues")

local suddenDeathRune = Spell("rune")

function suddenDeathRune.onCastSpell(creature, variant, isHotkey)
	return combat:execute(creature, variant)
end

suddenDeathRune:group("attack")
suddenDeathRune:id(21)
suddenDeathRune:name("Sudden Death Rune")
suddenDeathRune:runeId(2268)
suddenDeathRune:allowFarUse(true)
suddenDeathRune:charges(3)
suddenDeathRune:level(45)
suddenDeathRune:magicLevel(15)
suddenDeathRune:cooldown(2000)
suddenDeathRune:groupCooldown(2000)
suddenDeathRune:needTarget(true)
suddenDeathRune:isBlocking(true)
suddenDeathRune:register()
