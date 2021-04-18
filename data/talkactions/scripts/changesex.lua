local premiumDaysCost = 3

function onSay(player, words, param)
	if player:getGroup():getAccess() then
		player:setSex(player:getSex() == PLAYERSEX_FEMALE and PLAYERSEX_MALE or PLAYERSEX_FEMALE)
		player:sendTextMessage(MESSAGE_INFO_DESCR, "You have changed your sex.")
		return false
	end

	if player:getPremiumDays() >= premiumDaysCost then
		player:removePremiumDays(premiumDaysCost)
		player:setSex(player:getSex() == PLAYERSEX_FEMALE and PLAYERSEX_MALE or PLAYERSEX_FEMALE)
		player:sendTextMessage(MESSAGE_INFO_DESCR, "You have changed your sex for " .. premiumDaysCost .. " days of your premium account.")
	else
		player:sendCancelMessage("You do not have enough premium days, changing sex costs " .. premiumDaysCost .. " days of your premium account.")
		player:getPosition():sendMagicEffect(CONST_ME_POFF)
	end
	return false
end
