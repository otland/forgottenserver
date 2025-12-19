local talkaction = TalkAction("/t")

function talkaction.onSay(player, words, param)
	player:teleportTo(player:getTown():getTemplePosition())
	return false
end

talkaction:access(true)
talkaction:register()
