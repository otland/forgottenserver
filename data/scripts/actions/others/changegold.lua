local config = {}
local changeGold = Action()

function changeGold.onUse(player, item, fromPosition, target, toPosition, isHotkey)
	local coin = config[item:getId()]
	if coin then
		if coin.changeTo and item.type == 100 then
			item:remove()
			player:addItem(coin.changeTo, 1)
		elseif coin.changeBack then
			item:remove(1)
			player:addItem(coin.changeBack, 100)
		end
		return true
	end
	return false
end

local moneys = Game.getMoneyItems()
for index, money in pairs(moneys) do
	local back, to = moneys[index-1], moneys[index+1]
	config[money.itemId] = { changeBack = back and back.itemId, changeTo = to and to.itemId }
	changeGold:id(money.itemId)
end
changeGold:register()
