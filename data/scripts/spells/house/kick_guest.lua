local spell = Spell(SPELL_INSTANT)

function spell.onCastSpell(creature, variant)
	local target = Player(variant:getString()) or creature
	local house = target:getTile():getHouse()
	if not house or not house:kickPlayer(creature, target) then
		creature:sendCancelMessage(RETURNVALUE_NOTPOSSIBLE)
		creature:getPosition():sendMagicEffect(CONST_ME_POFF)
		return false
	end
	return true
end

spell:name("Kick Guest")
spell:words("alana sio")
spell:isAggressive(false)
spell:hasPlayerNameParam(true)
spell:hasParams(true)
spell:register()
