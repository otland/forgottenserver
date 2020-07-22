local teleports = {
	--[itemid] = {upFloor, client_version}
	[430] = {false, 0}, -- sewer grate
	[1369] = {false, 0}, -- draw well
	[1386] = {true, 0}, -- ladder
	[3678] = {true, 750}, -- ladder
	[5543] = {true, 780}, -- rope-ladder
	[8580] = {false, 820}, -- sewer grate
	[8599] = {true, 820}, -- ladder
	[10035] = {true, 850}, -- ladder
	[22845] = {true, 1030}, -- ladder rung
	[22846] = {true, 1030}, -- ladder rung
}

local teleport = Action()

function teleport.onUse(player, item, fromPosition, target, toPosition, isHotkey)
	local floorTeleport = teleports[item.itemid]
	if floorTeleport and floorTeleport[1] then
		fromPosition:moveUpstairs()
	else
		fromPosition.z = fromPosition.z + 1
	end

	if player:isPzLocked() and Tile(fromPosition):hasFlag(TILESTATE_PROTECTIONZONE) then
		player:sendCancelMessage(RETURNVALUE_PLAYERISPZLOCKED)
		return true
	end

	player:teleportTo(fromPosition, false)
	return true
end

for i, t in pairs(teleports) do
	if Game.getClientVersion().min >= t[2] then
		teleport:id(i)
	end
end
teleport:register()
