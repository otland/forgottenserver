function onUse(player, item, fromPosition, target, toPosition, isHotkey)
	local partyTrumpet = Item(item.uid)
	partyTrumpet:transform(13578)
	partyTrumpet:decay()
	player:say("TOOOOOOT!", TALKTYPE_MONSTER_SAY)
	fromPosition:sendMagicEffect(CONST_ME_SOUND_BLUE)
	return true
end
