local transformID = {
	[22737] = 22736, [22736] = 22737, --rift carpet
	[23537] = 23536, [23536] = 23537, --void carpet
	[23431] = 23453, [23453] = 23431, --yalaharian carpet
	[23432] = 23454, [23454] = 23432, --white fur carpet
	[23433] = 23455, [23455] = 23433, --bamboo mat carpet
	[23715] = 23707, [23707] = 23715, --crimson carpet
	[23710] = 23716, [23716] = 23710, --azure carpet
	[23711] = 23717, [23717] = 23711, --emerald carpet
	[23712] = 23718, [23718] = 23712, --light parquet carpet
	[23713] = 23719, [23719] = 23713, --dark parquet carpet
	[23714] = 23720, [23720] = 23714 --marble floor
}

local carpets = Action()

function carpets.onUse(player, item, fromPosition, target, toPosition, isHotkey)
	local carpet = transformID[item.itemid]
	if not carpet then
		return false
	end
	local tile = Tile(item:getPosition())
	local carpetStack = 0
	for _, carpetId in pairs(transformID) do
		carpetStack = carpetStack + tile:getItemCountById(carpetId)
	end
	if fromPosition.x == CONTAINER_POSITION then
		player:sendTextMessage(MESSAGE_STATUS_SMALL, "Put the item on the floor first.")
		return true
	elseif not tile or not tile:getHouse() then
		player:sendTextMessage(MESSAGE_STATUS_SMALL, "You may use this only inside a house.")
		return true
	elseif carpetStack > 1 then
		player:sendCancelMessage(RETURNVALUE_NOTPOSSIBLE)
		return true
	end
	item:transform(carpet)
	return true
end

for k,v in pairs(transformID) do
	carpets:id(k)
end

carpets:register()
