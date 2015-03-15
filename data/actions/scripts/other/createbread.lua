local liquidContainers = {1775, 2005, 2006, 2007, 2008, 2009, 2011, 2012, 2013, 2014, 2015, 2023, 2031, 2032, 2033}
local millstones = {1381, 1382, 1383, 1384}

function onUse(player, item, fromPosition, target, toPosition, isHotkey)
	if item.itemid == 2692 and target.type == 1 and isInArray(liquidContainers, target.itemid) then
		Item(item.uid):transform(item.itemid, item.type - 1)
		player:addItem(2693, 1)
		Item(target.uid):transform(target.itemid, 0)
	elseif isInArray(millstones, target.itemid) then
		Item(item.uid):transform(item.itemid, item.type - 1)
		player:addItem(2692, 1)
	else
		return false
	end
	return true
end
