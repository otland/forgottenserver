function onUse(cid, item, fromPosition, itemEx, toPosition, isHotkey)
	if fromPosition.x ~= CONTAINER_POSITION then
		fromPosition:sendMagicEffect(math.random(CONST_ME_FIREWORK_YELLOW, CONST_ME_FIREWORK_BLUE))
	else
		local player = Player(cid)
		local pos = player:getPosition()
		pos:sendMagicEffect(CONST_ME_HITBYFIRE)
		pos:sendMagicEffect(CONST_ME_EXPLOSIONAREA)
		player:say("Ouch! Rather place it on the ground next time.", TALKTYPE_MONSTER_SAY)
		player:addHealth(-10)
	end
	Item(item.uid):remove()

	return true
end
