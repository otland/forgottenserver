function onUse(cid, item, fromPosition, itemEx, toPosition)
	if item.uid == getPlayerSlotItem(cid, CONST_SLOT_HEAD).uid then
		doSendMagicEffect(getPlayerPosition(cid), CONST_ME_GIFT_WRAPS)
		return TRUE
	end
	return FALSE
end