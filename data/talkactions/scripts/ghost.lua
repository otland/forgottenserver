function onSay(player, words, param)
	if not player:getGroup():getAccess() then
		return true
	end

	if player:getAccountType() < ACCOUNT_TYPE_GOD then
		return false
	end

	local position = player:getPosition()
	local isGhost = not player:isInGhostMode()

	player:setGhostMode(isGhost)

	if not isGhost then
		player:sendTextMessage(MESSAGE_INFO_DESCR, "You are visible again.")
		position.x = position.x + 1
		position:sendMagicEffect(CONST_ME_SMOKE)
		return false
	end

	player:sendTextMessage(MESSAGE_INFO_DESCR, "You are now invisible.")
	position:sendMagicEffect(CONST_ME_YALAHARIGHOST)
	return false
end
