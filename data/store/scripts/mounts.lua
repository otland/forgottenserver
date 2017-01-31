function onRender(player, offer)
	return not player:hasMount(offer:getName())
end

function onBuy(player, offer)
	if player:hasMount(offer:getName()) then
		player:sendStoreError(STORE_ERROR_PURCHASE, "You already have '" .. offer:getName() .. "'.")
		return false
	end

	local success = player:addMount(offer:getName())
	if not success then
		player:sendStoreError(STORE_ERROR_PURCHASE, "Mount '" .. offer:getName() .. "' was not found. Please contact the administrator.")
		return false
	end

	return true
end
