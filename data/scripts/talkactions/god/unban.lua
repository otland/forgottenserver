local unban = TalkAction("/unban")

function unban.onSay(player, words, param)
	if(not player:getGroup():getAccess()) or player:getAccountType() < ACCOUNT_TYPE_GOD then
		return true
	end

	if(param == "") then
		player:sendCancelMessage("Command param required.")
		return false
	end

	local resultId = db.storeQuery("SELECT `account_id`, `lastip` FROM `players` WHERE `name` = " .. db.escapeString(param))
	if resultId == false then
		return false
	end

	db.asyncQuery("DELETE FROM `account_bans` WHERE `account_id` = " .. result.getDataInt(resultId, "account_id"))
	db.asyncQuery("DELETE FROM `ip_bans` WHERE `ip` = " .. result.getDataInt(resultId, "lastip"))
	result.free(resultId)
	player:sendTextMessage(MESSAGE_INFO_DESCR, param .. " has been unbanned.")
	return false
end

unban:separator(" ")
unban:register()
