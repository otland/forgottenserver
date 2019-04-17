local reloadTypes = {
	["all"] = RELOAD_TYPE_ALL,
	["global"] = RELOAD_TYPE_GLOBAL,
	["libs"] = RELOAD_TYPE_GLOBAL
}

function onSay(player, words, param)
	if not player:getGroup():getAccess() then
		return true
	end

	if player:getAccountType() < ACCOUNT_TYPE_GOD then
		return false
	end

	logCommand(player, words, param)

	local reloadType = reloadTypes[param:lower()]
	if not reloadType then
		player:sendTextMessage(MESSAGE_STATUS_CONSOLE_BLUE, "Reload type not found.")
		return false
	end

	Game.reload(reloadType)
	player:sendTextMessage(MESSAGE_STATUS_CONSOLE_BLUE, string.format("Reloaded %s.", param:lower()))
	return false
end
