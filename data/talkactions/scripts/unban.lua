function onSay(player, words, param)
	if not player:getGroup():getAccess() then
		return true
	end

	local resultId = db.storeQuery("SELECT `account_id`, `lastip` FROM `players` WHERE `name` = " .. db.escapeString(param))
	if resultId == false then
		return false
	end

	db.asyncQuery("DELETE FROM `account_bans` WHERE `account_id` = " .. result.getDataInt(resultId, "account_id"))
	db.asyncQuery("DELETE FROM `ip_bans` WHERE `ip` = " .. result.getDataInt(resultId, "lastip"))
	result.free(resultId)
	player:sendTextMessage(MESSAGE_EVENT_ADVANCE, param .. " has been unbanned.")
	return false
end
