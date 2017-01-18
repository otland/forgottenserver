function onCastSpell(player, variant)
	local targetPlayer = Player(variant:getString()) or player
	local house = targetPlayer:getTile():getHouse()
	if not house or not house:kickPlayer(player, targetPlayer) then
		player:sendCancelMessage(RETURNVALUE_NOTPOSSIBLE)
		player:getPosition():sendMagicEffect(CONST_ME_POFF)
		return false
	end
	return true
end
