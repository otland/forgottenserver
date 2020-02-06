local combat = Combat()
combat:setParameter(COMBAT_PARAM_TYPE, COMBAT_ICEDAMAGE)
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_ICETORNADO)
combat:setArea(createCombatArea(AREA_CIRCLE5X5))

function onGetFormulaValues(player, level, magicLevel)
	local min = (level / 5) + (magicLevel * 5.5) + 25
	local max = (level / 5) + (magicLevel * 11) + 50
	return -min, -max
end

combat:setCallback(CALLBACK_PARAM_LEVELMAGICVALUE, "onGetFormulaValues")

local spell = Spell("instant")

function spell.onCastSpell(creature, variant)
	return combat:execute(creature, variant)
end

spell:name("Eternal Winter")
spell:words("exevo gran mas frigo")
spell:group("attack")
spell:vocation("druid", "elder druid")
spell:id(118)
spell:cooldown(40 * 1000)
spell:groupCooldown(4 * 1000)
spell:level(60)
spell:mana(1050)
spell:isSelfTarget(true)
spell:isPremium(true)
spell:register()
