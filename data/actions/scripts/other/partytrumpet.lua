function onUse(cid, item, fromPosition, itemEx, toPosition)
	local ITem = Item(item.uid)
	ITem:transform(13578)
	ITem:decay()
	Player(cid):say("TOOOOOOT!", TALKTYPE_ORANGE_1)
	fromPosition:sendMagicEffect(CONST_ME_SOUND_BLUE)
	return true
end
