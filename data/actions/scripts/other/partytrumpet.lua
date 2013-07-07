function onUse(cid, item, fromPosition, itemEx, toPosition)
	doTransformItem(item.uid, 6573)
	doCreatureSay(cid, "TOOOOOOT!", TALKTYPE_ORANGE_1)
	doSendMagicEffect(fromPosition, CONST_ME_SOUND_BLUE)
	doDecayItem(item.uid)
	return TRUE
end