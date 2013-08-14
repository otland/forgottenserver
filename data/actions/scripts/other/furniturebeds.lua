local BEDS = {
	[7904] = {{1754, 1755}, {1760, 1761}}, -- blue kit
	[7905] = {{7811, 7812}, {7813, 7814}}, -- green kit
	[7906] = {{7815, 7816}, {7817, 7818}}, -- red kit
	[7907] = {{7819, 7820}, {7821, 7822}}, -- yellow kit
	[20252] = {{20197, 20198}, {20199, 20200}} -- canopy bed
}

local function internalBedTransform(item, itemEx, toPosition, ids)
	doTransformItem(itemEx.uid, ids[1])
	doTransformItem(getThingfromPos(toPosition).uid, ids[2])

	doSendMagicEffect(getThingPos(itemEx.uid), CONST_ME_POFF)
	doSendMagicEffect(toPosition, CONST_ME_POFF)

	doRemoveItem(item.uid)
end

function onUse(cid, item, fromPosition, itemEx, toPosition)
	local newBed = BEDS[item.itemid]
	if(not newBed or not getTileHouseInfo(getCreaturePosition(cid))) then
		return false
	end

	--TODO
	--Is it possible in real tibia, to use same modification on current used?
	if(isInArray({newBed[1][1], newBed[2][1]}, itemEx.itemid)) then
		doPlayerSendCancel(cid, "You already have this bed modification.")
		return true
	end

	for kit, bed in pairs(BEDS) do
		if(bed[1][1] == itemEx.itemid or itemEx.itemid == 1758) then
			toPosition.y = toPosition.y + 1
			internalBedTransform(item, itemEx, toPosition, newBed[1])
			break
		elseif(bed[2][1] == itemEx.itemid or itemEx.itemid == 1756) then
			toPosition.x = toPosition.x + 1
			internalBedTransform(item, itemEx, toPosition, newBed[2])
			break
		end
	end

	return true
end
