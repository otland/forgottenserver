local combat = Combat()
combat:setParameter(COMBAT_PARAM_TYPE, COMBAT_ICEDAMAGE)
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_ICEAREA)
combat:setArea(createCombatArea(AREA_WAVE4))

function onGetFormulaValues(player, level, magicLevel)
	level = math.min(level, 20)
	magicLevel = math.min(magicLevel, 20)
	local min = (level / 5) + (magicLevel * 0.3) + 2
	local max = (level / 5) + (magicLevel * 0.45) + 3
	return -min, -max
end

combat:setCallback(CALLBACK_PARAM_LEVELMAGICVALUE, "onGetFormulaValues")

local spell = Spell("instant")

function spell.onCastSpell(creature, variant)
	return combat:execute(creature, variant)
end

spell:name("Chill Out")
spell:words("exevo infir frigo hur")
spell:group("attack")
spell:vocation("druid", "elder druid")
spell:id(173)
spell:cooldown(4 * 1000)
spell:groupCooldown(2 * 1000)
spell:level(1)
spell:mana(8)
spell:needDirection(true)
spell:register()
