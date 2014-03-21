function onUse(cid, item, fromPosition, itemEx, toPosition)
	local partyTrumpet = Item(item.uid)
	partyTrumpet:transform(13578)
	partyTrumpet:decay()
	Player(cid):say("TOOOOOOT!", TALKTYPE_MONSTER_SAY)
	fromPosition:sendMagicEffect(CONST_ME_SOUND_BLUE)
	return true
end
