local event = CreatureEvent("Skull Decay")

function event.onThink(player, interval)
	if Game.getWorldType() == WORLD_TYPE_PVP_ENFORCED then
		return true
	end

	local skullTime = player:getSkullTime()
	if skullTime <= 0 then
		return true
	end

	local elapsed = interval / 1000
	skullTime = math.max(0, skullTime - elapsed)

	player:setSkullTime(skullTime)

	if skullTime > 0 then
		return true
	end

	if player:hasCondition(CONDITION_INFIGHT) then
		return true
	end

	local skull = player:getSkull()
	if skull == SKULL_RED or skull == SKULL_BLACK then
		player:setSkull(SKULL_NONE)
	end
	return true
end

event:register()
