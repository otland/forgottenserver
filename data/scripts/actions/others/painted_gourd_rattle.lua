local paintedGourdRattle = Action()

function paintedGourdRattle.onUse(player, item, fromPosition, target, toPosition, isHotkey)
	if math.random(5) == 1 then
		player:getPosition():sendMagicEffect(CONST_ME_YALAHARIGHOST)
		player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "You're shaking the gourd rattle. It clatters and suddenly you feel strange. Have you annoyed the spirits?")
	else
		player:getPosition():sendMagicEffect(CONST_ME_SOUND_RED)
		player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "You're shaking the gourd rattle. It clatters, yet nothing else happens.")
	end
	return true
end

paintedGourdRattle:id(24846)
paintedGourdRattle:register()
