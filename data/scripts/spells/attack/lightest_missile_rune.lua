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

local spell = Spell("rune")

function spell.onCastSpell(creature, variant, isHotkey)
	return combat:execute(creature, variant)
end

spell:name("Lightest Missile Rune")
spell:group("attack")
spell:vocation("druid", "elder druid", "paladin", "royal paladin", "sorcerer", "master sorcerer")
spell:id(7)
spell:cooldown(2 * 1000)
spell:groupCooldown(2 * 1000)
spell:level(1)
spell:magicLevel(0)
spell:runeId(23723)
spell:charges(10)
spell:needTarget(true)
spell:isBlocking(true)
spell:allowFarUse(true)
spell:register()
