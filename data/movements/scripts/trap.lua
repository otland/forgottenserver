function onStepIn(cid, item, position, fromPosition)
	if item.itemid == 2579 then -- trap item
		doTargetCombatHealth(0, cid, COMBAT_PHYSICALDAMAGE, -15, -30, CONST_ME_NONE)
		Item(item.uid):transform(item.itemid - 1)
	elseif item.itemid == 1510 then -- blades from slit
		doTargetCombatHealth(0, cid, COMBAT_PHYSICALDAMAGE, -50, -100, CONST_ME_NONE)
		Item(item.uid):transform(item.itemid + 1)
	elseif item.itemid == 1513 then -- spikes from strange holes
		doTargetCombatHealth(0, cid, COMBAT_PHYSICALDAMAGE, -50, -100, CONST_ME_NONE)
	elseif item.itemid == 4208 then -- jungle maw
		doTargetCombatHealth(0, cid, COMBAT_EARTHDAMAGE, -15, -30, CONST_ME_NONE)
		Item(item.uid):transform(item.itemid + 1)
	end
	return true
end

function onStepOut(cid, item, position, fromPosition)
	Item(item.uid):transform(item.itemid - 1)
	return true
end

function onRemoveItem(item, tile, position)
	local trap = Item(item.uid)
	local thingPos = trap:getPosition()
	if getDistanceBetween(thingPos, position) > 0 then
		trap:transform(item.itemid - 1)
		thingPos:sendMagicEffect(CONST_ME_POFF)
	end
	return true
end
