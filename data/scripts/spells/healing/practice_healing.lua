local combat = Combat()
combat:setParameter(COMBAT_PARAM_TYPE, COMBAT_HEALING)
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_MAGIC_BLUE)
combat:setParameter(COMBAT_PARAM_DISPEL, CONDITION_PARALYZE)
combat:setParameter(COMBAT_PARAM_AGGRESSIVE, false)
combat:setFormula(COMBAT_FORMULA_DAMAGE, -5, 0, -9, 0)

function onGetFormulaValues(player, level, magicLevel)
	return 5, 9
end

combat:setCallback(CALLBACK_PARAM_LEVELMAGICVALUE, "onGetFormulaValues")

local spell = Spell("instant")

function spell.onCastSpell(creature, variant)
	return combat:execute(creature, variant)
end

spell:name("Practice Healing")
spell:words("exura dis")
spell:group("healing")
spell:vocation("none")
spell:id(166)
spell:cooldown(1 * 1000)
spell:groupCooldown(1 * 1000)
spell:level(1)
spell:mana(5)
spell:isSelfTarget(true)
spell:isAggressive(false)
spell:register()
