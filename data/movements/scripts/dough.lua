function onAddItem(moveitem, tileitem, position)

	if moveItem.itemid == 2693 then
		Item(moveItem.uid):transform(2689)
		position:sendMagicEffect(CONST_ME_HITBYFIRE)
	elseif moveItem.itemid == 6277 then
		Item(moveItem.uid):transform(2687, 12)
		position:sendMagicEffect(CONST_ME_HITBYFIRE)
	end

	return true
end
