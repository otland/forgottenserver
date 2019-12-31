function onUse(player, item, fromPosition, target, toPosition, isHotkey)
	if player:getStorageValue(43000) <= os.time() then
		local chance = math.random(100)
		local msg = ""
		if chance <= 16 then
			player:addHealth(-200)
			msg = "Ouch! You squeezed your fingers."
		elseif chance > 16 and chance <= 64 then
			Game.createItem(math.random(7632,7633), 1, player:getPosition())
			msg = "You found a beautiful pearl."
		else
			msg = "Nothing is inside."
		end
		player:say(msg, TALKTYPE_MONSTER_SAY, false, player, item:getPosition())
		item:transform(7553)
		player:setStorageValue(43000, os.time() + 72000)
		item:getPosition():sendMagicEffect(CONST_ME_BUBBLES)
		return true
	else
		player:say("You have already opened a shell today.", TALKTYPE_MONSTER_SAY, false, player, item:getPosition())
		return false
	end
	return false
end
