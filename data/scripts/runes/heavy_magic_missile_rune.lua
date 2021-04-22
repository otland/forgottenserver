local combat = Combat()
combat:setParameter(COMBAT_PARAM_TYPE, COMBAT_ENERGYDAMAGE)
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_ENERGYHIT)
combat:setParameter(COMBAT_PARAM_DISTANCEEFFECT, CONST_ANI_ENERGY)

function onGetFormulaValues(player, level, magicLevel)
	local min = (level / 5) + (magicLevel * 0.8) + 5
	local max = (level / 5) + (magicLevel * 1.6) + 10
	return -min, -max
end

combat:setCallback(CALLBACK_PARAM_LEVELMAGICVALUE, "onGetFormulaValues")

local heavyMagicMissileRune = Spell("rune")

function heavyMagicMissileRune.onCastSpell(creature, variant, isHotkey)
	return combat:execute(creature, variant)
end

heavyMagicMissileRune:group("attack")
heavyMagicMissileRune:id(8)
heavyMagicMissileRune:name("Heavy Magic Missile Rune")
heavyMagicMissileRune:runeId(2311)
heavyMagicMissileRune:allowFarUse(true)
heavyMagicMissileRune:charges(10)
heavyMagicMissileRune:level(25)
heavyMagicMissileRune:magicLevel(3)
heavyMagicMissileRune:cooldown(2000)
heavyMagicMissileRune:groupCooldown(2000)
heavyMagicMissileRune:needTarget(true)
heavyMagicMissileRune:isBlocking(true)
heavyMagicMissileRune:register()
