function onCastSpell(player, variant)
	local targetPlayer = Player(variant:getString())
	if not targetPlayer then
		targetPlayer = player
	end

	local house = targetPlayer:getTile():getHouse()
	if not house or not house:kickPlayer(player, targetPlayer) then
		player:sendTextMessage(MESSAGE_STATUS_SMALL, "Sorry, not possible.")
		player:getPosition():sendMagicEffect(CONST_ME_POFF)
		return false
	end
	return true
end
