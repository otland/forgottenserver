function onCastSpell(creature, variant)
	local target = Player(variant:getString()) or creature
	local house = target:getTile():getHouse()
	if not house or not house:kickPlayer(creature, target) then
		creature:sendCancelMessage(RETURNVALUE_NOTPOSSIBLE)
		creature:getPosition():sendMagicEffect(CONST_ME_POFF)
		return false
	end
	return true
end
