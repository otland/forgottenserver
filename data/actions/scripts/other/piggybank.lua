function onUse(cid, item, fromPosition, itemEx, toPosition)
	local player = Player(cid)

	if math.random(6) == 1 then
		fromPosition:sendMagicEffect(CONST_ME_POFF)
		player:addItem(2148, 1) -- gold coin
		Item(item.uid):transform(2115)
	else
		fromPosition:sendMagicEffect(CONST_ME_SOUND_YELLOW)
		player:addItem(2152, 1) -- platinum coin
	end

	return true
end
