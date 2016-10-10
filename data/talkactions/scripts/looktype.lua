local blockLookType = {
				1, 135, 411, 415, 424, 439, 440, 468, 469, 518,
				519, 520, 524, 525, 536, 543, 549, 576, 581, 582,
				597, 616, 623, 635
			}

function onSay(player, words, param)
	if not player:getGroup():getAccess() then
		return true
	end

	local lookType = tonumber(param)

	if not lookType then
		player:sendCancelMessage("Param incorrect.")
		return false 
	end

	if lookType >= 0 				and
		not isInArray(blockLookType, lookType)	and
		(lookType <= 160 or lookType >= 192)	and
		(lookType < 474 or lookType > 485)	and
		(lookType <= 637 or lookType >= 644)	and
		(lookType <= 644 or lookType >= 647)	and
		(lookType <= 651 or lookType >= 664)	and
		lookType <= 699	then
			local playerOutfit = player:getOutfit()
			playerOutfit.lookType = lookType
			player:setOutfit(playerOutfit)
	else
		player:sendCancelMessage("A look type with that id does not exist.")
	end

	return false
end
