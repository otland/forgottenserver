local bluePresent = {2687, 6394, 6280, 6574, 6578, 6575, 6577, 6569, 6576, 6572, 2114}
local redPresent = {2152, 2152, 2152, 2153, 5944, 2112, 6568, 6566, 2492, 2520, 2195, 2114, 2114, 2114, 6394, 6394, 6576, 6576, 6578, 6578, 6574, 6574}

function onUse(cid, item, fromPosition, itemEx, toPosition)
	local count = 1
	if item.itemid == 6570 then
		local randomChance = math.random(1, 11)
		if randomChance == 1 then
			count = 10
		elseif randomChance == 2 then
			count = 3
		end
		doPlayerAddItem(cid, bluePresent[randomChance], count)
	elseif item.itemid == 6571 then
		local randomChance = math.random(1, 22)
		if randomChance > 0 and randomChance < 4 then
			count = 10
		end
		doPlayerAddItem(cid, redPresent[randomChance], count)
	end
	doSendMagicEffect(fromPosition, CONST_ME_GIFT_WRAPS)
	doRemoveItem(item.uid, 1)
	return TRUE
end