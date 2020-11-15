function onUse(player, item, fromPosition, target, toPosition, isHotkey)
	if fromPosition.x ~= CONTAINER_POSITION then
		fromPosition:sendMagicEffect(math.random(CONST_ME_FIREWORK_YELLOW, CONST_ME_FIREWORK_BLUE))
	else
		local position = player:getPosition()
		position:sendMagicEffect(CONST_ME_FIREAREA)
		player:say("Ouch! Rather place it on the ground next time.", TALKTYPE_MONSTER_SAY)
		player:addAchievementProgress("Rocket in Pocket", 3)
		doTargetCombat(0, player, COMBAT_PHYSICALDAMAGE, -10, -10, CONST_ME_EXPLOSIONAREA)
	end
	player:addAchievementProgress("Fireworks in the Sky", 250)
	item:remove()
	return true
end
