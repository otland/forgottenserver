local config = {
	[ITEM_GOLD_COIN] = {changeTo = ITEM_PLATINUM_COIN},
	[ITEM_PLATINUM_COIN] = {changeBack = ITEM_GOLD_COIN, changeTo = ITEM_CRYSTAL_COIN},
	[ITEM_CRYSTAL_COIN] = {changeBack = ITEM_PLATINUM_COIN}
}

local goldConverter = Action()

function goldConverter.onUse(player, item, fromPosition, target, toPosition, isHotkey)
	local coin = config[target.itemid]

	if not coin then
		return false
	end

	local charges = item:getCharges()
	if coin.changeTo and target.type == 100 then
		target:remove()
		player:addItem(coin.changeTo, 1)
		item:transform(item:getId(), charges -1)
	elseif coin.changeBack then
		target:transform(target.itemid, target.type - 1)
		player:addItem(coin.changeBack, 100)
		item:transform(item:getId(), charges -1)
	else
		return false
	end

	if charges == 0 then
		item:remove()
	end
	return true
end

goldConverter:id(26378)
goldConverter:register()
