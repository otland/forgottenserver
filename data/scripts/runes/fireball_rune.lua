local combat = Combat()
combat:setParameter(COMBAT_PARAM_TYPE, COMBAT_FIREDAMAGE)
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_FIREATTACK)
combat:setParameter(COMBAT_PARAM_DISTANCEEFFECT, CONST_ANI_FIRE)

function onGetFormulaValues(player, level, magicLevel)
	local min = (level / 5) + (magicLevel * 1.8) + 12
	local max = (level / 5) + (magicLevel * 3) + 17
	return -min, -max
end

combat:setCallback(CALLBACK_PARAM_LEVELMAGICVALUE, "onGetFormulaValues")

local fireballRune = Spell("rune")

function fireballRune.onCastSpell(creature, variant, isHotkey)
	return combat:execute(creature, variant)
end

fireballRune:group("attack")
fireballRune:id(15)
fireballRune:name("Fireball Rune")
fireballRune:runeId(2302)
fireballRune:allowFarUse(true)
fireballRune:charges(5)
fireballRune:level(27)
fireballRune:magicLevel(4)
fireballRune:cooldown(2000)
fireballRune:groupCooldown(2000)
fireballRune:needTarget(true)
fireballRune:isBlocking(true)
fireballRune:register()
