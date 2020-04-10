local frozenUrsagrodon = Action()

function frozenUrsagrodon.onUse(player, item, fromPosition, target, toPosition, isHotkey)
	if player:hasMount(38) then
		player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "You already have the obedience of the ursagrodon.")
		return true
	end

	if target.itemid == 22729 then
		local chance = math.random(100)
		if chance > 12 then
			player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "The ice cracked and the frozen creature with it - be more careful next time!")
			item:remove(1)
			target:transform(22732)
			target:decay()
			return true
		else
			player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "You managed to melt about half of the ice blook. Quickly now, it's ice cold here and the ice block could freeze over again.")
			target:transform(22730)
			target:decay()
			return true
		end
	end
	if target.itemid == 22730 then
		local chance = math.random(100)
		if chance > 12 then
			player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "The ice cracked and the frozen creature with it - be more careful next time!")
			item:remove(1)
			target:transform(22732)
			target:decay()
			return true
		else
			player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "You managed to melt almost the whole block, only the feet of the creature are still stuck in the ice. Finish the job!")
			target:transform(22731)
			target:decay()
			return true
		end
	end
	if target.itemid == 22731 then
		local chance = math.random(100)
		if chance > 12 then
			item:remove(1)
			target:transform(22732)
			target:decay()
			player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "The ice cracked and the frozen creature with it - be more careful next time!")
			return true
		else
			item:remove(1)
			target:transform(22732)
			target:decay()
			player:addMount(38)
			player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "The freed ursagrodon look at you with glowing, obedient eyes.")
		end
	end
	return true
end

frozenUrsagrodon:id(22726)
frozenUrsagrodon:register()
