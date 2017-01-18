function onReport(player, message, position, category)
	if player:getAccountType() == ACCOUNT_TYPE_NORMAL then
		return false
	end

	local name, playerPos = player:getName(), player:getPosition()
	local file = io.open("data/reports/" .. name .. " report.txt", "a")

	if not file then
		player:sendTextMessage(MESSAGE_EVENT_DEFAULT, "There was an error when processing your report, please contact a gamemaster.")
		return true
	end

	io.output(file)
	io.write("------------------------------\n")
	io.write("Name: " .. name)
	if category == BUG_CATEGORY_MAP then
		io.write(" [Map position: " .. position.x .. ", " .. position.y .. ", " .. position.z .. "]")
	end
	io.write(" [Player position: " .. playerPos.x .. ", " .. playerPos.y .. ", " .. playerPos.z .. "]\n")
	io.write("Comment: " .. message .. "\n")
	io.close(file)

	player:sendTextMessage(MESSAGE_EVENT_DEFAULT, "Your report has been sent to " .. configManager.getString(configKeys.SERVER_NAME) .. ".")
	return true
end
