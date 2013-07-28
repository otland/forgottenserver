function onSay(cid, words, param)
	if getPlayerAccess(cid) <= 0 then
		return false
	end
	
	local accountId = getPlayerAccountNumberByName(param)
	if accountId == 0 then
		return false
	end
	
	local timeNow = os.time()
	db:query("INSERT INTO `account_bans` (`account_id`, `reason`, `banned_at`, `expires_at`, `banned_by`) VALUES (" ..
			accountId .. ", '', " .. timeNow .. ", " .. timeNow + (getConfigInfo("banDays") * 86400) .. ", " .. getPlayerGUIDByName(getCreatureName(cid)) .. ")")

	local targetCid = getPlayerByName(param)
	if targetCid ~= false then
		doRemoveCreature(targetCid)
	end
end
