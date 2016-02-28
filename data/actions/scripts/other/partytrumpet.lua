function onUse(player, item, fromPosition, target, toPosition, isHotkey)
	item:transform(6573)
	item:decay()
	player:say("TOOOOOOT!", TALKTYPE_MONSTER_SAY)
	fromPosition:sendMagicEffect(CONST_ME_SOUND_BLUE)
	return true
end
