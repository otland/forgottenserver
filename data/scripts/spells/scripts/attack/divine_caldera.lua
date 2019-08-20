local combat = Combat()
combat:setParameter(COMBAT_PARAM_TYPE, COMBAT_HOLYDAMAGE)
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_HOLYAREA)
combat:setArea(createCombatArea(AREA_CIRCLE3X3))

function onGetFormulaValues(player, level, magicLevel)
	local min = (level / 5) + (magicLevel * 5) + 25
	local max = (level / 5) + (magicLevel * 6.2) + 45
	return -min, -max
end

combat:setCallback(CALLBACK_PARAM_LEVELMAGICVALUE, "onGetFormulaValues")

local spell = Spell("instant")

function spell.onCastSpell(creature, variant)
	return combat:execute(creature, variant)
end

spell:name("Divine Caldera")
spell:words("exevo mas san")
spell:group("attack")
spell:vocation("paladin", "royal paladin")
spell:id(124)
spell:cooldown(4 * 1000)
spell:groupCooldown(2 * 1000)
spell:level(50)
spell:mana(160)
spell:isSelfTarget(true)
spell:isPremium(true)
spell:register()
