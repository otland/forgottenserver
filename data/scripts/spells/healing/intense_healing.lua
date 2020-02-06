local combat = Combat()
combat:setParameter(COMBAT_PARAM_TYPE, COMBAT_HEALING)
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_MAGIC_BLUE)
combat:setParameter(COMBAT_PARAM_DISPEL, CONDITION_PARALYZE)
combat:setParameter(COMBAT_PARAM_AGGRESSIVE, false)

function onGetFormulaValues(player, level, magicLevel)
	local min = (level / 5) + (magicLevel * 3.2) + 20
	local max = (level / 5) + (magicLevel * 5.4) + 40
	return min, max
end

combat:setCallback(CALLBACK_PARAM_LEVELMAGICVALUE, "onGetFormulaValues")

local spell = Spell("instant")

function spell.onCastSpell(creature, variant)
	return combat:execute(creature, variant)
end

spell:name("Intense Healing")
spell:words("exura gran")
spell:group("healing")
spell:vocation("druid", "elder druid", "paladin", "royal paladin", "sorcerer", "master sorcerer")
spell:id(2)
spell:cooldown(1 * 1000)
spell:groupCooldown(1 * 1000)
spell:level(20)
spell:mana(70)
spell:isSelfTarget(true)
spell:isAggressive(false)
spell:register()
