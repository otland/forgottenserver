-- register event in player
local event = CreatureEvent("RegisterPreventGenerateLoot")

function event.onLogin(player)
	player:registerEvent("PreventGenerateLoot")
	return true
end

event:register()

-- event to check if player has flag to prevent generate loot
event = CreatureEvent("PreventGenerateLoot")

function event.onKill(player, target, lastHit)
	if player:hasFlag(PlayerFlag_NotGenerateLoot) then
		target:setDropLoot(false)
	end
	return true
end

event:register()
