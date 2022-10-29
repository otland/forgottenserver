local yellowPillow = MoveEvent()
yellowPillow:type("stepin")

function yellowPillow.onStepIn(player, item, position, fromPosition)
	if not player or player:isInGhostMode() then
		return true
	end
	player:say("Faaart!", TALKTYPE_MONSTER_SAY)
	item:getPosition():sendMagicEffect(CONST_ME_POFF)
	return true
end

yellowPillow:id(8072)
yellowPillow:register()
