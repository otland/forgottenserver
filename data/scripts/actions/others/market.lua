local market = Action()

function market.onUse(player, item, fromPosition, target, toPosition, isHotkey)
	player:sendEnterMarket()
	return true
end

market:id(ITEM_MARKET)
market:register()
