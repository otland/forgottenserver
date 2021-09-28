local config = {}

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

local currencyItems = Game.getCurrencyItems()
for index, currency in pairs(currencyItems) do
	local back, to = currencyItems[index-1], currencyItems[index+1]
	local currencyId = currency:getId()
	config[currencyId] = { changeBack = back and back:getId(), changeTo = to and to:getId() }
end

goldConverter:id(26378)
goldConverter:register()
