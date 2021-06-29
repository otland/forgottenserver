local combat = Combat()
combat:setParameter(COMBAT_PARAM_TYPE, COMBAT_HOLYDAMAGE)
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_HOLYAREA)
combat:setParameter(COMBAT_PARAM_DISTANCEEFFECT, CONST_ANI_HOLY)

function onGetFormulaValues(player, level, magicLevel)
	local min = (level / 5) + (magicLevel * 1.8) + 11
	local max = (level / 5) + (magicLevel * 3.8) + 23
	return -min, -max
end

combat:setCallback(CALLBACK_PARAM_LEVELMAGICVALUE, "onGetFormulaValues")

local holyMissileRune = Spell("rune")

function holyMissileRune.onCastSpell(creature, variant, isHotkey)
	return combat:execute(creature, variant)
end

holyMissileRune:group("attack")
holyMissileRune:id(130)
holyMissileRune:name("Holy Missile Rune")
holyMissileRune:runeId(2295)
holyMissileRune:allowFarUse(true)
holyMissileRune:charges(5)
holyMissileRune:level(27)
holyMissileRune:magicLevel(4)
holyMissileRune:cooldown(2000)
holyMissileRune:groupCooldown(2000)
holyMissileRune:needTarget(true)
holyMissileRune:isBlocking(true)
holyMissileRune:vocation("Paladin;true", "Royal Paladin")
holyMissileRune:register()
