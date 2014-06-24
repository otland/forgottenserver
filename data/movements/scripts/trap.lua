function onStepIn(cid, item, position, fromPosition)
	local traps = {
		[1510] = {toItem = 1511, combat = {-50, -100}},
		[1513] = {combat = {-50, -100}},
		[2579] = {toItem = 2578, combat = {-15, -30}},
		[4208] = {toItem = 4209, combat = {-15, -30}, type = COMBAT_EARTHDAMAGE}
	}
	
	local trap = traps[item.itemid]
	if(trap and Player(cid) ~= nil) then
		doTargetCombatHealth(0, cid, trap.type == nil and COMBAT_PHYSICALDAMAGE or trap.type, trap.combat[1], trap.combat[2], CONST_ME_NONE)
		
		if(trap.toItem ~= nil) then
			Item(item.uid):transform(trap.toItem)
		end
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
