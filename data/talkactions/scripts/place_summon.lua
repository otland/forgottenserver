function onSay(player, words, param)
	if not player:getGroup():getAccess() then
		return true
	end

	if player:getAccountType() < ACCOUNT_TYPE_GOD then
		return false
	end

	local position = player:getPosition()
	local monster = Game.createMonster(param, position)
	if not monster then
		player:sendCancelMessage("There is not enough room.")
		position:sendMagicEffect(CONST_ME_POFF)
		return false
	end

	player:addSummon(monster)
	position:sendMagicEffect(CONST_ME_MAGIC_RED)
	return false
end
