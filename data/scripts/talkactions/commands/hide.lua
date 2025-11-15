local talkaction = TalkAction("/hide")

function talkaction.onSay(player, words, param)
	if not player:getGroup():getAccess() then
		return true
	end

	if player:getAccountType() < ACCOUNT_TYPE_GOD then
		return false
	end

	player:setHiddenHealth(not player:isHealthHidden())
	return false
end

talkaction:register()
