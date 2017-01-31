function onRender(player, offer)
	return true
end

function onBuy(player, offer, param)
	local result = db.storeQuery("SELECT `id` FROM `players` WHERE `name` = " .. db.escapeString(param))
	if result then
		player:sendStoreError(STORE_ERROR_PURCHASE, "The name '" .. param .. "' is already being used.")
		return false
	end

	if not db.query("UPDATE `players` SET `name` = " .. db.escapeString(param) .. " WHERE `id` = " .. player:getGuid()) then
		player:sendStoreError(STORE_ERROR_PURCHASE, "Something went wrong, please try again later.")
		return false
	end
	
	return true
end
