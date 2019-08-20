local combat = Combat()
combat:setParameter(COMBAT_PARAM_TYPE, COMBAT_ENERGYDAMAGE)
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_ENERGYAREA)
combat:setArea(createCombatArea(AREA_BEAM8))

function onGetFormulaValues(player, level, magicLevel)
	local min = (level / 5) + (magicLevel * 3.6) + 22
	local max = (level / 5) + (magicLevel * 6) + 37
	return -min, -max
end

combat:setCallback(CALLBACK_PARAM_LEVELMAGICVALUE, "onGetFormulaValues")

local spell = Spell("instant")

function spell.onCastSpell(creature, variant)
	return combat:execute(creature, variant)
end

spell:name("Great Energy Beam")
spell:words("exevo gran vis lux")
spell:group("attack")
spell:vocation("sorcerer", "master sorcerer")
spell:id(23)
spell:cooldown(6 * 1000)
spell:groupCooldown(2 * 1000)
spell:level(29)
spell:mana(110)
spell:needDirection(true)
spell:register()
