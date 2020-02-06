local combat = Combat()
combat:setParameter(COMBAT_PARAM_TYPE, COMBAT_HEALING)
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_MAGIC_BLUE)
combat:setParameter(COMBAT_PARAM_DISPEL, CONDITION_PARALYZE)
combat:setParameter(COMBAT_PARAM_AGGRESSIVE, false)

function onGetFormulaValues(player, level, magicLevel)
	level = math.min(level, 20)
	magicLevel = math.min(magicLevel, 20)
	local min = (level / 5) + (magicLevel * 1.4) + 8
	local max = (level / 5) + (magicLevel * 1.8) + 11
	return min, max
end

combat:setCallback(CALLBACK_PARAM_LEVELMAGICVALUE, "onGetFormulaValues")

local spell = Spell("instant")

function onCastSpell(creature, variant)
	return combat:execute(creature, variant)
end

spell:name("Bruise Bane")
spell:words("exura infir ico")
spell:group("healing")
spell:vocation("knight", "elite knight")
spell:id(175)
spell:cooldown(1 * 1000)
spell:groupCooldown(1 * 1000)
spell:level(1)
spell:mana(10)
spell:isSelfTarget(true)
spell:isAggressive(false)
spell:register()
