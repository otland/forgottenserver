local save = TalkAction("/save")

function save.onSay(player, words, param)
	if(not player:getGroup():getAccess()) or player:getAccountType() < ACCOUNT_TYPE_GOD then
		return true
	end
	
	saveServer()
	player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "Server is saved ...")
end

save:register()
