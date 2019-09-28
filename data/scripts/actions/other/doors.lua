local doorsIds = {
	{1209, 1214}, {1219, 1262}, {1539, 1542}, {2086, 2092}, {3535, 3552}, {4913, 4918},
	{5082, 5085}, {5098, 5145}, {5278, 5295}, {5515, 5518}, {5732, 5737}, {5745, 5749},
	{6192, 6209}, {6249, 6266}, {6795, 6802}, {6891, 6908}, {7033, 7050}, {7054, 7057},
	{8541, 8558}, {9165, 9184}, {9267, 9284}, {10032, 10032}, {10268, 10285}, {10468, 10486},
	{10775, 10777}, {10780, 10786}, {10789, 10792}, {12092, 12105}, {12188, 12190}, {12193, 12199},
	{12202, 12205}, {19840, 19857}, {19980, 19997}, {20273, 20290}, {17235, 17238}, {18208, 18209},
	{13020, 13023}
}

local action = Action()

function action.onUse(player, item, fromPosition, target, toPosition, isHotkey)
	local itemId = item:getId()
	if table.contains(questDoors, itemId) then
		if player:getStorageValue(item.actionid) ~= -1 then
			item:transform(itemId + 1)
			player:teleportTo(toPosition, true)
		else
			player:sendTextMessage(MESSAGE_INFO_DESCR, "The door seems to be sealed against unwanted intruders.")
		end
		return true
	elseif table.contains(levelDoors, itemId) then
		if item.actionid > 0 and player:getLevel() >= item.actionid - 1000 then
			item:transform(itemId + 1)
			player:teleportTo(toPosition, true)
		else
			player:sendTextMessage(MESSAGE_INFO_DESCR, "Only the worthy may pass.")
		end
		return true
	elseif table.contains(keys, itemId) then
		if target.actionid > 0 then
			if item.actionid == target.actionid and doors[target.itemid] then
				target:transform(doors[target.itemid])
				return true
			end
			player:sendTextMessage(MESSAGE_STATUS_SMALL, "The key does not match.")
			return true
		end
		return false
	end

	if table.contains(horizontalOpenDoors, itemId) or table.contains(verticalOpenDoors, itemId) then
		local doorCreature = Tile(toPosition):getTopCreature()
		if doorCreature then
			toPosition.x = toPosition.x + 1
			local query = Tile(toPosition):queryAdd(doorCreature, bit.bor(FLAG_IGNOREBLOCKCREATURE, FLAG_PATHFINDING))
			if query ~= RETURNVALUE_NOERROR then
				toPosition.x = toPosition.x - 1
				toPosition.y = toPosition.y + 1
				query = Tile(toPosition):queryAdd(doorCreature, bit.bor(FLAG_IGNOREBLOCKCREATURE, FLAG_PATHFINDING))
			end

			if query ~= RETURNVALUE_NOERROR then
				player:sendTextMessage(MESSAGE_STATUS_SMALL, Game.getReturnMessage(query))
				return true
			end

			doorCreature:teleportTo(toPosition, true)
		end

		if not table.contains(openSpecialDoors, itemId) then
			item:transform(itemId - 1)
		end
		return true
	end

	if doors[itemId] then
		if item.actionid == 0 then
			item:transform(doors[itemId])
		else
			player:sendTextMessage(MESSAGE_INFO_DESCR, "It is locked.")
		end
		return true
	end
	return false
end

for _, value in pairs(doorsIds) do
	for id = value[1], value[2] do
		action:id(id)
	end
end

action:register()
