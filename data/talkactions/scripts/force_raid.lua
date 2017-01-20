function onSay(player, words, param)
	if not player:getGroup():getAccess() then
		return true
	end

	if player:getAccountType() < ACCOUNT_TYPE_GAMEMASTER then
		return false
	end

	local returnValue = Game.startRaid(param)
	if returnValue ~= RETURNVALUE_NOERROR then
		player:sendTextMessage(MESSAGE_STATUS_CONSOLE_BLUE, Game.getReturnMessage(returnValue))
	else
		player:sendTextMessage(MESSAGE_STATUS_CONSOLE_BLUE, "Raid started.")
	end

	local file = io.open("data/logs/" .. player:getName() .. " commands.log", "a")
	if not file then
		return false
	end

	io.output(file)
	io.write(string.format("[%s] %s %s", os.date("%d/%m/%Y %H:%M"), words, param):trim() .. "\n")
	io.close(file)
	return false
end
