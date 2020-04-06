-- Locked key door (is always the first id (in items.xml has the description "It is locked")
local doorId = {}
local keyDoorLocked = {}

local door = Action()

for index, value in ipairs(keyUnlockedDoor) do
	if not table.contains(doorId, value.openDoor) then
		table.insert(doorId, value.openDoor)
	end
	if not table.contains(doorId, value.closedDoor) then
		table.insert(doorId, value.closedDoor)
	end
end

for index, value in ipairs(keyLockedDoor) do
	if not table.contains(doorId, value.closedDoor) then
		table.insert(doorId, value.closedDoor)
	end
	if not table.contains(keyDoorLocked, value.closedDoor) then
		table.insert(keyDoorLocked, value.closedDoor)
	end
end

for keys = 2967, 2973 do
	table.insert(doorId, keys)
end

function door.onUse(player, item, fromPosition, target, toPosition, isHotkey)

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
			toPosition.y = toPosition.y - 2
			query = Tile(toPosition):queryAdd(doorCreature, bit.bor(FLAG_IGNOREBLOCKCREATURE, FLAG_PATHFINDING))
		end
		if query ~= RETURNVALUE_NOERROR then
			toPosition.x = toPosition.x - 1
			toPosition.y = toPosition.y + 1
			query = Tile(toPosition):queryAdd(doorCreature, bit.bor(FLAG_IGNOREBLOCKCREATURE, FLAG_PATHFINDING))
		end
		if query ~= RETURNVALUE_NOERROR then
			player:sendCancelMessage(RETURNVALUE_NOTPOSSIBLE)
			return true
		end
		doorCreature:teleportTo(toPosition, true)
	end

	if table.contains(keyDoorLocked, item.itemid) then
		player:sendTextMessage(MESSAGE_INFO_DESCR, "It is locked.")
		return true
	end

	for index, value in ipairs(keyUnlockedDoor) do
		 if value.closedDoor == item.itemid then
				item:transform(value.openDoor)
			return true
		end
	end
	for index, value in ipairs(keyUnlockedDoor) do
		if value.openDoor == item.itemid then
			item:transform(value.closedDoor)
			return true
		end
	end

	-- Key use on door
	if target.actionid > 0 then
	for index, value in ipairs(keyLockedDoor) do
		for i, v in ipairs(keyUnlockedDoor) do
			if item.actionid == target.actionid and value.closedDoor == target.itemid then
				target:transform(value.openDoor)
				return true
			elseif item.actionid == target.actionid and value.openDoor == target.itemid then
			 target:transform(value.closedDoor)
				return true
			elseif item.actionid == target.actionid and v.closedDoor == target.itemid then
				target:transform(value.closedDoor)
				return true
			end
		end
	end
	else
		player:sendCancelMessage("The key does not match.")
	end
	return true
end

for key, value in pairs(doorId) do
	door:id(value)
end

door:register()
