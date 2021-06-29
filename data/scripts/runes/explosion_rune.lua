local combat = Combat()
combat:setParameter(COMBAT_PARAM_TYPE, COMBAT_PHYSICALDAMAGE)
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_EXPLOSIONAREA)
combat:setParameter(COMBAT_PARAM_DISTANCEEFFECT, CONST_ANI_EXPLOSION)
combat:setParameter(COMBAT_PARAM_BLOCKARMOR, true)
combat:setArea(createCombatArea(AREA_CROSS1X1))

function onGetFormulaValues(player, level, magicLevel)
	local min = (level / 5) + (magicLevel * 1.6) + 9
	local max = (level / 5) + (magicLevel * 3.2) + 19
	return -min, -max
end

combat:setCallback(CALLBACK_PARAM_LEVELMAGICVALUE, "onGetFormulaValues")

local explosionRune = Spell("rune")

function explosionRune.onCastSpell(creature, variant, isHotkey)
	return combat:execute(creature, variant)
end

explosionRune:group("attack")
explosionRune:id(18)
explosionRune:name("Explosion Rune")
explosionRune:runeId(2313)
explosionRune:allowFarUse(true)
explosionRune:charges(6)
explosionRune:level(31)
explosionRune:magicLevel(6)
explosionRune:cooldown(2000)
explosionRune:groupCooldown(2000)
explosionRune:isBlocking(true)
explosionRune:register()
