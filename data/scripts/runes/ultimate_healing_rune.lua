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

local ultimateHealingRune = Spell("rune")

function ultimateHealingRune.onCastSpell(creature, variant, isHotkey)
	return combat:execute(creature, variant)
end

ultimateHealingRune:group("healing")
ultimateHealingRune:id(5)
ultimateHealingRune:name("Ultimate Healing Rune")
ultimateHealingRune:runeId(2273)
ultimateHealingRune:allowFarUse(true)
ultimateHealingRune:charges(1)
ultimateHealingRune:level(24)
ultimateHealingRune:magicLevel(4)
ultimateHealingRune:cooldown(2000)
ultimateHealingRune:groupCooldown(2000)
ultimateHealingRune:needTarget(true)
ultimateHealingRune:isBlocking(true)
ultimateHealingRune:isAggressive(false)
ultimateHealingRune:register()