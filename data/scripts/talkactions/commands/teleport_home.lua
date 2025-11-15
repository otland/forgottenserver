local talkaction = TalkAction("/t")

function talkaction.onSay(player, words, param)
	if not player:getGroup():getAccess() then
		return true
	end

	player:teleportTo(player:getTown():getTemplePosition())
	return false
end

talkaction:register()
