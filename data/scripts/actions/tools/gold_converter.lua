local config = {}
local moneys = Game.getMoneyItems()
for index, money in pairs(moneys) do
	local back, to = moneys[index-1], moneys[index+1]
	config[money.itemId] = { changeBack = back and back.itemId, changeTo = to and to.itemId }
end

local goldConverter = Action()

function goldConverter.onUse(player, item, fromPosition, target, toPosition, isHotkey)
	local coin = config[target.itemid]
	if coin then
		local charges = item:getCharges()
		if coin.changeTo and target.type == 100 then
			target:remove()
			player:addItem(coin.changeTo, 1)
			item:transform(item:getId(), charges -1)
		elseif coin.changeBack then
			target:transform(target.itemid, target.type - 1)
			player:addItem(coin.changeBack, 100)
			item:transform(item:getId(), charges -1)
		end
		if charges == 0 then
			item:remove()
		end
		return true
	end

	return false
end

goldConverter:id(26378)
goldConverter:register()
