local banDays = 7

function onSay(cid, words, param)
	if getPlayerAccess(cid) <= 0 then
		return false
	end

	local accountId = getAccountNumberByPlayerName(param)
	if accountId == 0 then
		return false
	end

	local resultId = db.storeQuery("SELECT 1 FROM `account_bans` WHERE `account_id` = " .. accountId)
	if resultId ~= false then
		result.free(resultId)
		return false
	end

	local timeNow = os.time()
	db:query("INSERT INTO `account_bans` (`account_id`, `reason`, `banned_at`, `expires_at`, `banned_by`) VALUES (" ..
			accountId .. ", '', " .. timeNow .. ", " .. timeNow + (banDays * 86400) .. ", " .. getPlayerGUIDByName(getCreatureName(cid)) .. ")")

	local targetCid = getPlayerByName(param)
	if targetCid ~= false then
		doRemoveCreature(targetCid)
	end
	doPlayerSendTextMessage(cid, MESSAGE_EVENT_ADVANCE, param .. " has been banned.")
end
