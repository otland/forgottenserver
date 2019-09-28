local combat = Combat()
combat:setParameter(COMBAT_PARAM_TYPE, COMBAT_ICEDAMAGE)
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_ICEAREA)
combat:setArea(createCombatArea(AREA_WAVE3))

function onGetFormulaValues(player, level, magicLevel)
	local min = (level / 5) + (magicLevel * 4.5) + 20
	local max = (level / 5) + (magicLevel * 7.6) + 48
	return -min, -max
end

combat:setCallback(CALLBACK_PARAM_LEVELMAGICVALUE, "onGetFormulaValues")

local spell = Spell("instant")

function spell.onCastSpell(creature, variant)
	return combat:execute(creature, variant)
end

spell:name("Strong Ice Wave")
spell:words("exevo gran frigo hur")
spell:group("attack")
spell:vocation("druid", "elder druid")
spell:id(43)
spell:cooldown(8 * 1000)
spell:groupCooldown(2 * 1000)
spell:level(40)
spell:mana(170)
spell:needDirection(true)
spell:isPremium(true)
spell:register()
