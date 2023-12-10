local event = CreatureEvent("WorldTime")

function event.onLogin(player)
    local worldTime = Game.getWorldTime()
    player:sendWorldTime(worldTime)
	return true
end

event:register()

event = CreatureEvent("WorldLight")

function event.onLogin(player)
    local worldLightColor, worldLightLevel = Game.getWorldLight()
	player:sendWorldLight(worldLightColor, worldLightLevel)
	return true
end

event:register()