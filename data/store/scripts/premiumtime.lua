function onRender(player, offer)
	return true
end

function onBuy(player, offer)
	local days = tonumber(offer:getName():match('(%d+) days$'))
	if days then
		player:addPremiumDays(days)
		return true
	end

	player:sendStoreError(STORE_ERROR_PURCHASE, 'Offer "' .. offer:getName() .. '" not found. Please contact the administrator.')
	return false
end
