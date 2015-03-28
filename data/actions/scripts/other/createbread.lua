local liquidContainers = {1775, 2005, 2006, 2007, 2008, 2009, 2011, 2012, 2013, 2014, 2015, 2023, 2031, 2032, 2033}
local millstones = {1381, 1382, 1383, 1384}

function onUse(player, item, fromPosition, target, toPosition, isHotkey)
	local itemId = item:getId()
	if itemId == 2692 then
		if target.type == 1 and isInArray(liquidContainers, target.itemid) then
			item:remove(1)
			player:addItem(2693, 1)
			target:transform(target.itemid, 0)
			return true
		end
	elseif isInArray(millstones, target.itemid) then
		item:remove(1)
		player:addItem(2692, 1)
		return true
	end
	return false
end
