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

local spell = Spell(SPELL_INSTANT)

function spell.onCastSpell(creature, variant)
	return combat:execute(creature, variant)
end

spell:group("attack")
spell:id(120)
spell:name("Terra Wave")
spell:words("exevo tera hur")
spell:level(38)
spell:mana(170)
spell:needDirection(true)
spell:cooldown(4000)
spell:groupCooldown(2000)
spell:vocation("druid;true", "elder druid;true")
spell:register()
