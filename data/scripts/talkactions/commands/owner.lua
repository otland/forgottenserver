local talkaction = TalkAction("/owner")

function talkaction.onSay(player, words, param)
	local tile = Tile(player:getPosition())
	local house = tile and tile:getHouse()
	if not house then
		player:sendCancelMessage("You are not inside a house.")
		return false
	end

	if param == "" or param == "none" then
		house:setOwnerGuid(0)
		return false
	end

	local targetPlayer = Player(param)
	if not targetPlayer then
		player:sendCancelMessage("Player not found.")
		return false
	end

	house:setOwnerGuid(targetPlayer:getGuid())
	return false
end

talkaction:separator(" ")
talkaction:access(true)
talkaction:accountType(ACCOUNT_TYPE_GOD)
talkaction:register()
