function onAddItem(moveitem, tileitem, position)

	if moveitem.itemid == 2693 then
		Item(moveitem.uid):transform(2689)
		position:sendMagicEffect(CONST_ME_HITBYFIRE)
	elseif moveitem.itemid == 6277 then
		Item(moveitem.uid):transform(2687, 12)
		position:sendMagicEffect(CONST_ME_HITBYFIRE)
	end

	return true
end
