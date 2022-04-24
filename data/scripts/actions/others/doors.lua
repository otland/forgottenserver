local positionOffsets = {
	{x = 1, y = 0}, -- east
	{x = 0, y = 1}, -- south
	{x = -1, y = 0}, -- west
	{x = 0, y = -1}, -- north
}

--[[
When closing a door with a creature in it findPushPosition will find the most appropriate
adjacent position following a prioritization order.
The function returns the position of the first tile that fulfills all the checks in a round.
The function loops trough east -> south -> west -> north on each following line in that order.
In round 1 it checks if there's an unhindered walkable tile without any creature.
In round 2 it checks if there's a tile with a creature.
In round 3 it checks if there's a tile blocked by a movable tile-blocking item.
In round 4 it checks if there's a tile blocked by a magic wall or wild growth.
]]
local function findPushPosition(creature, round)
	local pos = creature:getPosition()
	for _, offset in ipairs(positionOffsets) do
		local offsetPosition = Position(pos.x + offset.x, pos.y + offset.y, pos.z)
		local tile = Tile(offsetPosition)
		if tile then
			local creatureCount = tile:getCreatureCount()
			if round == 1 then
				if tile:queryAdd(creature) == RETURNVALUE_NOERROR and creatureCount == 0 then
					if not tile:hasFlag(TILESTATE_PROTECTIONZONE) or (tile:hasFlag(TILESTATE_PROTECTIONZONE) and creature:canAccessPz()) then
						return offsetPosition
					end
				end
			elseif round == 2 then
				if creatureCount > 0 then
					if not tile:hasFlag(TILESTATE_PROTECTIONZONE) or (tile:hasFlag(TILESTATE_PROTECTIONZONE) and creature:canAccessPz()) then
						return offsetPosition
					end
				end
			elseif round == 3 then
				local topItem = tile:getTopDownItem()
				if topItem then
					if topItem:getType():isMovable() then
						return offsetPosition
					end
				end
			else
				if tile:getItemById(ITEM_MAGICWALL) or tile:getItemById(ITEM_WILDGROWTH) then
					return offsetPosition
				end
			end
		end
	end
	if round < 4 then
		return findPushPosition(creature, round + 1)
	end
end

--[[
First value should be lockedDoor or closedDoor(if no lock), second closedDoor, third openDoor.
Odd ID doors must be added to this table and their respective ones. IDs don't matter, only their order.
]]
local oddDoors = {
	[1] = {12692, 12692, 12695}, [2] = {12701, 12701, 12703}, [3] = {33431, 33430, 33433}, [4] = {33429, 33428, 33432}
}

local function findOddDoor(itemId, vi)
	local k = 1
	local v = 1
	local newId = 0
	for k, _ in pairs(oddDoors) do
		for v, _ in pairs(oddDoors[k]) do
			if itemId == oddDoors[k][v] then
				if vi == nil then
					return true
				else
					newId = oddDoors[k][vi]
					return newId
				end
			end
		end
	end
	return false
end

local door = Action()

function door.onUse(player, item, fromPosition, target, toPosition, isHotkey)
	local itemId = item:getId()
	if table.contains(closedQuestDoors, itemId) then
		if player:getStorageValue(item.actionid) ~= -1 or player:getGroup():getAccess() then
			item:transform(itemId + 1)
			player:teleportTo(toPosition, true)
		else
			player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "The door seems to be sealed against unwanted intruders.")
		end
		return true
	elseif table.contains(closedLevelDoors, itemId) then
		if item.actionid > 0 and player:getLevel() >= item.actionid - actionIds.levelDoor or player:getGroup():getAccess() then
			item:transform(itemId + 1)
			player:teleportTo(toPosition, true)
		else
			player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "Only the worthy may pass.")
		end
		return true
	elseif table.contains(keys, itemId) then
		local tile = Tile(toPosition)
		if not tile then
			return false
		end
		target = tile:getTopVisibleThing()
		if target.actionid == 0 then
			return false
		end
		if table.contains(keys, target.itemid) then
			return false
		end
		if not table.contains(openDoors, target.itemid) and not table.contains(closedDoors, target.itemid) and not table.contains(lockedDoors, target.itemid) then
			return false
		end
		if item.actionid ~= target.actionid then
			player:sendTextMessage(MESSAGE_STATUS_SMALL, "The key does not match.")
			return true
		end
		local transformTo = target.itemid + 2
		if findOddDoor(target.itemid) then
			transformTo = findOddDoor(target.itemid, 3)
		end
		if table.contains(openDoors, target.itemid) then
			if findOddDoor(target.itemid) then
				transformTo = findOddDoor(target.itemid, 1)
			else
				transformTo = target.itemid - 2
			end
		elseif table.contains(closedDoors, target.itemid) then
			if findOddDoor(target.itemid) then
				transformTo = findOddDoor(target.itemid, 1)
			else
				transformTo = target.itemid - 1
			end
		end
		target:transform(transformTo)
		return true
	elseif table.contains(lockedDoors, itemId) then
		player:sendTextMessage(MESSAGE_INFO_DESCR, "It is locked.")
		return true
	elseif table.contains(openDoors, itemId) or table.contains(openExtraDoors, itemId) or table.contains(openHouseDoors, itemId) then
		local creaturePositionTable = {}
		local doorCreatures = Tile(toPosition):getCreatures()
		if doorCreatures and #doorCreatures > 0 then
			for _, doorCreature in pairs(doorCreatures) do
				local pushPosition = findPushPosition(doorCreature, 1)
				if not pushPosition then
					player:sendCancelMessage(RETURNVALUE_NOTENOUGHROOM)
					return true
				end
				table.insert(creaturePositionTable, {creature = doorCreature, position = pushPosition})
			end
			for _, tableCreature in ipairs(creaturePositionTable) do
				tableCreature.creature:teleportTo(tableCreature.position, true)
			end
		end
		if findOddDoor(itemId) then
			item:transform(findOddDoor(itemId, 2))
		else
			item:transform(itemId - 1)
		end
		return true
	elseif table.contains(closedDoors, itemId) or table.contains(closedExtraDoors, itemId) or table.contains(closedHouseDoors, itemId) then
		if findOddDoor(itemId) then
			item:transform(findOddDoor(itemId, 3))
		else
			item:transform(itemId + 1)
		end
		return true
	end
	return false
end

local doorTables = {keys, openDoors, closedDoors, lockedDoors, openExtraDoors, closedExtraDoors, openHouseDoors, closedHouseDoors, closedQuestDoors, closedLevelDoors}
for _, doors in pairs(doorTables) do
	for _, doorId in pairs(doors) do
		door:id(doorId)
	end
end
door:register()
