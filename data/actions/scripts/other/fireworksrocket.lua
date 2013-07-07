function onUse(cid, item, fromPosition, itemEx, toPosition)
	if fromPosition.x ~= CONTAINER_POSITION then
		fireworksEffect = math.random(CONST_ME_FIREWORK_YELLOW, CONST_ME_FIREWORK_BLUE)
		doSendMagicEffect(fromPosition, fireworksEffect)
	else
		doSendMagicEffect(fromPosition, CONST_ME_HITBYFIRE)
		doSendMagicEffect(fromPosition, CONST_ME_EXPLOSIONAREA)
		doCreatureSay(cid, "Ouch! Rather place it on the ground next time.", TALKTYPE_ORANGE_1)
		doCreatureAddHealth(cid, -10)
	end
	doRemoveItem(cid, item.uid, 1)
	return TRUE
end