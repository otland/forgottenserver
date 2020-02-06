local combat = Combat()
combat:setParameter(COMBAT_PARAM_TYPE, COMBAT_EARTHDAMAGE)
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_SMALLPLANTS)
combat:setArea(createCombatArea(AREA_SQUAREWAVE5, AREADIAGONAL_SQUAREWAVE5))

function onGetFormulaValues(player, level, magicLevel)
	local min = (level / 5) + (magicLevel * 3.25) + 5
	local max = (level / 5) + (magicLevel * 6.75) + 30
	return -min, -max
end

combat:setCallback(CALLBACK_PARAM_LEVELMAGICVALUE, "onGetFormulaValues")

local spell = Spell("instant")

function spell.onCastSpell(creature, variant)
	return combat:execute(creature, variant)
end

spell:name("Terra Wave")
spell:words("exevo tera hur")
spell:group("attack")
spell:vocation("druid", "elder druid")
spell:id(120)
spell:cooldown(4 * 1000)
spell:groupCooldown(2 * 1000)
spell:level(38)
spell:mana(210)
spell:needDirection(true)
spell:register()
