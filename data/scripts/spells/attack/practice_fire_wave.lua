local combat = Combat()
combat:setParameter(COMBAT_PARAM_TYPE, COMBAT_FIREDAMAGE)
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_HITBYFIRE)
combat:setArea(createCombatArea(AREA_WAVE4, AREADIAGONAL_WAVE4))

function onGetFormulaValues(player, level, magicLevel)
	return -11, -14
end

combat:setCallback(CALLBACK_PARAM_LEVELMAGICVALUE, "onGetFormulaValues")

local spell = Spell("instant")

function spell.onCastSpell(creature, variant)
	return combat:execute(creature, variant)
end
spell:name("Practice Fire Wave")
spell:words("exevo dis flam hur")
spell:group("attack")
spell:vocation("none")
spell:id(167)
spell:cooldown(4 * 1000)
spell:groupCooldown(2 * 1000)
spell:level(1)
spell:mana(5)
spell:needDirection(true)
spell:register()
