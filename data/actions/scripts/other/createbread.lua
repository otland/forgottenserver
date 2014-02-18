local liquidContainers = {1775, 2005, 2006, 2007, 2008, 2009, 2011, 2012, 2013, 2014, 2015, 2023, 2031, 2032, 2033}
local millstones = {1381, 1382, 1383, 1384}
function onUse(cid, item, fromPosition, itemEx, toPosition)
	if item.itemid == 2692 and isInArray(liquidContainers, itemEx.itemid) == TRUE and itemEx.type == 1 then
		doChangeTypeItem(item.uid, item.type - 1)
		doPlayerAddItem(cid, 2693, 1)
		doChangeTypeItem(itemEx.uid, item.type - item.type)
	elseif isInArray(millstones, itemEx.itemid) == TRUE then
		doChangeTypeItem(item.uid, item.type - 1)
		doPlayerAddItem(cid, 2692, 1)
	else
		return false
	end
	return true
end
