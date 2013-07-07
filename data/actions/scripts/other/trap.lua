function onUse(cid, item, fromPosition, itemEx, toPosition)
	doTransformItem(item.uid, item.itemid - 1)
	doSendMagicEffect(fromPosition, CONST_ME_POFF)
	return TRUE
end