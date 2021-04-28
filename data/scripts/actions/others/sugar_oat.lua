local machines = {
	[5469] = 5513,
	[5470] = 5514
}

local sugarOat = Action()

function sugarOat.onUse(player, item, fromPosition, target, toPosition, isHotkey)
	local targetId = target.itemid
	if targetId == 2694 then
		if toPosition.x ~= CONTAINER_POSITION then
			Game.createItem(13939, 1, toPosition)
		else
			player:addItem(13939, 1)
			toPosition = player:getPosition()
		end
		toPosition:sendMagicEffect(CONST_ME_MAGIC_BLUE)
		item:remove(1)
		target:remove(1)
	else
		local machine = machines[targetId]
		if machine then
			target:transform(machine)
			target:decay()
			item:remove(1)
			player:getPosition():sendMagicEffect(CONST_ME_MAGIC_RED)
			player:addAchievementProgress("Homebrewed", 50)
		end
	end
	return true
end

sugarOat:id(5467)
sugarOat:register()
