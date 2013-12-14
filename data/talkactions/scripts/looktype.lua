function onSay(cid, words, param)
	local player = Player(cid)
	if not player:getGroup():getAccess() then
		return true
	end

	local lookType = tonumber(param)
	if lookType >= 0 and lookType ~= 1 and lookType ~= 135 and lookType ~= 411 and lookType ~= 415 and lookType ~= 424 and (lookType <= 160 or lookType >= 192) and lookType ~= 439 and lookType ~= 440 and lookType ~= 468 and lookType ~= 469 and (lookType < 474 or lookType > 485) and lookType ~= 501 and lookType ~= 518 and lookType ~= 519 and lookType ~= 520 and lookType ~= 524 and lookType ~= 525 and lookType ~= 536 and lookType ~= 543 and lookType ~= 549 and lookType ~= 576 and lookType ~= 579 and lookType ~= 581 and lookType ~= 582 and lookType <= 595 then
		local playerOutfit = player:getOutfit()
		playerOutfit.lookType = lookType
		player:setOutfit(playerOutfit)
	else
		player:sendCancelMessage("A look type with that id does not exist.")
	end
	return false
end
