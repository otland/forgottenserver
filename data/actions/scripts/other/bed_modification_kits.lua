local BEDS = {
	[7904] = {{7811, 7812}, {7813, 7814}}, -- green kit
	[7905] = {{7819, 7820}, {7821, 7822}}, -- yellow kit
	[7906] = {{7815, 7816}, {7817, 7818}}, -- red kit
	[7907] = {{1754, 1755}, {1760, 1761}},  -- removal kit
	[20252] = {{20197, 20198}, {20199, 20200}} -- canopy kit
}

local function internalBedTransform(item, target, toPosition, ids)
	local targetItem = Item(target.uid)
	targetItem:transform(ids[1])
	targetItem:getPosition():sendMagicEffect(CONST_ME_POFF)

	toPosition:getTile():getItemByType(ITEM_TYPE_BED):transform(ids[2])
	toPosition:sendMagicEffect(CONST_ME_POFF)

	Item(item.uid):remove()
end

function onUse(player, item, fromPosition, target, toPosition, isHotkey)
	local newBed = BEDS[item.itemid]
	if not newBed then
		return false
	end

	local tile = toPosition:getTile()
	if not tile or not tile:getHouse() then
		return false
	end

	if target.itemid == newBed[1][1] or target.itemid == newBed[2][1] then
		player:sendTextMessage(MESSAGE_STATUS_SMALL, "You already have this bed modification.")
		return true
	end

	for kit, bed in pairs(BEDS) do
		if bed[1][1] == target.itemid or isInArray({1758, 5502, 18027}, target.itemid) then
			toPosition:sendMagicEffect(CONST_ME_POFF)
			toPosition.y = toPosition.y + 1
			internalBedTransform(item, target, toPosition, newBed[1])
			break
		elseif bed[2][1] == target.itemid or isInArray({1756, 5500, 18029}, target.itemid) then
			toPosition:sendMagicEffect(CONST_ME_POFF)
			toPosition.x = toPosition.x + 1
			internalBedTransform(item, target, toPosition, newBed[2])
			break
		end
	end
	return true
end
