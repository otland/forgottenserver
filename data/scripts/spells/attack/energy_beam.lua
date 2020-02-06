local combat = Combat()
combat:setParameter(COMBAT_PARAM_TYPE, COMBAT_ENERGYDAMAGE)
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_ENERGYHIT)
combat:setArea(createCombatArea(AREA_BEAM5, AREADIAGONAL_BEAM5))

function onGetFormulaValues(player, level, magicLevel)
	local min = (level / 5) + (magicLevel * 1.8) + 11
	local max = (level / 5) + (magicLevel * 3) + 19
	return -min, -max
end

combat:setCallback(CALLBACK_PARAM_LEVELMAGICVALUE, "onGetFormulaValues")

local spell = Spell("instant")

function spell.onCastSpell(creature, variant)
	return combat:execute(creature, variant)
end

spell:name("Energy Beam")
spell:words("exevo vis lux")
spell:group("attack")
spell:vocation("sorcerer", "master sorcerer")
spell:id(22)
spell:cooldown(4 * 1000)
spell:groupCooldown(2 * 1000)
spell:level(23)
spell:mana(40)
spell:needDirection(true)
spell:register()
