local combat = Combat()
combat:setParameter(COMBAT_PARAM_TYPE, COMBAT_HEALING)
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_MAGIC_BLUE)
combat:setParameter(COMBAT_PARAM_DISPEL, CONDITION_PARALYZE)
combat:setParameter(COMBAT_PARAM_AGGRESSIVE, false)

function onGetFormulaValues(player, level, magicLevel)
	local min = (level / 5) + (magicLevel * 6.9) + 40
	local max = (level / 5) + (magicLevel * 13.2) + 82
	return min, max
end

combat:setCallback(CALLBACK_PARAM_LEVELMAGICVALUE, "onGetFormulaValues")

local spell = Spell("instant")

function spell.onCastSpell(creature, variant)
	return combat:execute(creature, variant)
end

spell:name("Divine Healing")
spell:words("exura san")
spell:group("healing")
spell:vocation("paladin", "royal paladin")
spell:id(125)
spell:cooldown(1 * 1000)
spell:groupCooldown(1 * 1000)
spell:level(35)
spell:mana(160)
spell:isSelfTarget(true)
spell:isAggressive(false)
spell:register()
