function onSay(cid, words, param)
	if getPlayerAccess(cid) <= 0 then
		return false
	end

	local resultId = db.storeQuery("SELECT `account_id`, `lastip` FROM `players` WHERE `name` = " .. db.escapeString(param))
	if resultId == false then
		return false
	end

	db:query("DELETE FROM `account_bans` WHERE `account_id` = " .. result.getDataInt(resultId, "account_id"))
	db:query("DELETE FROM `ip_bans` WHERE `ip` = " .. result.getDataInt(resultId, "lastip"))
	result.free(resultId)
	doPlayerSendTextMessage(cid, MESSAGE_EVENT_ADVANCE, param .. " has been unbanned.")
end
