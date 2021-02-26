local talk = TalkAction("/m")

function talk.onSay(player, words, param)
	local position = player:getPosition()
	local monster = Game.createMonster(param, position)
	if monster then
		monster:getPosition():sendMagicEffect(CONST_ME_TELEPORT)
		position:sendMagicEffect(CONST_ME_MAGIC_RED)
	else
		player:sendCancelMessage("There is not enough room.")
		position:sendMagicEffect(CONST_ME_POFF)
	end
	return false
end

talk:access(true)
talk:accountType(ACCOUNT_TYPE_GOD)
talk:separator(" ")
talk:register()
