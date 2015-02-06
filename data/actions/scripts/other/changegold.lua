local config = {
	[ITEM_GOLD_COIN] = {changeTo = ITEM_PLATINUM_COIN},
	[ITEM_PLATINUM_COIN] = {changeBack = ITEM_GOLD_COIN, changeTo = ITEM_CRYSTAL_COIN},
	[ITEM_CRYSTAL_COIN] = {changeBack = ITEM_PLATINUM_COIN}
}

function onUse(player, item, fromPosition, itemEx, toPosition, isHotkey)
	local coin = config[item.itemid]
	if not coin then
		return false
	end

	if coin.changeTo and item.type == ITEMCOUNT_MAX then
		item:remove()
		player:addItem(coin.changeTo, 1)
	elseif coin.changeBack then
		item:transform(item.itemid, item.type - 1)
		player:addItem(coin.changeBack, ITEMCOUNT_MAX)
	else
		return false
	end
	return true
end
