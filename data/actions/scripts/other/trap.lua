function onUse(cid, item, fromPosition, itemEx, toPosition, isHotkey)
	Item(item.uid):transform(item.itemid - 1)
	fromPosition:sendMagicEffect(CONST_ME_POFF)
	return true
end
