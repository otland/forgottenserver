function onUse(cid, item, fromPosition, itemEx, toPosition)

	if fromPosition.x ~= CONTAINER_POSITION then
		fromPosition:sendMagicEffect(math.random(CONST_ME_FIREWORK_YELLOW, CONST_ME_FIREWORK_BLUE))
	else
		local p = Player(cid)
		local pos = p:getPosition()
		pos:sendMagicEffect(CONST_ME_HITBYFIRE)
		pos:sendMagicEffect(CONST_ME_EXPLOSIONAREA)
		p:say("Ouch! Rather place it on the ground next time.", TALKTYPE_ORANGE_1)
		p:addHealth(-10)
	end
	Item(item.uid):remove()

	return true
end
