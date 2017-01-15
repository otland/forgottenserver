local beds = {
	[7904] = {{7811, 7812}, {7813, 7814}}, -- green kit
	[7905] = {{7819, 7820}, {7821, 7822}}, -- yellow kit
	[7906] = {{7815, 7816}, {7817, 7818}}, -- red kit
	[7907] = {{1754, 1755}, {1760, 1761}}, -- removal kit
	[20252] = {{20197, 20198}, {20199, 20200}} -- canopy kit
}

local function internalBedTransform(item, targetItem, toPosition, itemArray)
	targetItem:transform(itemArray[1])
	targetItem:getPosition():sendMagicEffect(CONST_ME_POFF)

	Tile(toPosition):getItemByType(ITEM_TYPE_BED):transform(itemArray[2])
	toPosition:sendMagicEffect(CONST_ME_POFF)

	item:remove()
end

function onUse(player, item, fromPosition, target, toPosition, isHotkey)
	local newBed = beds[item:getId()]
	if not newBed or target == nil or not target:isItem() then
		return false
	end

	local tile = Tile(toPosition)
	if not tile or not tile:getHouse() then
		return false
	end

	local targetItemId = target:getId()
	if targetItemId == newBed[1][1] or targetItemId == newBed[2][1] then
		player:sendTextMessage(MESSAGE_STATUS_SMALL, "You already have this bed modification.")
		return true
	end

	for _, bed in pairs(beds) do
		if bed[1][1] == targetItemId or isInArray({1758, 5502, 18027}, targetItemId) then
			toPosition:sendMagicEffect(CONST_ME_POFF)
			toPosition.y = toPosition.y + 1
			internalBedTransform(item, target, toPosition, newBed[1])
			break
		elseif bed[2][1] == targetItemId or isInArray({1756, 5500, 18029}, targetItemId) then
			toPosition:sendMagicEffect(CONST_ME_POFF)
			toPosition.x = toPosition.x + 1
			internalBedTransform(item, target, toPosition, newBed[2])
			break
		end
	end
	return true
end
