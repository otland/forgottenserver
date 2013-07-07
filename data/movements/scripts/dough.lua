function onAddItem(moveitem, tileitem, position)
	if moveitem.itemid == 2693 then
		doTransformItem(moveitem.uid, 2689)
		doSendMagicEffect(position, CONST_ME_HITBYFIRE)
	end
	return TRUE
end