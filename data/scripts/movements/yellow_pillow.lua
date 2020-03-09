local yellowPillow = MoveEvent()
yellowPillow:type("stepin")

function yellowPillow.onStepIn(player, item, position, fromPosition)
	if not player or player:isInGhostMode() then
		return true
	end
	player:say("Faaart!", TALKTYPE_MONSTER_SAY)
	return true
end

yellowPillow:id(8072)
yellowPillow:register()
