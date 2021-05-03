local fruits = {2673, 2674, 2675, 2676, 2677, 2678, 2679, 2680, 2681, 2682, 5097, 8839, 8840, 8841}

local juiceSquizer = Action()

function juiceSquizer.onUse(player, item, fromPosition, target, toPosition, isHotkey)
	if table.contains(fruits, target:getId()) and player:removeItem(2006, 1, 0) then -- 2006 is vial
		target:remove(1)
		player:addItem(2006, target:getId() == 2678 and 14 or 21) -- if target is a coconut, create coconut milk, otherwise create fruit juice
		return true
	end
end

juiceSquizer:id(5865) -- juice squeezer
juiceSquizer:register()
