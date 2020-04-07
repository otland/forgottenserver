local traps = {
	[2145] = {transformTo = 2146, damage = {-50, -100}},
	[2147] = {transformTo = 2148, damage = {-50, -100}},
	[3481] = {transformTo = 3482, damage = {-15, -30}},
	[3944] = {transformTo = 3945, damage = {-15, -30}, type = COMBAT_EARTHDAMAGE}
}

-- onStepIn
local stepInTraps = MoveEvent()

function stepInTraps.onStepIn(creature, item, position, fromPosition)
	local trap = traps[item.itemid]
	if not trap then
		return true
	end

	if creature:isMonster() or creature:isPlayer() then
		doTargetCombatHealth(0, creature, trap.type or COMBAT_PHYSICALDAMAGE, trap.damage[1], trap.damage[2], CONST_ME_NONE)
	end

	if trap.transformTo then
		item:transform(trap.transformTo)
	end
	return true
end

stepInTraps:id(2145,2147,3481,3944)
stepInTraps:register()

-- onStepOut
local stepOutTraps = MoveEvent()

function stepOutTraps.onStepOut(creature, item, position, fromPosition)
	item:transform(item.itemid - 1)
	return true
end

stepOutTraps:id(2146,2148,3482,3945)
stepOutTraps:register()
