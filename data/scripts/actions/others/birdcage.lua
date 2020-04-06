local birdcage = Action()

function birdcage.onUse(player, item, fromPosition, target, toPosition, isHotkey)
	if math.random(100) == 1 then
		item:transform(2975)
	else
		item:getPosition():sendMagicEffect(CONST_ME_SOUND_YELLOW)
	end
	return true
end

birdcage:id(2976)
birdcage:register()
