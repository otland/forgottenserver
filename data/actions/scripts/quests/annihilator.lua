local playerPosition = {
	{x = 247, y = 659, z = 13},
	{x = 247, y = 660, z = 13},
	{x = 247, y = 661, z = 13},
	{x = 247, y = 662, z = 13}
}
local newPosition = {
	{x = 189, y = 650, z = 13},
	{x = 189, y = 651, z = 13},
	{x = 189, y = 652, z = 13},
	{x = 189, y = 653, z = 13}
}

function onUse(cid, item, fromPosition, target, toPosition, isHotkey)
	if item.itemid == 1945 then
		local players = {}
		for _, pos in ipairs(playerPosition) do
			local player = Tile(pos):getTopCreature()
			if player == nil or not player:isPlayer() or player:getLevel() < 100 or player:getStorageValue(30015) ~= -1 then
				Player(cid):sendTextMessage(MESSAGE_STATUS_SMALL, Game.getReturnMessage(RETURNVALUE_NOTPOSSIBLE))
				return false
			end
			players[#players+1] = player
		end
		for i, player in ipairs(players) do
			Position(playerPosition[i]):sendMagicEffect(CONST_ME_POFF)
			player:teleportTo(newPosition[i], false)
			player:getPosition():sendMagicEffect(CONST_ME_ENERGYAREA)
		end
		Item(item.uid):transform(1946)
	elseif item.itemid == 1946 then
		Player(cid):sendTextMessage(MESSAGE_STATUS_SMALL, Game.getReturnMessage(RETURNVALUE_NOTPOSSIBLE))
	end

	return true
end
