local combat = Combat()
combat:setParameter(COMBAT_PARAM_TYPE, COMBAT_HEALING)
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_MAGIC_GREEN)
combat:setParameter(COMBAT_PARAM_DISPEL, CONDITION_PARALYZE)
combat:setParameter(COMBAT_PARAM_AGGRESSIVE, false)

function onGetFormulaValues(player, level, magicLevel)
	local min = ((level / 5) + (magicLevel * 6.3) + 45) * 2
	local max = ((level / 5) + (magicLevel * 14.4) + 90) * 2
	return min, max
end

combat:setCallback(CALLBACK_PARAM_LEVELMAGICVALUE, "onGetFormulaValues")

local spell = Spell(SPELL_INSTANT)

function spell.onCastSpell(creature, variant)
	if creature:isPlayer() and variant:getNumber() == creature:getId() then
		creature:sendCancelMessage("You can't cast this spell on yourself.")
		creature:getPosition():sendMagicEffect(CONST_ME_POFF)
		return false
	end
	creature:getPosition():sendMagicEffect(CONST_ME_MAGIC_BLUE)
	return combat:execute(creature, variant)
end

spell:group("healing")
spell:id(242)
spell:name("Nature's Embrace")
spell:words("exura gran sio")
spell:level(300)
spell:mana(400)
spell:isAggressive(false)
spell:blockWalls(true)
spell:needTarget(true)
spell:hasPlayerNameParam(true)
spell:hasParams(true)
spell:cooldown(60000)
spell:groupCooldown(1000)
spell:vocation("druid;true", "elder druid;true")
spell:register()
