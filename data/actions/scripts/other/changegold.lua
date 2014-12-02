function onUse(player, item, fromPosition, targetEx, toPosition, isHotkey)
	if item:getId() == ITEM_GOLD_COIN and item:getSubType() == ITEMCOUNT_MAX then
		item:remove()
		player:addItem(ITEM_PLATINUM_COIN, 1)
	elseif item:getId() == ITEM_PLATINUM_COIN and item:getSubType() == ITEMCOUNT_MAX then
		item:remove()
		player:addItem(ITEM_CRYSTAL_COIN, 1)
	elseif item:getId() == ITEM_PLATINUM_COIN and item:getSubType() < ITEMCOUNT_MAX then
		item:transform(item:getId(), item:getSubType() - 1)
		player:addItem(ITEM_GOLD_COIN, ITEMCOUNT_MAX)
	elseif item:getId() == ITEM_CRYSTAL_COIN then
		item:transform(item:getId(), item:getSubType() - 1)
		player:addItem(ITEM_PLATINUM_COIN, ITEMCOUNT_MAX)
	else
		return false
	end
	return true
end
