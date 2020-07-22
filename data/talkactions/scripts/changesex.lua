function onSay(player, words, param)
	if not configManager.getBoolean(configKeys.CHANGE_SEX_ENABLED) then
		return false
	end

	if player:getGroup():getAccess() then
		player:setSex(player:getSex() == PLAYERSEX_FEMALE and PLAYERSEX_MALE or PLAYERSEX_FEMALE)
		player:sendTextMessage(MESSAGE_INFO_DESCR, "You have changed your sex.")
		return false
	end

	local changeSexDaysCost = configManager.getNumber(configKeys.CHANGE_SEX_DAYS_COST)
	if player:getPremiumDays() >= changeSexDaysCost then
		player:removePremiumDays(changeSexDaysCost)
		player:setSex(player:getSex() == PLAYERSEX_FEMALE and PLAYERSEX_MALE or PLAYERSEX_FEMALE)
		player:sendTextMessage(MESSAGE_INFO_DESCR, "You have changed your sex for " .. changeSexDaysCost .. " days of your premium account.")
	else
		player:sendCancelMessage("You do not have enough premium days, changing sex costs " .. changeSexDaysCost .. " days of your premium account.")
		player:getPosition():sendMagicEffect(CONST_ME_POFF)
	end
	return false
end
