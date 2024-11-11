local talk = TalkAction("/ban")

function talk.onSay(player, words, param)
	if not player:getGroup():getAccess() or player:getAccountType() < ACCOUNT_TYPE_GAMEMASTER then
		return true
	end

	local params = param:split(",")
	if #params < 3 then
		player:sendCancelMessage("Command requires 3 parameters: /ban <player name>, <duration in days>, <reason>")
		return true
	end

	local name = params[1]:trim()
	local banDuration = tonumber(params[2]:trim())
	local banReason = params[3]:trim()

	if not banDuration or banDuration <= 0 then
		player:sendCancelMessage("Ban duration must be a positive number.")
		return true
	end

	local accountId = getAccountNumberByPlayerName(name)
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

	local target = Player(name)
	if target then
		player:sendTextMessage(MESSAGE_EVENT_ADVANCE, string.format("%s has been banned for %d days.", target:getName(), banDuration))
		target:remove()
	else
		player:sendTextMessage(MESSAGE_EVENT_ADVANCE, string.format("%s has been banned for %d days.", name, banDuration))
	end
	return true
end

talk:separator(" ")
talk:groupType("gamemaster")
talk:register()
