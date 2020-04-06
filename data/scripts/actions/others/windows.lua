local windows = Action()

local windowId = {}
for index, value in ipairs(window) do
    if not table.contains(windowId, value.openWindow) then
        table.insert(windowId, value.openWindow)
    end

    if not table.contains(windowId, value.closedWindow) then
        table.insert(windowId, value.closedWindow)
    end
end

function windows.onUse(player, item, fromPosition, target, toPosition, isHotkey)

	local tile = Tile(fromPosition)
	local house = tile and tile:getHouse()
	if not house then
		fromPosition.y = fromPosition.y - 1
		tile = Tile(fromPosition)
		house = tile and tile:getHouse()
		if not house then
			fromPosition.y = fromPosition.y + 1
			fromPosition.x = fromPosition.x - 1
			tile = Tile(fromPosition)
			house = tile and tile:getHouse()
		end
	end
	if house then
		if Tile(fromPosition):getHouse() and player:getAccountType() < ACCOUNT_TYPE_GAMEMASTER then
			return false
		end
	end

	for index, value in ipairs(window) do
		 if value.closedWindow == item.itemid then
				item:transform(value.openWindow)
				return true
		end
	end

	for index, value in ipairs(window) do
		if value.openWindow == item.itemid then
			item:transform(value.closedWindow)
		end
	end
	return true
end

for index, value in ipairs(windowId) do
    windows:id(value)
end

windows:register()
