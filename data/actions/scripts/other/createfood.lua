local liquidContainers = {1775, 2005, 2006, 2007, 2008, 2009, 2011, 2012, 2013, 2014, 2015, 2023, 2031, 2032, 2033}
local millstones = {1381, 1382, 1383, 1384}
local chimney = {8684, 8685, 8686, 8687}

function onUse(player, item, fromPosition, target, toPosition, isHotkey)
	local itemId = item:getId()
	if itemId == 2692 then
		if target.type == 1 and isInArray(liquidContainers, target.itemid) then
			item:remove(1)
			player:addItem(2693, 1)
			target:transform(target.itemid, 0)
			return true
		elseif target.type == 6 and isInArray(liquidContainers, target.itemid) then
			item:remove(1)
			player:addItem(6277, 1)
			target:transform(target.itemid, 0)
			return true
		end
	elseif isInArray(millstones, target.itemid) then
		item:remove(1)
		player:addItem(2692, 1)
		return true
	elseif item:getId() == 8848 and isInArray(chimney, target.itemid) then
		item:remove(1)
		player:addItem(2687, 12)
		return true
	elseif item.itemid == 6277 and target.itemid == 6574 then
		item:remove(1)
		target:remove(1)
		player:addItem(8846, 1)
		return true
	elseif item.itemid == 6277 and target.itemid == 2561 then
		item:remove(1)
		target:remove(1)
		player:addItem(8848, 1)
		return true
	end
	return false
end
