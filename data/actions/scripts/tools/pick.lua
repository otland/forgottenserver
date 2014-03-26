function onUse(cid, item, fromPosition, itemEx, toPosition)
	if (itemEx.uid <= 65535 or itemEx.actionid > 0) and (itemEx.itemid == 354 or itemEx.itemid == 355) then
		local iEx = Item(itemEx.uid)
		iEx:transform(392)
		iEx:decay()
		toPosition:sendMagicEffect(CONST_ME_POFF)
		return true
	end

	return false
end
