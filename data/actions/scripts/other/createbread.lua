local liquidContainers = {1775, 2005, 2006, 2007, 2008, 2009, 2011, 2012, 2013, 2014, 2015, 2023, 2031, 2032, 2033}
local millstones = {1381, 1382, 1383, 1384}
function onUse(cid, item, fromPosition, itemEx, toPosition)

local player = Player(cid)
local itemx = Item(item.uid)
local itemex = Item(itemEx.uid)

    if item.itemid == 2692 then
		if isInArray(liquidContainers, itemEx.itemid) and itemEx.type == 1 then
			itemx:remove(1)
			player:addItem(2693, 1)
			itemex:transform(itemEx.itemid, 0)
		end
    elseif item.itemid == 2694 then
		if isInArray(millstones, itemEx.itemid) then
			player:addItem(2692, 1)
			itemx:remove(1)
		end
	else
		return false
    end
    return true
end
