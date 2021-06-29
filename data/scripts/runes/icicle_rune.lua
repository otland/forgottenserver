local combat = Combat()
combat:setParameter(COMBAT_PARAM_TYPE, COMBAT_ICEDAMAGE)
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_ICEATTACK)
combat:setParameter(COMBAT_PARAM_DISTANCEEFFECT, CONST_ANI_SMALLICE)

function onGetFormulaValues(player, level, magicLevel)
	local min = (level / 5) + (magicLevel * 1.8) + 12
	local max = (level / 5) + (magicLevel * 3) + 17
	return -min, -max
end

combat:setCallback(CALLBACK_PARAM_LEVELMAGICVALUE, "onGetFormulaValues")

local icicleRune = Spell("rune")

function icicleRune.onCastSpell(creature, variant, isHotkey)
	return combat:execute(creature, variant)
end

icicleRune:group("attack")
icicleRune:id(114)
icicleRune:name("Icicle Rune")
icicleRune:runeId(2271)
icicleRune:allowFarUse(true)
icicleRune:charges(5)
icicleRune:level(28)
icicleRune:magicLevel(4)
icicleRune:cooldown(2000)
icicleRune:groupCooldown(2000)
icicleRune:needTarget(true)
icicleRune:register()
