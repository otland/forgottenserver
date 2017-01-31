local blessings = {
	['the wisdom of solitude'] = 1,
	['the spark of the phoenix'] = 2,
	['the fire of the suns'] = 3,
	['the spiritual shielding'] = 4,
	['the embrace of tibia'] = 5,
	['twist of fate'] = 6
}

local function getAffectedBlessings(offerName)
	if offerName == 'all regular blessings' then
		return {1, 2, 3, 4, 5}
	end

	return {blessings[offerName]}
end

function onRender(player, offer)
	local checkBlessings = getAffectedBlessings(offer:getName():lower())
	for _, blessing in ipairs(checkBlessings) do
		if player:hasBlessing(blessing) then
			return false
		end
	end

	return true
end

function onBuy(player, offer)
	local affectedBlessings = getAffectedBlessings(offer:getName():lower())
	for _, blessing in ipairs(affectedBlessings) do
		player:addBlessing(blessing)
	end

	return true
end
