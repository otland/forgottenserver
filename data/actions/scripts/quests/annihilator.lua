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

function onUse(cid, item, fromPosition, itemEx, toPosition)
	if item.itemid == 1945 then
		local players = {}
		for _, posi in ipairs(playerPosition) do
			local pl = Tile(posi):getTopCreature()
			if pl == nil or not pl:isPlayer() or pl:getLevel() < 100 or pl:getStorageValue(30015) ~= -1 then
				Player(cid):sendTextMessage(MESSAGE_STATUS_SMALL, Game.getReturnMessage(RETURNVALUE_NOTPOSSIBLE))
				return false
			end
			players[#players+1] = pl
		end
		for i, pl in ipairs(players) do
			Position(playerPosition[i]):sendMagicEffect(CONST_ME_POFF)
			pl:teleportTo(newPosition[i], false)
			pl:getPosition():sendMagicEffect(CONST_ME_ENERGYAREA)
		end
		Item(item.uid):transform(1946)
	elseif item.itemid == 1946 then
		Player(cid):sendTextMessage(MESSAGE_STATUS_SMALL, Game.getReturnMessage(RETURNVALUE_NOTPOSSIBLE))
	end

	return true
end
