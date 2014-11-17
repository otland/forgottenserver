local playerPosition = {
	Position(247, 659, 13),
	Position(247, 660, 13),
	Position(247, 661, 13),
	Position(247, 662, 13)
}
local newPosition = {
	Position(189, 650, 13),
	Position(189, 651, 13),
	Position(189, 652, 13),
	Position(189, 653, 13)
}

function onUse(player, item, fromPosition, targetEx, toPosition, isHotkey)
	if item:getId() == 1945 then
		local players = {}
		for _, pos in ipairs(playerPosition) do
			local tile = Tile(pos)
			if tile then
				local creature = tile:getTopCreature()
				if creature == nil or not creature:isPlayer() or creature:getLevel() < 100 or creature:getStorageValue(30015) ~= -1 then
					creature:sendTextMessage(MESSAGE_STATUS_SMALL, Game.getReturnMessage(RETURNVALUE_NOTPOSSIBLE))
					return false
				end
				players[#players+1] = creature
			end
		end
		for i, questPlayer in ipairs(players) do
			playerPosition[i]:sendMagicEffect(CONST_ME_POFF)
			questPlayer:teleportTo(newPosition[i], false)
			questPlayer:getPosition():sendMagicEffect(CONST_ME_ENERGYAREA)
		end
		item:transform(1946)
	elseif item:getId() == 1946 then
		player:sendTextMessage(MESSAGE_STATUS_SMALL, Game.getReturnMessage(RETURNVALUE_NOTPOSSIBLE))
	end

	return true
end
