local combat = Combat()
combat:setParameter(COMBAT_PARAM_TYPE, COMBAT_EARTHDAMAGE)
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_SMALLPLANTS)
combat:setArea(createCombatArea(AREA_CIRCLE6X6))

function onGetFormulaValues(player, level, magicLevel)
	local min = (level / 5) + (magicLevel * 3) + 32
	local max = (level / 5) + (magicLevel * 9) + 40
	return -min, -max
end

combat:setCallback(CALLBACK_PARAM_LEVELMAGICVALUE, "onGetFormulaValues")

local spell = Spell("instant")

function spell.onCastSpell(creature, variant)
	return combat:execute(creature, variant)
end

spell:name("Wrath of Nature")
spell:words("exevo gran mas tera")
spell:group("attack")
spell:vocation("druid", "elder druid")
spell:id(56)
spell:cooldown(40 * 1000)
spell:groupCooldown(4 * 1000)
spell:level(55)
spell:mana(700)
spell:isSelfTarget(true)
spell:isPremium(true)
spell:register()
