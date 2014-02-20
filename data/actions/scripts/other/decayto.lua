local itemIdArray = {[2041] = 2042, [2042] = 2041, [2044] = 2045, [2045] = 2044, [2047] = 2048, [2048] = 2047, [2050] = 2051, [2051] = 2050, [2052] = 2053, [2053] = 2051, [2054] = 2055, [2054] = 2055}
function onUse(cid, item, fromPosition, itemEx, toPosition)
	if itemIdArray[item.itemid] then
		Item(item.uid):transform(itemIdArray[item.itemid])
		Item(item.uid):decay()
		return true
	end

	return false
end
