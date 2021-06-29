local combat = Combat()
combat:setParameter(COMBAT_PARAM_TYPE, COMBAT_ENERGYDAMAGE)
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_ENERGYHIT)
combat:setParameter(COMBAT_PARAM_DISTANCEEFFECT, CONST_ANI_ENERGY)

function onGetFormulaValues(player, level, magicLevel)
	local min = (level / 5) + (magicLevel * 0.4) + 3
	local max = (level / 5) + (magicLevel * 0.8) + 5
	return -min, -max
end

combat:setCallback(CALLBACK_PARAM_LEVELMAGICVALUE, "onGetFormulaValues")

local lightMagicMissileRune = Spell("rune")

function lightMagicMissileRune.onCastSpell(creature, variant, isHotkey)
	return combat:execute(creature, variant)
end

lightMagicMissileRune:group("attack")
lightMagicMissileRune:id(7)
lightMagicMissileRune:name("Light Magic Missile Rune")
lightMagicMissileRune:runeId(2287)
lightMagicMissileRune:allowFarUse(true)
lightMagicMissileRune:charges(10)
lightMagicMissileRune:level(15)
lightMagicMissileRune:magicLevel(0)
lightMagicMissileRune:cooldown(2000)
lightMagicMissileRune:groupCooldown(2000)
lightMagicMissileRune:needTarget(true)
lightMagicMissileRune:isBlocking(true)
lightMagicMissileRune:register()
