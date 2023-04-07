local depotTiles = {11062, 11063}
local diceEnabledOnDepot = true

function onUse(player, item, fromPosition, target, toPosition, isHotkey)
	local position = item:getPosition()
	local value = math.random(1, 6)
	local isInGhostMode = player:isInGhostMode()
	local tile = Tile(player:getPosition())
	if not tile then
		return false
	end
	if table.contains(depotTiles, tile:getGround():getId()) and not diceEnabledOnDepot then
		return false
	end

	position:sendMagicEffect(CONST_ME_CRAPS, isInGhostMode and player)

	local spectators = Game.getSpectators(position, false, true, 3, 3)
	for _, spectator in ipairs(spectators) do
		player:say(player:getName() .. " rolled a " .. value .. ".", TALKTYPE_MONSTER_SAY, isInGhostMode, spectator, position)
	end

	item:transform(5791 + value)
	return true
end
