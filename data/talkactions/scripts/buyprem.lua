function onSay(player, words, param)
	if configManager.getBoolean(configKeys.FREE_PREMIUM) then
		return true
	end

	if not buyPremiumEnabled then
		return false
	end

	if player:getPremiumDays() <= buyPremiumMaxDays then
		if player:removeMoney(buyPremiumPrice) then
			player:addPremiumDays(buyPremiumDays)
			player:sendTextMessage(MESSAGE_INFO_DESCR, "You have bought " .. buyPremiumDays .." days of premium account.")
		else
			player:sendCancelMessage("You don't have enough money, " .. buyPremiumDays .. " days premium account costs " .. buyPremiumPrice .. " gold coins.")
			player:getPosition():sendMagicEffect(CONST_ME_POFF)
		end
	else
		player:sendCancelMessage("You can not buy more than " .. buyPremiumMaxDays .. " days of premium account.")
		player:getPosition():sendMagicEffect(CONST_ME_POFF)
	end
	return false
end
