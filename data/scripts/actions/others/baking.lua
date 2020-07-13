local liquidContainers = {1775, 2005, 2006, 2007, 2008, 2009, 2011, 2012, 2013, 2014, 2015, 2023, 2031, 2032, 2033}
local millstones = {1381, 1382, 1383, 1384}

local baking = Action()

function baking.onUse(player, item, fromPosition, target, toPosition, isHotkey)
	if item.itemid == 2692 and table.contains(liquidContainers, target.itemid) then
		if target.type == 1 then -- water
			item:transform(item.itemid, item.type - 1)
			player:addItem(2693, 1) -- lump of dough
			target:transform(target.itemid, 0)
		elseif target.type == 6 then -- milk
			item:transform(item.itemid, item.type - 1)
			player:addItem(6277, 1) -- lump of cake dough
			target:transform(target.itemid, 0)
		end
	elseif item.itemid == 2692 and target.itemid == 7494 then -- special flask (holy water)
		item:remove(1)
		target:remove(1)
		player:addItem(9112, 1) -- lump of holy water dough
	elseif item.itemid == 6277 and target.itemid == 6574 then -- bar of chocolate
		item:transform(8846) -- lump of chocolate dough
		target:remove(1)
	elseif item.itemid == 9112 and target.itemid == 9114 then -- bulb of garlic
		item:transform(9113) -- lump of garlic dough
		target:remove(1)
	elseif item.itemid == 2694 and table.contains(millstones, target.itemid) then
		item:transform(item.itemid, item.type - 1)
		player:addItem(2692, 1) -- flour
	else
		return false
	end

	return true
end

baking:id(6277, 9112, 2692, 2694)
baking:register()
