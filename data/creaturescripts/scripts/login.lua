dofile("./config.lua")

function onLogin(cid)
	local player = Player(cid)
	if player == nil then
		return TRUE
	end

	local loginStr = loginMessage
	if player:getLastLoginSaved() <= 0 then
		loginStr = loginStr .. " Please choose your outfit."
		player:sendOutfitWindow()
	else
		if loginStr ~= "" then
			player:sendTextMessage(MESSAGE_STATUS_DEFAULT, loginStr)
		end

		loginStr = string.format("Your last visit was on %s.", os.date("%a %b %d %X %Y", player:getLastLoginSaved()))
	end
	player:sendTextMessage(MESSAGE_STATUS_DEFAULT, loginStr)

	registerCreatureEvent(cid, "PlayerDeath")
	return TRUE
end