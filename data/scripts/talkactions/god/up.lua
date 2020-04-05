local up = TalkAction("/up")

function up.onSay(player, words, param)
	if(not player:getGroup():getAccess()) or player:getAccountType() < ACCOUNT_TYPE_GOD then
		return true
	end

	local position = player:getPosition()
	position.z = position.z - 1
	player:teleportTo(position)
	return false
end

up:register()
