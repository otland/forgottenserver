local transformID = {
	[25393] = 25392, [25392] = 25393, --rift carpet
	[26193] = 26192, [26192] = 26193, --void carpet
	[26087] = 26109, [26109] = 26087, --yalaharian carpet
	[26088] = 26110, [26110] = 26088, --white fur carpet
	[26089] = 26111, [26111] = 26089, --bamboo mat carpet
	[26371] = 26363, [26363] = 26371, --crimson carpet
	[26366] = 26372, [26372] = 26366, --azure carpet
	[26367] = 26373, [26373] = 26367, --emerald carpet
	[26368] = 26374, [26374] = 26368, --light parquet carpet
	[26369] = 26375, [26375] = 26369, --dark parquet carpet
	[26370] = 26376, [26376] = 26370 --marble floor
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
	elseif not tile or not tile:getHouse() then
		player:sendTextMessage(MESSAGE_STATUS_SMALL, "You may use this only inside a house.")
	else if carpetStack > 1 then
		player:sendCancelMessage(RETURNVALUE_NOTPOSSIBLE)
		return true
	else
		item:transform(carpet)
		end
	end
	return true
end

for k,v in pairs(transformID) do
	carpets:id(k)
end
carpets:register()
