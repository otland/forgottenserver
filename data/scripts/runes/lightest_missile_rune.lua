local combat = Combat()
combat:setParameter(COMBAT_PARAM_TYPE, COMBAT_ENERGYDAMAGE)
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_ENERGYHIT)
combat:setParameter(COMBAT_PARAM_DISTANCEEFFECT, CONST_ANI_ENERGY)

function onGetFormulaValues(player, level, magicLevel)
	level = math.min(level, 20)
	magicLevel = math.min(magicLevel, 20)
	local min = (level / 5) + (magicLevel * 0.45) + 3
	local max = (level / 5) + (magicLevel * 0.7) + 4
	return -min, -max
end

combat:setCallback(CALLBACK_PARAM_LEVELMAGICVALUE, "onGetFormulaValues")

local lightestMissileRune = Spell("rune")

function lightestMissileRune.onCastSpell(creature, variant, isHotkey)
	return combat:execute(creature, variant)
end

lightestMissileRune:group("attack")
lightestMissileRune:id(7)
lightestMissileRune:name("Lightest Missile Rune")
lightestMissileRune:runeId(23723)
lightestMissileRune:allowFarUse(true)
lightestMissileRune:charges(10)
lightestMissileRune:level(1)
lightestMissileRune:magicLevel(0)
lightestMissileRune:cooldown(2000)
lightestMissileRune:groupCooldown(2000)
lightestMissileRune:needTarget(true)
lightestMissileRune:isBlocking(true)
lightestMissileRune:register()
