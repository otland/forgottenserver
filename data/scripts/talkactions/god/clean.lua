local clean = TalkAction("/clean")

function clean.onSay(player, words, param)
	if(not player:getGroup():getAccess()) or player:getAccountType() < ACCOUNT_TYPE_GOD then
		return true
	end

	local itemCount = cleanMap()
	if itemCount ~= 0 then
		player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "Cleaned " .. itemCount .. " item" .. (itemCount > 1 and "s" or "") .. " from the map.")
	end
	return false
end

clean:separator(" ")
clean:register()
