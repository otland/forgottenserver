local traps = {
	[1510] = {transformTo = 1511, damage = {-50, -100}},
	[1513] = {damage = {-50, -100}},
	[2579] = {transformTo = 2578, damage = {-15, -30}},
	[4208] = {transformTo = 4209, damage = {-15, -30}, type = COMBAT_EARTHDAMAGE}
}

function onStepIn(creature, item, position, fromPosition)
	local trap = traps[item.itemid]
	if not trap then
		return true
	end

	if creature:isMonster() then
		doTargetCombatHealth(0, creature, trap.type or COMBAT_PHYSICALDAMAGE, trap.damage[1], trap.damage[2], CONST_ME_NONE)
	end

	if trap.transformTo then
		item:transform(trap.transformTo)
	end
	return true
end

function onStepOut(creature, item, position, fromPosition)
	item:transform(item.itemid - 1)
	return true
end

function onRemoveItem(item, tile, position)
	local itemPosition = item:getPosition()
	if itemPosition:getDistance(position) > 0 then
		item:transform(item.itemid - 1)
		itemPosition:sendMagicEffect(CONST_ME_POFF)
	end
	return true
end
