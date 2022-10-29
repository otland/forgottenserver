local juiceSquizer = Action()

function juiceSquizer.onUse(player, item, fromPosition, target, toPosition, isHotkey)
	local fruits = {
		2673, -- pear
		2674, -- red apple
		2675, -- orange
		2676, -- banana
		2677, -- blueberry
		2678, -- coconut
		2679, -- cherry
		2680, -- strawberry
		2681, -- grapes
		2682, -- melon
		5097, -- mango
		8839, -- plum
		8840, -- raspberry
		8841 -- lemon
	}
	if table.contains(fruits, target.itemid) and player:removeItem(2006, 1, 0) then
		target:remove(1)
		player:addItem(2006, target.itemid == 2678 and 14 or 21) -- if target is a coconut, create coconut milk, otherwise create fruit juice
		return true
	end
end

juiceSquizer:id(5865)
juiceSquizer:register()
