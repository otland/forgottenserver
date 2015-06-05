function onThink(player)
	if Game.getWorldType() == WORLD_TYPE_PVP_ENFORCED or (player:getSkullTime() == 0 and not isInArray({SKULL_RED, SKULL_BLACK}, player:getSkull())) then
		return true
	end

	local ticks = player:getSkullTime() - 1000
	if ticks < 0 then
		player:setSkullTime(0)
	else
		player:setSkullTime(ticks)
	end

	if isInArray({SKULL_RED, SKULL_BLACK}, player:getSkull()) and ticks < 1000 and not player:getCondition(CONDITION_INFIGHT, CONDITIONID_DEFAULT) then
		player:setSkull(SKULL_NONE)
	end
	return true
end
