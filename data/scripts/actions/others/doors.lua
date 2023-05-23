local openOddDoors = {
	[12695] = { locked = 13237, closed = 12692 },
	[12703] = { locked = 13236, closed = 12701 },
	[14635] = { locked = 14634, closed = 14633 },
	[17435] = { locked = 14641, closed = 14640 },
	[26533] = { locked = 26530, closed = 26529 },
	[26534] = { locked = 26532, closed = 26531 },
	[31176] = { locked = 31175, closed = 27559 },
	[31024] = { locked = 31021, closed = 31020 },
	[31025] = { locked = 31023, closed = 31022 },
	[31541] = { locked = 31314, closed = 31314 },
	[31542] = { locked = 31315, closed = 31315 },
	[32691] = { locked = 32705, closed = 32689 },
	[32692] = { locked = 32706, closed = 32690 },
	[32695] = { locked = 32707, closed = 32693 },
	[32696] = { locked = 32708, closed = 32694 },
	[33432] = { locked = 33429, closed = 33428 },
	[33433] = { locked = 33431, closed = 33430 },
	[33493] = { locked = 33490, closed = 33489 },
	[33494] = { locked = 33492, closed = 33491 },
	[34152] = { locked = 34150, closed = 34150 },
	[34153] = { locked = 34151, closed = 34151 },
	[36292] = { locked = 36289, closed = 36288 },
	[36293] = { locked = 36291, closed = 36290 },
	[37596] = { locked = 37593, closed = 37592 },
	[37597] = { locked = 37595, closed = 37594 },
	[39119] = { locked = 39116, closed = 39115 },
	[39120] = { locked = 39118, closed = 39117 }
}
local closedOddDoors = {
	[12692] = { locked = 13237, open = 12695 },
	[12701] = { locked = 13236, open = 12703 },
	[14633] = { locked = 14634, open = 14635 },
	[14640] = { locked = 14641, open = 17435 },
	[26529] = { locked = 26530, open = 26533 },
	[26531] = { locked = 26532, open = 26534 },
	[27559] = { locked = 31175, open = 31176 },
	[31020] = { locked = 31021, open = 31024 },
	[31022] = { locked = 31023, open = 31025 },
	[31314] = { locked = 31314, open = 31541 },
	[31315] = { locked = 31315, open = 31542 },
	[32689] = { locked = 32705, open = 32691 },
	[32690] = { locked = 32706, open = 32692 },
	[32693] = { locked = 32707, open = 32695 },
	[32694] = { locked = 32708, open = 32696 },
	[33428] = { locked = 33429, open = 33432 },
	[33430] = { locked = 33431, open = 33433 },
	[33489] = { locked = 33490, open = 33493 },
	[33491] = { locked = 33492, open = 33494 },
	[34150] = { locked = 34150, open = 34152 },
	[34151] = { locked = 34151, open = 34153 },
	[36288] = { locked = 36289, open = 36292 },
	[36290] = { locked = 36291, open = 36293 },
	[37592] = { locked = 37593, open = 37596 },
	[37594] = { locked = 37595, open = 37597 },
	[39115] = { locked = 39116, open = 39119 },
	[39117] = { locked = 39118, open = 39120 }
}
local lockedOddDoors = {
	[13237] = { closed = 12692, open = 12695 },
	[13236] = { closed = 12701, open = 12703 },
	[14634] = { closed = 14633, open = 14635 },
	[14641] = { closed = 14640, open = 17435 },
	[26530] = { closed = 26529, open = 26533 },
	[26532] = { closed = 26531, open = 26534 },
	[31175] = { closed = 27559, open = 31176 },
	[31021] = { closed = 31020, open = 31024 },
	[31023] = { closed = 31022, open = 31025 },
	[32705] = { closed = 32689, open = 32691 },
	[32706] = { closed = 32690, open = 32692 },
	[32707] = { closed = 32693, open = 32695 },
	[32708] = { closed = 32694, open = 32696 },
	[33429] = { closed = 33428, open = 33432 },
	[33431] = { closed = 33430, open = 33433 },
	[33490] = { closed = 33489, open = 33493 },
	[33492] = { closed = 33491, open = 33494 },
	[36289] = { closed = 36288, open = 36292 },
	[36291] = { closed = 36290, open = 36293 },
	[37593] = { closed = 37592, open = 37596 },
	[37595] = { closed = 37594, open = 37597 },
	[39116] = { closed = 39115, open = 39119 },
	[39118] = { closed = 39117, open = 39120 }
}

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

local door = Action()

function door.onUse(player, item, fromPosition, target, toPosition, isHotkey)
	local itemId = item:getId()
	local transformTo = 0
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
		if lockedOddDoors[target.itemid] then
			transformTo = lockedOddDoors[target.itemid].open
		else
			transformTo = target.itemid + 2
		end
		if table.contains(openDoors, target.itemid) then
			if openOddDoors[target.itemid] then
				transformTo = openOddDoors[target.itemid].locked
			else
				transformTo = target.itemid - 2
			end
		elseif table.contains(closedDoors, target.itemid) then
			if closedOddDoors[target.itemid] then
				transformTo = closedOddDoors[target.itemid].locked
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
		if openOddDoors[itemId] then
			transformTo = openOddDoors[itemId].closed
		else
			transformTo = itemId - 1
		end
		item:transform(transformTo)
		return true
	elseif table.contains(closedDoors, itemId) or table.contains(closedExtraDoors, itemId) or table.contains(closedHouseDoors, itemId) then
		if closedOddDoors[itemId] then
			transformTo = closedOddDoors[itemId].open
		else
			transformTo = itemId + 1
		end
		item:transform(transformTo)
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
