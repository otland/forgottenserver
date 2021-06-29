local combat = Combat()
combat:setParameter(COMBAT_PARAM_TYPE, COMBAT_ENERGYDAMAGE)
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_ENERGYAREA)
combat:setParameter(COMBAT_PARAM_DISTANCEEFFECT, CONST_ANI_ENERGY)

function onGetFormulaValues(player, level, magicLevel)
	return -3, -7
end

combat:setCallback(CALLBACK_PARAM_LEVELMAGICVALUE, "onGetFormulaValues")

local lightestMagicMissileRune = Spell("rune")

function lightestMagicMissileRune.onCastSpell(creature, variant, isHotkey)
	return combat:execute(creature, variant)
end

lightestMagicMissileRune:group("attack")
lightestMagicMissileRune:id(7)
lightestMagicMissileRune:name("Lightest Magic Missile Rune")
lightestMagicMissileRune:runeId(19392)
lightestMagicMissileRune:allowFarUse(true)
lightestMagicMissileRune:charges(10)
lightestMagicMissileRune:level(1)
lightestMagicMissileRune:magicLevel(0)
lightestMagicMissileRune:cooldown(2000)
lightestMagicMissileRune:groupCooldown(2000)
lightestMagicMissileRune:needTarget(true)
lightestMagicMissileRune:isBlocking(true)
lightestMagicMissileRune:register()
