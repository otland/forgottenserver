local combat = Combat()
combat:setParameter(COMBAT_PARAM_TYPE, COMBAT_HEALING)
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_MAGIC_BLUE)
combat:setParameter(COMBAT_PARAM_DISPEL, CONDITION_PARALYZE)
combat:setParameter(COMBAT_PARAM_TARGETCASTERORTOPMOST, true)
combat:setParameter(COMBAT_PARAM_AGGRESSIVE, false)

function onGetFormulaValues(player, level, magicLevel)
	local min = (level / 5) + (magicLevel * 7.3) + 42
	local max = (level / 5) + (magicLevel * 12.4) + 90
	return min, max
end

combat:setCallback(CALLBACK_PARAM_LEVELMAGICVALUE, "onGetFormulaValues")

local spell = Spell("rune")

function spell.onCastSpell(creature, variant, isHotkey)
	return combat:execute(creature, variant)
end

spell:name("Ultimate Healing Rune")
spell:group("healing")
spell:id(5)
spell:cooldown(2 * 1000)
spell:groupCooldown(2 * 1000)
spell:level(24)
spell:magicLevel(4)
spell:runeId(2273)
spell:charges(1)
spell:needTarget(true)
spell:isBlocking(true)
spell:isAggressive(false)
spell:allowFarUse(true)
spell:register()
