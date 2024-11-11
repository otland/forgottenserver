local ban = TalkAction("/ban")

function ban.onSay(player, words, param)
	if not player:getGroup():getAccess() then
		return true
	end

	local params = param:split(",")
	if #params < 3 then
		player:sendCancelMessage("Command requires 3 parameters: /ban <player name>, <duration in days>, <reason>")
		return true
	end

	local targetName = params[1]:trim()
	local banDuration = tonumber(params[2]:trim())
	local banReason = params[3]:trim()

	if not banDuration or banDuration <= 0 then
		player:sendCancelMessage("Ban duration must be a positive number.")
		return true
	end

	local accountId = Game.getPlayerAccountId(targetName)
	if accountId == 0 then
		return true
	end

	local resultId = db.storeQuery("SELECT 1 FROM `account_bans` WHERE `account_id` = " .. accountId)
	if resultId then
		result.free(resultId)
		return true
	end

	local currentTime = os.time()
	local expirationTime = currentTime + (banDuration * 24 * 60 * 60)
	db.query(string.format("INSERT INTO `account_bans` (`account_id`, `reason`, `banned_at`, `expires_at`, `banned_by`) VALUES (%d, %s, %d, %d, %d)", accountId, db.escapeString(banReason), currentTime, expirationTime, player:getGuid()))

	local target = Player(targetName)
	if target then
		player:sendTextMessage(MESSAGE_EVENT_ADVANCE, string.format("%s has been banned for %d days.", target:getName(), banDuration))
		target:remove()
	else
		player:sendTextMessage(MESSAGE_EVENT_ADVANCE, string.format("%s has been banned for %d days.", targetName, banDuration))
	end
	return true
end

ban:separator(" ")
ban:register()

local ipban = TalkAction("/ipban")

function ipban.onSay(player, words, param)
	if not player:getGroup():getAccess() then
		return true
	end

	local params = param:split(",")
	if #params < 3 then
		player:sendCancelMessage("Command requires 3 parameters: /ipban <player name>, <duration in days>, <reason>")
		return true
	end

	local targetName = params[1]:trim()
	local ipBanDuration = tonumber(params[2]:trim())
	local ipBanReason = params[3]:trim()

	if not ipBanDuration or ipBanDuration <= 0 then
		player:sendCancelMessage("Ban duration must be a positive number.")
		return true
	end

	local resultId = db.storeQuery("SELECT `name`, `lastip` FROM `players` WHERE `name` = " .. db.escapeString(targetName))
	if not resultId then
		return true
	end

	local targetIp = result.getString(resultId, "lastip")
	result.free(resultId)

	if targetIp == "0" then
		player:sendTextMessage(MESSAGE_STATUS_WARNING, string.format("Invalid IP for player %s.", targetName))
		return true
	end

	local checkBanQuery = db.storeQuery("SELECT 1 FROM `ip_bans` WHERE `ip` = " .. db.escapeString(targetIp))
	if checkBanQuery then
		player:sendTextMessage(MESSAGE_EVENT_ADVANCE, string.format("%s is already IP banned.", targetName))
		result.free(checkBanQuery)
		return true
	end

	local currentTime = os.time()
	local expirationTime = currentTime + (ipBanDuration * 24 * 60 * 60)
	db.query(string.format("INSERT INTO `ip_bans` (`ip`, `reason`, `banned_at`, `expires_at`, `banned_by`) VALUES (%s, %s, %d, %d, %d)", db.escapeString(targetIp), db.escapeString(ipBanReason), currentTime, expirationTime, player:getGuid()))

	player:sendTextMessage(MESSAGE_EVENT_ADVANCE, string.format("%s has been IP banned for %d days.", targetName, ipBanDuration))
	return true
end

ipban:separator(" ")
ipban:register()

local unban = TalkAction("/unban")

function unban.onSay(player, words, param)
	if not player:getGroup():getAccess() then
		return true
	end

	if param == "" then
		player:sendTextMessage(MESSAGE_STATUS_WARNING, "Command requires 1 parameter: /unban <player name>")
		return true
	end

	local resultId = db.storeQuery("SELECT `account_id`, `lastip` FROM `players` WHERE `name` = " .. db.escapeString(param))
	if not resultId then
		return true
	end

	local accountId = result.getNumber(resultId, "account_id")
	local lastIp = result.getString(resultId, "lastip")
	result.free(resultId)

	db.asyncQuery("DELETE FROM `account_bans` WHERE `account_id` = " .. db.escapeString(tostring(accountId)))
	db.asyncQuery("DELETE FROM `ip_bans` WHERE `ip` = " .. db.escapeString(lastIp))

	player:sendTextMessage(MESSAGE_EVENT_ADVANCE, string.format("%s has been unbanned.", param))
	return true
end

unban:separator(" ")
unban:register()
