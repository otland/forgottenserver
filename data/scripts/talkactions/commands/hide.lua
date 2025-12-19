local talkaction = TalkAction("/hide")

function talkaction.onSay(player, words, param)
	player:setHiddenHealth(not player:isHealthHidden())
	return false
end

talkaction:access(true)
talkaction:accountType(ACCOUNT_TYPE_GOD)
talkaction:register()
