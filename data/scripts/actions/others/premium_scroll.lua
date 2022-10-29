local premiumScroll = Action()

function premiumScroll.onUse(player, item, fromPosition, target, toPosition, isHotkey)
	item:remove(1)
	player:addPremiumDays(30)
	player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "You have activated your 30 day premium time, relog to make it effective.")
	player:getPosition():sendMagicEffect(CONST_ME_TELEPORT)
	return true
end

premiumScroll:id(16101)
premiumScroll:register()
