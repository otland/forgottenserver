local talkaction = TalkAction("/ghost")

function talkaction.onSay(player, words, param)
	local position = player:getPosition()
	local isGhost = not player:isInGhostMode()

	player:setGhostMode(isGhost)
	if isGhost then
		player:sendTextMessage(MESSAGE_INFO_DESCR, "You are now invisible.")
		position:sendMagicEffect(CONST_ME_YALAHARIGHOST)
	else
		player:sendTextMessage(MESSAGE_INFO_DESCR, "You are visible again.")
		position.x = position.x + 1
		position:sendMagicEffect(CONST_ME_SMOKE)
	end
	return false
end

talkaction:access(true)
talkaction:accountType(ACCOUNT_TYPE_GOD)
talkaction:register()
