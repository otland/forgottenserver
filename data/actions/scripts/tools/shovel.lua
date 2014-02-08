local holes = {468, 481, 483}
function onUse(cid, item, fromPosition, itemEx, toPosition)
	if(isInArray(holes, itemEx.itemid)) then
		Item(itemEx.uid):transform(itemEx.itemid + 1)
		Item(itemEx.uid):decay()
	elseif(itemEx.itemid == 231 or 9059) then
		local rand = math.random(1, 100)
		if(rand == 1) then
			doCreateItem(2159, 1, toPosition)
		elseif(rand > 95) then
			doSummonCreature("Scarab", toPosition)
		end
		Position(toPosition):sendMagicEffect(CONST_ME_POFF)
	else
		return false
	end
return true
end
