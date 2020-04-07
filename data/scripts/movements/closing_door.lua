-- Level and quests closing door (onStepIn).
-- This prevents a player who has not yet done the quest, from crossing the player who has already done so, skipping the entire quest and going straight to the final reward.
local door = MoveEvent()

local doorIds = {}
for index, value in ipairs(questDoor) do
    if not table.contains(doorIds, value.openDoor) then
        table.insert(doorIds, value.openDoor)
    end
end
for index, value in ipairs(levelDoor) do
    if not table.contains(doorIds, value.openDoor) then
        table.insert(doorIds, value.openDoor)
    end
end

function door.onStepIn(creature, item, position, fromPosition)
	local player = creature:getPlayer()
	if not player then
		return
	end

    for index, value in ipairs(questDoor) do
		 if value.openDoor == item.itemid then
			if player:getStorageValue(item.actionid) ~= -1 then
				return true
			else
				player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "The door seems to be sealed against unwanted intruders.")
				player:teleportTo(fromPosition, true)
			return false
			end
		end
	end
	for index, value in ipairs(levelDoor) do
		 if value.openDoor == item.itemid then
			if item.actionid > 0 and player:getLevel() >= item.actionid - 1000 then
				return true
			else
				player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "Only the worthy may pass.")
				player:teleportTo(fromPosition, true)
			return false
			end
		end
	end
	return true
end

for index, value in ipairs(doorIds) do
    door:id(value)
end

door:register()

-- Level and quests closing door (onStepOut).
-- This closes the door after the player passes through it.
local door = MoveEvent()

local doorIds = {}
for index, value in ipairs(questDoor) do
    if not table.contains(doorIds, value.openDoor) then
        table.insert(doorIds, value.openDoor)
    end
end
for index, value in ipairs(levelDoor) do
    if not table.contains(doorIds, value.openDoor) then
        table.insert(doorIds, value.openDoor)
    end
end

function door.onStepOut(creature, item, position, fromPosition)
	local player = creature:getPlayer()
	if not player then
		return
	end

	local tile = Tile(position)
	if tile:getCreatureCount() > 0 then
		return true
	end

	local newPosition = {x = position.x + 1, y = position.y, z = position.z}
	local query = Tile(newPosition):queryAdd(creature)
	if query ~= RETURNVALUE_NOERROR or query == RETURNVALUE_NOTENOUGHROOM then
		newPosition.x = newPosition.x - 1
		newPosition.y = newPosition.y + 1
		query = Tile(newPosition):queryAdd(creature)
	end

	if query == RETURNVALUE_NOERROR or query ~= RETURNVALUE_NOTENOUGHROOM then
		doRelocate(position, newPosition)
	end

	local i, tileItem, tileCount = 1, true, tile:getThingCount()
	while tileItem and i < tileCount do
		tileItem = tile:getThing(i)
		if tileItem and tileItem:getUniqueId() ~= item.uid and tileItem:getType():isMovable() then
			tileItem:remove()
		else
			i = i + 1
		end
	end

	for index, value in ipairs(levelDoor) do
		if value.openDoor == item.itemid then
			item:transform(value.closedDoor)
		end
	end
	for index, value in ipairs(questDoor) do
		if value.openDoor == item.itemid then
			item:transform(value.closedDoor)
		end
	end
	return true
end

for index, value in ipairs(doorIds) do
    door:id(value)
end

door:register()
