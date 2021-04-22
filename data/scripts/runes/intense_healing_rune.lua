local combat = Combat()
combat:setParameter(COMBAT_PARAM_TYPE, COMBAT_HEALING)
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_MAGIC_BLUE)
combat:setParameter(COMBAT_PARAM_DISPEL, CONDITION_PARALYZE)
combat:setParameter(COMBAT_PARAM_TARGETCASTERORTOPMOST, true)
combat:setParameter(COMBAT_PARAM_AGGRESSIVE, false)

function onGetFormulaValues(player, level, magicLevel)
	local min = (level / 5) + (magicLevel * 3.2) + 20
	local max = (level / 5) + (magicLevel * 5.4) + 40
	return min, max
end

combat:setCallback(CALLBACK_PARAM_LEVELMAGICVALUE, "onGetFormulaValues")

local intenseHealingRune = Spell("rune")

function intenseHealingRune.onCastSpell(creature, variant, isHotkey)
	return combat:execute(creature, variant)
end

intenseHealingRune:group("healing")
intenseHealingRune:id(4)
intenseHealingRune:name("Intense Healing Rune")
intenseHealingRune:runeId(2265)
intenseHealingRune:allowFarUse(true)
intenseHealingRune:charges(1)
intenseHealingRune:level(15)
intenseHealingRune:magicLevel(1)
intenseHealingRune:cooldown(2000)
intenseHealingRune:groupCooldown(2000)
intenseHealingRune:needTarget(true)
intenseHealingRune:isBlocking(true)
intenseHealingRune:isAggressive(false)
intenseHealingRune:register()
