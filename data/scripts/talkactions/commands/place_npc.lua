local talkaction = TalkAction("/s")

function talkaction.onSay(player, words, param)
	local position = player:getPosition()
	local npc = Game.createNpc(param, position)
	if npc then
		npc:setMasterPos(position)
		position:sendMagicEffect(CONST_ME_MAGIC_RED)
	else
		player:sendCancelMessage("There is not enough room.")
		position:sendMagicEffect(CONST_ME_POFF)
	end
	return false
end

talkaction:separator(" ")
talkaction:access(true)
talkaction:accountType(ACCOUNT_TYPE_GOD)
talkaction:register()
