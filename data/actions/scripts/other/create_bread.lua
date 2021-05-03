local liquidContainers = {1775, 2005, 2006, 2007, 2008, 2009, 2011, 2012, 2013, 2014, 2015, 2023, 2031, 2032, 2033}

local millstones = {1381, 1382, 1383, 1384}

function onUse(player, item, fromPosition, target, toPosition, isHotkey)
	if item:getId() == 2692 then -- flour
		if target.type == 1 and table.contains(liquidContainers, target:getId()) then
			item:remove(1)
			player:addItem(2693, 1) -- lump of dough
			target:transform(target:getId(), 0)
			return true
		end
	elseif table.contains(millstones, target:getId()) then
		item:remove(1)
		player:addItem(2692, 1) -- flour
		return true
	end
	return false
end
