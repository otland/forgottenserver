function onSay(player, words, param)
	local changePrice = configManager.getNumber(configKeys.CHANGESEX_COST)
	if changePrice == -1 then
		return true
	end

	if player:getGroup():getAccess() then
		player:setSex(player:getSex() == PLAYERSEX_FEMALE and PLAYERSEX_MALE or PLAYERSEX_FEMALE)
		player:sendTextMessage(MESSAGE_INFO_DESCR, "You have changed your sex.")
		return false
	end

	if player:getPremiumDays() >= changePrice then
		player:removePremiumDays(changePrice)
		player:setSex(player:getSex() == PLAYERSEX_FEMALE and PLAYERSEX_MALE or PLAYERSEX_FEMALE)
		player:sendTextMessage(MESSAGE_INFO_DESCR, "You have changed your sex for " .. changePrice .. " days of your premium account.")
	else
		player:sendCancelMessage("You do not have enough premium days, changing sex costs " .. changePrice .. " days of your premium account.")
		player:getPosition():sendMagicEffect(CONST_ME_POFF)
	end
	return false
end
